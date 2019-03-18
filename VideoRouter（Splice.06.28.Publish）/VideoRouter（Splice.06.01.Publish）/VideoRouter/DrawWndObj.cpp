// DrawWndObj.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "DrawWndObj.h"
#include "DrawWndPage.h"


int CDrawWndObj::m_sxPhyScalar = 1024;
int CDrawWndObj::m_syPhyScalar = 768;
int CDrawWndObj::m_sxLogScalar = 1024;
int CDrawWndObj::m_syLogScalar = 768;

void CDrawWndObj::SetScalar(const int ixPhy, const int iyPhy, const int ixLog, const int iyLog)
{
	m_sxPhyScalar = ixPhy;
	m_syPhyScalar = ixPhy;
	m_sxLogScalar = ixLog;
	m_syLogScalar = iyLog;
}

// CDrawWndObj

IMPLEMENT_DYNAMIC(CDrawWndObj, CCmdTarget)


CDrawWndObj::CDrawWndObj(CDrawWndPage* pOwner)
{
	m_HitTest = MYHTNOWHERE;
	m_ptLBtnDown = CPoint(-1, -1);
	m_pOwner = pOwner;
}


CDrawWndObj::~CDrawWndObj()
{
	m_pOwner = NULL;
}

void CDrawWndObj::SetParentPage(CDrawWndPage* pOwner)
{
	ASSERT(pOwner);
	m_pOwner = pOwner;
}

int CDrawWndObj::GetObjectSize()
{
	return \
	int(1 + sizeof(int) + sizeof(int) + sizeof(int) + sizeof(BOOL) + sizeof(COLORREF) + sizeof(long) + sizeof(long) + sizeof(long) + sizeof(long) + _MAX_WND_NAME + 1); 
	// '<'   m_iWndID     m_iSrcPort     m_iZorder      m_bLock          m_clrBk        m_rcPhy.left   m_rcPhy.top   m_rcPhy.right   m_rcPhy.bottom    m_szName    '>'
}

int CDrawWndObj::Write(__in char* pBlock, __in const long lsize, __out long &lPos) const
{
	if (lPos + GetObjectSize() > lsize) return -1;

	int nSize = 0;

	memcpy(pBlock+nSize, "<", 1); nSize += 1;  // 起始标志
	memcpy(pBlock+nSize, &m_iWndID, sizeof(int));  nSize += sizeof(int);  // 窗口ID
	memcpy(pBlock+nSize, &m_iSrcPort, sizeof(int)); nSize += sizeof(int); // 信号源端口
	memcpy(pBlock+nSize, &m_iZorder, sizeof(int)); nSize += sizeof(int);  // 叠放顺序
	memcpy(pBlock+nSize, &m_bLocked, sizeof(BOOL)); nSize += sizeof(BOOL);// 锁定标志
	memcpy(pBlock+nSize, &m_clrBk, sizeof(COLORREF)); nSize += sizeof(COLORREF);// 窗口颜色
	memcpy(pBlock+nSize, &m_rcPhy.left, sizeof(long)); nSize += sizeof(long);   //左
	memcpy(pBlock+nSize, &m_rcPhy.top, sizeof(long)); nSize += sizeof(long);    //上
	memcpy(pBlock+nSize, &m_rcPhy.right, sizeof(long)); nSize += sizeof(long);  //右
	memcpy(pBlock+nSize, &m_rcPhy.bottom, sizeof(long)); nSize += sizeof(long); //下
	memcpy(pBlock+nSize, m_szName, _MAX_WND_NAME); nSize += _MAX_WND_NAME;
	memcpy(pBlock+nSize, ">", 1); nSize += 1; // 终止标志

	lPos += nSize;

	return nSize;
}

int CDrawWndObj::Read(__in char* pBlock, __in const long lsize, __out long &lPos)
{
	if (lPos + GetObjectSize() > lsize) return -1;

	int nSize = 0;
	
	if (pBlock[nSize++] != '<') return -1; //起始标志

	memcpy(&m_iWndID, pBlock+nSize, sizeof(int));  nSize += sizeof(int);  // 窗口ID
	memcpy(&m_iSrcPort, pBlock+nSize, sizeof(int)); nSize += sizeof(int); // 信号源端口
	memcpy(&m_iZorder, pBlock+nSize, sizeof(int)); nSize += sizeof(int);  // 叠放顺序
	memcpy(&m_bLocked, pBlock+nSize, sizeof(BOOL)); nSize += sizeof(BOOL);// 锁定标志
	memcpy(&m_clrBk, pBlock+nSize, sizeof(COLORREF)); nSize += sizeof(COLORREF);// 窗口颜色
	memcpy(&m_rcPhy.left, pBlock+nSize, sizeof(long)); nSize += sizeof(long);   //左
	memcpy(&m_rcPhy.top, pBlock+nSize, sizeof(long)); nSize += sizeof(long);    //上
	memcpy(&m_rcPhy.right, pBlock+nSize, sizeof(long)); nSize += sizeof(long);  //右
	memcpy(&m_rcPhy.bottom, pBlock+nSize, sizeof(long)); nSize += sizeof(long); //下
	memcpy(m_szName, pBlock+nSize, _MAX_WND_NAME); nSize += _MAX_WND_NAME;

	if (pBlock[nSize++] != '>') return -1; // 终止标志

	lPos += nSize;

	// 其它控制量
	SetPhyPos(m_rcPhy);
	m_bActived = FALSE;

	return nSize;
}

int CDrawWndObj::Write(__in UINT* pBlock, __in const long lsize, __out long &lPos) const
{
	const int nSegLength = 9; //保存指令中每个窗口占的数据长度
	if (lPos + nSegLength > lsize) return -1;

	int nSize = 0;

	pBlock[nSize++] = 0x01; //有效性标志

	pBlock[nSize++] = m_iWndID;
	pBlock[nSize++] = m_iSrcPort;
	pBlock[nSize++] = m_iZorder;
	pBlock[nSize++] = m_iAlpha;
	pBlock[nSize++] = m_rcPhy.left;
	pBlock[nSize++] = m_rcPhy.top;
	pBlock[nSize++] = m_rcPhy.right;
	pBlock[nSize++] = m_rcPhy.bottom;

	lPos += nSize;
	
	return nSize;
}

int CDrawWndObj::Read(__in UINT* pBlock, __in const long lsize, __out long &lPos)
{
	const int nSegLength = 9; //读取指令中每个窗口占的数据长度
	if (lPos + nSegLength > lsize) return -1;

	int nSize = 0;

	if (pBlock[nSize++] != 0x01) return -1; //有效性标志

	m_iWndID   = pBlock[nSize++];
	m_iSrcPort = pBlock[nSize++];
	m_iZorder  = pBlock[nSize++];
	m_iAlpha   = pBlock[nSize++];
	m_rcPhy.left  = pBlock[nSize++];
	m_rcPhy.top   = pBlock[nSize++];
	m_rcPhy.right = pBlock[nSize++]+1;
	m_rcPhy.bottom= pBlock[nSize++]+1;

	SetPhyPos(m_rcPhy);

	CString strName;
	strName.Format(_T("Source[%d]"), m_iSrcPort);
	SetName(strName.GetBuffer());

	SetBkColor(m_pOwner->GetColor(m_iSrcPort));
	m_bActived = FALSE;
	m_bLocked = CDrawWndPage::m_sLockAll;

	lPos += nSize;

	return nSize;
}

BEGIN_MESSAGE_MAP(CDrawWndObj, CCmdTarget)
END_MESSAGE_MAP()



// CDrawWndObj 消息处理程序
void CDrawWndObj::SetWndID(const int iID)
{
	m_iWndID = iID;
}

void CDrawWndObj::SetSourcePort(const int iSrcPort)
{
	m_iSrcPort = iSrcPort;
}

void CDrawWndObj::SetZorder(const int iZorder)
{
	m_iZorder = iZorder;
}

void CDrawWndObj::SetAlpha(const int iAlpha)
{
	m_iAlpha = iAlpha;
}

void CDrawWndObj::SetName(const char* szName)
{
	memset(m_szName, 0, _MAX_WND_NAME);

	int nLen = strlen(szName);
	strncpy_s(m_szName, _MAX_WND_NAME, szName, nLen);
}

void CDrawWndObj::SetLock(BOOL bLock)
{
	m_bLocked = bLock;
}

void CDrawWndObj::Active(BOOL bActive)
{
	m_bActived = bActive;
}

void CDrawWndObj::SetPhyPos(const CRect rcPhy)
{
	m_rcPhy = rcPhy;
	m_rcLog = rcPhy.MulDiv(m_sxLogScalar, m_sxPhyScalar);

	m_rcLog.OffsetRect(m_pOwner->m_rcPage.TopLeft());

	RecalcLayOut();
}

void CDrawWndObj::SetPhyPos(const Rect rcPhy)
{
	m_rcPhy = CRect(rcPhy.X, rcPhy.Y, rcPhy.GetRight(), rcPhy.GetBottom());
	m_rcLog = m_rcPhy.MulDiv(m_sxLogScalar, m_sxPhyScalar);

	m_rcLog.OffsetRect(m_pOwner->m_rcPage.TopLeft());

	RecalcLayOut();
}

void CDrawWndObj::SetLogPos(const CRect rcLog)
{
	m_rcLog = rcLog;

	CRect rc = rcLog;
	rc.OffsetRect(-m_pOwner->m_rcPage.TopLeft());
	m_rcPhy = rc.MulDiv(m_sxPhyScalar, m_sxLogScalar);

	RecalcLayOut();
}

void CDrawWndObj::SetPosMark()
{
	m_rcLastLog = m_rcLog;
	m_rcLastPhy = m_rcPhy;
}

void CDrawWndObj::RecalcLayOut()
{
	//标题位置
	m_rcTitle = CRect(m_rcLog.left, m_rcLog.top, m_rcLog.right, m_rcLog.top+18);

	//关闭按钮位置
	m_rcBtnExit = CRect(m_rcTitle.right - 18, m_rcTitle.top+2, m_rcTitle.right-2, m_rcTitle.top+18);

	//还原按钮位置
	m_rcBtnRestore = CRect(m_rcBtnExit.left-17, m_rcBtnExit.top, m_rcBtnExit.left-1, m_rcBtnExit.bottom);
	if (m_rcBtnRestore.left < m_rcTitle.left + 10)
		m_rcBtnRestore.SetRectEmpty();

	//最大化按钮的位置
	m_rcBtnMaximize = CRect(m_rcBtnRestore.left-17, m_rcBtnRestore.top, m_rcBtnRestore.left-1, m_rcBtnRestore.bottom);
	if (m_rcBtnMaximize.left < m_rcTitle.left + 10)
		m_rcBtnMaximize.SetRectEmpty();

	//对齐按钮的位置
	m_rcBtnAlign = CRect(m_rcBtnMaximize.left-17, m_rcBtnMaximize.top, m_rcBtnMaximize.left-1, m_rcBtnMaximize.bottom);
	if (m_rcBtnAlign.left < m_rcTitle.left + 10)
		m_rcBtnAlign.SetRectEmpty();
}

void CDrawWndObj::SetBkColor(COLORREF clrBk)
{
	m_clrBk = clrBk;
}

void CDrawWndObj::Draw(CDC *pDC)
{
	if (!m_pOwner->IsObjVisible(m_rcLog))
		return;

	ASSERT(pDC);

	CRect rcObj = m_rcLog;
	rcObj.OffsetRect(-rcObj.left, -rcObj.top);

	CDC *pObjDC = new CDC;
	pObjDC->CreateCompatibleDC(pDC);
	CBitmap bkBmp;
	bkBmp.CreateCompatibleBitmap(pDC, rcObj.Width(), rcObj.Height());
	pObjDC->SelectObject(&bkBmp);

	Graphics gp(pObjDC->m_hDC);
	gp.SetSmoothingMode(SmoothingModeHighSpeed);

	int iTransp = 255;
	if (m_bLocked)
		iTransp = 100;

	//画窗口背景
	SolidBrush sBrush(Color(iTransp, GetRValue(m_clrBk), GetGValue(m_clrBk), GetBValue(m_clrBk)));
	gp.FillRectangle(&sBrush, 0, 0, rcObj.Width(), rcObj.Height());

	//标题栏
	LinearGradientBrush GradientBrAc(Point(rcObj.left, rcObj.top), Point(rcObj.left, rcObj.bottom), Color(255, 188, 188, 255), Color(255, 188, 188, 205));
	LinearGradientBrush GradientBrDe(Point(rcObj.left, rcObj.top), Point(rcObj.left, rcObj.bottom), Color(255, 48, 48, 105), Color(255, 48, 48, 65));

	CRect rcTmp = m_rcTitle;
	rcTmp.OffsetRect(-m_rcLog.left, -m_rcLog.top);

	if (m_bActived)
		gp.FillRectangle(&GradientBrAc, rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height());
	else
		gp.FillRectangle(&GradientBrDe, rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height());

	//边框
	Pen blackPen(Color(iTransp, 114, 115, 153), 1);
	gp.DrawRectangle(&blackPen, rcObj.left, rcObj.top, rcObj.Width()-1, rcObj.Height()-1);

	//绘制文本
	FontFamily fontFamily(L"宋体");
	Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);

	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingNone);	
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	//标题
	RectF textLayout;
	rcTmp = m_rcTitle;
	rcTmp.OffsetRect(-m_rcLog.left, -m_rcLog.top);
	textLayout = RectF((REAL)rcTmp.left+2, (REAL)rcTmp.top+1, (REAL)rcTmp.Width()-2, (REAL)rcTmp.Height()-1);

	WCHAR wszText[_MAX_SIGNAL_NAME];
	CString strText;
	strText.Format(_T("%s"), m_szName);
	MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);

	sBrush.SetColor(Color(iTransp, 255, 250, 220));
	gp.DrawString(wszText, -1, &font, textLayout, &format, &sBrush);

	//窗口ID
	if (GetRValue(m_clrBk)+GetGValue(m_clrBk) < 128 || m_bLocked)
		sBrush.SetColor(Color(iTransp, 193, 194, 144));
	else if(GetBValue(m_clrBk) > 192)
		sBrush.SetColor(Color(iTransp, 64, 64, 0));
	else
		sBrush.SetColor(Color(iTransp, 0, 0, 0));

	textLayout.Offset(0, 16.0f);
	strText.Format(_T("窗口ID: %d"), m_iWndID);
	MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
	gp.DrawString(wszText, -1, &font, textLayout, &format, &sBrush);

	//信号源端口号
	textLayout.Offset(0, 16.0f);
	strText.Format(_T("信号源: %d"), m_iSrcPort);
	MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
	gp.DrawString(wszText, -1, &font, textLayout, &format, &sBrush);

	//叠放顺序
	textLayout.Offset(0, 16.0f);
	strText.Format(_T("叠放顺序: %d"), m_iZorder);
	MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
	gp.DrawString(wszText, -1, &font, textLayout, &format, &sBrush);

	//窗口位置
	textLayout.Offset(0, 16.f);
	strText.Format(_T("窗口位置: <%d,%d>"), m_rcPhy.left, m_rcPhy.top);
	MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
	gp.DrawString(wszText, -1, &font, textLayout, &format, &sBrush);

	//窗口大小
	textLayout.Offset(0, 16.f);
	strText.Format(_T("窗口大小: <%d*%d>"), m_rcPhy.Width(), m_rcPhy.Height());
	MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
	gp.DrawString(wszText, -1, &font, textLayout, &format, &sBrush);

	if (!m_bLocked)
	{
		//关闭按钮
		CBitmap* pBitmap = new CBitmap;
		CBitmap* pOldBitmap = NULL;
		CDC* pDisplayMemDC=new CDC;
		pDisplayMemDC->CreateCompatibleDC(pObjDC);

		pBitmap->LoadBitmap(IDB_BMP_CLOSE);
		pOldBitmap = pDisplayMemDC->SelectObject(pBitmap);

		rcTmp = m_rcBtnExit;
		rcTmp.OffsetRect(-m_rcLog.left, -m_rcLog.top);
		pObjDC->BitBlt(rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pBitmap->DeleteObject();

		//还原按钮
		pBitmap->LoadBitmap(IDB_BMP_RESTORE);
		pDisplayMemDC->SelectObject(pBitmap);

		rcTmp = m_rcBtnRestore;
		rcTmp.OffsetRect(-m_rcLog.left, -m_rcLog.top);
		pObjDC->BitBlt(rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pBitmap->DeleteObject();

		//最大化按钮
		pBitmap->LoadBitmap(IDB_BMP_MAX);
		pDisplayMemDC->SelectObject(pBitmap);

		rcTmp = m_rcBtnMaximize;
		rcTmp.OffsetRect(-m_rcLog.left, -m_rcLog.top);
		pObjDC->BitBlt(rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pBitmap->DeleteObject();

		//对齐按钮
		pBitmap->LoadBitmap(IDB_BMP_ALIGN);
		pDisplayMemDC->SelectObject(pBitmap);

		rcTmp = m_rcBtnAlign;
		rcTmp.OffsetRect(-m_rcLog.left, -m_rcLog.top);
		pObjDC->BitBlt(rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pBitmap->DeleteObject();

		SAFE_DELETE(pBitmap);
		SAFE_DELETE(pDisplayMemDC);
	}
	else
	{
		//sBrush.SetColor(Color(10, 110, 135, 90));
		//gp.FillRectangle(&sBrush, rcObj.left, rcObj.top, rcObj.Width(), rcObj.Height());
	}

	pDC->BitBlt(m_rcLog.left, m_rcLog.top, rcObj.Width(),rcObj.Height(), pObjDC, 0, 0, SRCCOPY);

	SAFE_DELETE(pObjDC);
}

void CDrawWndObj::DrawMethod2(Graphics *pGraphics)
{
	if (!m_pOwner->IsObjVisible(m_rcLog))
		return;

	ASSERT(pGraphics);

	CRect rcObj = m_rcLog;
	if (rcObj.Width() < 20 || rcObj.Height() < 20)
		return;

	pGraphics->SetSmoothingMode(SmoothingModeHighSpeed);

	int iTransp = 255;
	if (m_bLocked)
		iTransp = 100;

	//画窗口背景
	SolidBrush sBrush(Color(iTransp, GetRValue(m_clrBk), GetGValue(m_clrBk), GetBValue(m_clrBk)));
	pGraphics->FillRectangle(&sBrush, rcObj.left, rcObj.top, rcObj.Width(), rcObj.Height());

	//标题栏
	LinearGradientBrush GradientBrAc(Point(rcObj.left, rcObj.top), Point(rcObj.left, rcObj.bottom), Color(iTransp, 188, 188, 255), Color(iTransp, 188, 188, 205));
	LinearGradientBrush GradientBrDe(Point(rcObj.left, rcObj.top), Point(rcObj.left, rcObj.bottom), Color(iTransp, 48, 48, 105), Color(iTransp, 48, 48, 65));

	CRect rcTmp = m_rcTitle;
	if (m_bActived)
		pGraphics->FillRectangle(&GradientBrAc, rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height());
	else
		pGraphics->FillRectangle(&GradientBrDe, rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height());


	//边框
	Pen blackPen(Color(iTransp, 114, 115, 153), 1);
	pGraphics->DrawRectangle(&blackPen, rcObj.left, rcObj.top, rcObj.Width()-1, rcObj.Height()-1);

	//绘制文本
	FontFamily fontFamily(L"宋体");
	Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);

	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	format.SetLineAlignment(StringAlignmentNear);
	format.SetTrimming(StringTrimmingNone);	
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	//标题
	RectF textLayout;
	rcTmp = m_rcTitle;
	textLayout = RectF((REAL)rcTmp.left+2, (REAL)rcTmp.top+1, (REAL)rcTmp.Width()-2, (REAL)rcTmp.Height()-1);

	WCHAR wszText[_MAX_SIGNAL_NAME];
	CString strText;
	strText.Format(_T("%s"), m_szName);
	MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);

	sBrush.SetColor(Color(iTransp, 255, 250, 220));
	pGraphics->DrawString(wszText, -1, &font, textLayout, &format, &sBrush);

	//窗口ID
	if (GetRValue(m_clrBk)+GetGValue(m_clrBk) < 128 || m_bLocked)
		sBrush.SetColor(Color(iTransp, 240, 240, 240));
	else if(GetBValue(m_clrBk) > 192)
		sBrush.SetColor(Color(iTransp, 64, 64, 0));
	else
		sBrush.SetColor(Color(iTransp, 0, 0, 0));

	textLayout = RectF((REAL)rcTmp.left+2, (REAL)rcTmp.top+1+16, (REAL)rcTmp.Width()-2, (REAL)rcTmp.Height()-1);
	if (textLayout.GetBottom() > (REAL)rcObj.bottom)
		textLayout.Height = (REAL)rcObj.bottom - textLayout.GetTop();
	if (textLayout.Height > 0.000001)
	{
		strText.Format(_T("窗口ID: %d"), m_iWndID);
		MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
		pGraphics->DrawString(wszText, -1, &font, textLayout, &format, &sBrush);
	}	

	//信号源端口号
	textLayout = RectF((REAL)rcTmp.left+2, (REAL)rcTmp.top+1+32, (REAL)rcTmp.Width()-2, (REAL)rcTmp.Height()-1);
	if (textLayout.GetBottom() > (REAL)rcObj.bottom)
		textLayout.Height = (REAL)rcObj.bottom - textLayout.GetTop();
	if (textLayout.Height > 0.000001)
	{
		strText.Format(_T("信号源: %d"), m_iSrcPort);
		MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
		pGraphics->DrawString(wszText, -1, &font, textLayout, &format, &sBrush);
	}

	//叠放顺序
	textLayout = RectF((REAL)rcTmp.left+2, (REAL)rcTmp.top+1+48, (REAL)rcTmp.Width()-2, (REAL)rcTmp.Height()-1);
	if (textLayout.GetBottom() > (REAL)rcObj.bottom)
		textLayout.Height = (REAL)rcObj.bottom - textLayout.GetTop();
	if (textLayout.Height > 0.000001)
	{
		strText.Format(_T("叠放顺序: %d"), m_iZorder);
		MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
		pGraphics->DrawString(wszText, -1, &font, textLayout, &format, &sBrush);
	}

	//窗口位置
	textLayout = RectF((REAL)rcTmp.left+2, (REAL)rcTmp.top+1+64, (REAL)rcTmp.Width()-2, (REAL)rcTmp.Height()-1);
	if (textLayout.GetBottom() > (REAL)rcObj.bottom)
		textLayout.Height = (REAL)rcObj.bottom - textLayout.GetTop();
	if (textLayout.Height > 0.000001)
	{
		strText.Format(_T("窗口位置: <%d,%d>"), m_rcPhy.left, m_rcPhy.top);
		MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
		pGraphics->DrawString(wszText, -1, &font, textLayout, &format, &sBrush);
	}

	//窗口大小
	textLayout = RectF((REAL)rcTmp.left+2, (REAL)rcTmp.top+1+80, (REAL)rcTmp.Width()-2, (REAL)rcTmp.Height()-1);
	if (textLayout.GetBottom() > (REAL)rcObj.bottom)
		textLayout.Height = (REAL)rcObj.bottom - textLayout.GetTop();
	if (textLayout.Height > 0.000001)
	{
		strText.Format(_T("窗口大小: <%d*%d>"), m_rcPhy.Width(), m_rcPhy.Height());
		MultiByteToWideChar(CP_ACP, 0, strText.GetBuffer(), -1, wszText, _MAX_SIGNAL_NAME);
		pGraphics->DrawString(wszText, -1, &font, textLayout, &format, &sBrush);
	}

	if (!m_bLocked)
	{
		//关闭按钮
		CBitmap* pBitmap = new CBitmap;

		pBitmap->LoadBitmap(IDB_BMP_CLOSE);

		Image* pImgClose;
		if (BitmapToImage(pBitmap, pImgClose))
		{
			rcTmp = m_rcBtnExit;
			pGraphics->DrawImage(pImgClose, rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height());
		}

		pBitmap->DeleteObject();

		//还原按钮
		pBitmap->LoadBitmap(IDB_BMP_RESTORE);

		Image* pImgRestore;
		if (BitmapToImage(pBitmap, pImgRestore))
		{
			rcTmp = m_rcBtnRestore;
			pGraphics->DrawImage(pImgRestore, rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height());
		}

		pBitmap->DeleteObject();

		//最大化按钮
		pBitmap->LoadBitmap(IDB_BMP_MAX);

		Image* pImgMax;
		if (BitmapToImage(pBitmap, pImgMax))
		{
			rcTmp = m_rcBtnMaximize;
			pGraphics->DrawImage(pImgMax, rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height());
		}

		pBitmap->DeleteObject();

		//对齐按钮
		pBitmap->LoadBitmap(IDB_BMP_ALIGN);

		Image* pImgAlign;
		if (BitmapToImage(pBitmap, pImgAlign))
		{
			rcTmp = m_rcBtnAlign;
			pGraphics->DrawImage(pImgAlign, rcTmp.left, rcTmp.top, rcTmp.Width(), rcTmp.Height());
		}

		pBitmap->DeleteObject();

		SAFE_DELETE(pBitmap);
	}
	else
	{
		sBrush.SetColor(Color(iTransp, 110, 135, 90));
		pGraphics->FillRectangle(&sBrush, rcObj.left, rcObj.top, rcObj.Width(), rcObj.Height());
	}
}

// 原则，物理尺寸不变，缩放逻辑尺寸
void CDrawWndObj::Zoom()
{
	m_rcLastLog = m_rcLog;

	CRect rcPage = m_pOwner->GetPosition();

	m_rcLog = m_rcPhy.MulDiv(m_sxLogScalar, m_sxPhyScalar);
	m_rcLog.OffsetRect(rcPage.left, rcPage.top);

	RecalcLayOut();
}

void CDrawWndObj::OnGraphChanged()
{
	m_pOwner->Repaint(FALSE);
}

void CDrawWndObj::OnPopupMenu(CPoint point)
{
	//弹出菜单
	if (afxContextMenuManager == NULL)
		return;

	if ((::GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0 && (::GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0)
		return;

	CWnd* pWnd = m_pOwner->m_pOwner;
	if (pWnd == NULL)
		return;

	CMFCPopupMenu* pPopupMenu = DYNAMIC_DOWNCAST(CMFCPopupMenu, pWnd);
	
	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, ID_RIBBON_CMD_WND_PROPERTY, _T("属性"));
	menu.AppendMenu(MF_SEPARATOR);

	menu.AppendMenu(MF_STRING, ID_RIBBON_CMD_WND_TOP, _T("置顶"));
	menu.AppendMenu(MF_STRING, ID_RIBBON_CMD_WND_BOTTOM, _T("置底"));
	menu.AppendMenu(MF_STRING, ID_RIBBON_CMD_WND_FRONT, _T("上移"));
	menu.AppendMenu(MF_STRING, ID_RIBBON_CMD_WND_BACK, _T("下移"));
	menu.AppendMenu(MF_SEPARATOR);

	menu.AppendMenu(MF_STRING, ID_RIBBON_CMD_FILL_ANCHORED_SCREEN, _T("缩放到单个屏"));
	menu.AppendMenu(MF_SEPARATOR);
	//menu.AppendMenu(MF_STRING, ID_RIBBON_CMD_FILL_ACROSSED_SCREEN, _T("铺满到所占屏"));
	menu.AppendMenu(MF_STRING, ID_RIBBON_CMD_WND_LOCK, _T("锁定此窗口"));
	menu.CheckMenuItem(ID_RIBBON_CMD_WND_LOCK, (m_bLocked ? MF_CHECKED : MF_UNCHECKED)|MF_BYCOMMAND);
	menu.AppendMenu(MF_SEPARATOR);
	menu.AppendMenu(MF_STRING, ID_RIBBON_CMD_WND_CLOSE, _T("关闭"));
	//menu.EnableMenuItem(ID_RIBBON_CMD_WND_CLOSE, (m_bLocked ? MF_DISABLED : MF_ENABLED)|MF_BYCOMMAND);

	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	if (pPopupMenu != NULL)
		afxContextMenuManager->SetDontCloseActiveMenu();

	int nMenuResult = afxContextMenuManager->TrackPopupMenu(menu, ptCursor.x, ptCursor.y, pWnd);

	if (pPopupMenu != NULL)
		afxContextMenuManager->SetDontCloseActiveMenu(FALSE);

	switch(nMenuResult)
	{
	case ID_RIBBON_CMD_WND_TOP:
		{
			m_pOwner->MovetoTop(this);
			break;
		}
	case ID_RIBBON_CMD_WND_BOTTOM:
		{
			m_pOwner->MovetoBottom(this);		
			break;
		}
	case ID_RIBBON_CMD_WND_FRONT:
		{
			m_pOwner->MoveForward(this);
			break;
		}
	case ID_RIBBON_CMD_WND_BACK:
		{
			m_pOwner->MoveBackward(this);
			break;
		}
	case ID_RIBBON_CMD_FILL_ANCHORED_SCREEN:
		{
			m_pOwner->FillWndtoAnchoredScreen(this);
			break;
		}
	case ID_RIBBON_CMD_FILL_ACROSSED_SCREEN:
		{
			m_pOwner->FillWndtoAcrossedScreen(this);
			break;
		}
	case ID_RIBBON_CMD_WND_LOCK:
		{
			SetLock(!m_bLocked);
			m_pOwner->Repaint(FALSE);
			break;
		}
	case ID_RIBBON_CMD_WND_PROPERTY:
		{
			m_pOwner->ActiveWnd(this);
			::SendMessage(m_pOwner->m_pOwner->m_hWnd, WM_COMMAND, WPARAM(ID_RIBBON_CMD_WND_PROPERTY), NULL);
			break;
		}
	case ID_RIBBON_CMD_WND_CLOSE:
		{
			m_pOwner->ActiveWnd(this);
			::SendMessage(m_pOwner->m_pOwner->m_hWnd, WM_COMMAND, WPARAM(ID_RIBBON_CMD_WND_CLOSE), NULL);
			break;
		}
	default:break;
	}

	if (pPopupMenu != NULL)
	{
		pPopupMenu->SendMessage(WM_CLOSE);
	}
}

void CDrawWndObj::OnLBtnDown(CPoint point)
{
	m_ptLBtnDown = point;

	if (!m_bLocked)
	{
		//SetCursor Shape
		m_HitTest = OnHitTest(point, TRUE);
	}

	if (!m_bActived)
		m_pOwner->ActiveWnd(this);
}

void CDrawWndObj::OnLBtnUp(CPoint point)
{
	//鼠标左键弹起事务处理

	//{
	// 窗口上的按钮点击处理
	if (PtInRect(m_rcBtnExit, point) && (m_HitTest == MYHTCLOSE))
	{
		return OnClose();
	}
	else if (PtInRect(m_rcBtnMaximize, point) && (m_HitTest == MYHTMAXMIZE))
	{
		OnMaximize();
	}
	else if (PtInRect(m_rcBtnAlign, point) && (m_HitTest == MYHTALIGNTO))
	{
		OnAlignTo();
	}
	else if (PtInRect(m_rcBtnRestore, point) && (m_HitTest == MYHTRESTORE))
	{
		OnRestore();
	}
	//}

	if ((m_HitTest & MYHTCLIENT) == MYHTCLIENT)
	{
		// 发送MOVE命令
		Move();
	}
	else if ((m_HitTest & MYHTRESIZE) > 0)
	{
		// 发送SIZE命令
		Size();
	}

	//重置鼠标追踪标志
	LBtnUpReturn();
}

void CDrawWndObj::LBtnUpReturn()
{
	m_HitTest = MYHTNOWHERE;
	m_ptLBtnDown = CPoint(-1, -1);
}

void CDrawWndObj::OnLBtnDblClick(CPoint point)
{
	if (m_bLocked)
		return;

	if (PtInRect(m_rcTitle, point))
	{// 标题上双击
		if (m_pOwner->IsMaximized(this))
			OnRestore();
		else
			OnMaximize();
	}
	else
	{// 窗体上双击
		if (m_pOwner->IsAligned(this))
			OnRestore();
		else
			OnAlignTo();
	}
}

void CDrawWndObj::OnMouseMove(CPoint point)
{
	// 锁定的窗口
	if (m_bLocked)
		return;

	// 仅仅是鼠标移动
	if ((m_HitTest & MYHTNOWHERE) == MYHTNOWHERE)
	{
		OnHitTest(point, FALSE);
		return;
	}

	// 功能按钮上的点击
	if (m_HitTest > MYHTNOWHERE)
	{
		return;
	}

	CRect rcPage = m_pOwner->GetPosition();
	CPoint pt = point;	
	pt.Offset(-rcPage.TopLeft());

	int iMinSize = 20;
	CRect rcPhy= m_rcPhy;

	double dScale = double(m_sxPhyScalar)/double(m_sxLogScalar);

	if ((m_HitTest & MYHTLEFT) == MYHTLEFT)
	{// 调整左边框
		if ((point.x + iMinSize) < m_rcLog.right)
			rcPhy.left = Round((pt.x)*dScale);
		else
			rcPhy.left = rcPhy.right - Round(iMinSize*dScale);
	}
	if ((m_HitTest & MYHTTOP) == MYHTTOP)
	{// 调整上边框
		if ((point.y + iMinSize) < m_rcLog.bottom)
			rcPhy.top = Round(pt.y*dScale);
		else
			rcPhy.top = rcPhy.bottom - Round(iMinSize*dScale);
	}
	if ((m_HitTest & MYHTRIGHT) == MYHTRIGHT)
	{// 调整右边框
		if ((point.x - iMinSize) > m_rcLog.left)
			rcPhy.right = Round(pt.x*dScale);
		else
			rcPhy.right = rcPhy.left + Round(iMinSize*dScale);
	}
	if ((m_HitTest & MYHTBOTTOM) == MYHTBOTTOM)
	{// 调整下边框
		if ((point.y - iMinSize) > m_rcLog.top)
			rcPhy.bottom = Round(pt.y*dScale);
		else
			rcPhy.bottom = rcPhy.top + Round(iMinSize*dScale);
	}
	if ((m_HitTest & MYHTCLIENT) == MYHTCLIENT)
	{// 移动
		rcPhy.OffsetRect(Round((point.x-m_ptLBtnDown.x)*dScale), Round((point.y-m_ptLBtnDown.y)*dScale));
		m_ptLBtnDown = point;
	}

	if ((m_HitTest & 0x1F) > 0)
	{
		SetPhyPos(rcPhy);
		m_pOwner->Repaint(FALSE);
	}

	OnHitTest(point, FALSE);
}

int CDrawWndObj::OnHitTest(CPoint point, BOOL bTest)
{
	CRect rcL( m_rcLog.left,    m_rcLog.top+6,    m_rcLog.left+5,   m_rcLog.bottom-6);
	CRect rcT( m_rcLog.left+6,  m_rcLog.top,      m_rcLog.right-6,  m_rcLog.top+5);
	CRect rcR( m_rcLog.right-5, m_rcLog.top+6,    m_rcLog.right,    m_rcLog.bottom-6);
	CRect rcB( m_rcLog.left+6,  m_rcLog.bottom-5, m_rcLog.right-6,  m_rcLog.bottom);
	CRect rcTL(m_rcLog.left,    m_rcLog.top,      m_rcLog.left+5,   m_rcLog.top+5);
	CRect rcTR(m_rcLog.right-5, m_rcLog.top,      m_rcLog.right,    m_rcLog.top+5);
	CRect rcBL(m_rcLog.left,    m_rcLog.bottom-5, m_rcLog.left+5,   m_rcLog.bottom);
	CRect rcBR(m_rcLog.right-5, m_rcLog.bottom-5, m_rcLog.right,    m_rcLog.bottom);

	int HitTest = MYHTNOWHERE;

	if (PtInRect(rcL, point))      { HitTest = MYHTLEFT;        SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE))); }
	else if (PtInRect(rcT, point)) { HitTest = MYHTTOP;         SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); }
	else if (PtInRect(rcR, point)) { HitTest = MYHTRIGHT;       SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE))); }
	else if (PtInRect(rcB, point)) { HitTest = MYHTBOTTOM;      SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); }
	else if (PtInRect(rcTL, point)){ HitTest = MYHTTOPLEFT;     SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))); }
	else if (PtInRect(rcTR, point)){ HitTest = MYHTTOPRIGHT;    SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); }
	else if (PtInRect(rcBL, point)){ HitTest = MYHTBOTTOMLEFT;  SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); }
	else if (PtInRect(rcBR, point)){ HitTest = MYHTBOTTOMRIGHT; SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))); }
	else if (PtInRect(m_rcBtnAlign, point))   { HitTest = MYHTALIGNTO; SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))); }
	else if (PtInRect(m_rcBtnMaximize, point)){ HitTest = MYHTMAXMIZE; SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))); }
	else if (PtInRect(m_rcBtnRestore, point)) { HitTest = MYHTRESTORE; SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))); }
	else if (PtInRect(m_rcBtnExit, point))    { HitTest = MYHTCLOSE;   SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))); }
	else {
		if (bTest) {
			HitTest = MYHTCLIENT; SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL)));
		}
	}

	if (!bTest)
		HitTest = MYHTNOWHERE;

	return HitTest;
}

void CDrawWndObj::OnClose()
{
	if (m_bLocked)
		return;

	// 不能自己销毁自己
	m_pOwner->ActiveWnd(this);
	::SendMessage(m_pOwner->m_pOwner->m_hWnd, WM_COMMAND, WPARAM(ID_RIBBON_CMD_WND_CLOSE), NULL);
}

void CDrawWndObj::OnMaximize()
{
	if (m_bLocked)
		return;

	SetPosMark();

	m_pOwner->MaximizeWnd(this);

	Size();
}

void CDrawWndObj::OnAlignTo()
{
	if (m_bLocked)
		return;

	SetPosMark();

	m_pOwner->AlignWnd(this);

	Size();
}

void CDrawWndObj::OnRestore()
{
	if (m_bLocked)
		return;

	m_pOwner->RestoreWnd(this);

	Size();
}

int CDrawWndObj::Open()
{
	CRect rc = m_rcPhy;
	rc.InflateRect(0,0,-1,-1);

	return \
	GetCmdManager()->CmdOPEN(SYSBOARD_PORT, 
                             eBoardTypeCtrl, 
                             m_iWndID,
                             m_iSrcPort,
                             0,
                             rc);
}

int CDrawWndObj::Move()
{
	CRect rc = m_rcPhy;
	rc.InflateRect(0,0,-1,-1);

	return \
	GetCmdManager()->CmdMOVE(SYSBOARD_PORT, 
                             eBoardTypeCtrl, 
                             m_iWndID,
                             m_iSrcPort,
                             0,
                             rc);
}

int CDrawWndObj::Size()
{
	CRect rc = m_rcPhy;
	rc.InflateRect(0,0,-1,-1);

	return \
	GetCmdManager()->CmdSIZE(SYSBOARD_PORT, 
                             eBoardTypeCtrl, 
                             m_iWndID,
                             m_iSrcPort,
                             0,
                             rc);
}

int CDrawWndObj::MoveS()
{
	return \
	GetCmdManager()->CmdMOVS(SYSBOARD_PORT, eBoardTypeCtrl, m_iWndID, m_iSrcPort);
}

int CDrawWndObj::MoveZ()
{
	return \
	GetCmdManager()->CmdMOVZ(SYSBOARD_PORT, eBoardTypeCtrl, m_iWndID, m_iZorder);
}

int CDrawWndObj::Shut( const int nID )
{
	return \
	GetCmdManager()->CmdSHUT(SYSBOARD_PORT, eBoardTypeCtrl, nID);
}