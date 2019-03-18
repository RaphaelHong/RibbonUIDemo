// SwitchPage.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "SwitchPage.h"


//int CSwitchPage::m_sFirstVisibleInput  = 1;
//int CSwitchPage::m_sFirstVisibleOutput = 1;
emZoomLevel CSwitchPage::m_sZoomLvl = eZLLarge;
// SwitchPage

IMPLEMENT_DYNAMIC(CSwitchPage, CWnd)

CSwitchPage::CSwitchPage()
{
	m_rcPage = CRect(0, 0, 800, 600);
	//SetMargins();
	m_pHelptext = NULL;
}

CSwitchPage::~CSwitchPage()
{
	if (m_pHelptext != NULL)
	{
		m_pHelptext->DestroyWindow();
		SAFE_DELETE(m_pHelptext);
	}
}


// SwitchPage 成员函数
BEGIN_MESSAGE_MAP(CSwitchPage, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_MESSAGE(ID_BNITEM_FARBOTTOM, &CSwitchPage::OnFarBottom)
	ON_MESSAGE(ID_BNITEM_PAGEDOWN, &CSwitchPage::OnPageDown)
END_MESSAGE_MAP()


int CSwitchPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!CreateHeadBar())
		return -1;

	if (!CreateSourceBar())
		return -1;

	if (!CreateSelectBar())
		return -1;

	SetBnItem();

	SetHScrollBar();

	SetTooltips();

	return 0;
}

void CSwitchPage::SetTooltips()
{
	m_tips.Create(this);
	if (m_bnFarBottom.GetSafeHwnd())
		m_tips.AddTool(&m_bnFarBottom, _T("信号列表滚动至末页"));

	if (m_bnPageDown.GetSafeHwnd())
		m_tips.AddTool(&m_bnPageDown, _T("信号列表向下滚动一页"));

	m_tips.SetDelayTime(200);
}

void CSwitchPage::SetHScrollBar()
{
	CRect rcScroll;
	rcScroll.left = m_rcPage.left + m_Margins.HeadBarMargin.iColumnWidthFst;
	rcScroll.top = m_rcPage.top;
	rcScroll.right = rcScroll.left + m_Margins.HeadBarMargin.iColumnWidthSec;
	rcScroll.bottom = m_rcPage.top + m_Margins.iTopBorderHeight;

	if (m_Horzbar.GetSafeHwnd())
	{
		m_Horzbar.MoveWindow(rcScroll);
	}
	else
	{
		m_Horzbar.Create(SBS_TOPALIGN|SBS_HORZ|WS_CHILD|WS_VISIBLE|WS_BORDER, rcScroll, this, 1124);	
	}

	int iMin = 0;
	int iMax = GetOutputManager()->GetCount() - m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz;
	iMax = max (0, iMax);
	m_Horzbar.SetScrollRange(iMin, iMax, FALSE);
	m_Horzbar.SetScrollPos(CSwitchHeadBar::m_sFirstVisiableItem);
	//m_Horzbar.EnableWindow(iMax != 0);
	m_Horzbar.ShowWindow((iMax != 0) ? SW_SHOW : SW_HIDE);
}

void CSwitchPage::SetBnItem()
{
	Rect rcBn;
	rcBn.X = m_rcPage.left + 53;
	rcBn.Y = m_rcPage.bottom - m_Margins.iBotBorderHeight;
	rcBn.Width = 18;
	rcBn.Height = m_Margins.iBotBorderHeight;

	m_bnFarBottom.SetItemRect(rcBn);
	if (m_bnFarBottom.GetSafeHwnd())
		m_bnFarBottom.MoveWindow(rcBn.X, rcBn.Y, rcBn.Width, rcBn.Height);
	else
	{
		m_bnFarBottom.SetImages(IDB_PNG_PAGELEFT, IDB_PNG_PAGELEFT_FOCUS, IDB_PNG_PAGELEFT_CK, IDB_PNG_PAGELEFT_DIS);
		m_bnFarBottom.SetRotateFlipType(Rotate270FlipNone);
		m_bnFarBottom.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBn, this, ID_BNITEM_FARBOTTOM);
		m_bnFarBottom.ShowBoxFrame(FALSE);
	}

	rcBn.X = rcBn.GetRight() + 5;
	
	m_bnPageDown.SetItemRect(rcBn);
	if (m_bnPageDown.GetSafeHwnd())
		m_bnPageDown.MoveWindow(rcBn.X, rcBn.Y, rcBn.Width, rcBn.Height);
	else
	{
		m_bnPageDown.SetImages(IDB_PNG_PAGEPREV, IDB_PNG_PAGEPREV_FOCUS, IDB_PNG_PAGEPREV_CK, IDB_PNG_PAGEPREV_DIS);
		m_bnPageDown.SetRotateFlipType(Rotate270FlipNone);
		m_bnPageDown.BnCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBn, this, ID_BNITEM_PAGEDOWN);
		m_bnPageDown.ShowBoxFrame(FALSE);
	}
}

inline void CSwitchPage::CalcHeadBarPos(CRect &rcPos)
{
	rcPos.top    = m_rcPage.top + m_Margins.iTopBorderHeight;
	rcPos.left   = m_rcPage.left;
	rcPos.right  = m_rcPage.right;
	rcPos.bottom = rcPos.top + m_Margins.HeadBarMargin.iBarHeight;
}

inline void CSwitchPage::CalcSourceBarPos(CRect &rcPos)
{
	rcPos.top    = m_wndHeadBar.GetBarRect().bottom + 1;
	rcPos.left   = m_rcPage.left;
	rcPos.right  = rcPos.left + m_Margins.SourceBarMargin.iBarWidth;
	rcPos.bottom = rcPos.top + m_Margins.SourceBarMargin.iBarHeight;
}

inline void CSwitchPage::CalcSelectBarPos(CRect &rcPos)
{
	rcPos.top    = m_wndHeadBar.GetBarRect().bottom + 1;
	rcPos.left   = m_wndSourceBar.GetBarRect().right + 1;
	rcPos.right  = rcPos.left + m_Margins.SelectBarMargin.iBarWidth;
	rcPos.bottom = rcPos.top + m_Margins.SelectBarMargin.iBarHeight;
}

BOOL CSwitchPage::CreateHeadBar()
{
	CRect rcBar;
	CalcHeadBarPos(rcBar);

	m_wndHeadBar.SetBarRect(rcBar);
		
	if (!m_wndHeadBar.Create(NULL, "", WS_CHILD|WS_VISIBLE, rcBar, this, ID_WND_HEADBAR))
	//if (!m_wndHeadBar.BarCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBar, this, ID_WND_HEADBAR))
		return FALSE;

	return TRUE;
}

BOOL CSwitchPage::CreateSourceBar()
{
	CRect rcBar;
	CalcSourceBarPos(rcBar);

	m_wndSourceBar.SetBarRect(rcBar);

	if (!m_wndSourceBar.Create(NULL, "", WS_CHILD|WS_VISIBLE, rcBar, this, ID_WND_SOURCEBAR))
	//if (!m_wndSourceBar.BarCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBar, this, ID_WND_SOURCEBAR))
		return FALSE;

	return TRUE;
}

BOOL CSwitchPage::CreateSelectBar()
{
	CRect rcBar;
	CalcSelectBarPos(rcBar);

	m_wndSelectBar.SetBarRect(rcBar);

	//if (!m_wndSelectBar.Create(NULL, "", WS_CHILD|WS_VISIBLE, rcBar, this, ID_WND_SELECTBAR))
	if (!m_wndSelectBar.BarCreate(NULL, "", WS_CHILD|WS_VISIBLE, rcBar, this, ID_WND_SELECTBAR))
		return FALSE;

	return TRUE;
}


CWnd* CSwitchPage::GetHeadBar()
{
	if (m_wndHeadBar.GetSafeHwnd())
		return &m_wndHeadBar;
	else
		return NULL;
}

CWnd* CSwitchPage::GetSourceBar()
{
	if (m_wndSourceBar.GetSafeHwnd())
		return &m_wndSourceBar;
	else
		return NULL;
}

CWnd* CSwitchPage::GetSelectBar()
{
	if (m_wndSelectBar)
		return &m_wndSelectBar;
	else
		return NULL;
}

void CSwitchPage::SetMargins()
{
	// 准备图片素材
	Image* imgTopBorder=NULL; // 上边框图
	Image* imgBtmBorder=NULL; // 下边框图
	Image* imgHeadBar=NULL;   // 头栏背景图
	Image* imgCheckBox=NULL;  // 复选框按钮图
	Image* imgPageLeft=NULL;  // 滚动箭头图
	Image* imgResizeBar=NULL; // 位置调整按钮

	ImageFromIDResource(IDB_PNG_TOP_BORDER, "png", imgTopBorder);
	ImageFromIDResource(IDB_PNG_BOTTOM_BORDER, "png", imgBtmBorder);
	ImageFromIDResource(IDB_PNG_HEADBAR, "png", imgHeadBar);
	ImageFromIDResource(IDB_PNG_PAGELEFT, "png", imgPageLeft);
	ImageFromIDResource(IDB_PNG_RESIZEBAR_VERT, "png", imgResizeBar);
	
	if (m_sZoomLvl == eZLLarge) {
		m_Margins.PageScale = CLargeScale();
		ImageFromIDResource(IDB_PNG_CHECKBOX_ORIGNAL24, "png", imgCheckBox);
	}
	else if (m_sZoomLvl == eZLMedium) {
		m_Margins.PageScale = CMediumScale();
		ImageFromIDResource(IDB_PNG_CHECKBOX_ORIGNAL18, "png", imgCheckBox);
	}
	else if (m_sZoomLvl == eZLSmall) {
		m_Margins.PageScale = CSmallScale();
		ImageFromIDResource(IDB_PNG_CHECKBOX_ORIGNAL12, "png", imgCheckBox);
	}

	int iItemDimDef = m_Margins.PageScale.iSelectBoxSize;

	//m_Margins.PageScale.iSourceBarWidth = 300;

	// 页面的边距属性
	//{
		m_Margins.iPageWidth  = m_rcPage.Width();
		m_Margins.iPageHeight = m_rcPage.Height();

		m_Margins.iTopBorderHeight = imgTopBorder ? imgTopBorder->GetHeight() : 18;
		m_Margins.iBotBorderHeight = imgBtmBorder ? imgBtmBorder->GetHeight() : 18;
	//}

	// 头栏的边距属性
	//{
		m_Margins.HeadBarMargin.iBarWidth   = m_Margins.iPageWidth;
		m_Margins.HeadBarMargin.iBarHeight  = imgHeadBar ? imgHeadBar->GetHeight() : 30;
		m_Margins.HeadBarMargin.iItemWidth  = (imgCheckBox ? imgCheckBox->GetWidth() : iItemDimDef) + m_Margins.PageScale.iHorzSpace;
		m_Margins.HeadBarMargin.iItemHeight = m_Margins.HeadBarMargin.iBarHeight;
		m_Margins.HeadBarMargin.iColumnWidthFst = /*imgHeadBar ? imgHeadBar->GetWidth() : */m_Margins.PageScale.iSourceBarWidth;
		m_Margins.HeadBarMargin.iColumnWidthThd = (imgPageLeft ? imgPageLeft->GetWidth() : 12) * 5;
		m_Margins.HeadBarMargin.iColumnWidthSec = m_Margins.HeadBarMargin.iBarWidth - \
			                                      m_Margins.HeadBarMargin.iColumnWidthFst - \
												  m_Margins.HeadBarMargin.iColumnWidthThd;
		m_Margins.HeadBarMargin.iMaxVisiableItemsOnVert = 1;
		m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz = (m_Margins.HeadBarMargin.iColumnWidthSec - (imgResizeBar ? imgResizeBar->GetWidth() : 3)*2 - 4) \
			                                              / m_Margins.HeadBarMargin.iItemWidth;

		if (m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz > GetOutputManager()->GetCount()) {
			// 如果当前头栏能展示的输出通道数大于实际输出通道数，则重新调整头栏各列宽度
			m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz = GetOutputManager()->GetCount();
			m_Margins.HeadBarMargin.iColumnWidthSec = m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz * m_Margins.HeadBarMargin.iItemWidth \
				                                      + ((imgResizeBar ? imgResizeBar->GetWidth() : 3) + 2)*2;
			m_Margins.HeadBarMargin.iColumnWidthThd = m_Margins.HeadBarMargin.iBarWidth - \
				                                      m_Margins.HeadBarMargin.iColumnWidthFst - \
												      m_Margins.HeadBarMargin.iColumnWidthSec;
		}
	//}

	// 切换按钮栏的边距属性
	//{
		m_Margins.SelectBarMargin.iBarWidth   = m_Margins.HeadBarMargin.iColumnWidthSec + m_Margins.HeadBarMargin.iColumnWidthThd;
		m_Margins.SelectBarMargin.iBarHeight  = m_Margins.iPageHeight - m_Margins.iTopBorderHeight - m_Margins.iBotBorderHeight - m_Margins.HeadBarMargin.iBarHeight;
		m_Margins.SelectBarMargin.iItemWidth  = m_Margins.HeadBarMargin.iItemWidth;
		m_Margins.SelectBarMargin.iItemHeight = (imgCheckBox ? imgCheckBox->GetHeight() : iItemDimDef) + m_Margins.PageScale.iVertSpace;
		// 限定最小项高度
		if (m_Margins.SelectBarMargin.iItemHeight < (18+3)) m_Margins.SelectBarMargin.iItemHeight = 18+3;

		m_Margins.SelectBarMargin.iMaxVisiableItemsOnHorz = m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz;
		m_Margins.SelectBarMargin.iMaxVisiableItemsOnVert = m_Margins.SelectBarMargin.iBarHeight / m_Margins.SelectBarMargin.iItemHeight;
		if (m_Margins.SelectBarMargin.iMaxVisiableItemsOnVert > GetInputManager()->GetCount()) {
			// 当前切换按钮栏能展示的输入通道数大于实际输入通道数
			m_Margins.SelectBarMargin.iMaxVisiableItemsOnVert = GetInputManager()->GetCount();
		}
	//}

	// 信号栏的边距属性
	//{
		m_Margins.SourceBarMargin.iBarWidth   = m_Margins.PageScale.iSourceBarWidth;
		m_Margins.SourceBarMargin.iBarHeight  = m_Margins.iPageHeight - m_Margins.iTopBorderHeight - m_Margins.iBotBorderHeight - m_Margins.HeadBarMargin.iBarHeight;
		m_Margins.SourceBarMargin.iItemWidth  = m_Margins.SourceBarMargin.iBarWidth;
		m_Margins.SourceBarMargin.iItemHeight = m_Margins.SelectBarMargin.iItemHeight;
		m_Margins.SourceBarMargin.iMaxVisiableItemsOnHorz = 1;
		m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert = m_Margins.SelectBarMargin.iMaxVisiableItemsOnVert;
	//}

	// 清理图片缓存
	//{
		SAFE_DELETE(imgBtmBorder);
		SAFE_DELETE(imgCheckBox);
		SAFE_DELETE(imgHeadBar);
		SAFE_DELETE(imgPageLeft);
		SAFE_DELETE(imgResizeBar);
		SAFE_DELETE(imgTopBorder);
	//}
}

void CSwitchPage::RecalLayout()
{
	SetMargins();

	CRect rcBar;
	CalcHeadBarPos(rcBar);
	m_wndHeadBar.SetBarRect(rcBar);
	if (m_wndHeadBar.GetSafeHwnd())
	{
		if (CSwitchHeadBar::m_sFirstVisiableItem + m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz > GetOutputManager()->GetCount())
		{
			CSwitchHeadBar::m_sFirstVisiableItem = GetOutputManager()->GetCount() - m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz;
			CSwitchHeadBar::m_sFirstVisiableItem = max (0, CSwitchHeadBar::m_sFirstVisiableItem);
		}
		m_wndHeadBar.MoveWindow(rcBar, TRUE);
		m_wndHeadBar.RecalcLayout();
	}

	CalcSourceBarPos(rcBar);
	m_wndSourceBar.SetBarRect(rcBar);
	if (m_wndSourceBar.GetSafeHwnd())
	{
		if (CSwitchSourceBar::m_sFirstVisiableItem + m_Margins.HeadBarMargin.iMaxVisiableItemsOnVert > GetInputManager()->GetCount())
		{
			CSwitchSourceBar::m_sFirstVisiableItem = GetInputManager()->GetCount() - m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert;
			CSwitchSourceBar::m_sFirstVisiableItem = max (0, CSwitchSourceBar::m_sFirstVisiableItem);
		}
		m_wndSourceBar.MoveWindow(rcBar, TRUE);
		m_wndSourceBar.RecalcLayout();
	}

	CalcSelectBarPos(rcBar);
	m_wndSelectBar.SetBarRect(rcBar);
	if (m_wndSelectBar.GetSafeHwnd())
		m_wndSelectBar.MoveWindow(rcBar, TRUE);

	SetHScrollBar();

	SetBnItem();
}

void CSwitchPage::SetPageRect(const CRect rc)
{
	m_rcPage = rc;
}

void CSwitchPage::OnPaint()
{
	CPaintDC dc(this);

	Graphics gp(dc.m_hDC);

	Image* imgTopBorder;
	Image* imgBtmBorder;

	SolidBrush whiteBr(Color(255, 240, 240, 255));
	gp.FillRectangle(&whiteBr, m_rcPage.left, m_rcPage.top, m_rcPage.Width(), m_rcPage.Height());

	Rect rcBorder, rcPageDown, rcPageNext;

	LinearGradientBrush lgBrushTop(Point(0, 0), Point(0, m_Margins.iTopBorderHeight), Color(255, 213, 220, 255), Color(255, 245, 250, 255));
	LinearGradientBrush lgBrushBtm(Point(0, 0), Point(0, m_Margins.iBotBorderHeight), Color(255, 245, 250, 255), Color(255, 213, 220, 255));

	// 上边框带条
	rcBorder = Rect(0, 0, m_Margins.iPageWidth, m_Margins.iTopBorderHeight);
	if (ImageFromIDResource(IDB_PNG_TOP_BORDER, "png", imgTopBorder)) {
		gp.DrawImage(imgTopBorder, rcBorder);
		SAFE_DELETE(imgTopBorder);
	}
	else
		gp.FillRectangle(&lgBrushTop, rcBorder);

	// 下边框带条
	rcBorder = Rect(0, m_Margins.iTopBorderHeight + m_Margins.HeadBarMargin.iBarHeight + m_Margins.SourceBarMargin.iBarHeight, m_Margins.iPageWidth, m_Margins.iBotBorderHeight);
	if (ImageFromIDResource(IDB_PNG_BOTTOM_BORDER, "png", imgBtmBorder)) {
		gp.DrawImage(imgBtmBorder, rcBorder);
		SAFE_DELETE(imgBtmBorder);
	}
	else
		gp.FillRectangle(&lgBrushBtm, rcBorder);

	// 输入输出通道总数
	FontFamily fontFamily(L"宋体");
	//Gdiplus::Font font(&fontFamily, 12.0f, FontStyleRegular, UnitPixel);
	Gdiplus::Font Bfont(&fontFamily, 12.0f, FontStyleBold, UnitPixel);
	SolidBrush blackBr(Color(255, 116, 101, 131));
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingEllipsisWord);	
	format.SetFormatFlags(StringFormatFlagsNoWrap);

	CStringW strChlName;
	RectF rcItem;
	strChlName.Format(L"[ IN :%d, OUT :%d ]", GetInputManager()->GetRealCount(), GetOutputManager()->GetRealCount());
	rcItem.X = REAL(m_rcPage.left + 5);
	rcItem.Y = REAL(m_rcPage.top + 1);
	rcItem.Width = REAL(m_Margins.HeadBarMargin.iColumnWidthFst - 10);
	rcItem.Height = REAL(m_Margins.iTopBorderHeight-1);

	gp.DrawString(strChlName, -1, &Bfont, rcItem, &format, &blackBr);	
}

BOOL CSwitchPage::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CWnd::OnEraseBkgnd(pDC);
}

void CSwitchPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int minpos = 0;
	int maxpos = 0;
	pScrollBar->GetScrollRange(&minpos, &maxpos);

	// Get the current position of scroll box.
	int curpos = 0;
	curpos = CSwitchHeadBar::m_sFirstVisiableItem;

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
	if (CSwitchHeadBar::m_sFirstVisiableItem == curpos) return;

	CSwitchHeadBar::m_sFirstVisiableItem = curpos;

	m_wndHeadBar.ReDrawColumnSec();
	m_wndSelectBar.RedrawWindow();

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CSwitchPage::OnFarBottom(WPARAM wParam, LPARAM lParam)
{
	FarBottom();

	return 0;
}

LRESULT CSwitchPage::OnPageDown(WPARAM wParam, LPARAM lParam)
{
	PageDown();

	return 0;
}

void CSwitchPage::FarTop()
{
	if (CSwitchSourceBar::m_sFirstVisiableItem == 0)
		return;
	else
		CSwitchSourceBar::m_sFirstVisiableItem = 0;

	m_wndSourceBar.ScrollItem();
	m_wndSourceBar.RedrawWindow();
	m_wndSelectBar.RedrawWindow();
}

void CSwitchPage::PageUp()
{
	if (CSwitchSourceBar::m_sFirstVisiableItem == 0)
		return;
	else if (CSwitchSourceBar::m_sFirstVisiableItem - m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert >= 0)
	{
		CSwitchSourceBar::m_sFirstVisiableItem -= m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert;
	}
	else if (CSwitchSourceBar::m_sFirstVisiableItem - m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert < 0)
	{
		CSwitchSourceBar::m_sFirstVisiableItem = 0;
	}

	m_wndSourceBar.ScrollItem();
	m_wndSourceBar.RedrawWindow();
	m_wndSelectBar.RedrawWindow();

}

void CSwitchPage::FarLeft()
{
	if (CSwitchHeadBar::m_sFirstVisiableItem == 0)
		return;
	else
		CSwitchHeadBar::m_sFirstVisiableItem = 0;

	m_wndHeadBar.ReDrawColumnSec();
	m_Horzbar.SetScrollPos(CSwitchHeadBar::m_sFirstVisiableItem);
	m_wndSelectBar.RedrawWindow();

}

void CSwitchPage::PageLeft()
{
	if (CSwitchHeadBar::m_sFirstVisiableItem == 0)
		return;
	else if (CSwitchHeadBar::m_sFirstVisiableItem - m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz >= 0)
	{
		CSwitchHeadBar::m_sFirstVisiableItem -= m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz;
	}
	else if (CSwitchHeadBar::m_sFirstVisiableItem - m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz < 0)
	{
		CSwitchHeadBar::m_sFirstVisiableItem = 0;
	}

	m_wndHeadBar.ReDrawColumnSec();
	m_Horzbar.SetScrollPos(CSwitchHeadBar::m_sFirstVisiableItem);
	m_wndSelectBar.RedrawWindow();

}

void CSwitchPage::FarRight()
{
	if (CSwitchHeadBar::m_sFirstVisiableItem + m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz == GetOutputManager()->GetCount())
		return;
	else{
		CSwitchHeadBar::m_sFirstVisiableItem = GetOutputManager()->GetCount() - m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz;
		CSwitchHeadBar::m_sFirstVisiableItem = max (0, CSwitchHeadBar::m_sFirstVisiableItem);
	}

	m_wndHeadBar.ReDrawColumnSec();
	m_Horzbar.SetScrollPos(CSwitchHeadBar::m_sFirstVisiableItem);
	m_wndSelectBar.RedrawWindow();

}

void CSwitchPage::PageRight()
{
	if (CSwitchHeadBar::m_sFirstVisiableItem + m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz == GetOutputManager()->GetCount())
		return;
	else if (CSwitchHeadBar::m_sFirstVisiableItem + m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz*2 < GetOutputManager()->GetCount())
	{
		CSwitchHeadBar::m_sFirstVisiableItem += m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz;
	}
	else if (CSwitchHeadBar::m_sFirstVisiableItem + m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz*2 >= GetOutputManager()->GetCount())
	{
		CSwitchHeadBar::m_sFirstVisiableItem = GetOutputManager()->GetCount() - m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz;
	}

	m_wndHeadBar.ReDrawColumnSec();
	m_Horzbar.SetScrollPos(CSwitchHeadBar::m_sFirstVisiableItem);
	m_wndSelectBar.RedrawWindow();

}

void CSwitchPage::FarBottom()
{
	if (CSwitchSourceBar::m_sFirstVisiableItem + m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert == GetInputManager()->GetCount())
		return;
	else
	{
		CSwitchSourceBar::m_sFirstVisiableItem = GetInputManager()->GetCount() - m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert;
		CSwitchSourceBar::m_sFirstVisiableItem = max (0, CSwitchSourceBar::m_sFirstVisiableItem);
	}

	m_wndSourceBar.ScrollItem();
	m_wndSourceBar.RedrawWindow();
	m_wndSelectBar.RedrawWindow();

}

void CSwitchPage::PageDown()
{
	if (CSwitchSourceBar::m_sFirstVisiableItem + m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert == GetInputManager()->GetCount())
		return;
	else if (CSwitchSourceBar::m_sFirstVisiableItem + m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert*2 < GetInputManager()->GetCount())
	{
		CSwitchSourceBar::m_sFirstVisiableItem += m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert;
	}
	else if (CSwitchSourceBar::m_sFirstVisiableItem + m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert*2 >= GetInputManager()->GetCount())
	{
		CSwitchSourceBar::m_sFirstVisiableItem = GetInputManager()->GetCount() - m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert;
	}

	m_wndSourceBar.ScrollItem();
	m_wndSourceBar.RedrawWindow();
	m_wndSelectBar.RedrawWindow();

}

void CSwitchPage::RefreshAll()
{
	m_wndSourceBar.RedrawWindow();
}

void CSwitchPage::ManageWizardBn()
{
	m_wndHeadBar.EnableWizardBn(WizardLeft, CSwitchHeadBar::m_sFirstVisiableItem > 0);

	m_wndHeadBar.EnableWizardBn(WizardRight, CSwitchHeadBar::m_sFirstVisiableItem + m_Margins.HeadBarMargin.iMaxVisiableItemsOnHorz < GetOutputManager()->GetCount());
	
	m_wndHeadBar.EnableWizardBn(WizardTop, CSwitchSourceBar::m_sFirstVisiableItem > 0);

	m_bnFarBottom.Enable(CSwitchSourceBar::m_sFirstVisiableItem + m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert < GetInputManager()->GetCount());
	m_bnPageDown.Enable(CSwitchSourceBar::m_sFirstVisiableItem + m_Margins.SourceBarMargin.iMaxVisiableItemsOnVert < GetInputManager()->GetCount());
}

BOOL CSwitchPage::PreTranslateMessage(MSG* pMsg)
{
	if (m_tips.GetSafeHwnd())
		m_tips.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}

void CSwitchPage::SetNeedHelpPort(const int nID, const int nType)
{
	if (m_pHelptext != NULL)
	{
		m_pHelptext->SetPortInfo(nID, nType);
	}
}

INT_PAIR CSwitchPage::GetNeedHelpPort()
{
	if (m_pHelptext != NULL)
	{
		return m_pHelptext->GetPortInfo();
	}
	else
		return INT_PAIR(-1, -1);
}

BOOL CSwitchPage::IsHelpTextVisible()
{
	if (m_pHelptext != NULL)
	{
		return m_pHelptext->IsWindowVisible();
	}
	else
		return FALSE;
}

void CSwitchPage::RedrawHelpText()
{
	if (m_pHelptext != NULL)
	{
		m_pHelptext->Invalidate();
	}
}

void CSwitchPage::ShowHelpText(BOOL bShow, CStringW strText, CPoint ptText)
{
	if (m_pHelptext == NULL)
	{
		m_pHelptext = new CPortHelpText;
		m_pHelptext->Create(IDD_DLG_HELPTEXT, this);
	}

	if (bShow)
	{
		CRect rcCli;
		m_pHelptext->GetClientRect(&rcCli);

		CRect rcPage=m_rcPage;
		ClientToScreen(&rcPage);
		if (ptText.y+rcCli.Height() > rcPage.bottom)
			ptText.y = rcPage.bottom - m_Margins.iBotBorderHeight - rcCli.Height();

		m_pHelptext->MoveWindow(ptText.x, ptText.y, rcCli.Width(), rcCli.Height());

		if (!m_pHelptext->IsWindowVisible())
			m_pHelptext->ShowWindow(SW_SHOW);
		else
			m_pHelptext->Invalidate();
		//m_pHelptext->SetWindowPos(&wndTop, ptText.x, ptText.y, rcCli.Width(), rcCli.Height(), SWP_SHOWWINDOW | SWP_NOACTIVATE);
	}
	else
	{
		m_pHelptext->SetPortInfo(-1, -1);

		if (m_pHelptext->IsWindowVisible()) 
			m_pHelptext->ShowWindow(SW_HIDE);
	}
}

//****************************************************************************
// Matrix scene file format:
//         [HeadFlag: 00FFFFFFFF00]
//         [BCC code: 0x4C]
//         [Output Num: 20]
//         [Input-Output mapping: 1-1 2-2 3-3 1-4 1-5 ...]
// Buffer format : 00 FF FF FF FF 00    4C    14 00 00 00 01 00 01 00 02 00 02 00 03 00 03 00 01 00 04 00 01 00 05 00 ...
//                 [    HeadFlag   ][BCC code][Output Num][   1-1   ] [   2-2   ] [   3-3   ] [   1-4   ] [   1-5   ] ...
void CSwitchPage::PackFile(__in LPCTSTR lpFile)
{

	int nPortNum = GetOutputManager()->GetCount();
	if (nPortNum <= 0)
		return (void)AfxMessageBox(_T("Output channel is empty!"), MB_OK|MB_ICONSTOP);

	//****************** File open
	CFile save;
	if (!save.Open(lpFile, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary))	
		return (void)AfxMessageBox(_T("Create file error!"), MB_OK|MB_ICONSTOP);

	//****************** Set File Flag
	const char HeadFlag[] = "00FFFFFFFF00";
	save.Write(HeadFlag, strlen(HeadFlag));

	//****************** Allocate buffer
	long lSize = sizeof(char) + sizeof(int) + nPortNum*sizeof(INT_PAIR);
	//             BCC code      port num       Input-Output mapping
	CMemoryAllocate<char> alloc(lSize+1);

	//****************** Write Output Channel Number
	int nPos = 1; //First char stores BCC code.
	memcpy_s(alloc.GetAddress()+nPos, lSize-1, &nPortNum, sizeof(int));
	nPos += sizeof(int);

	//****************** Write Input-Output mapping to file
	INT_PAIR pairs[_MAX_OUTPUT_PORT];
	memset(pairs, -1, sizeof(pairs));

	ASSERT(nPortNum <= _MAX_OUTPUT_PORT);

	int iFeederID = -1;
	int iSubID = -1;
	for (int i=0; i<nPortNum; i++)
	{
		iFeederID = GetOutputManager()->GetFeederID(i, FALSE);
		pairs[i].key = GetOutputManager()->GetCmdPort(i, FALSE);
		pairs[i].val = GetInputManager()->GetCmdPort(iFeederID);

		iSubID = GetInputManager()->GetSubID(iFeederID);
		pairs[i].val |= (iSubID == 1 ? SUBPORT_A : iSubID==2 ? SUBPORT_B : 0);
	}

	memcpy_s(alloc.GetAddress()+nPos, lSize-nPos, pairs, sizeof(INT_PAIR)*nPortNum);
	nPos += sizeof(INT_PAIR)*nPortNum;

	// Encrypt buffer
	CBaseFileManager::Encrypt(alloc.GetAddress()+1, alloc.GetAddress()+1, nPos-1);

	// Add BCC code
	CBaseFileManager::SetBCCvalue(alloc.GetAddress()+1, alloc.GetAddress()[0], nPos-1);

	//****************** File close
	save.Write(alloc.GetAddress(), nPos);
	save.Close();

	//****************** Set Default file path
	CSysConfManager sysconf;
	sysconf.SetDefaultSceneFilePath(lpFile);

	CString strMsg;
	strMsg.Format(_T("File saved: [%s]"), lpFile);
	AfxMessageBox(strMsg, MB_OK|MB_ICONINFORMATION);

}

void CSwitchPage::ParseFile(__in LPCTSTR lpFile)
{
	//****************** File open
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

	//******************* Check file flag
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

	int nPortNum = 0;
	memcpy_s(&nPortNum, sizeof(int), alloc.GetAddress()+pos, sizeof(int));
	pos += sizeof(int);

	if (nPortNum != GetOutputManager()->GetCount())
	{
		CString str;
		str.Format(_T("Err：端口数不匹配！当前: %d, 文件: %d"), GetOutputManager()->GetCount(), nPortNum);
		AfxMessageBox(str, MB_OK|MB_ICONSTOP);
		return;
	}

	INT_PAIR pairs[_MAX_OUTPUT_PORT];
	memset(pairs, -1, sizeof(pairs));
	memcpy_s(pairs, sizeof(pairs), alloc.GetAddress()+pos, sizeof(INT_PAIR)*nPortNum);
	
	// Validate the scene datas
	GetOutputManager()->MappingSwitch(nPortNum, pairs);

	m_wndSelectBar.Invalidate();
	m_wndHeadBar.ReDrawColumnSec();

	AfxMessageBox(_T("Scene file loaded!"), MB_OK|MB_ICONINFORMATION);

}

void CSwitchPage::ParseSceneBlock(__in UINT* pBlock, __in int nBlockLen)
{
	ASSERT_POINTER(pBlock, UINT);
	ASSERT(nBlockLen > 1);

	// 解析输出端口数
	int nOutNum = pBlock[0];

	// 解析各输出端口的输入源
	int inIndex = -1;     //输入口索引
	int outIndex = -1;    //输出口索引
	int iFeederPort = -1; //来源端口号
	int iSubPort = -1;    //子端口号
	for (int i=1; i < nOutNum+1; i++)
	{
		// 确认该ID对应的输出口是否存在
		outIndex = GetOutputManager()->FindCmdPort(i);
		if (outIndex == INVALID_VALUE)
			continue;

		// 来源端口是否是子端口
		if ((pBlock[i] & 0xFFFF0000) == SUBPORT_A)
			iSubPort = 1;
		else if ((pBlock[i] & 0xFFFF0000) == SUBPORT_B)
			iSubPort = 2;
		else
			iSubPort = 0;

		iFeederPort = (pBlock[i] & 0xFFFF);
		inIndex = GetInputManager()->FindCmdPort(iFeederPort, iSubPort);

		if (inIndex == INVALID_VALUE)
			GetOutputManager()->SetFeederID(outIndex, -1, FALSE, FALSE);
		else
			GetOutputManager()->SetFeederID(outIndex, GetInputManager()->GetID(inIndex), FALSE, FALSE);
	}

	m_wndHeadBar.ReDrawColumnSec();
	m_wndSelectBar.RedrawWindow();
}