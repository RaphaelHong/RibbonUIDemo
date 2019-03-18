// SwitchSourceBar.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "SwitchSourceBar.h"
#include "SwitchPage.h"
#include "Dialog components/SourcePropertyDlg.h"

IMPLEMENT_DYNAMIC(CDrawSourceItem, CCmdTarget)

//////////////////////////////////////////////////////////////////////////
CDrawSourceItem::CDrawSourceItem()
{
	m_pOwner = NULL;
	m_bActive = FALSE;
}

CDrawSourceItem::CDrawSourceItem(CWnd* pOwner)
{
	m_pOwner = pOwner;
	m_bActive = FALSE;
}

CDrawSourceItem::~CDrawSourceItem()
{
	
}

BEGIN_MESSAGE_MAP(CDrawSourceItem, CCmdTarget)
END_MESSAGE_MAP()

void CDrawSourceItem::SetItemIndex(const int iIndex)
{
	m_nSourceIndex = iIndex;
}

void CDrawSourceItem::SetItemRect(const Rect rect)
{
	m_rcItem = rect;
}

void CDrawSourceItem::Active(BOOL bActive)
{
	m_bActive = bActive;
}

Rect CDrawSourceItem::GetItemRect() const
{
	return m_rcItem;
}

CRect CDrawSourceItem::GetItemRect()
{
	CRect rc(m_rcItem.GetLeft(), m_rcItem.GetTop(), m_rcItem.GetRight(), m_rcItem.GetBottom());
	
	return rc;
}

void CDrawSourceItem::Draw(Graphics* pGraphics)
{
	ASSERT(pGraphics);

	// 文字绘制准备
	FontFamily fontFamily(L"宋体");
	Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
	Gdiplus::Font Bfont(&fontFamily, 12, FontStyleBold, UnitPixel);
	SolidBrush blackBr(Color(255, 55, 30, 71));
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingEllipsisWord);	
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	RectF rcText;

	// 开始绘制
	//************************************

	int nID = GetInputManager()->GetID(m_nSourceIndex);
	int nSubID = GetInputManager()->GetSubID(nID);

	Rect rcImg;
	emVideoType evt = GetInputManager()->GetSignalType(nID);

	// 项背景
	Image* imgBkg;
	UINT nPNG = 0;
	if (m_bActive)
		nPNG = IDB_PNG_MARKERBAR_FOCUS;
	else
	{
		if (evt == vtUndef)
			nPNG = IDB_PNG_MARKBAR_DIS;
		else
			nPNG = IDB_PNG_MAKERBAR;
	}

	if (ImageFromIDResource(nPNG, "PNG", imgBkg))
	{
		rcImg.X = m_rcItem.GetLeft();
		rcImg.Y = m_rcItem.GetTop()+1;//(m_rcItem.Height- imgBkg->GetHeight())/2+m_rcItem.GetTop();
		rcImg.Width = m_rcItem.Width;
		rcImg.Height = m_rcItem.Height-2;//imgBkg->GetHeight();

		pGraphics->DrawImage(imgBkg, rcImg);
		SAFE_DELETE(imgBkg);
	}

	// 信号属性图标
	Image* imgSigIcon;	
	//UINT uPNG;
	//if (evt == vtUndef)      uPNG = IDB_PNG_SIGNAL_OFF;
	//else if (evt == vtCVBS)  uPNG = IDB_PNG_SIGMARK_CVBS;
	//else if (evt == vtYpbpr) uPNG = IDB_PNG_SIGMARK_YPBPR;
	//else if (evt == vtVGA)   uPNG = IDB_PNG_SIGMARK_VGA;
	//else if (evt == vtHDMI)  uPNG = IDB_PNG_SIGMARK_HDMI;
	//else if (evt == vtSDI)   uPNG = IDB_PNG_SIGMARK_SDI;
	//else if (evt == vtDVI)   uPNG = IDB_PNG_SIGMARK_DVI;
	//else if (evt == vtDLDVI) uPNG = IDB_PNG_SIGMARK_DLDVI;
	if (nSubID != 2)
	{
		if (ImageFromIDResource(evt==vtUndef ? IDB_PNG_SIGNAL_OFF : IDB_PNG_SIGNAL_ON, "PNG", imgSigIcon))
			//if (ImageFromIDResource(uPNG, "png", imgSigIcon))
		{
			rcImg.X = m_rcItem.GetRight() - imgSigIcon->GetWidth();
			rcImg.Y = (m_rcItem.Height-imgSigIcon->GetHeight())/2 + m_rcItem.GetTop();
			rcImg.Width = imgSigIcon->GetWidth();
			rcImg.Height= imgSigIcon->GetHeight();

			pGraphics->DrawImage(imgSigIcon, rcImg);
			SAFE_DELETE(imgSigIcon);

			if (evt!=vtUndef)
			{
				rcText = RectF((REAL)rcImg.GetLeft(), (REAL)rcImg.GetTop(), (REAL)rcImg.Width, (REAL)rcImg.Height);
				COLORREF clr = GetVideoColor(evt);
				blackBr.SetColor(Color(255, GetRValue(clr), GetGValue(clr), GetBValue(clr)));
				//blackBr.SetColor(Color(255, 48, 51, 21));
				pGraphics->DrawString(GetVideoName(evt), -1, m_bActive ? &Bfont : &font, rcText, &format, &blackBr);
			}
		}
	}


	// 信号源名
	rcText.X = REAL(m_rcItem.X + 28 + 2);
	rcText.Y = REAL(m_rcItem.GetTop());
	rcText.Width = REAL(m_rcItem.Width - 30 - 48);
	rcText.Height= REAL(m_rcItem.Height);

	format.SetAlignment(StringAlignmentNear);

	if (evt == vtUndef)
		blackBr.SetColor(Color(255, 135, 147, 134));
	else
		blackBr.SetColor(Color(255, 35, 67, 134));

	CString strChlName, strSubName;
	WCHAR wszText[_MAX_SIGNAL_NAME];

	
	if (nSubID == 0)
		strChlName.Format(_T("%-2d.%s"), GetInputManager()->GetRealIndex(nID)+1, GetInputManager()->GetName(nID));
	else if (nSubID == 1)
		strChlName.Format(_T("%-2d.%s #1"), GetInputManager()->GetRealIndex(nID)+1, GetInputManager()->GetName(nID));
	else if (nSubID == 2)
		strChlName.Format(_T("   %s #2"), GetInputManager()->GetName(nID));
	else
		strChlName.Format(_T("Error"));
	//strChlName.Format(_T("%d. %s"), m_nSourceIndex+1, GetInputManager()->GetName(m_nSourceIndex, FALSE));
	//if (GetInputManager()->GetSubID(m_nSourceIndex, FALSE) != 0)
	//{
	//	strSubName.Format(_T(" #%d"), GetInputManager()->GetSubID(m_nSourceIndex, FALSE));
	//	strChlName += strSubName;
	//}

	MultiByteToWideChar(CP_ACP, 0, strChlName.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
	pGraphics->DrawString(wszText, -1, m_bActive? &Bfont : &font, rcText, &format, &blackBr);
}

//////////////////////////////////////////////////////////////////////////

int CSwitchSourceBar::m_sFirstVisiableItem = 0;
// CSwitchSourceBar

IMPLEMENT_DYNAMIC(CSwitchSourceBar, CWnd)

CSwitchSourceBar::CSwitchSourceBar()
{
	m_FocusItem = -1;

	m_nItemOnEdit = -1;
}

CSwitchSourceBar::~CSwitchSourceBar()
{
	RemoveAll();
}


// CSwitchSourceBar 成员函数
BEGIN_MESSAGE_MAP(CSwitchSourceBar, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_VSCROLL()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEWHEEL()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(WM_USER_CANCEL_EDIT, &CSwitchSourceBar::OnCancelEdit)
	ON_MESSAGE(WM_USER_END_EDIT, &CSwitchSourceBar::OnEndEdit)
	ON_MESSAGE(ID_BNITEM_FRESH, &CSwitchSourceBar::OnRefreshThis)
END_MESSAGE_MAP()

void CSwitchSourceBar::SetBarRect(const CRect rc)
{
	m_rcBar = rc;
}

BOOL CSwitchSourceBar::BarCreate(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const CRect& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /* = NULL */ )
{
	if (CreateEx(0,
		AfxRegisterWndClass(0, LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))),
		0,
		dwStyle, 
		rect,
		pParentWnd,
		nID ) )
	{
		CFont font;
		VERIFY(font.CreateFont(
			12,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                 // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("Arial")));                 // lpszFacename

		SetFont(&font);

		SetVScrollBar();

		SetBnItem();

		ResetSourceItem();

		SetTooltips();

		return TRUE;
	}

	return FALSE;
}

int CSwitchSourceBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont font;
	VERIFY(font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));             // lpszFacename

	SetFont(&font);

	SetVScrollBar();

	SetBnItem();

	ResetSourceItem();

	SetTooltips();

	m_Edit.Create(WS_CHILD|WS_BORDER|ES_LEFT, CRect(0, 0, 0, 0), this, 1001);
	//m_Edit.SetFont(&font);
	m_Edit.ShowWindow(SW_HIDE);

	return 0;
}

void CSwitchSourceBar::SetTooltips()
{
	m_tips.Create(this);
	m_tips.SetDelayTime(200);
	if (m_bnFresh.GetSafeHwnd())
		m_tips.AddTool(&m_bnFresh, _T("点击刷新此信号"));
}

void CSwitchSourceBar::SetVScrollBar()
{
	CRect rcVBar = m_rcBar;
	rcVBar.OffsetRect(-rcVBar.TopLeft());
	rcVBar.left = 0;
	rcVBar.right = 19;
	//rcVBar.InflateRect(0, -1);

	if (m_Vertbar.GetSafeHwnd())
		m_Vertbar.MoveWindow(rcVBar);
	else
		m_Vertbar.Create(SB_VERT|SBS_LEFTALIGN|WS_CHILD|WS_VISIBLE|WS_BORDER, rcVBar, this, 1125);

	CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

	int iMin = 0;
	int iMax = GetInputManager()->GetCount() - pMargins->SourceBarMargin.iMaxVisiableItemsOnVert;
	iMax = max (0, iMax);

	m_Vertbar.SetScrollRange(iMin, iMax);
	m_Vertbar.SetScrollPos(m_sFirstVisiableItem);
	//m_Vertbar.EnableWindow(iMax != 0);
	m_Vertbar.ShowWindow((iMax != 0) ? SW_SHOW : SW_HIDE);
}

void CSwitchSourceBar::SetBnItem()
{
	m_bnFresh.SetItemRect(0, 0, 18, 18);
	m_bnFresh.SetImages(IDB_PNG_REFRESH, IDB_PNG_REFRESH_FOCUS, IDB_PNG_REFRESH_CK, IDB_PNG_REFRESH);
	m_bnFresh.ShowBoxFrame(FALSE);
	m_bnFresh.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, Rect(0, 0, 18, 18), this, ID_BNITEM_FRESH);
	m_bnFresh.ShowWindow(SW_HIDE);
}

inline void CSwitchSourceBar::ShowFreshBtn(BOOL bShow)
{
	if (m_bnFresh.GetSafeHwnd() && m_bnFresh.IsWindowVisible()!=bShow)
	{
		m_bnFresh.EnableWindow(bShow);
		m_bnFresh.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}

inline void CSwitchSourceBar::RepositionFreshBtn(CDrawSourceItem* pActiveItem)
{
	if (pActiveItem)
	{
		Rect rcBnItem;
		Rect rcSrcItem = pActiveItem->m_rcItem;

		rcBnItem.X = rcSrcItem.X + 5;
		rcBnItem.Y = (rcSrcItem.Height - 18)/2 + rcSrcItem.Y;
		rcBnItem.Width = 18;
		rcBnItem.Height = 18;

		m_bnFresh.SetItemRect(rcBnItem);

		if (m_bnFresh.GetSafeHwnd())
			m_bnFresh.MoveWindow(rcBnItem.X, rcBnItem.Y, rcBnItem.Width, rcBnItem.Height);
	}
}

void CSwitchSourceBar::CreateSourceItem()
{
	CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

	CDrawSourceItem* pNewItem=NULL;

	int nItem=0;

	CRect rcWnd = m_rcBar;
	rcWnd.OffsetRect(-rcWnd.TopLeft());

	Rect rcItem;

	m_arrItem.RemoveAll();

	while (nItem < pMargins->SourceBarMargin.iMaxVisiableItemsOnVert)
	{
		rcItem.X = rcWnd.left + 20;
		rcItem.Y = rcWnd.top+pMargins->SourceBarMargin.iItemHeight*nItem;
		rcItem.Width  = rcWnd.Width() - 20;
		rcItem.Height = pMargins->SourceBarMargin.iItemHeight; 

		pNewItem = new CDrawSourceItem(this);
		pNewItem->SetItemIndex(nItem+m_sFirstVisiableItem);
		pNewItem->SetItemRect(rcItem);

		m_arrItem.Add(pNewItem);

		nItem++;
	}
}

void CSwitchSourceBar::ResetSourceItem()
{
	CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

	int nSize = m_arrItem.GetSize();
	//if (nSize == pMargins->SourceBarMargin.iMaxVisiableItemsOnVert)
	//	return;

	CRect rcWnd = m_rcBar;
	rcWnd.OffsetRect(-rcWnd.TopLeft());

	CDrawSourceItem* pNewItem = NULL;
	CDrawSourceItem* pOldItem = NULL;
	int nItem = 0;
	Rect rcItem;

	if (nSize > pMargins->SourceBarMargin.iMaxVisiableItemsOnVert)
	{
		while(nItem < pMargins->SourceBarMargin.iMaxVisiableItemsOnVert)
		{
			rcItem.X = rcWnd.left + 20;
			rcItem.Y = rcWnd.top+pMargins->SourceBarMargin.iItemHeight*nItem;
			rcItem.Width  = rcWnd.Width() - 20;
			rcItem.Height = pMargins->SourceBarMargin.iItemHeight; 

			pOldItem = m_arrItem[nItem];
			pOldItem->SetItemIndex(nItem+m_sFirstVisiableItem);
			pOldItem->SetItemRect(rcItem);

			nItem++;
		}

		for (int i=nSize-1; i>=nItem; i--)
		{
			m_arrItem[i]->InternalRelease();
			m_arrItem.RemoveAt(i);
		}
	}
	else if (nSize < pMargins->SourceBarMargin.iMaxVisiableItemsOnVert)
	{
		while (nItem < pMargins->SourceBarMargin.iMaxVisiableItemsOnVert)
		{
			rcItem.X = rcWnd.left + 20;
			rcItem.Y = rcWnd.top+pMargins->SourceBarMargin.iItemHeight*nItem;
			rcItem.Width  = rcWnd.Width() - 20;
			rcItem.Height = pMargins->SourceBarMargin.iItemHeight; 

			if (nItem < nSize)
			{
				pOldItem = m_arrItem[nItem];
				pOldItem->SetItemIndex(nItem+m_sFirstVisiableItem);
				pOldItem->SetItemRect(rcItem);
			}
			else
			{
				pNewItem = new CDrawSourceItem(this);
				pNewItem->SetItemIndex(nItem+m_sFirstVisiableItem);
				pNewItem->SetItemRect(rcItem);

				m_arrItem.Add(pNewItem);
			}

			nItem++;
		}
	}
	else
	{
		while (nItem < pMargins->SourceBarMargin.iMaxVisiableItemsOnVert)
		{
			rcItem.X = rcWnd.left + 20;
			rcItem.Y = rcWnd.top+pMargins->SourceBarMargin.iItemHeight*nItem;
			rcItem.Width  = rcWnd.Width() - 20;
			rcItem.Height = pMargins->SourceBarMargin.iItemHeight; 

			pOldItem = m_arrItem[nItem];
			pOldItem->SetItemIndex(nItem+m_sFirstVisiableItem);
			pOldItem->SetItemRect(rcItem);

			nItem++;
		}
	}
}

void CSwitchSourceBar::ScrollItem()
{
	CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

	int iSafeRange = m_arrItem.GetSize();

	for (int i=0; i<iSafeRange; i++)
	{
		m_arrItem[i]->SetItemIndex(i+m_sFirstVisiableItem);
	}

	m_Vertbar.SetScrollPos(m_sFirstVisiableItem);
}

void CSwitchSourceBar::AddItem(CDrawSourceItem* pItem)
{
	if (pItem == NULL)
		return;

	if (m_arrItem.IsEmpty()) {
		m_arrItem.Add(pItem);
		return;
	}

	if (pItem->m_nSourceIndex > m_arrItem[m_arrItem.GetCount()-1]->m_nSourceIndex) {
		m_arrItem.Add(pItem);
		return;
	}

	if (pItem->m_nSourceIndex < m_arrItem[0]->m_nSourceIndex)	{
		m_arrItem.InsertAt(0, pItem);
		return;
	}

	for (int i=0; i<m_arrItem.GetCount()-1; i++)
	{
		if ( pItem->m_nSourceIndex > m_arrItem[i]->m_nSourceIndex
			&& pItem->m_nSourceIndex < m_arrItem[i+1]->m_nSourceIndex )
		{
			m_arrItem.InsertAt(i+1, pItem);
			break;
		}
	}
}

void CSwitchSourceBar::RemoveAll()
{
	if (m_arrItem.IsEmpty())
		return;

	for (int i=0; i< m_arrItem.GetCount(); i++)
	{
		m_arrItem[i]->InternalRelease();
	}

	m_arrItem.RemoveAll();
}

void CSwitchSourceBar::RemoveAt(int iItemIndex)
{
	if (m_arrItem.IsEmpty())
		return;

	if (iItemIndex < 0 || iItemIndex >= m_arrItem.GetSize())
		return;

	m_arrItem[iItemIndex]->InternalRelease();
	m_arrItem.RemoveAt(iItemIndex);
}

void CSwitchSourceBar::ActiveItem(int iItemIndex)
{
	if (iItemIndex == -1)
	{
		for (int i=0; i<m_arrItem.GetSize(); i++)
		{
			if (m_arrItem[i]->m_bActive) {
				m_arrItem[i]->Active(FALSE);
			}
		}

		return;
	}

	if (iItemIndex >= m_arrItem.GetSize())
		return;

	CDrawSourceItem* pItem = m_arrItem[iItemIndex];

	if (pItem->m_bActive)
		return;

	for (int i=0; i<m_arrItem.GetSize(); i++)
	{
		if (m_arrItem[i]->m_bActive) {
			m_arrItem[i]->Active(FALSE);
			break;
		}
	}

	pItem->Active(TRUE);
}

CDrawSourceItem* CSwitchSourceBar::GetSourceItem(int iItemIndex)
{
	if (iItemIndex < 0 || iItemIndex >= m_arrItem.GetSize())
		return NULL;

	return m_arrItem[iItemIndex];
}

CDrawSourceItem* CSwitchSourceBar::GetActiveItem()
{
	for (int i=0; i<m_arrItem.GetSize(); i++)
	{
		if (m_arrItem[i]->IsActived())
			return m_arrItem[i];
	}

	return NULL;
}

void CSwitchSourceBar::OnPaint()
{
	CPaintDC dc(this);

	CRect rcCli;
	GetClientRect(&rcCli);

	Graphics gp(dc.m_hDC);
	gp.SetSmoothingMode(SmoothingModeHighSpeed);

	// 除非窗口背景色与父窗口不同，否则不提供背景绘制
	//SolidBrush bkBr(Color(255, 101, 147, 207));
	Pen pen(Color(255, 101, 147, 207), 1.0);
	gp.DrawRectangle(&pen, 19, 0, rcCli.Width()-19, rcCli.Height()-1);

	for (int i=0; i<m_arrItem.GetSize(); i++)
	{
		m_arrItem[i]->Draw(&gp);
	}
}


void CSwitchSourceBar::RedrawItem(int iItemIndex)
{
	if (iItemIndex < 0 || iItemIndex >= m_arrItem.GetSize())
		return;

	CDC* pDC=GetDC();
	Graphics gp(pDC->m_hDC);

	m_arrItem[iItemIndex]->Draw(&gp);

	ReleaseDC(pDC);
}

void CSwitchSourceBar::OnHitTest(CPoint point)
{
	int iCurFocus = -1;
	for (int i=0; i<m_arrItem.GetSize(); i++)
	{
		if (m_arrItem[i]->m_rcItem.Contains(point.x, point.y))
		{
			iCurFocus = i;
			break;
		}
	}

	if (iCurFocus != m_FocusItem)
	{
		ActiveItem(iCurFocus);

		// 计算刷新区域，刷新整个窗口会带来闪动
		CDrawSourceItem* pCurItem = GetSourceItem(iCurFocus);
		if (pCurItem != NULL)
		{
			InvalidateRect(pCurItem->GetItemRect());
		}

		CDrawSourceItem* pFocusItem = GetSourceItem(m_FocusItem);
		if (pFocusItem != NULL)
		{
			InvalidateRect(pFocusItem->GetItemRect());
		}

		m_FocusItem = iCurFocus;
		
		// 更新刷新按钮的状态
		if (m_FocusItem != -1 && GetInputManager()->GetSubID(pCurItem->m_nSourceIndex) != 2)
		{
			ShowFreshBtn(TRUE);
			RepositionFreshBtn(pCurItem);		
		}
		else
		{
			ShowFreshBtn(FALSE);
		}

		SetTimer(TIMER_KILLFOCUS, 100, NULL);
	}

	// 显示提示框
	CSwitchPage* pParent = (CSwitchPage*)GetParent();
	if (m_FocusItem != -1 )
	{
		CDrawSourceItem* pCurItem = GetSourceItem(m_FocusItem);
		if (pCurItem != NULL)
		{
			CRect rcHot;
			rcHot = pCurItem->GetItemRect();
			rcHot.left = rcHot.right - 48;
			if (rcHot.PtInRect(point))
			{
				if (pParent->GetNeedHelpPort() == INT_PAIR(GetInputManager()->FindID(pCurItem->m_nSourceIndex), 1))
					return;

				point.Offset(pCurItem->GetItemRect().right-point.x, pCurItem->GetItemRect().top-point.y);
				ClientToScreen(&point);	

				pParent->SetNeedHelpPort(GetInputManager()->FindID(pCurItem->m_nSourceIndex), 1);
				pParent->ShowHelpText(TRUE, L"", point);
			}
			else
			{
				pParent->ShowHelpText(FALSE);
			}
		}
	}
	else
		pParent->ShowHelpText(FALSE);

	((CSwitchSelectBar*)pParent->GetSelectBar())->SetFocusBox(-1, m_FocusItem);
}

void CSwitchSourceBar::SetFocuseItem(int nItem)
{
	if (m_FocusItem == nItem)
		return;

	int nPrevItem = m_FocusItem;
	m_FocusItem = nItem;

	if (IsEditing())
		return;

	ActiveItem(m_FocusItem);

	CDrawSourceItem* pPrevItem = GetSourceItem(nPrevItem);
	if (pPrevItem != NULL)
	{
		InvalidateRect(pPrevItem->GetItemRect());
	}

	CDrawSourceItem* pFocusItem = GetSourceItem(m_FocusItem);
	if (pFocusItem != NULL)
	{
		InvalidateRect(pFocusItem->GetItemRect());
	}

}

void CSwitchSourceBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	if (IsEditing())
		EndEdit();

	SetFocus();

	//OnHitTest(point);

	CWnd::OnLButtonDown(nFlags, point);
}

void CSwitchSourceBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CWnd::OnLButtonUp(nFlags, point);
}

void CSwitchSourceBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if (IsEditing())
		return CWnd::OnMouseMove(nFlags, point);

	SetFocus();
	
	OnHitTest(point);

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CSwitchSourceBar::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CWnd::OnEraseBkgnd(pDC);
}

void CSwitchSourceBar::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_KILLFOCUS:
		{
			CPoint ptCursor;
			GetCursorPos(&ptCursor);

			CRect rcWnd;
			GetWindowRect(&rcWnd);
			rcWnd.left += 18;

			if (!rcWnd.PtInRect(ptCursor))
			{
				// 预先得到当前被激活的项
				CDrawSourceItem* pItem = GetSourceItem(m_FocusItem);

				// 反激活此项
				m_FocusItem = -1;
				ActiveItem(m_FocusItem);

				ShowFreshBtn(FALSE);

				// 刷新此项所在的区域
				if (pItem != NULL)
				{
					InvalidateRect(pItem->GetItemRect());
				}

				((CSwitchSelectBar*)((CSwitchPage*)GetParent())->GetSelectBar())->SetFocusBox( -1, m_FocusItem );

				((CSwitchPage*)GetParent())->ShowHelpText(FALSE);

				KillTimer(nIDEvent);
			}
		}
		break;

	default:break;
	}

	CWnd::OnTimer(nIDEvent);
}


void CSwitchSourceBar::RecalcLayout()
{
	if (IsEditing())
		EndEdit();

	CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

	if (pMargins->SourceBarMargin.iMaxVisiableItemsOnVert + m_sFirstVisiableItem > GetInputManager()->GetCount())
	{
		m_sFirstVisiableItem = GetInputManager()->GetCount()-pMargins->SourceBarMargin.iMaxVisiableItemsOnVert;
	}

	ResetSourceItem();

	RepositionFreshBtn(GetActiveItem());

	SetVScrollBar();
}

void CSwitchSourceBar::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int minpos = 0;
	int maxpos = 0;
	pScrollBar->GetScrollRange(&minpos, &maxpos);

	// Get the current position of scroll box.
	int curpos = 0;
	curpos = m_sFirstVisiableItem;

	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		curpos = minpos;
		break;

	case SB_RIGHT:      // Scroll to far right.
		curpos = maxpos;
		break;

	case SB_ENDSCROLL:   // End scroll.
		break;

	case SB_LINELEFT:      // Scroll left.
		curpos--;
		if (curpos < minpos) {curpos = minpos;}
		pScrollBar->SetScrollPos(curpos);
		break;

	case SB_LINERIGHT:   // Scroll right.
		curpos++;
		if (curpos > maxpos) {curpos = maxpos;}
		pScrollBar->SetScrollPos(curpos);
		break;

	case SB_PAGELEFT:    // Scroll one page left.
		curpos--;
		if (curpos < minpos) {curpos = minpos;}
		pScrollBar->SetScrollPos(curpos);
		break;

	case SB_PAGERIGHT:      // Scroll one page right.
		curpos++;
		if (curpos > maxpos) {curpos = maxpos;}
		pScrollBar->SetScrollPos(curpos);
		break;

	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;

	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		curpos = nPos;     // position that the scroll box has been dragged to.
		pScrollBar->SetScrollPos(curpos);
		break;
	}

	// Set the new position of the thumb (scroll box).
	if (m_sFirstVisiableItem == curpos)	return;

	m_sFirstVisiableItem = curpos;

	ScrollItem();

	RedrawWindow();
	((CSwitchPage*)GetParent())->GetSelectBar()->RedrawWindow();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CSwitchSourceBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
}

BOOL CSwitchSourceBar::PreTranslateMessage(MSG* pMsg)
{
	if (m_tips.GetSafeHwnd())
		m_tips.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CSwitchSourceBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (IsEditing())
		return TRUE;

	CSwitchPage* pParent = (CSwitchPage*)GetParent();
	CPageMargins* pMargins = (CPageMargins*)pParent->GetMargins();

	int curpos = m_sFirstVisiableItem;
	curpos -= zDelta/120;

	int iMin = 0;
	int iMax = GetInputManager()->GetCount() - pMargins->SourceBarMargin.iMaxVisiableItemsOnVert;
	iMax = max (0, iMax);

	curpos = max (curpos, 0);
	curpos = min (curpos, iMax);
	

	if (m_sFirstVisiableItem == curpos)
		return TRUE;

	m_sFirstVisiableItem = curpos;

	ScrollItem();

	RedrawWindow();
	pParent->GetSelectBar()->RedrawWindow();

	if (pParent->IsHelpTextVisible())
	{
		CDrawSourceItem* pItem = GetActiveItem();
		if (pItem != NULL)
		{
			pParent->SetNeedHelpPort(GetInputManager()->FindID(pItem->m_nSourceIndex), 1);
			pParent->RedrawHelpText();
		}
	}	

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CSwitchSourceBar::OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/)
{
	if (IsEditing())
		return;

	CDrawSourceItem *pItem = GetActiveItem();
	if (pItem == NULL)
		return;

	//弹出菜单
	if (afxContextMenuManager == NULL)
		return;

	if ((::GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0 && (::GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0)
		return;

	CWnd* pWnd = (CWnd*)GetMainFrame()->GetActiveView();
	if (pWnd == NULL)
		return;

	// 弹出菜单时，关闭去激活定时器
	KillTimer(TIMER_KILLFOCUS);

	CMFCPopupMenu* pPopupMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, pWnd);

	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, ID_ONE_TO_ALL, _T("切换至所有输出"));
	menu.AppendMenu(MF_STRING, ID_SHUT_ALL, _T("关闭所有输出"));
	menu.AppendMenu(MF_STRING, ID_LOAD_EDID, _T("EDID 更新"));
	menu.AppendMenu(MF_STRING, ID_VGA_PROPERTY, _T("VGA 属性"));
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, ID_RENAME, _T("重命名（双击信号名）"));


	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	if (pPopupMenu != NULL)
		afxContextMenuManager->SetDontCloseActiveMenu();

	int nMenuResult = afxContextMenuManager->TrackPopupMenu(menu, ptCursor.x, ptCursor.y, pWnd);

	if (pPopupMenu != NULL)
		afxContextMenuManager->SetDontCloseActiveMenu(FALSE);

	switch(nMenuResult)
	{
	case ID_ONE_TO_ALL:
		{
			GetOutputManager()->AllSwitch(GetInputManager()->GetID(pItem->m_nSourceIndex), FALSE);

			CSwitchHeadBar* pHeadBar = (CSwitchHeadBar*)((CSwitchPage*)GetParent())->GetHeadBar();
			pHeadBar->ReDrawColumnSec();

			CSwitchSelectBar* pSelBar = (CSwitchSelectBar*)((CSwitchPage*)GetParent())->GetSelectBar();
			pSelBar->Invalidate();
		}

		break;

	case ID_SHUT_ALL:
		{
			GetOutputManager()->AllSwitch(INVALID_VALUE, FALSE);

			CSwitchHeadBar* pHeadBar = (CSwitchHeadBar*)((CSwitchPage*)GetParent())->GetHeadBar();
			pHeadBar->ReDrawColumnSec();

			CSwitchSelectBar* pSelBar = (CSwitchSelectBar*)((CSwitchPage*)GetParent())->GetSelectBar();
			pSelBar->Invalidate();
		}

		break;

	case ID_LOAD_EDID:

		GetInputManager()->UpdateEDID(pItem->m_nSourceIndex, FALSE);

		break;
	
	case ID_VGA_PROPERTY:
		{
			CSourcePropertyDlg DLG;
			DLG.m_nSourcePort = GetInputManager()->GetCmdPort(pItem->m_nSourceIndex);
			DLG.m_eSourceType = GetInputManager()->GetSignalType(pItem->m_nSourceIndex, FALSE);
			DLG.DoModal();
		}
		
		break;

	case ID_RENAME:

		m_nItemOnEdit = pItem->m_nSourceIndex-m_sFirstVisiableItem;
		BeginEdit();

		break;

	default:break;
	}

	if (pPopupMenu != NULL)
	{
		pPopupMenu->SendMessage(WM_CLOSE);
	}
}

void CSwitchSourceBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDrawSourceItem* pItem = GetActiveItem();
	if (pItem == NULL)
		return CWnd::OnLButtonDblClk(nFlags, point);

	// 鼠标是否双击在信号名称上
	CRect rcHot;
	rcHot = pItem->GetItemRect();
	rcHot.left = pItem->GetItemRect().left + 28;
	rcHot.right = pItem->GetItemRect().right - 48;

	if (rcHot.PtInRect(point))
	{
		m_nItemOnEdit = pItem->m_nSourceIndex-m_sFirstVisiableItem;
		BeginEdit();
	}	

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CSwitchSourceBar::BeginEdit()
{
	if (m_nItemOnEdit == -1)
		return;

	m_bnFresh.Enable(FALSE);
	m_Vertbar.EnableWindow(FALSE);

	CDrawSourceItem* pItem = GetSourceItem(m_nItemOnEdit);
	if (pItem == NULL)
		return;

	CRect rcItem = pItem->GetItemRect();
	rcItem.InflateRect(-2, -2);
	m_Edit.MoveWindow(rcItem);
	m_Edit.SetWindowText(GetInputManager()->GetName(pItem->m_nSourceIndex, FALSE));
	m_Edit.SetSel(0, -1);
	m_Edit.ShowWindow(SW_SHOW);
	m_Edit.SetFocus();
}

BOOL CSwitchSourceBar::IsEditing() const
{
	if (m_Edit.GetSafeHwnd() && m_Edit.IsWindowVisible())
		return TRUE;

	return FALSE;
}

void CSwitchSourceBar::EndEdit()
{
	m_Edit.ShowWindow(SW_HIDE);
	m_bnFresh.Enable(TRUE);
	m_Vertbar.EnableWindow(m_Vertbar.IsWindowVisible());
	m_nItemOnEdit = -1;
}

LRESULT CSwitchSourceBar::OnCancelEdit(WPARAM wParam, LPARAM lParam)
{
	EndEdit();

	return 0;
}

LRESULT CSwitchSourceBar::OnEndEdit(WPARAM wParam, LPARAM lParam)
{
	if (m_nItemOnEdit != -1)
	{
		CString str;
		m_Edit.GetWindowText(str);

		// 所有有子信号的端口应该用同一个名字
		if (GetInputManager()->GetSubID(m_nItemOnEdit+m_sFirstVisiableItem, FALSE) != 0)
		{
			int nCmdPort = GetInputManager()->GetCmdPort(m_nItemOnEdit+m_sFirstVisiableItem, FALSE);
			
			int nIndex   = GetInputManager()->FindCmdPort(nCmdPort, 1);
			GetInputManager()->SetName(nIndex, str.GetBuffer(), FALSE);
			InvalidateRect(GetSourceItem(nIndex-m_sFirstVisiableItem)->GetItemRect());

			nIndex = GetInputManager()->FindCmdPort(nCmdPort, 2);
			GetInputManager()->SetName(nIndex, str.GetBuffer(), FALSE);
			InvalidateRect(GetSourceItem(nIndex-m_sFirstVisiableItem)->GetItemRect());
		}
		else
		{
			GetInputManager()->SetName(m_nItemOnEdit+m_sFirstVisiableItem, str.GetBuffer(0), FALSE);
			InvalidateRect(GetSourceItem(m_nItemOnEdit)->GetItemRect());
		}
	}

	EndEdit();

	return 0;
}

LRESULT CSwitchSourceBar::OnRefreshThis(WPARAM wParam, LPARAM lParam)
{
	CWaitCursor wait;

	CDrawSourceItem* pItem = GetActiveItem();
	if (pItem == NULL)
		return 0;

	// 刷新此信号源
	GetInputManager()->RefreshThisSignal(pItem->m_nSourceIndex);

	InvalidateRect(pItem->GetItemRect());

	return 0;
}