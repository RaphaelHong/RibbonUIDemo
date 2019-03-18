#include "StdAfx.h"
#include "VideoRouter.h"
#include "DrawWndPage.h"
#include "DrawWndObj.h"
#include "GridView.h"


//////////////////////////////////////////////////////////////////////////
// CResizeAnchor 对齐功能类

CResizeAnchor::CResizeAnchor()
{

}

CResizeAnchor::CResizeAnchor(SPLICELAYOUT &layout)
{
	ResetAnchor(layout);
}

CResizeAnchor::~CResizeAnchor()
{

}

void CResizeAnchor::ResetAnchor(SPLICELAYOUT &layout)
{
	m_layout = layout;

	if (m_arAnchor.GetSize() > 0)
		m_arAnchor.RemoveAll();

	int iHorz = layout.iLogHorz;
	int iVert = layout.iLogVert;
	double dWid = layout.iPhyHorzPixel*1.0/iHorz;
	double dHei = layout.iPhyVertPixel*1.0/iVert;

	CPoint ptAnchor;

	for (int v=0; v<iVert; v++)
		for (int h=0; h<iHorz; h++)
		{
			ptAnchor.x = Round(h*dWid);
			ptAnchor.y = Round(v*dHei);
			m_arAnchor.Add(ptAnchor);
		}
}

long CResizeAnchor::GetLeftAnchor(const long l) const
{
	int iHorz = m_layout.iLogHorz;
	long left=0;

	//超过背景左边
	if (l <= 0) {
		left = 0;
		return left;
	}

	//超过背景右边
	if (l >= m_arAnchor[iHorz-1].x) {
		left = m_arAnchor[iHorz-1].x;
		return left;
	}

	//其它
	for (int i=0; i<iHorz-1; i++)
	{
		if (l >= m_arAnchor[i].x && l < m_arAnchor[i+1].x) {
			left = m_arAnchor[i].x;
			break;
		}
	}

	return left;
}

long CResizeAnchor::GetTopAnchor(const long t) const
{
	int iVert = m_layout.iLogVert;
	int iHorz = m_layout.iLogHorz;
	int top=0;

	//超过背景顶端
	if (t <= 0) {
		top = 0;
		return top;
	}

	//超过背景底端
	if (t >= m_arAnchor[iHorz*(iVert-1)].y) {
		top = m_arAnchor[iHorz*(iVert-1)].y;
		return top;
	}

	//其它
	for (int i=0; i<iVert-1; i++)
	{
		if (t >= m_arAnchor[iHorz*i].y && t < m_arAnchor[iHorz*(i+1)].y) {
			top = m_arAnchor[iHorz*i].y;
			break;
		}
	}

	return top;
}

long CResizeAnchor::GetRightAnchor(const long r) const
{
	int iHorz = m_layout.iLogHorz;
	long right= m_layout.iPhyHorzPixel;

	//超过背景左边
	if (r <= 0) {
		right = m_arAnchor[1].x;
		return right;
	}

	//超过背景右边
	if (r > m_arAnchor[iHorz-1].x) {
		right = m_layout.iPhyHorzPixel;
		return right;
	}

	//其它
	for ( int i=0; i<iHorz-1; i++)
	{
		if (r > m_arAnchor[i].x && r <= m_arAnchor[i+1].x) {
			right = m_arAnchor[i+1].x;
			break;
		}
	}

	return right;
}

long CResizeAnchor::GetBottomAnchor(const long b) const
{
	int iVert = m_layout.iLogVert;
	int iHorz = m_layout.iLogHorz;
	int bottom=0;

	//超过背景顶端
	if (b <= 0) {
		bottom = m_arAnchor[iHorz].y;
		return bottom;
	}

	//超过背景底端
	if (b > m_arAnchor[iHorz*(iVert-1)].y)	{
		bottom = m_layout.iPhyVertPixel;
		return bottom;
	}

	//其它
	for (int i=0; i<iVert-1; i++)
	{
		if (b > m_arAnchor[iHorz*i].y && b <= m_arAnchor[iHorz*(i+1)].y) {
			bottom = m_arAnchor[iHorz*(i+1)].y;
			break;
		}
	}

	return bottom;
}

CRect CResizeAnchor::GetResizeRect(const CRect rcOrg) const 
{
	CRect rc;
	rc.left   = GetLeftAnchor(rcOrg.left);
	rc.top    = GetTopAnchor(rcOrg.top);
	rc.right  = GetRightAnchor(rcOrg.right);
	rc.bottom = GetBottomAnchor(rcOrg.bottom);

	return rc;
}

//////////////////////////////////////////////////////////////////////////
// CPageMouseManager  鼠标消息处理

CPageMouseManager::CPageMouseManager(void)
{
	m_pOwnerPage = NULL;
	m_pWndLBtnDown = NULL;
	m_pWndLBtnUp = NULL;
	m_pScnLBtnDown = NULL;
	m_pScnLBtnUp = NULL;

	m_bDynamicBuilt = FALSE;

	m_dwLBtnDown = 0;
}

CPageMouseManager::CPageMouseManager(CDrawWndPage* pOwnerPage)
{
	m_pOwnerPage = pOwnerPage;

	m_pWndLBtnDown = NULL;
	m_pWndLBtnUp = NULL;
	m_pScnLBtnDown = NULL;
	m_pScnLBtnUp = NULL;

	m_bDynamicBuilt = FALSE;

	m_dwLBtnDown = 0;
}

CPageMouseManager::~CPageMouseManager(void)
{
	m_pOwnerPage = NULL;

	m_pWndLBtnDown = NULL;
	m_pWndLBtnUp = NULL;
	m_pScnLBtnDown = NULL;
	m_pScnLBtnUp = NULL;
}

void CPageMouseManager::SetOwner(CDrawWndPage* pOwner)
{
	m_pOwnerPage = pOwner;
}

void CPageMouseManager::OnContextMenu(CPoint point)
{
	//按功能逻辑顺序，先判断窗口上的右键消息，再是屏幕
	//窗体右键？
	CDrawWndObj* pWndObj = GetWndUnderCursor(point, FALSE);
	if (pWndObj)
	{
		pWndObj->OnPopupMenu(point);
		return;
	}

	//屏幕右键？
	CDrawScreenObj* pScnObj = GetScnUnderCursor(point);
	if (pScnObj)
	{
		pScnObj->OnPopupMenu(point);
		return;
	}
}

void CPageMouseManager::OnLBtnDown(CPoint point)
{
	m_ptLBtnDown = point;
	m_dwLBtnDown = GetTickCount();

	//按功能逻辑顺序，先判断窗口上的右键消息，再是屏幕
	//窗体左键按下？
	m_pWndLBtnDown = GetWndUnderCursor(point, TRUE);
	if (m_pWndLBtnDown && !m_pWndLBtnDown->IsLocked())
	{
		//m_pWndLBtnDown->SetPressed(TRUE);
		m_pWndLBtnDown->OnLBtnDown(point);
		return;
	}

	//开窗
	m_pWndLBtnDown = NULL;
	if (CInputManager::m_sCurPort > 0)
		m_bDynamicBuilt = TRUE;

}

void CPageMouseManager::OnLBtnUp(CPoint point)
{
	m_ptLBtnUp = point;

	//按功能逻辑顺序，先判断窗口上的右键消息，再是屏幕
	if (m_bDynamicBuilt)
	{
		//没有创建成功
		if (m_pWndLBtnDown == NULL)
			return LBtnUpReturn();

		//计算新窗口的最终坐标
		CRect rc(m_ptLBtnDown, point);
		if (rc.Width() < 20 || rc.Height() < 20)
		{
			m_pOwnerPage->RemoveWnd(m_pWndLBtnDown);
			m_pOwnerPage->Repaint(FALSE);
			return LBtnUpReturn();
		}

		//发送命令
		m_pWndLBtnDown->SetLogPos(rc);
		m_pWndLBtnDown->SetPosMark();
		m_pWndLBtnDown->Open();
		m_pOwnerPage->Repaint(FALSE);

		return LBtnUpReturn();
	}


	//窗体左键弹起？
	if (m_pWndLBtnDown != NULL)
	{
		m_pWndLBtnDown->OnLBtnUp(point);

		return LBtnUpReturn();
	}

	//其它处理

	LBtnUpReturn();
}

void CPageMouseManager::OnMouseMove(CPoint point)
{
	//防止鼠标抖动造成的误操作
	if (GetTickCount() - m_dwLBtnDown < 80)
	{
		return;
	}

	//按功能逻辑顺序，先判断窗口上的鼠标消息，再是屏幕
	if (m_bDynamicBuilt)
	{
		if (m_pWndLBtnDown == NULL)
			m_pWndLBtnDown = m_pOwnerPage->DynamicBuilt(m_ptLBtnDown, point);
		else
			m_pWndLBtnDown->SetLogPos(CRect(m_ptLBtnDown, point));

		if (m_pWndLBtnDown != NULL)
			m_pOwnerPage->Repaint(FALSE);

		return;
	}

	//窗体
	if (m_pWndLBtnDown != NULL)
		m_pWndLBtnDown->OnMouseMove(point);
	else
	{
		CDrawWndObj *pObj = GetWndUnderCursor(point, TRUE);
		if (pObj != NULL)
			pObj->OnMouseMove(point);

		return;
	}

	//屏幕
	if (m_pScnLBtnDown != NULL)
		m_pScnLBtnDown->OnMouseMove(point);
}


void CPageMouseManager::OnLBtnDbClick(CPoint point)
{
	//按功能逻辑顺序，先判断窗口上的右键消息，再是屏幕
	//窗体左键双击
	CDrawWndObj* pWndObj = GetWndUnderCursor(point, FALSE);
	if (pWndObj)
	{
		pWndObj->OnLBtnDblClick(point);
		return ;
	}

	//屏幕左键双击
	CDrawScreenObj* pScnObj = GetScnUnderCursor(point);
	if (pScnObj)
	{
		pScnObj->OnLBtnDblClick(point);
		return ;
	}
}

void CPageMouseManager::OnDropSource(const int iSrcPort, const CPoint point)
{
	// 信号源拖放处理，新开一个窗口或者改变一个窗口的信号源。
	CDrawWndObj* pWndObj = GetWndUnderCursor(point, FALSE);
	if (pWndObj == NULL || (pWndObj != NULL && pWndObj->IsLocked()))
	{
		CDrawScreenObj* pScn = GetScnUnderCursor(point);
		if (pScn == NULL)
			return;

		//新开窗口
		pWndObj = new CDrawWndObj(m_pOwnerPage);

		CString str;
		str.Format(_T("Source[%d]"), iSrcPort);

		pWndObj->SetWndID(m_pOwnerPage->PrepareWndID());
		pWndObj->SetSourcePort(CInputManager::m_sCurPort);
		pWndObj->SetZorder(m_pOwnerPage->GetWndCount()+1);
		pWndObj->SetPhyPos(pScn->GetPhyCoords());
		pWndObj->SetName(str.GetBuffer());
		pWndObj->SetBkColor(m_pOwnerPage->GetColor(CInputManager::m_sCurPort));
		pWndObj->Active(FALSE);
		pWndObj->SetLock(CDrawWndPage::m_sLockAll);
		pWndObj->SetPosMark();

		m_pOwnerPage->AddWnd(pWndObj);

		m_pOwnerPage->ActiveWnd(pWndObj);

		pWndObj->Open();

	}
	else
	{
		m_pOwnerPage->ActiveWnd(pWndObj);
		m_pOwnerPage->OnSourceChanged(iSrcPort);
	}	
}

void CPageMouseManager::OnDropOutput(const int iOutPort, const CPoint point)
{
	// Make sure the cursor was not located in a window.
	CDrawWndObj* pWndObj = GetWndUnderCursor(point, FALSE);
	if (pWndObj != NULL)
		return;

	// Get the screen that the cursor was located in.
	CDrawScreenObj* pScnObj = GetScnUnderCursor(point);
	if (pScnObj == NULL)
		return;

	// Make sure the output channel was not invalid.
	if (iOutPort < 0)
		return;

	// Close the screen while its ouput channel was equaled to the asigned channel.
	CDrawScreenObj* pClosedScreen = m_pOwnerPage->GetScreenByOutPort(iOutPort);
	if (pClosedScreen != NULL)
	{
		pClosedScreen->SetOutPort(INVALID_VALUE);
	}

	// Set output channel of the screen.
	pScnObj->SetOutPort(iOutPort);
	// Send command.
	pScnObj->OnOutputChange();

	// Redraw
	m_pOwnerPage->Repaint(TRUE);
}

inline CDrawWndObj* CPageMouseManager::GetWndUnderCursor(CPoint point, BOOL bDistinct) const
{
	CDrawWndObj *pObj = NULL;

	if (m_pOwnerPage->GetWndCount() == 0)
		return pObj;

	for (int i=m_pOwnerPage->GetWndCount()-1; i >= 0; i--)
	{
		if (PtInRect(m_pOwnerPage->GetWndObj(i)->m_rcLog, point))
		{
			if (!bDistinct)
			{
				pObj = m_pOwnerPage->GetWndObj(i);
				break;
			}
			else
			{
				if (m_pOwnerPage->GetWndObj(i)->IsLocked() && (pObj != NULL && pObj->IsLocked()))
					continue;

				pObj = m_pOwnerPage->GetWndObj(i);

				if (pObj->IsLocked())
					continue;
				else
					break;
			}	
		}
	}

	return pObj;
}

inline CDrawScreenObj* CPageMouseManager::GetScnUnderCursor(CPoint point) const
{
	CDrawScreenObj* pObj = NULL;

	if (m_pOwnerPage->GetScreenCount() == 0)
		return pObj;

	for (int i=m_pOwnerPage->GetScreenCount()-1; i>=0; i--)
	{
		if (m_pOwnerPage->GetScreenObj(i)->m_rcLog.Contains(PointF((REAL)point.x, (REAL)point.y)))
		{
			pObj = m_pOwnerPage->GetScreenObj(i);

			break;
		}
	}

	return pObj;
}

void CPageMouseManager::LBtnUpReturn()
{
	m_pScnLBtnDown = NULL;
	m_pScnLBtnUp = NULL;
	if (m_pWndLBtnDown != NULL)
	{
		m_pWndLBtnDown = NULL;
	}

	if (m_pWndLBtnUp != NULL)
	{
		m_pWndLBtnUp = NULL;
	}

	m_bDynamicBuilt = FALSE;

	m_dwLBtnDown = 0;
}


//////////////////////////////////////////////////////////////////////////
// CPageKeyboardManager 键盘消息处理

CPageKeyboardManager::CPageKeyboardManager()
{
}

CPageKeyboardManager::CPageKeyboardManager(CDrawWndPage* pOwner)
{
	m_pOwner = pOwner;
}

CPageKeyboardManager::~CPageKeyboardManager()
{

}

void CPageKeyboardManager::OnArrowKeyUp()
{
	CDrawWndObj* pObj = m_pOwner->GetActivedWnd();
	if (pObj==NULL)
		return;

	CRect rcPhy = pObj->GetPhyPos();

	if ((::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
	{
		rcPhy.bottom--;
		pObj->SetPhyPos(rcPhy);
		pObj->Size();
	}
	else
	{
		rcPhy.OffsetRect(0, -1);
		pObj->SetPhyPos(rcPhy);
		pObj->Move();
	}

	m_pOwner->Repaint(FALSE);
}

void CPageKeyboardManager::OnArrowKeyDown()
{
	CDrawWndObj* pObj = m_pOwner->GetActivedWnd();
	if (pObj==NULL)
		return;

	CRect rcPhy = pObj->GetPhyPos();

	if ((::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
	{
		rcPhy.bottom++;
		pObj->SetPhyPos(rcPhy);
		pObj->Size();
	}
	else
	{
		rcPhy.OffsetRect(0,1);
		pObj->SetPhyPos(rcPhy);
		pObj->Move();
	}

	m_pOwner->Repaint(FALSE);
}

void CPageKeyboardManager::OnArrowKeyLeft()
{
	CDrawWndObj* pObj = m_pOwner->GetActivedWnd();
	if (pObj==NULL)
		return;

	CRect rcPhy = pObj->GetPhyPos();

	if ((::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
	{
		rcPhy.right--;
		pObj->SetPhyPos(rcPhy);
		pObj->Size();
	}
	else
	{
		rcPhy.OffsetRect(-1, 0);
		pObj->SetPhyPos(rcPhy);
		pObj->Move();
	}

	m_pOwner->Repaint(FALSE);
}

void CPageKeyboardManager::OnArrowKeyRight()
{
	CDrawWndObj* pObj = m_pOwner->GetActivedWnd();
	if (pObj==NULL)
		return;

	CRect rcPhy = pObj->GetPhyPos();

	if ((::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
	{
		rcPhy.right++;
		pObj->SetPhyPos(rcPhy);
		pObj->Size();
	}
	else
	{
		rcPhy.OffsetRect(1, 0);
		pObj->SetPhyPos(rcPhy);
		pObj->Move();
	}

	m_pOwner->Repaint(FALSE);
}



BOOL CDrawWndPage::m_sLockAll = FALSE;
BOOL CDrawWndPage::m_sShowGridLines = TRUE;

//////////////////////////////////////////////////////////////////////////
// CDrawWndPage 

//IMPLEMENT_DYNAMIC(CDrawWndPage, CWnd)

CDrawWndPage::CDrawWndPage(void)
{

}

CDrawWndPage::~CDrawWndPage(void)
{
	Clear();
	m_pOwner = NULL;
}

void CDrawWndPage::SetOwner(CWnd *pOwner)
{
	m_pOwner = pOwner;
}

inline void CDrawWndPage::GetViewSite(CRect &rc) const
{
	CRect rcOwner;
	CGridView* pOwner = (CGridView*)m_pOwner;
	if (pOwner->GetSafeHwnd())
	{
		pOwner->GetClientRect(rcOwner);
		rcOwner.OffsetRect(pOwner->m_ptViewOrg);

		rc = rcOwner;
	}
}

BOOL CDrawWndPage::IsObjVisible(const CRect &rcObj) const
{
	CRect rcViewSite;
	GetViewSite(rcViewSite);

	if ( rcObj.left <= rcViewSite.right
		&& rcObj.right > rcViewSite.left
		&& rcObj.top <= rcViewSite.bottom
		&& rcObj.bottom > rcViewSite.top)
	{
		return TRUE;
	}
	else
		return FALSE;

	return FALSE;
}

// 消息处理程序

void CDrawWndPage::Clear()
{
	RemoveAllWnd();
	RemoveAllScreen();
}

void CDrawWndPage::SetPosition(const CRect rcPos)
{
	m_rcPage = rcPos;

	//为窗体设置比例标量
	CDrawWndObj::SetScalar(m_Layout.iPhyHorzPixel, m_Layout.iPhyVertPixel, rcPos.Width(), rcPos.Height());

	//缩放窗体
	for (int i=0; i<m_arWndObj.GetSize(); i++)
	{
		ASSERT(m_arWndObj[i]);
		m_arWndObj[i]->Zoom();
	}

	for (int i=0; i<m_arScreenObj.GetSize(); i++)
	{
		m_arScreenObj[i]->Zoom();
	}
}

void CDrawWndPage::SetLayoutInfo(SPLICELAYOUT layout)
{
	m_Layout = layout;

	CDrawScreenObj::m_iHorz = layout.iLogHorz;
	CDrawScreenObj::m_iVert = layout.iLogVert;
}

void CDrawWndPage::UpdateAligningLine(const int iHorz, const int iVert)
{
	m_Layout.iLogHorz = iHorz;
	m_Layout.iLogVert = iVert;
}

//////////////////////////////////////////////////////////////////////////
//  窗体控制部分
//

inline int CDrawWndPage::GetWndCount() const
{
	return m_arWndObj.GetSize();
}

CDrawWndObj* CDrawWndPage::GetWndObj(int nIndex) const
{
	if (nIndex < 0 || nIndex >= m_arWndObj.GetSize())
		return NULL;

	return m_arWndObj[nIndex];
}

void CDrawWndPage::RemoveWndAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arWndObj.GetSize())
		return;

	// Remove the object from the array.
	CDrawWndObj* pObj = m_arWndObj[nIndex];
	m_arWndObj.RemoveAt(nIndex);

	// Send shut command.
	pObj->Shut(pObj->m_iWndID);

	// Release the object.
	pObj->InternalRelease();
}

void CDrawWndPage::RemoveAllWnd()
{
	if (m_arWndObj.GetSize() == 0)
		return;

	// Release objects.
	for (int i=0; i<m_arWndObj.GetSize(); i++)
	{
		m_arWndObj[i]->InternalRelease();
	}

	// Empty the array.
	m_arWndObj.RemoveAll();
}

void CDrawWndPage::AddWnd(CDrawWndObj* pObj)
{
	m_arWndObj.Add(pObj);
}

void CDrawWndPage::ArrangeAdd(CDrawWndObj* pObj)
{
	if (m_arWndObj.GetSize() == 0)
	{// Add directly when this array was empty.
		m_arWndObj.Add(pObj);
		return;
	}

	if (pObj->GetZorder() > m_arWndObj[m_arWndObj.GetSize()-1]->GetZorder())
	{// Add directly when the new window's z-order is greater than the last window's z-order in the array
		m_arWndObj.Add(pObj);
		return;
	}

	if (pObj->GetZorder() < m_arWndObj[0]->GetZorder())
	{// Insert at the first position of the array when new window's z-order is smallest
		m_arWndObj.InsertAt(0, pObj);
		return;
	}

	// Find a proper position to insert on other conditions
	for (int i=0; i<m_arWndObj.GetSize()-1; i++)
	{
		if (pObj->GetZorder() > m_arWndObj[i]->GetZorder()
			&& pObj->GetZorder() < m_arWndObj[i+1]->GetZorder())
		{
			m_arWndObj.InsertAt(i+1, pObj);
			break;
		}
	}
}

void CDrawWndPage::NewWnd(CPoint point)
{
	// Select a input channel first before create new windows.
	if (CInputManager::m_sCurPort < 0)
	{
		AfxMessageBox(_T("请先在信号源列表中选中一个信号源再试！"));
		return;
	}

	CPoint ptCenter = point;

	CString str;
	str.Format(_T("Source[%d]"), CInputManager::m_sCurPort);

	int iPhyWid = m_Layout.iPhyHorzPixel/m_Layout.iPhyHorz;
	int iPhyHei = m_Layout.iPhyVertPixel/m_Layout.iPhyVert;
	int iLogWid = MulDiv(iPhyWid, m_rcPage.Width(), m_Layout.iPhyHorzPixel);
	int iLogHei = MulDiv(iPhyHei, m_rcPage.Width(), m_Layout.iPhyHorzPixel);

	CRect rcWnd;
	rcWnd.left   = ptCenter.x - Round(iLogWid*0.5);
	rcWnd.right  = rcWnd.left + iLogWid;
	rcWnd.top    = ptCenter.y - Round(iLogHei*0.5);
	rcWnd.bottom = rcWnd.top + iLogHei;

	rcWnd.OffsetRect(-m_rcPage.TopLeft());
	CRect rcPhy;
	rcPhy = rcWnd.MulDiv(m_Layout.iPhyHorzPixel, m_rcPage.Width());
	rcPhy.right  = rcPhy.left + iPhyWid;
	rcPhy.bottom = rcPhy.top + iPhyHei;

	CDrawWndObj* pNewWnd = new CDrawWndObj(this);

	pNewWnd->SetWndID(PrepareWndID());
	pNewWnd->SetSourcePort(CInputManager::m_sCurPort);
	pNewWnd->SetZorder(GetWndCount()+1);
	pNewWnd->SetPhyPos(rcPhy);
	pNewWnd->SetName(str.GetBuffer());
	pNewWnd->SetBkColor(GetColor(CInputManager::m_sCurPort));
	pNewWnd->Active(FALSE);
	pNewWnd->SetLock(m_sLockAll);

	m_arWndObj.Add(pNewWnd);

	ActiveWnd(pNewWnd);

	pNewWnd->Open();
}

CDrawWndObj* CDrawWndPage::DynamicBuilt(const CPoint ptStart, const CPoint ptEnd)
{
	// Window is too small to create.
	if (ptEnd.x-ptStart.x < 20 || ptEnd.y-ptStart.y < 20)
		return NULL;

	// Select a input channel first before builting new windows.
	if (CInputManager::m_sCurPort < 0)
		return NULL;

	CRect rcWnd(ptStart, ptEnd);

	CString str;
	str.Format(_T("Source[%d]"), CInputManager::m_sCurPort);

	CDrawWndObj* pNewWnd = new CDrawWndObj(this);

	pNewWnd->SetWndID(PrepareWndID());
	pNewWnd->SetSourcePort(CInputManager::m_sCurPort);
	pNewWnd->SetZorder(GetWndCount()+1);
	pNewWnd->SetLogPos(rcWnd);
	pNewWnd->SetName(str.GetBuffer());
	pNewWnd->SetBkColor(GetColor(CInputManager::m_sCurPort));
	pNewWnd->Active(FALSE);
	pNewWnd->SetLock(m_sLockAll);

	m_arWndObj.Add(pNewWnd);

	ActiveWnd(pNewWnd);

	return pNewWnd;
}

void CDrawWndPage::ParseSceneBlocks(__in UINT* pBlocks, __in int nLen)
{
	long pos = 0;
	CRect rcPhy;
	CString strName;

	// Empty the array.
	RemoveAllWnd();
	CDrawWndObj* pNewObj = NULL;

	// Read objects from the buffer.
	while (1)
	{
		pNewObj = new CDrawWndObj(this);

		if (pNewObj->Read(pBlocks+pos, nLen, pos) < 0)
		{
			SAFE_DELETE(pNewObj);
			break;
		}

		ArrangeAdd(pNewObj);
	}

	if (m_arWndObj.GetSize() > 0)
		ActiveWnd(m_arWndObj[m_arWndObj.GetSize()-1]);
}

void CDrawWndPage::PackSceneBlocks(__out UINT* pBlocks, __out int nLen)
{
	long pos = 0;
	for (int i=0; i<m_arWndObj.GetSize(); i++)
	{
		if (m_arWndObj[i]->Write(pBlocks+pos, nLen, pos) < 0)
		{
			break;
		}
	}
}

void CDrawWndPage::PackFile(__in LPCTSTR lpFile)
{

	//****************** File operations
	CFile save;
	if (!save.Open(lpFile, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))
	{
		AfxMessageBox(_T("Create file error!"), MB_OK|MB_ICONSTOP);
		return;
	}

	long lSize = m_arWndObj.GetSize()*CDrawWndObj::GetObjectSize() + sizeof(char);
	CMemoryAllocate<char> alloc(lSize+1);

	//****************** Fill Flag
	const char HeadFlag[] = "00FFFFFFFF00";
	save.Write(HeadFlag, strlen(HeadFlag));

	//****************** Write objects
	long pos = 1; //First char stores BCC code.
	for (int i=0; i<m_arWndObj.GetSize(); i++)
	{
		if (m_arWndObj[i]->Write(alloc.GetAddress()+pos, lSize, pos) < 0)
			break;
	}

	// Encrypt buffer
	CBaseFileManager::Encrypt(alloc.GetAddress()+1, alloc.GetAddress()+1, pos-1);

	// Add BCC code
	CBaseFileManager::SetBCCvalue(alloc.GetAddress()+1, alloc.GetAddress()[0], pos-1);

	//****************** File close
	save.Write(alloc.GetAddress(), pos);
	save.Close();

	//****************** Set Default config path
	CSysConfManager sysconf;
	sysconf.SetDefaultSceneFilePath(lpFile);

	CString strMsg;
	strMsg.Format(_T("File saved: [%s]"), lpFile);
	AfxMessageBox(strMsg, MB_OK|MB_ICONINFORMATION);

}

void CDrawWndPage::ParseFile(__in LPCTSTR lpFile)
{

	//****************** File operations
	CFile open;
	if (!open.Open(lpFile, CFile::modeRead|CFile::typeBinary))
	{
		AfxMessageBox(_T("Open file error!"), MB_OK|MB_ICONSTOP);
		return;
	}

	long lSize = (long)open.GetLength();
	CMemoryAllocate<char> alloc(lSize+1);
	open.Read(alloc.GetAddress(), lSize);
	open.Close();

	//******************* File flag check
	const char HeadFlag[] = "00FFFFFFFF00";
	const int FlagLen = strlen(HeadFlag);
	if (memcmp(alloc.GetAddress(), HeadFlag, FlagLen) != 0)
	{
		AfxMessageBox(_T("Illegal input file!"), MB_OK|MB_ICONSTOP);
		return;
	}

	//******************* BCC check
	char bcc = alloc.GetAddress()[FlagLen]; // BCC code
	long pos = FlagLen+1; // useful data address.
	BOOL bValid = CBaseFileManager::BlockCheckCharaters(alloc.GetAddress()+pos, bcc, lSize-pos);
	if (!bValid)
	{
		AfxMessageBox(_T("Block check charaters failed!"), MB_OK|MB_ICONSTOP);
		return;
	}

	//******************* Read object from file
	// Decrypt buffer.
	CBaseFileManager::Decrypt(alloc.GetAddress()+pos, alloc.GetAddress()+pos, lSize-pos);

	//******************* Clear array
	RemoveAllWnd();
	CDrawWndObj::Shut(0);

	//******************* Read object
	CDrawWndObj* pNewObj = NULL;
	while (1)
	{
		pNewObj = new CDrawWndObj(this);
		if (pNewObj->Read(alloc.GetAddress()+pos, lSize, pos) < 0)
		{
			SAFE_DELETE(pNewObj);
			break;
		}

		pNewObj->Open();
		ArrangeAdd(pNewObj);
	}

	if (m_arWndObj.GetSize() > 0)
	{
		ActiveWnd(m_arWndObj[m_arWndObj.GetSize()-1]);
		Repaint(FALSE);
	}

	AfxMessageBox(_T("Scene file loaded!"), MB_OK|MB_ICONINFORMATION);

}

int CDrawWndPage::PrepareWndID() const
{
	int nPrepareID = -1;
	for (int i=1; i<= m_arWndObj.GetSize(); i++)
	{
		nPrepareID = i;
		for (int j=0; j<m_arWndObj.GetSize(); j++)
		{
			if (m_arWndObj[j]->GetWndID() == i)
			{
				nPrepareID = -1;
				break;
			}
		}

		if (nPrepareID != -1)
			break;
	}

	if (nPrepareID == -1)
		nPrepareID = (m_arWndObj.GetSize() > 0) ? m_arWndObj.GetSize()+1 : 1;

	return nPrepareID;
}

void CDrawWndPage::ActiveWnd(CDrawWndObj* pObj)
{
	if (pObj == NULL)
		return;

	if (pObj->IsActived())
		return;

	for (int i=0; i<m_arWndObj.GetSize(); i++)
	{
		if (m_arWndObj[i]->IsActived())
		{
			m_arWndObj[i]->Active(FALSE);
			break;
		}
	}

	pObj->Active(TRUE);
	Repaint(FALSE);
}

CDrawWndObj* CDrawWndPage::GetActivedWnd() const
{
	for (int i=0; i<m_arWndObj.GetSize(); i++)
	{
		if (m_arWndObj[i]->IsActived())
			return m_arWndObj[i];
	}

	return NULL;
}

void CDrawWndPage::OnSourceChanged(const int &iInputCh)
{
	if (iInputCh < 0)
		return;

	CDrawWndObj* pActiveWnd = GetActivedWnd();
	if (pActiveWnd == NULL)
		return;

	//pActiveWnd->SetBkColor(GetColor(iInputCh));
	pActiveWnd->SetSourcePort(iInputCh);
	pActiveWnd->MoveS();

	Repaint( FALSE );
}

// rand window color
inline COLORREF CDrawWndPage::GetColor(const int iSrcPort) const
{
	//for (int i=0; i<m_arWndObj.GetSize(); i++)
	//{
	//	if (m_arWndObj[i]->GetSourcePort() == iSrcPort)
	//	{
	//		return m_arWndObj[i]->m_clrBk;
	//	}
	//}

	return RGB(rand()%255, rand()%255, rand()%255);

	return RGB(((rand()+10)%255), ((rand()+20)%255), ((rand()+30)%255));
}

void CDrawWndPage::LockAll()
{
	m_sLockAll = !m_sLockAll;

	for (int i=0; i<m_arWndObj.GetSize(); i++)
	{
		m_arWndObj[i]->SetLock(m_sLockAll);
	}
}

void CDrawWndPage::MovetoTop(CDrawWndObj* pObj)
{
	if (pObj == NULL)
		return;

	int nSize = m_arWndObj.GetSize();

	// Less than 2 windows
	if (nSize < 2)
		return;

	// This window is already topmost
	if (m_arWndObj[nSize-1] == pObj)
		return;	

	for (int i=0; i<nSize; i++)
	{
		if (m_arWndObj[i] == pObj)
		{
			m_arWndObj.RemoveAt(i);
			m_arWndObj.Add(pObj);

			for (int z=i; z<nSize; z++) {
				m_arWndObj[z]->SetZorder(z+1);
			}

			pObj->MoveZ();

			Repaint(FALSE);

			break;
		}
	}
}


void CDrawWndPage::MovetoBottom(CDrawWndObj* pObj)
{
	if (pObj == NULL)
		return;

	int nSize = m_arWndObj.GetSize();

	// Less than 2 windows
	if (nSize < 2)
		return;

	// This window is already bottommost
	if (m_arWndObj[0] == pObj)
		return;

	for (int i=0; i<nSize; i++)
	{
		if (m_arWndObj[i] == pObj)
		{
			m_arWndObj.RemoveAt(i);
			m_arWndObj.InsertAt(0, pObj);

			UpdateZorder();

			pObj->MoveZ();

			Repaint(FALSE);

			break;
		}
	}
}

void CDrawWndPage::MoveForward(CDrawWndObj* pObj)
{
	if (pObj == NULL)
		return;

	int nSize = m_arWndObj.GetSize();

	// Less than 2 windows
	if (nSize < 2)
		return;

	// Already Topmost
	if (m_arWndObj[nSize-1] == pObj)
		return;

	for (int i=0; i<nSize-1; i++)
	{
		if (m_arWndObj[i] == pObj)
		{
			m_arWndObj[i+1]->SetZorder(i+1);

			m_arWndObj.RemoveAt(i);
			m_arWndObj.InsertAt(i+1, pObj);

			pObj->SetZorder(i+2);

			pObj->MoveZ();

			Repaint(FALSE);

			break;
		}
	}
}

void CDrawWndPage::MoveBackward(CDrawWndObj* pObj)
{
	if (pObj == NULL)
		return;

	int nSize = m_arWndObj.GetSize();

	// Less than 2 windows
	if (nSize < 2)
		return;

	// Already bottommost
	if (m_arWndObj[0] == pObj)
		return;

	for (int i=1; i<nSize; i++)
	{
		if (m_arWndObj[i] == pObj)
		{
			m_arWndObj[i-1]->SetZorder(i+1);

			m_arWndObj.RemoveAt(i);
			m_arWndObj.InsertAt(i-1, pObj);

			pObj->SetZorder(i);

			pObj->MoveZ();

			Repaint(FALSE);

			break;
		}
	}
}

void CDrawWndPage::RemoveWnd(CDrawWndObj* pObj)
{
	for (int i=0; i<m_arWndObj.GetSize(); i++)
	{
		if (m_arWndObj[i] == pObj)
		{
			m_arWndObj.RemoveAt(i);

			if (pObj->IsActived())
			{
				if (m_arWndObj.GetSize() > 0)
					m_arWndObj[m_arWndObj.GetSize()-1]->Active(TRUE);
			}

			pObj->InternalRelease();
			UpdateZorder();

			break;
		}
	}
}


void CDrawWndPage::MaximizeWnd(CDrawWndObj* pObj)
{
	pObj->SetPhyPos(CRect(0, 0, m_Layout.iPhyHorzPixel, m_Layout.iPhyVertPixel));
	Repaint(FALSE);
}

void CDrawWndPage::AlignWnd(CDrawWndObj* pObj)
{
	CResizeAnchor anchor(m_Layout);
	pObj->SetPhyPos(anchor.GetResizeRect(pObj->m_rcPhy));
	Repaint(FALSE);
}

void CDrawWndPage::RestoreWnd(CDrawWndObj* pObj)
{
	pObj->SetPhyPos(pObj->m_rcLastPhy);
	Repaint(FALSE);
}

BOOL CDrawWndPage::IsMaximized(CDrawWndObj* pObj)
{
	return pObj->m_rcPhy == CRect(0, 0, m_Layout.iPhyHorzPixel, m_Layout.iPhyVertPixel);
}

BOOL CDrawWndPage::IsAligned(CDrawWndObj* pObj)
{
	CResizeAnchor anchor(m_Layout);
	return pObj->m_rcPhy == anchor.GetResizeRect(pObj->m_rcPhy);
}

void CDrawWndPage::UpdateZorder()
{
	for (int i=0; i<m_arWndObj.GetSize(); i++)
		m_arWndObj[i]->SetZorder(i+1);
}

void CDrawWndPage::FillWndtoAnchoredScreen(CDrawWndObj* pObj)
{
	ASSERT(pObj);

	for (int i=m_arScreenObj.GetSize()-1; i >= 0; i--)
	{
		if (m_arScreenObj[i]->GetPhyCoords().Contains(pObj->GetPhyPos().left, pObj->GetPhyPos().top))
		{
			pObj->SetPhyPos(m_arScreenObj[i]->GetPhyCoords());
			pObj->Size();
			Repaint(FALSE);
			break;
		}
	}
}

void CDrawWndPage::FillWndtoAcrossedScreen(CDrawWndObj* pOjb)
{
	//ASSERT(pOjb);

	//for (int i=m_arScreenObj.GetSize()-1; i >= 0; i--)
	//{

	//}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  屏幕控制部分
//

int CDrawWndPage::GetScreenCount() const
{
	return m_arScreenObj.GetSize();
}

CDrawScreenObj* CDrawWndPage::GetScreenObj( int nIndex ) const
{
	if (nIndex < 0 || nIndex >= m_arScreenObj.GetSize())
		return NULL;

	return m_arScreenObj[nIndex];
}


void CDrawWndPage::RemoveScreen(CDrawScreenObj* pObj)
{
	for (int i=0; i<m_arScreenObj.GetSize(); i++)
	{
		if (m_arScreenObj[i] == pObj)
		{
			m_arScreenObj.RemoveAt(i);
			pObj->InternalRelease();
			break;
		}
	}
}

void CDrawWndPage::RemoveScreenAt(int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arScreenObj.GetSize())
		return;

	CDrawScreenObj *pObj = m_arScreenObj[nIndex];

	m_arScreenObj.RemoveAt(nIndex);

	pObj->InternalRelease();
}

void CDrawWndPage::RemoveAllScreen()
{
	if (m_arScreenObj.GetSize() == 0)
		return;

	for (int i=0; i<m_arScreenObj.GetSize(); i++)
	{
		m_arScreenObj[i]->InternalRelease();
	}

	m_arScreenObj.RemoveAll();
}

void CDrawWndPage::AddScreen(CDrawScreenObj* pObj)
{
	ASSERT(pObj);

	m_arScreenObj.Add(pObj);
}

void CDrawWndPage::ArrangeAdd(CDrawScreenObj* pObj)
{
	ASSERT(pObj);

	// 空数组
	if (m_arScreenObj.GetSize() == 0)
		m_arScreenObj.Add(pObj);

	// 待添加ID比数组最后一个项的ID大
	if (pObj->GetID() > m_arScreenObj[m_arScreenObj.GetSize()-1]->GetID())
		m_arScreenObj.Add(pObj);

	// 其它情况
	for (int i=0; i<m_arScreenObj.GetSize()-1; i++)
	{
		if (pObj->GetID() > m_arScreenObj[i]->GetID()
			&& pObj->GetID() < m_arScreenObj[i+1]->GetID())
		{
			m_arScreenObj.InsertAt(i+1, pObj);
			break;
		}
	}

}

int CDrawWndPage::PrepareScreenID() const
{
	int nPrepareID = -1;
	int nMaxCount = m_Layout.iPhyHorz*m_Layout.iPhyVert;

	for (int i=1; i<= m_arScreenObj.GetSize(); i++)
	{
		nPrepareID = i;
		for (int j=0; j< m_arScreenObj.GetSize(); j++)
		{
			if (m_arScreenObj[j]->GetID() == i)
			{
				nPrepareID = -1;
				break;
			}
		}

		if (nPrepareID != -1)
			break;
	}

	if (nPrepareID == -1 && m_arScreenObj.GetSize() < nMaxCount)
	{
		nPrepareID = m_arScreenObj.GetSize()>0 ? m_arScreenObj.GetSize()+1 : 1;
	}

	return nPrepareID;
}

void CDrawWndPage::ResetScreens()
{
	//清空屏幕数组
	RemoveAllScreen();

	//重新分配屏幕
	int iHorz = m_Layout.iPhyHorz;
	int iVert = m_Layout.iPhyVert;
	int nID;
	RectF rc(0, 0, 0, 0);
	CString str;

	CDrawScreenObj* pNewScreen = NULL;

	for (int v=0; v<iVert; v++)
		for (int h=0; h<iHorz; h++)		
		{
			nID = v*iHorz+h;
			str.Format(_T("屏幕 %d"), nID+1);

			pNewScreen = new CDrawScreenObj(this);
			pNewScreen->SetID(PrepareScreenID());
			pNewScreen->SetName(str.GetBuffer());
			pNewScreen->SetOutPort(GetOutputManager()->GetCmdPort(nID, FALSE));
			pNewScreen->SetLogCoords(rc);
			pNewScreen->SetFocus(FALSE);

			m_arScreenObj.Add(pNewScreen);
		}
}

void CDrawWndPage::ResetScreens(pSCREENMAP pScrMap, const int iCount, BOOL bReverse)
{
	// 清空屏幕数组
	RemoveAllScreen();

	// 解析屏幕数据
	RectF rcLog;
	Rect  rcPhy;

	CRect rcL, rcP;
	CString str;

	CDrawScreenObj* pNewScreen = NULL;
	for (int i=0; i<iCount; i++)
	{
		str.Format(_T("屏幕 %d"), pScrMap[i].iScreenID);

		pNewScreen = new CDrawScreenObj(this);

		pNewScreen->SetID(pScrMap[i].iScreenID);

		pNewScreen->SetName(str.GetBuffer());

		if (GetOutputManager()->FindCmdPort(pScrMap[i].iOutputCh) >= 0)
			pNewScreen->SetOutPort(pScrMap[i].iOutputCh);
		else
			pNewScreen->SetOutPort(INVALID_VALUE);
	
		rcP = pScrMap[i].rcScreenPos;
		rcPhy = Rect(rcP.left, rcP.top, rcP.Width(), rcP.Height());
		pNewScreen->SetPhyCoords(rcPhy);

		rcL = rcP.MulDiv(m_rcPage.Width(), m_Layout.iPhyHorzPixel);
		rcL.OffsetRect(m_rcPage.TopLeft());
		rcLog = RectF(REAL(rcL.left), REAL(rcL.top), REAL(rcL.Width()), REAL(rcL.Height()));

		pNewScreen->SetLogCoords(rcLog);
		pNewScreen->SetFocus(FALSE);

		if (!bReverse)
			pNewScreen->OnOutputChange();

		ArrangeAdd(pNewScreen);
	}
}

void CDrawWndPage::InitScreenPos()
{
	int iPhyWid = m_Layout.BkgTiming.m_HActive;
	int iPhyHei = m_Layout.BkgTiming.m_VActive;
	double dxScale = m_rcPage.Width()*1.0/m_Layout.iPhyHorzPixel;
	double dyScale = m_rcPage.Height()*1.0/m_Layout.iPhyVertPixel;

	double iLogWid = iPhyWid*dxScale;
	double iLogHei = iPhyHei*dyScale;

	int   nIndex;
	RectF rcLog;
	Rect  rcPhy;

	ASSERT(m_arScreenObj.GetSize() == m_Layout.iPhyVert*m_Layout.iPhyHorz);

	for (int v=0; v<m_Layout.iPhyVert; v++)
		for (int h=0; h<m_Layout.iPhyHorz; h++)
		{
			nIndex = v*m_Layout.iPhyHorz + h;

			rcLog.X      = REAL(m_rcPage.left + h*iLogWid);
			rcLog.Width  = REAL(iLogWid);
			rcLog.Y      = REAL(m_rcPage.top + v*iLogHei);
			rcLog.Height = REAL(iLogHei);

			rcPhy.X      = iPhyWid*h;
			rcPhy.Width  = iPhyWid;
			rcPhy.Y      = iPhyHei*v;
			rcPhy.Height = iPhyHei;

			m_arScreenObj[nIndex]->SetLogCoords(rcLog);
			m_arScreenObj[nIndex]->SetPhyCoords(rcPhy);

			m_arScreenObj[nIndex]->OnOutputChange();
		}
}

void CDrawWndPage::FocusScreen(CDrawScreenObj* pObj)
{
	if (pObj->IsFocused())
	{
		pObj->SetFocus(FALSE);
		Repaint(TRUE);
		return;
	}

	for (int i=0; i<m_arScreenObj.GetSize(); i++) {
		if (m_arScreenObj[i]->IsFocused())
		{
			m_arScreenObj[i]->SetFocus(FALSE);
			break;
		}
	}

	pObj->SetFocus(TRUE);
	Repaint(TRUE);
}

CDrawScreenObj* CDrawWndPage::GetFocusScreen() const
{
	for (int i=0; i<m_arScreenObj.GetSize(); i++)
	{
		if (m_arScreenObj[i]->IsFocused())
			return m_arScreenObj[i];
	}

	return NULL;
}

void CDrawWndPage::OnOutPortChanged(const SCREENMAP &scrmap)
{
	if (scrmap.iOutputCh < 0)
		return;

	//得到当前选中的屏幕
	CDrawScreenObj* pFocusedScn = NULL;

	if (scrmap.iScreenID < 0)
		pFocusedScn = GetFocusScreen();
	else
		pFocusedScn = FindByID(scrmap.iScreenID-1);

	if (pFocusedScn == NULL)
		return;

	//输出端口没有变化
	//if (pFocusedScn->GetOutPort() == scrmap.iOutputCh)
	//	return;

	//如果有其它屏幕也是由当前选中的输出端口输出，关闭它
	CDrawScreenObj* pClosedScreen = GetScreenByOutPort(scrmap.iOutputCh);
	if (pClosedScreen != NULL)
	{
		pClosedScreen->SetOutPort(INVALID_VALUE);
	}

	//设置选中屏由该输出端口输出
	pFocusedScn->SetOutPort(scrmap.iOutputCh);

	if (scrmap.iScreenID < 0)
		pFocusedScn->OnOutputChange();

	//重绘视图
	Repaint(TRUE);
}


inline CDrawScreenObj* CDrawWndPage::GetScreenByOutPort(const int iOutPort) const
{
	for (int i=0; i<m_arScreenObj.GetSize(); i++)
	{
		if (m_arScreenObj[i]->m_iOutPort == iOutPort)
		{
			return m_arScreenObj[i];
		}
	}

	return NULL;
}


CDrawScreenObj* CDrawWndPage::FindByID(const int nScreenID) const
{
	for (int i=0; i<m_arScreenObj.GetSize(); i++)
	{
		if (m_arScreenObj[i]->m_iScreenID == nScreenID)
		{
			return m_arScreenObj[i];
		}
	}

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
//  绘制
//

void CDrawWndPage::DrawGrid(Graphics *pGraphics) const
{
	if (!m_sShowGridLines)
		return;

	Pen blackPen(Color(255, 0, 0, 0), 1);
	pGraphics->DrawRectangle(&blackPen, m_rcPage.left, m_rcPage.top, m_rcPage.Width(), m_rcPage.Height());


	Pen SolidPen(Color(255, 10, 28, 155), 1);
	Pen DotPen(Color(255, 50, 28, 55), 1);
	DotPen.SetDashStyle(DashStyleDot);

	// 横线
	int iVert = m_Layout.iLogVert;
	double dVSpa = m_Layout.iPhyVertPixel*1.0/iVert;
	double xScale = m_rcPage.Width()*1.0/m_Layout.iPhyHorzPixel;
	double yScale = m_rcPage.Height()*1.0/m_Layout.iPhyVertPixel;

	for (int i=0; i<= iVert; i++) {
		pGraphics->DrawLine(&DotPen, PointF(REAL(m_rcPage.left), REAL(m_rcPage.top + i*dVSpa*yScale)), PointF(REAL(m_rcPage.right), REAL(m_rcPage.top + i*dVSpa*yScale)));
	}

	// 竖线
	int iHorz = m_Layout.iLogHorz;
	double dHSpa = m_Layout.iPhyHorzPixel*1.0/iHorz;
	for (int i=0; i<=iHorz; i++) {
		pGraphics->DrawLine(&DotPen, PointF(REAL(m_rcPage.left + i*dHSpa*xScale), REAL(m_rcPage.top)), PointF(REAL(m_rcPage.left + i*dHSpa*xScale), REAL(m_rcPage.bottom)));
	}
}


void CDrawWndPage::DrawContent(CDC *pParentDC) const
{
	Graphics gp(pParentDC->m_hDC);

	for (int i=0; i<m_arScreenObj.GetSize(); i++) {
		m_arScreenObj[i]->DrawScreenBk(&gp);
		m_arScreenObj[i]->DrawGridLine(&gp);
	}

	DrawGrid(&gp);

	for (int i=0; i<m_arWndObj.GetSize(); i++)
	{
		m_arWndObj[i]->DrawMethod2(&gp);
	}
}

void CDrawWndPage::Repaint(BOOL bDeCreate) const
{
	if (bDeCreate)
		((CGridView*)m_pOwner)->RedrawGraphic();
	else
		m_pOwner->Invalidate();
}

CRect CDrawWndPage::GetBkgSizeAccordingtoScrInf(const pSCREENMAP pscrInf, const int iScrNum, BOOL bInflate)
{
	CRect rcBkg(0,0,0,0);
	if (!pscrInf)
		return rcBkg;

	for (int i=0; i<iScrNum; i++)
	{
		if (bInflate)
			pscrInf[i].rcScreenPos.InflateRect(0, 0, 1, 1);

		pscrInf[i].rcScreenPos.NormalizeRect();

		// Most left
		//if (pscrInf[i].rcScreenPos.left <= rcBkg.left)
		//	rcBkg.left = pscrInf[i].rcScreenPos.left;

		// Most top
		//if (pscrInf[i].rcScreenPos.top <= rcBkg.top)
		//	rcBkg.top = pscrInf[i].rcScreenPos.top;

		// Most right
		if (pscrInf[i].rcScreenPos.right > rcBkg.right)
			rcBkg.right = pscrInf[i].rcScreenPos.right;

		// Most bottom
		if (pscrInf[i].rcScreenPos.bottom > rcBkg.bottom)
			rcBkg.bottom = pscrInf[i].rcScreenPos.bottom;
	}

	return rcBkg;
}