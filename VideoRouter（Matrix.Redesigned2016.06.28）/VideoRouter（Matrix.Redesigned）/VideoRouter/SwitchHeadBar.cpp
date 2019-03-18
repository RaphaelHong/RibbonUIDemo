// SwitchHeadBar.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "SwitchHeadBar.h"
#include "SwitchPage.h"
#include "WaittingMsg.h"

//////////////////////////////////////////////////////////////////////////
// CSwitchHeadBar


int CSwitchHeadBar::m_sFirstVisiableItem = 0;

IMPLEMENT_DYNAMIC(CSwitchHeadBar, CWnd)

CSwitchHeadBar::CSwitchHeadBar()
{
	ResetFlags();

	m_FocusItem = -1;
}

CSwitchHeadBar::~CSwitchHeadBar()
{
}


// CSwitchHeadBar 成员函数
BEGIN_MESSAGE_MAP(CSwitchHeadBar, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(ID_BNITEM_FRESH_ALL, &CSwitchHeadBar::OnRefreshAll)
	ON_MESSAGE(ID_BNITEM_FARTOP, &CSwitchHeadBar::OnFarTop)
	ON_MESSAGE(ID_BNITEM_PAGEUP, &CSwitchHeadBar::OnPageUp)
	ON_MESSAGE(ID_BNITEM_FARLEFT, &CSwitchHeadBar::OnFarLeft)
	ON_MESSAGE(ID_BNITEM_PAGELEFT, &CSwitchHeadBar::OnPageLeft)
	ON_MESSAGE(ID_BNITEM_PAGERIGHT, &CSwitchHeadBar::OnPageRight)
	ON_MESSAGE(ID_BNITEM_FARRIGHT, &CSwitchHeadBar::OnFarRight)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void CSwitchHeadBar::SetBarRect(const CRect rc)
{
	m_rcBar = rc;
}

BOOL CSwitchHeadBar::BarCreate(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const CRect& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /* = NULL */ )
{
	//RECT rcWnd;
	//rcWnd.left = rect.GetLeft();
	//rcWnd.right = rect.GetRight();
	//rcWnd.top = rect.GetTop();
	//rcWnd.bottom = rect.GetBottom();

	if (CreateEx(0,
				AfxRegisterWndClass(0, LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))),
				0,
				dwStyle, 
				rect,
				pParentWnd,
				nID ) )
	{
		SetBnItems();
		
		SetTooltips();

		return TRUE;
	}

	return FALSE;		
}

int CSwitchHeadBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetBnItems();

	SetTooltips();

	return 0;
}

void CSwitchHeadBar::SetTooltips()
{
	m_tips.Create(this);
	if (m_bnFresh.GetSafeHwnd())
		m_tips.AddTool(&m_bnFresh, _T("点击刷新所有输入信号"));
	if (m_bnFarLeft.GetSafeHwnd())
		m_tips.AddTool(&m_bnFarLeft, _T("输出列表滚动至第一页"));
	if (m_bnPageLeft.GetSafeHwnd())
		m_tips.AddTool(&m_bnPageLeft, _T("输出列表向左滚动一页"));
	if (m_bnFarTop.GetSafeHwnd())
		m_tips.AddTool(&m_bnFarTop, _T("信号列表滚动至第一页"));
	if (m_bnPageUp.GetSafeHwnd())
		m_tips.AddTool(&m_bnPageUp, _T("信号列表向上滚动一页"));
	if (m_bnFarRight.GetSafeHwnd())
		m_tips.AddTool(&m_bnFarRight, _T("输出列表滚动至末页"));
	if (m_bnPageRight.GetSafeHwnd())
		m_tips.AddTool(&m_bnPageRight, _T("输出列表向右滚动一页"));
	if (m_bnLeftReBar.GetSafeHwnd())
		m_tips.AddTool(&m_bnLeftReBar, _T("调整信号源列表的宽度"));

	m_tips.SetDelayTime(200);
}

void CSwitchHeadBar::SetBnItems()
{
	CRect rc=m_rcBar;
	rc.OffsetRect(-rc.TopLeft());

	const CPageMargins *pMargins = ((CSwitchPage*)GetParent())->GetMargins();

	Rect rcBnItem;
	
	// 刷新按钮
	rcBnItem = Rect(rc.left+15, (rc.Height()-18)/2, 18, 18);
	m_bnFresh.SetItemRect(rcBnItem);
	if (m_bnFresh.GetSafeHwnd())
		m_bnFresh.MoveWindow(rcBnItem.X, rcBnItem.Y, rcBnItem.Width, rcBnItem.Height);
	else 
	{
		m_bnFresh.SetImages(IDB_PNG_REFRESH, IDB_PNG_REFRESH_FOCUS, IDB_PNG_REFRESH_CK, IDB_PNG_REFRESH);
		m_bnFresh.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBnItem, this, ID_BNITEM_FRESH_ALL);
	}

	// 上翻页按钮
	rcBnItem = Rect(rcBnItem.GetRight()+20, (rc.Height()-18)/2, 18, 18);
	m_bnFarTop.SetItemRect(rcBnItem);
	if (m_bnFarTop.GetSafeHwnd())
		m_bnFarTop.MoveWindow(rcBnItem.X, rcBnItem.Y, rcBnItem.Width, rcBnItem.Height);
	else
	{
		m_bnFarTop.SetImages(IDB_PNG_PAGELEFT, IDB_PNG_PAGELEFT_FOCUS, IDB_PNG_PAGELEFT_CK, IDB_PNG_PAGELEFT_DIS);
		m_bnFarTop.SetRotateFlipType(Rotate90FlipNone);
		m_bnFarTop.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBnItem, this, ID_BNITEM_FARTOP);
		//m_bnPageUp.Enable(FALSE);
	}

	// 上一个按钮
	rcBnItem = Rect(rcBnItem.GetRight()+5, (rc.Height()-18)/2, 18, 18);
	m_bnPageUp.SetItemRect(rcBnItem);
	if (m_bnPageUp.GetSafeHwnd())
		m_bnPageUp.MoveWindow(rcBnItem.X, rcBnItem.Y, rcBnItem.Width, rcBnItem.Height);
	else
	{
		m_bnPageUp.SetImages(IDB_PNG_PAGEPREV, IDB_PNG_PAGEPREV_FOCUS, IDB_PNG_PAGEPREV_CK, IDB_PNG_PAGEPREV_DIS);
		m_bnPageUp.SetRotateFlipType(Rotate90FlipNone);
		m_bnPageUp.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBnItem, this, ID_BNITEM_PAGEUP);
		//m_bnPrevUp.Enable(FALSE);
	}

	// 左一个按钮
	rcBnItem = Rect(pMargins->HeadBarMargin.iColumnWidthFst-5-18, (rc.Height()-18)/2, 18, 18);
	m_bnPageLeft.SetItemRect(rcBnItem);
	if (m_bnPageLeft.GetSafeHwnd())
		m_bnPageLeft.MoveWindow(rcBnItem.X, rcBnItem.Y, rcBnItem.Width, rcBnItem.Height);
	else
	{
		m_bnPageLeft.SetImages(IDB_PNG_PAGEPREV, IDB_PNG_PAGEPREV_FOCUS, IDB_PNG_PAGEPREV_CK, IDB_PNG_PAGEPREV_DIS);
		m_bnPageLeft.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBnItem, this, ID_BNITEM_PAGELEFT);
	}

	// 左翻页按钮
	rcBnItem.Offset(-5-rcBnItem.Width, 0);
	m_bnFarLeft.SetItemRect(rcBnItem);
	if (m_bnFarLeft.GetSafeHwnd())
		m_bnFarLeft.MoveWindow(rcBnItem.X, rcBnItem.Y, rcBnItem.Width, rcBnItem.Height);
	else
	{
		m_bnFarLeft.SetImages(IDB_PNG_PAGELEFT, IDB_PNG_PAGELEFT_FOCUS, IDB_PNG_PAGELEFT_CK, IDB_PNG_PAGELEFT_DIS);
		m_bnFarLeft.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBnItem, this, ID_BNITEM_FARLEFT);
	}

	// 左调整按钮
	rcBnItem = Rect(pMargins->HeadBarMargin.iColumnWidthFst+1, (rc.Height()-20)/2, 5, 20);
	m_bnLeftReBar.SetItemRect( rcBnItem );
	if (m_bnLeftReBar.GetSafeHwnd())
		m_bnLeftReBar.MoveWindow(rcBnItem.X, rcBnItem.Y, rcBnItem.Width, rcBnItem.Height);
	else
	{
		m_bnLeftReBar.SetImages(IDB_PNG_RESIZEBAR_VERT, IDB_PNG_RESIZEBAR_VERT, IDB_PNG_RESIZEBAR_VERT, IDB_PNG_RESIZEBAR_VERT);
		m_bnLeftReBar.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBnItem, this, ID_BNITEM_LREBAR);
	}

	// 右调整按钮
	rcBnItem = Rect(pMargins->HeadBarMargin.iColumnWidthFst+pMargins->HeadBarMargin.iColumnWidthSec-5, (rc.Height()-20)/2, 5, 20);
	m_bnRightReBar.SetItemRect(rcBnItem);
	if (m_bnRightReBar.GetSafeHwnd())
		m_bnRightReBar.MoveWindow(rcBnItem.X, rcBnItem.Y, rcBnItem.Width, rcBnItem.Height);
	else
	{
		m_bnRightReBar.SetImages(IDB_PNG_RESIZEBAR_VERT, IDB_PNG_RESIZEBAR_VERT, IDB_PNG_RESIZEBAR_VERT, IDB_PNG_RESIZEBAR_VERT);
		m_bnRightReBar.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBnItem, this, ID_BNITEM_RREBAR);
	}

	// 右一个按钮
	rcBnItem = Rect(rc.Width()-pMargins->HeadBarMargin.iColumnWidthThd+5, (rc.Height()-18)/2, 18, 18);
	m_bnPageRight.SetItemRect(rcBnItem);
	if (m_bnPageRight.GetSafeHwnd())
		m_bnPageRight.MoveWindow(rcBnItem.X, rcBnItem.Y, rcBnItem.Width, rcBnItem.Height);
	else
	{
		m_bnPageRight.SetImages(IDB_PNG_PAGEPREV, IDB_PNG_PAGEPREV_FOCUS, IDB_PNG_PAGEPREV_CK, IDB_PNG_PAGEPREV_DIS);
		m_bnPageRight.SetRotateFlipType(Rotate180FlipNone);
		m_bnPageRight.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBnItem, this, ID_BNITEM_PAGERIGHT);
	}

	// 右翻页按钮
	rcBnItem = Rect(rcBnItem.GetRight()+5, rcBnItem.GetTop(), 18, 18);
	m_bnFarRight.SetItemRect(rcBnItem);
	if (m_bnFarRight.GetSafeHwnd())
		m_bnFarRight.MoveWindow(rcBnItem.X, rcBnItem.Y, rcBnItem.Width, rcBnItem.Height);
	else
	{
		m_bnFarRight.SetImages(IDB_PNG_PAGELEFT, IDB_PNG_PAGELEFT_FOCUS, IDB_PNG_PAGELEFT_CK, IDB_PNG_PAGELEFT_DIS);
		m_bnFarRight.SetRotateFlipType(Rotate180FlipNone);
		m_bnFarRight.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBnItem, this, ID_BNITEM_FARRIGHT);
		//m_bnPageRight.Enable(FALSE);
	}
}

void CSwitchHeadBar::EnableWizardBn(UINT nWizardCode, BOOL bEnable/*=TRUE*/)
{
	if ((nWizardCode & WizardTop) == WizardTop)
	{
		m_bnFarTop.Enable(bEnable);
		m_bnPageUp.Enable(bEnable);
	}

	if ((nWizardCode & WizardLeft) == WizardLeft)
	{
		m_bnFarLeft.Enable(bEnable);
		m_bnPageLeft.Enable(bEnable);
		
	}

	if ((nWizardCode & WizardRight) == WizardRight)
	{
		m_bnPageRight.Enable(bEnable);
		m_bnFarRight.Enable(bEnable);
	}

	//if ((nWizardCode & Wizardbottom) == Wizardbottom)
	//{

	//}
}

void CSwitchHeadBar::RecalcLayout()
{
	SetBnItems();
}


void CSwitchHeadBar::OnPaint()
{
	CPaintDC dc(this);

	//****** 获取边距量
	const CPageMargins *pMargins = ((CSwitchPage*)GetParent())->GetMargins();

	//****** 获取客户区位置
	CRect rcCli;
	GetClientRect(&rcCli);

	//****** GDI+对象
	Graphics gp(dc.m_hDC);
	gp.SetSmoothingMode(SmoothingModeHighSpeed);

	//****** 填充背景图
	Image* imgHeadBar;
	if (ImageFromIDResource(IDB_PNG_HEADBAR, "png", imgHeadBar))
	{
		gp.DrawImage(imgHeadBar, rcCli.left, rcCli.top, rcCli.Width(), rcCli.Height());
		SAFE_DELETE(imgHeadBar);
	}

	CRect rcColumn;

	//****** 第一列
	rcColumn = rcCli;
	rcColumn.right = rcColumn.left + pMargins->HeadBarMargin.iColumnWidthFst;
	//DrawFirstColumn(&gp, rcColumn);

	//****** 第二列
	rcColumn.left  = rcCli.left + pMargins->HeadBarMargin.iColumnWidthFst + 1;
	rcColumn.right = rcColumn.left + pMargins->HeadBarMargin.iColumnWidthSec;
	DrawSecondColumn(&gp, rcColumn);

	//****** 第三列
	rcColumn.left  = rcColumn.right + 1;
	rcColumn.right = rcCli.right;
	//DrawThirdColumn(&gp, rcColumn);
}

void CSwitchHeadBar::DrawSecondColumn(Graphics* pGraphics, CRect rcColumn)
{
	ASSERT(pGraphics);

	//****** 获取边距量
	const CPageMargins *pMargins = ((CSwitchPage*)GetParent())->GetMargins();

	Rect rcImg;

	//****** 输出通道

	int iStartPos = rcColumn.left + 2 + 3;
	int iItemWidth  = pMargins->HeadBarMargin.iItemWidth;
	int iItemHeight = pMargins->HeadBarMargin.iItemHeight;
	int iMaxVisiableItems = pMargins->HeadBarMargin.iMaxVisiableItemsOnHorz;

	// 先画通道的背景图片
	Image* imgPortBk, *imgPortBkFocus, *imgPortBkDis;
	if (ImageFromIDResource( IDB_PNG_PORTBK, "png", imgPortBk)
	 && ImageFromIDResource( IDB_PNG_PORTBK_FOCUS, "png", imgPortBkFocus)
	 && ImageFromIDResource( IDB_PNG_PORTBK_DIS, "png", imgPortBkDis))
	{
		//rcImg.Y = 2;//(iItemHeight - imgPortBk->GetHeight())/2;
		rcImg.Width  = iItemWidth-4;//imgPortBk->GetWidth();
		rcImg.Height = iItemHeight-4;//imgPortBk->GetHeight();
		int mDim = min(rcImg.Width, rcImg.Height);
		rcImg.Width = mDim;
		rcImg.Height= mDim;
		rcImg.Y = (iItemHeight-rcImg.Height)/2;
		int offset = iStartPos + (iItemWidth - rcImg.Width)/2;

		for (int iItem=0; iItem < iMaxVisiableItems; iItem++)
		{
			rcImg.X = offset + iItem*iItemWidth;
			
			// 焦点通道标记图
			if (iItem == m_FocusItem)
				pGraphics->DrawImage(imgPortBkFocus, rcImg);
			else
			{
				if (GetOutputManager()->GetFeederID(iItem+m_sFirstVisiableItem, FALSE) != INVALID_VALUE)
					pGraphics->DrawImage(imgPortBk, rcImg);
				else
					pGraphics->DrawImage(imgPortBkDis, rcImg);
			}
		}

		SAFE_DELETE(imgPortBk);
		SAFE_DELETE(imgPortBkFocus);
		SAFE_DELETE(imgPortBkDis);
	}

	// 后画通道号
	FontFamily fontFamily(L"宋体");
	Gdiplus::Font font(&fontFamily, (REAL)pMargins->PageScale.iFontHeight, FontStyleRegular, UnitPixel);
	Gdiplus::Font Bfont(&fontFamily, (REAL)pMargins->PageScale.iFontHeight, FontStyleBold, UnitPixel);
	SolidBrush blackBr(Color(255, 116, 101, 131));
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingEllipsisWord);	
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	CStringW strChlName;

	RectF rcItem;	
	rcItem.Y = (REAL)rcColumn.top;
	rcItem.Width  = (REAL)iItemWidth;
	rcItem.Height = (REAL)iItemHeight;

	for (int iItem=0; iItem < iMaxVisiableItems; iItem++)
	{
		rcItem.X = (REAL)(iItem*iItemWidth + iStartPos);

		strChlName.Format(L"%d", GetOutputManager()->GetCmdPort(iItem + m_sFirstVisiableItem, FALSE));
		
		if (iItem == m_FocusItem)
			pGraphics->DrawString(strChlName, -1, &Bfont, rcItem, &format, &blackBr);
		else
			pGraphics->DrawString(strChlName, -1, &font, rcItem, &format, &blackBr);
	}

}


void CSwitchHeadBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	CWnd::OnLButtonDown(nFlags, point);
}

void CSwitchHeadBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);
}

void CSwitchHeadBar::OnMouseMove(UINT nFlags, CPoint point)
{
	CSwitchSourceBar* pSourceBar = (CSwitchSourceBar*)((CSwitchPage*)GetParent())->GetSourceBar();
	if (!pSourceBar->IsEditing())
		SetFocus();

	OnHitTest(point);

	CWnd::OnMouseMove(nFlags, point);
}

void CSwitchHeadBar::OnHitTest(CPoint point)
{
	CRect rcCli;
	GetClientRect(&rcCli);

	CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

	Rect rcColumnSec;

	//rcColumnFst = Rect(rcCli.left, rcCli.top, pMargins->HeadBarMargin.iColumnWidthFst, rcCli.Height());
	rcColumnSec = Rect(rcCli.left + pMargins->HeadBarMargin.iColumnWidthFst +6, rcCli.top, pMargins->HeadBarMargin.iColumnWidthSec-10, rcCli.Height());
	//rcColumnThd = Rect(rcColumnSec.GetRight()+1, rcCli.top, pMargins->HeadBarMargin.iColumnWidthThd, rcCli.Height());

	int PrevFocusItem = m_FocusItem;
	if (rcColumnSec.Contains(point.x, point.y))
	{
		m_FocusItem = (point.x - rcColumnSec.GetLeft())/pMargins->HeadBarMargin.iItemWidth;
	}
	else
		m_FocusItem = -1;

	if (PrevFocusItem != m_FocusItem)
	{
		CRect rcPrev, rcFocus;

		if (PrevFocusItem != -1)
		{
			rcPrev.left   = rcColumnSec.GetLeft() + pMargins->HeadBarMargin.iItemWidth*PrevFocusItem;
			rcPrev.top    = rcColumnSec.GetTop();
			rcPrev.right  = rcPrev.left + pMargins->HeadBarMargin.iItemWidth;
			rcPrev.bottom = rcColumnSec.GetBottom();
			InvalidateRect(rcPrev);
		}
		
		if (m_FocusItem != -1)
		{
			rcFocus.left  = rcColumnSec.GetLeft() + pMargins->HeadBarMargin.iItemWidth*m_FocusItem;
			rcFocus.top   = rcColumnSec.GetTop();
			rcFocus.right = rcFocus.left + pMargins->HeadBarMargin.iItemWidth;
			rcFocus.bottom = rcColumnSec.GetBottom();
			InvalidateRect(rcFocus);
		}

		SetTimer(TIMER_KILLFOCUS, 100, NULL);
	}

	((CSwitchSelectBar*)((CSwitchPage*)GetParent())->GetSelectBar())->SetFocusBox(m_FocusItem, -1);
}


void CSwitchHeadBar::SetFocusItem(int nItem)
{
	if (m_FocusItem == nItem)
		return;

	int PrevFocusItem = m_FocusItem;
	m_FocusItem = nItem;

	CRect rcCli;
	GetClientRect(&rcCli);

	CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

	Rect rcColumnSec;
	rcColumnSec = Rect(rcCli.left + pMargins->HeadBarMargin.iColumnWidthFst +6, rcCli.top, pMargins->HeadBarMargin.iColumnWidthSec-10, rcCli.Height());

	CRect rcPrev, rcFocus;
	if (PrevFocusItem != -1)
	{
		rcPrev.left   = rcColumnSec.GetLeft() + pMargins->HeadBarMargin.iItemWidth*PrevFocusItem;
		rcPrev.top    = rcColumnSec.GetTop();
		rcPrev.right  = rcPrev.left + pMargins->HeadBarMargin.iItemWidth;
		rcPrev.bottom = rcColumnSec.GetBottom();
		InvalidateRect(rcPrev);
	}

	if (m_FocusItem != -1)
	{
		rcFocus.left  = rcColumnSec.GetLeft() + pMargins->HeadBarMargin.iItemWidth*m_FocusItem;
		rcFocus.top   = rcColumnSec.GetTop();
		rcFocus.right = rcFocus.left + pMargins->HeadBarMargin.iItemWidth;
		rcFocus.bottom = rcColumnSec.GetBottom();
		InvalidateRect(rcFocus);
	}
}


void CSwitchHeadBar::ResetFlags()
{
	m_bLBnDown = FALSE;
	m_bResizing = FALSE;
}

LRESULT CSwitchHeadBar::OnRefreshAll(WPARAM wParam, LPARAM lParam)
{
	GetInitManager()->SetTransactionType(eTransaction_RefreshSignal);
	GetInitManager()->BeginTransaction(TRUE);

	return 0;
}

LRESULT CSwitchHeadBar::OnFarTop(WPARAM wParam, LPARAM lParam)
{
	((CSwitchPage*)GetParent())->FarTop();

	return 0;
}

LRESULT CSwitchHeadBar::OnPageUp(WPARAM wParam, LPARAM lParam)
{
	((CSwitchPage*)GetParent())->PageUp();

	return 0;
}

LRESULT CSwitchHeadBar::OnFarLeft(WPARAM wParam, LPARAM lParam)
{
	((CSwitchPage*)GetParent())->FarLeft();

	return 0;
}

LRESULT CSwitchHeadBar::OnPageLeft(WPARAM wParam, LPARAM lParam)
{
	((CSwitchPage*)GetParent())->PageLeft();

	return 0;
}

LRESULT CSwitchHeadBar::OnPageRight(WPARAM wParam, LPARAM lParam)
{
	((CSwitchPage*)GetParent())->PageRight();

	return 0;
}

LRESULT CSwitchHeadBar::OnFarRight(WPARAM wParam, LPARAM lParam)
{
	((CSwitchPage*)GetParent())->FarRight();

	return 0;
}

BOOL CSwitchHeadBar::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CWnd::OnEraseBkgnd(pDC);
}

void CSwitchHeadBar::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_KILLFOCUS:
		{
			// 此定时器是用来刷新窗口，当鼠标离开窗口时
			CPoint ptCursor;
			GetCursorPos(&ptCursor);

			CRect rcWnd;
			GetWindowRect(&rcWnd);

			CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

			CRect rcColumnSec;
			rcColumnSec.left  = rcWnd.left + pMargins->HeadBarMargin.iColumnWidthFst +6;
			rcColumnSec.top   = rcWnd.top;
			rcColumnSec.right = rcColumnSec.left + pMargins->HeadBarMargin.iColumnWidthSec-10; 
			rcColumnSec.bottom= rcWnd.bottom;

			if (!rcColumnSec.PtInRect(ptCursor))
			{
				ScreenToClient(&rcColumnSec);

				CRect rcRepaint;
				rcRepaint.left = rcColumnSec.left + m_FocusItem*pMargins->HeadBarMargin.iItemWidth;
				rcRepaint.top  = rcColumnSec.top;
				rcRepaint.right = rcRepaint.left + pMargins->HeadBarMargin.iItemWidth;
				rcRepaint.bottom = rcColumnSec.bottom;

				m_FocusItem = -1;
				InvalidateRect(rcRepaint);

				((CSwitchSelectBar*)((CSwitchPage*)GetParent())->GetSelectBar())->SetFocusBox(m_FocusItem, -1);

				KillTimer(nIDEvent);
			}
		}

		break;

	default:break;
	}

	CWnd::OnTimer(nIDEvent);
}


void CSwitchHeadBar::ReDrawColumnSec()
{
	CRect rcCli;
	GetClientRect(&rcCli);

	CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

	CRect rcCol2;
	rcCol2.left = rcCli.left + pMargins->HeadBarMargin.iColumnWidthFst + 5;
	rcCol2.top  = rcCli.top;
	rcCol2.right = rcCli.right - pMargins->HeadBarMargin.iColumnWidthThd - 5;
	rcCol2.bottom = rcCli.bottom;

	InvalidateRect(rcCol2);
}

BOOL CSwitchHeadBar::PreTranslateMessage(MSG* pMsg)
{
	if (m_tips.GetSafeHwnd())
		m_tips.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}

void CSwitchHeadBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
}

BOOL CSwitchHeadBar::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CSwitchPage* pParent = (CSwitchPage*)GetParent();
	CPageMargins* pMargins = (CPageMargins*)pParent->GetMargins();

	// 当前最大可视输出口数 >= 所有输出口数
	if (pMargins->HeadBarMargin.iMaxVisiableItemsOnHorz >= GetOutputManager()->GetCount())
		return TRUE;

	int curpos = m_sFirstVisiableItem;

	curpos -= zDelta/120;

	int iMin = 0;
	int iMax = GetOutputManager()->GetCount() - pMargins->HeadBarMargin.iMaxVisiableItemsOnHorz;
	iMax = max (0, iMax);

	curpos = max (curpos, 0);
	curpos = min (curpos, iMax);

	if (m_sFirstVisiableItem == curpos)
		return TRUE;

	m_sFirstVisiableItem = curpos;

	// 刷新输出口列表
	ReDrawColumnSec();

	// 刷新选择栏
	pParent->GetSelectBar()->RedrawWindow();

	// 更新水平滑动条
	pParent->SetHScrollBar();

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}