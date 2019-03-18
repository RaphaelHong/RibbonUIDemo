// SwitchSelectBar.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "SwitchSelectBar.h"
#include "SwitchPage.h"


// CSwitchSelectBar

IMPLEMENT_DYNAMIC(CSwitchSelectBar, CWnd)

CSwitchSelectBar::CSwitchSelectBar()
{
	m_FocusBox = BOXITEM(6, 7);
}

CSwitchSelectBar::~CSwitchSelectBar()
{
}


// CSwitchSelectBar 成员函数
BEGIN_MESSAGE_MAP(CSwitchSelectBar, CWnd)
	//ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


BOOL CSwitchSelectBar::BarCreate(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const CRect& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext /* = NULL */ )
{
	if (CreateEx(0,
		AfxRegisterWndClass(0, LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))),
		0,
		dwStyle, 
		rect,
		pParentWnd,
		nID ) )
	{
		return TRUE;
	}

	return FALSE;
}
int CSwitchSelectBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CSwitchSelectBar::SetBarRect(const CRect rc)
{
	m_rcBar = rc;
}

void CSwitchSelectBar::OnPaint()
{
	CPaintDC dc(this);

	//return;

	// 获取客户区大小
	CRect rcCli;
	GetClientRect(&rcCli);

	CDC* pDC = GetDC();
	CDC* pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(NULL);

	CBitmap bmpBkg, *pOldBmp;
	bmpBkg.CreateCompatibleBitmap(pDC, rcCli.Width(), rcCli.Height());
	pOldBmp = pMemDC->SelectObject(&bmpBkg);

	DrawBoxMap3(pMemDC);

	pDC->BitBlt(0, 0, rcCli.Width(), rcCli.Height(), pMemDC, 0, 0, SRCCOPY);

	pMemDC->SelectObject(pOldBmp);
	ReleaseDC(pMemDC);
	SAFE_DELETE(pMemDC);

	ReleaseDC(pDC);
}

void CSwitchSelectBar::DrawBoxMap(CDC* pValidDC)
{
	ASSERT(pValidDC);

	// 获取客户区大小
	CRect rcCli;
	GetClientRect(&rcCli);

	// 获取页面边距
	const CPageMargins *pMargins = ((CSwitchPage*)GetParent())->GetMargins();

	// 创建一个GDI+绘图对象
	Graphics gp(pValidDC->m_hDC);
	gp.SetSmoothingMode(SmoothingModeHighSpeed);

	SolidBrush br(Color(255, 181, 213, 255));
	//br.SetColor(Color(255, 151, 193, 235));

	//Color color[] = {
	//	Color(255, 245, 235, 235),
	//	Color(255, 181, 235, 235),
	//	Color(255, 245, 235, 235)
	//};

	//REAL positions[] = {
	//	0.0f, 
	//	0.45f,
	//	1.0f
	//};

	//LinearGradientBrush bkBr(Point(rcCli.left, rcCli.top), Point(rcCli.right, rcCli.bottom), Color(221, 233, 255), Color(180, 251, 253, 235));
	//bkBr.SetInterpolationColors(color, positions, 3);
	gp.FillRectangle(&br, rcCli.left, rcCli.top, rcCli.Width(), rcCli.Height());

	Pen pen(Color(255, 101, 147, 207), 1);
	gp.DrawRectangle(&pen, rcCli.left, rcCli.top, rcCli.Width(), rcCli.Height());

	FontFamily fontFamily(L"宋体");
	//Gdiplus::Font font(&fontFamily, 12.0f, FontStyleRegular, UnitPixel);
	Gdiplus::Font Bfont(&fontFamily, 48.0f, FontStyleBold, UnitPixel);
	SolidBrush blackBr(Color(150, 235, 245, 250));
	StringFormat format;
	format.SetFormatFlags(StringFormatFlagsDirectionRightToLeft|StringFormatFlagsDirectionVertical);
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingEllipsisWord);	
	//format.SetFormatFlags(StringFormatFlagsNoWrap);

	CStringW strChlName;
	RectF rcItem;
	strChlName.Format(L"Author: RaphaeL.H 2016-05-20");
	rcItem.X = REAL(rcCli.right - pMargins->HeadBarMargin.iColumnWidthThd);
	rcItem.Y = REAL(rcCli.top );
	rcItem.Width = REAL(pMargins->HeadBarMargin.iColumnWidthThd);
	rcItem.Height = REAL(rcCli.Height());

	gp.DrawString(strChlName, -1, &Bfont, rcItem, &format, &blackBr);

	//*** 加载图片资源
	//Image* imgOrg=NULL;
	//Image* imgCheck=NULL;
	//Image* imgFocus=NULL;

	//if (CSwitchPage::m_sModelSize == eZLLarge)
	//{
	//	ImageFromIDResource(IDB_PNG_CHECKBOX_ORIGNAL24, "png", imgOrg);
	//	ImageFromIDResource(IDB_PNG_CHECKBOX_CHECKED24, "png", imgCheck);
	//	ImageFromIDResource(IDB_PNG_CHECKBOX_FOCUSED24, "png", imgFocus);
	//}
	//else if (CSwitchPage::m_sModelSize == eZLMedium)
	//{
	//	ImageFromIDResource(IDB_PNG_CHECKBOX_ORIGNAL18, "png", imgOrg);
	//	ImageFromIDResource(IDB_PNG_CHECKBOX_CHECKED18, "png", imgCheck);
	//	ImageFromIDResource(IDB_PNG_CHECKBOX_FOCUSED18, "png", imgFocus);
	//}
	//else if (CSwitchPage::m_sModelSize == eZLSmall)
	//{
	//	ImageFromIDResource(IDB_PNG_CHECKBOX_ORIGNAL12, "png", imgOrg);
	//	ImageFromIDResource(IDB_PNG_CHECKBOX_CHECKED12, "png", imgCheck);
	//	ImageFromIDResource(IDB_PNG_CHECKBOX_FOCUSED12, "png", imgFocus);
	//}

	Rect rcBox, rcImg;
	rcBox = Rect(0, 0, pMargins->SelectBarMargin.iItemWidth, pMargins->SelectBarMargin.iItemHeight);
	rcImg = Rect(0, 0, pMargins->PageScale.iSelectBoxSize, pMargins->PageScale.iSelectBoxSize);
	//rcImg = Rect(0, 0, imgOrg ? imgOrg->GetWidth() : pMargins->PageScale.iSelectBoxSize, imgOrg ? imgOrg->GetHeight() : pMargins->PageScale.iSelectBoxSize);

	Pen bluePen(Color(255, 243, 250, 245), 1.0f);
	Pen YellPen(Color(255, 253, 250, 190), 2.0f);
	Pen blackPen(Color(255, 43, 50, 45), 2.0f);
	SolidBrush FocusBr(Color(255, 144, 185, 220));
	SolidBrush CheckBr(Color(255, 234, 225, 145));
	SolidBrush NormaBr(Color(255, 200, 205, 195));

	for (int y=0; y<pMargins->SelectBarMargin.iMaxVisiableItemsOnVert; y++)
	{
		for (int x=0; x<pMargins->SelectBarMargin.iMaxVisiableItemsOnHorz; x++)
		{
			rcBox.X = rcCli.left + 5 + x*pMargins->SelectBarMargin.iItemWidth;
			rcBox.Y = rcCli.top + y*pMargins->SelectBarMargin.iItemHeight;
			rcImg.X = rcBox.X + (rcBox.Width-rcImg.Width)/2;
			rcImg.Y = rcBox.Y + (rcBox.Height-rcImg.Height)/2;

/*			if (m_ptFocus.x == x && m_ptFocus.y == y)
			{
				gp.DrawRectangle(&blackPen, rcImg.X+1, rcImg.Y+1, rcImg.Width-2, rcImg.Height-2);
			}	*/		
			if (m_FocusBox.iColumn == -1 && y == m_FocusBox.iRow)
			{
				gp.DrawRectangle(&YellPen, rcImg.X+1, rcImg.Y+1, rcImg.Width-2, rcImg.Height-2);
			}
			else if (m_FocusBox.iRow == -1 && x == m_FocusBox.iColumn)
			{
				gp.DrawRectangle(&YellPen, rcImg.X+1, rcImg.Y+1, rcImg.Width-2, rcImg.Height-2);
			}
			else
			{
				if ((x <= m_FocusBox.iColumn && m_FocusBox.iRow == y) || (y <= m_FocusBox.iRow && m_FocusBox.iColumn == x))
				{
					gp.DrawRectangle(&YellPen, rcImg.X+1, rcImg.Y+1, rcImg.Width-2, rcImg.Height-2);
				}
				else
					gp.DrawRectangle(&bluePen, rcImg.X+1, rcImg.Y+1, rcImg.Width-2, rcImg.Height-2);
			}

			if (GetOutputManager()->GetFeederID(x+CSwitchHeadBar::m_sFirstVisiableItem, FALSE) == GetInputManager()->GetID(y+CSwitchSourceBar::m_sFirstVisiableItem))
			{
				gp.FillRectangle(&CheckBr, rcImg.X+3, rcImg.Y+3, rcImg.Width-5, rcImg.Height-5);

			}	
		}
	}
}

void CSwitchSelectBar::DrawBoxMap2(CDC* pValidDC)
{
	ASSERT(pValidDC);

	CRect rcCli;
	GetClientRect(&rcCli);

	// 获取页面边距
	const CPageMargins *pMargins = ((CSwitchPage*)GetParent())->GetMargins();

	Graphics gp(pValidDC->m_hDC);
	gp.SetSmoothingMode(SmoothingModeHighQuality);

	// 画背景
	SolidBrush br(Color(255, 181, 213, 255));
	gp.FillRectangle(&br, rcCli.left, rcCli.top, rcCli.Width(), rcCli.Height());

	FontFamily fontFamily(L"宋体");
	//Gdiplus::Font font(&fontFamily, 12.0f, FontStyleRegular, UnitPixel);
	Gdiplus::Font Bfont(&fontFamily, 48.0f, FontStyleBold, UnitPixel);
	SolidBrush blackBr(Color(150, 235, 245, 250));
	StringFormat format;
	format.SetFormatFlags(StringFormatFlagsDirectionRightToLeft|StringFormatFlagsDirectionVertical);
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetTrimming(StringTrimmingEllipsisWord);	
	//format.SetFormatFlags(StringFormatFlagsNoWrap);

	CStringW strChlName;
	RectF rcItem;
	strChlName.Format(L"Author: RaphaeL.H 2016-05-20");
	rcItem.X = REAL(rcCli.right - pMargins->HeadBarMargin.iColumnWidthThd);
	rcItem.Y = REAL(rcCli.top );
	rcItem.Width = REAL(pMargins->HeadBarMargin.iColumnWidthThd);
	rcItem.Height = REAL(rcCli.Height());

	gp.DrawString(strChlName, -1, &Bfont, rcItem, &format, &blackBr);

	// 画选择框
	CDC* pPasteDC = new CDC;
	pPasteDC->CreateCompatibleDC(pValidDC);

	CBitmap bmp, *pOldBmp;
	int iBmpWidth = 0;
	int iBmpHeight = 0;

	CRect rcBox;
	CPen NormPen(PS_SOLID, 1, RGB(243, 250, 245));
	CPen* pOldPen;
	CBrush NormBr(RGB(243, 250, 245));
	
	if (pMargins->HeadBarMargin.iMaxVisiableItemsOnHorz >= pMargins->SourceBarMargin.iMaxVisiableItemsOnVert)
	{
		iBmpWidth  = pMargins->HeadBarMargin.iMaxVisiableItemsOnHorz * pMargins->SelectBarMargin.iItemWidth;
		iBmpHeight = pMargins->SelectBarMargin.iItemHeight;
		bmp.CreateCompatibleBitmap(pValidDC, iBmpWidth, iBmpHeight);
		pOldBmp = pPasteDC->SelectObject(&bmp);

		pPasteDC->FillSolidRect(0, 0, iBmpWidth, iBmpHeight, RGB(181, 213, 255));

		pOldPen = pPasteDC->SelectObject(&NormPen);
		//pPasteDC->SelectObject(&NormBr);

		//pPasteDC->Rectangle(0, 0, iBmpWidth, iBmpHeight);

		for (int x=0; x<pMargins->HeadBarMargin.iMaxVisiableItemsOnHorz; x++)
		{
			rcBox.left  = x*pMargins->SelectBarMargin.iItemWidth + 5 + (pMargins->SelectBarMargin.iItemWidth - pMargins->PageScale.iSelectBoxSize)/2;
			rcBox.top   = (pMargins->SelectBarMargin.iItemHeight - pMargins->PageScale.iSelectBoxSize)/2;
			rcBox.right = rcBox.left + pMargins->PageScale.iSelectBoxSize;
			rcBox.bottom= rcBox.top + pMargins->PageScale.iSelectBoxSize;

			pPasteDC->Rectangle(rcBox);
		}

		for (int y=0; y<pMargins->SourceBarMargin.iMaxVisiableItemsOnVert; y++)
		{
			pValidDC->BitBlt(0, pMargins->SelectBarMargin.iItemHeight*y, iBmpWidth, iBmpHeight, pPasteDC, 0, 0, SRCCOPY);
		}
	}
	else
	{
		iBmpWidth  = pMargins->SelectBarMargin.iItemWidth;
		iBmpHeight = pMargins->SourceBarMargin.iMaxVisiableItemsOnVert * pMargins->SelectBarMargin.iItemHeight;
		bmp.CreateCompatibleBitmap(pValidDC, iBmpWidth, iBmpHeight);
		pOldBmp = pPasteDC->SelectObject(&bmp);

		pPasteDC->FillSolidRect(0, 0, iBmpWidth, iBmpHeight, RGB(181, 213, 255));

		pOldPen = pPasteDC->SelectObject(&NormPen);
		//pPasteDC->SelectObject(&NormBr);

		for (int y=0; y<pMargins->SourceBarMargin.iMaxVisiableItemsOnVert; y++)
		{
			rcBox.left = 5 + (pMargins->SelectBarMargin.iItemWidth - pMargins->PageScale.iSelectBoxSize)/2;
			rcBox.top  = pMargins->SelectBarMargin.iItemHeight * y + (pMargins->SelectBarMargin.iItemHeight - pMargins->PageScale.iSelectBoxSize)/2;
			rcBox.right = rcBox.left + pMargins->PageScale.iSelectBoxSize;
			rcBox.bottom = rcBox.top + pMargins->PageScale.iSelectBoxSize;

			pPasteDC->Rectangle(rcBox);
		}

		for (int x=0; x<pMargins->HeadBarMargin.iMaxVisiableItemsOnHorz; x++)
		{
			pValidDC->BitBlt(pMargins->SelectBarMargin.iItemWidth * x, 0, iBmpWidth, iBmpHeight, pPasteDC, 0, 0, SRCCOPY);
		}
	}

	pPasteDC->SelectObject(pOldPen);
	pPasteDC->SelectObject(pOldBmp);
	ReleaseDC(pPasteDC);
	SAFE_DELETE(pPasteDC);

	CPen FocusPen(PS_SOLID, 2, RGB(253, 250, 190));
	CBrush CheckBr(RGB(234, 225, 145));
	pOldPen = pValidDC->SelectObject(&FocusPen);
	CRect rcRt;

	pValidDC->SelectStockObject(NULL_BRUSH);

	for (int y=0; y<pMargins->SelectBarMargin.iMaxVisiableItemsOnVert; y++)
	{
		for (int x=0; x<pMargins->SelectBarMargin.iMaxVisiableItemsOnHorz; x++)
		{
			rcBox.left = rcCli.left + 5 + x*pMargins->SelectBarMargin.iItemWidth + (pMargins->SelectBarMargin.iItemHeight-pMargins->PageScale.iSelectBoxSize)/2;
			rcBox.top = rcCli.top + y*pMargins->SelectBarMargin.iItemHeight + (pMargins->SelectBarMargin.iItemHeight-pMargins->PageScale.iSelectBoxSize)/2;
			rcBox.right = rcBox.left + pMargins->PageScale.iSelectBoxSize;
			rcBox.bottom = rcBox.top + pMargins->PageScale.iSelectBoxSize;

			rcRt = rcBox;
			rcRt.InflateRect(-1, -1);
		
			if (m_FocusBox.iColumn == -1 && y == m_FocusBox.iRow)
			{
				pValidDC->FillSolidRect(rcBox, RGB(181, 213, 255));
				pValidDC->Rectangle(rcRt);
			}
			else if (m_FocusBox.iRow == -1 && x == m_FocusBox.iColumn)
			{
				pValidDC->FillSolidRect(rcBox, RGB(181, 213, 255));
				pValidDC->Rectangle(rcRt);
			}
			else
			{
				if ((x <= m_FocusBox.iColumn && m_FocusBox.iRow == y) || (y <= m_FocusBox.iRow && m_FocusBox.iColumn == x))
				{
					pValidDC->FillSolidRect(rcBox, RGB(181, 213, 255));
					pValidDC->Rectangle(rcRt);
				}
			}

			rcRt.InflateRect(-3, -3);
			if (GetOutputManager()->GetFeederID(x+CSwitchHeadBar::m_sFirstVisiableItem, FALSE) == GetInputManager()->GetID(y+CSwitchSourceBar::m_sFirstVisiableItem))
			{
				pValidDC->FillSolidRect(rcRt, RGB(234, 225, 145));
				//gp.FillRectangle(&CheckBr, rcImg.X+3, rcImg.Y+3, rcImg.Width-5, rcImg.Height-5);
			}	
		}
	}

	pValidDC->SelectObject(pOldPen);
}

void CSwitchSelectBar::DrawBoxMap3(CDC* pValidDC)
{
	ASSERT(pValidDC);

	// 获取客户区大小
	CRect rcCli;
	GetClientRect(&rcCli);

	// 获取页面边距
	const CPageMargins *pMargins = ((CSwitchPage*)GetParent())->GetMargins();

	// 填充窗口背景
	pValidDC->FillSolidRect(rcCli.left, rcCli.top, rcCli.Width(), rcCli.Height(), RGB(181, 213, 255));

	CBrush *pOldBr = (CBrush *)pValidDC->SelectStockObject(NULL_BRUSH);
	CPen OlivPen(PS_SOLID, 1, RGB(101, 147, 207));
	CPen* pOldPen;
	pOldPen = pValidDC->SelectObject(&OlivPen);

	// 窗口边框线
	pValidDC->Rectangle( rcCli.left, rcCli.top, rcCli.Width(), rcCli.Height());

	Rect rcBox/*选择框的大小*/, rcImg/*图像的大小*/;
	rcBox = Rect(0, 0, pMargins->SelectBarMargin.iItemWidth, pMargins->SelectBarMargin.iItemHeight);
	rcImg = Rect(0, 0, pMargins->PageScale.iSelectBoxSize, pMargins->PageScale.iSelectBoxSize);

	// 准备三种选择框边界线的颜色
	CPen YellPen(PS_SOLID, 2, RGB(253, 43,  245)); //RGB(253, 243, 245)
	CPen BluePen(PS_SOLID, 1, RGB(243, 250, 245));
	CPen BlacPen(PS_DOT,   2, RGB(43 , 60 , 115));
	//LOGBRUSH logBrush;
	//logBrush.lbStyle = BS_SOLID;
	//logBrush.lbColor = RGB(43 , 50 , 145);
	//BlacPen.CreatePen(PS_DOT|PS_GEOMETRIC|PS_ENDCAP_ROUND, 2, &logBrush);
	
	CPen GrayPen(PS_SOLID, 1, RGB(203, 210, 205));
	pValidDC->SelectObject(&BluePen);

	CBrush CheckBr(RGB(134, 125, 45));//有输入信号的标识色
	//pValidDC->Rectangle(100, 100, 200, 350);

	// 此段循环绘制当前窗口范围内所有的选择框以及动态提示线（由一系列小方格组成的）
	for (int y=0; y<pMargins->SelectBarMargin.iMaxVisiableItemsOnVert; y++)
	{
		for (int x=0; x<pMargins->SelectBarMargin.iMaxVisiableItemsOnHorz; x++)
		{
			// 循环获取每个选择框的位置
			rcBox.X = rcCli.left + 5 + x*pMargins->SelectBarMargin.iItemWidth;
			rcBox.Y = rcCli.top + y*pMargins->SelectBarMargin.iItemHeight;
			rcImg.X = rcBox.X + (rcBox.Width-rcImg.Width)/2;
			rcImg.Y = rcBox.Y + (rcBox.Height-rcImg.Height)/2;
	
			if (m_FocusBox.iColumn == x && m_FocusBox.iRow == y)
			{
				// 交叉点（鼠标悬停在此选择框）
				pValidDC->SelectObject(&BlacPen);
				pValidDC->Rectangle( rcImg.X+1, rcImg.Y+1, rcImg.GetRight(), rcImg.GetBottom() );
			}
			else if (m_FocusBox.iColumn == -1 && y == m_FocusBox.iRow)
			{
				// 交叉线所在的行
				pValidDC->SelectObject(&YellPen);
				pValidDC->Rectangle( rcImg.X+1, rcImg.Y+1, rcImg.GetRight(), rcImg.GetBottom() );
			}
			else if (m_FocusBox.iRow == -1 && x == m_FocusBox.iColumn)
			{
				// 交叉线所在的列
				pValidDC->SelectObject(&YellPen);
				pValidDC->Rectangle( rcImg.X+1, rcImg.Y+1, rcImg.GetRight(), rcImg.GetBottom() );
			}
			else
			{
				if ((x < m_FocusBox.iColumn && m_FocusBox.iRow == y) || (y < m_FocusBox.iRow && m_FocusBox.iColumn == x))
				{
					// 交叉线
					pValidDC->SelectObject(&YellPen);
					pValidDC->Rectangle( rcImg.X+1, rcImg.Y+1, rcImg.GetRight(), rcImg.GetBottom() );
				}
				else
				{
					// 其它普通选择框
					pValidDC->SelectObject(&BluePen);
					pValidDC->Rectangle( rcImg.X+1, rcImg.Y+1, rcImg.GetRight()-1, rcImg.GetBottom()-1 );
					pValidDC->SelectObject(&GrayPen);
					pValidDC->Rectangle( rcImg.X+2, rcImg.Y, rcImg.GetRight(), rcImg.GetBottom()-2);
				}					
			}

			if (GetOutputManager()->GetFeederID(x+CSwitchHeadBar::m_sFirstVisiableItem, FALSE) == GetInputManager()->GetID(y+CSwitchSourceBar::m_sFirstVisiableItem))
			{
				// 标识此输出端口有输入信号
				pValidDC->FillRect(CRect(rcImg.X+3, rcImg.Y+3, rcImg.GetRight()-3, rcImg.GetBottom()-3), &CheckBr);
			}	
		}
	}

	pValidDC->SelectObject(pOldPen);
	pValidDC->SelectObject(pOldBr);

	//Graphics gp(pValidDC->m_hDC);
	//
	//Image tipImg(L"res\\panel\\tipshow.png");
	//gp.DrawImage(&tipImg, 0, 2, 0, 2, tipImg.GetWidth(), tipImg.GetHeight(), UnitPixel);
}

void CSwitchSelectBar::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{

	case TIMER_KILLFOCUS:
		{
			CRect rcCli;
			GetWindowRect(&rcCli);

			CPoint ptCursor;
			GetCursorPos(&ptCursor);

			if ( !rcCli.PtInRect(ptCursor) )
			{
				m_FocusBox = BOXITEM(-1, -1);

				CSwitchHeadBar* pHeadBar = (CSwitchHeadBar*)((CSwitchPage*)GetParent())->GetHeadBar();	
				pHeadBar->SetFocusItem(m_FocusBox.iColumn);

				CSwitchSourceBar* pSourceBar = (CSwitchSourceBar*)((CSwitchPage*)GetParent())->GetSourceBar();
				pSourceBar->SetFocuseItem(m_FocusBox.iRow);

				KillTimer(nIDEvent);
			}
		}

		break;

	default:break;;
	}

	CWnd::OnTimer(nIDEvent);
}

BOOL CSwitchSelectBar::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CWnd::OnEraseBkgnd(pDC);
}

void CSwitchSelectBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOXITEM boxitem = GetFocusBox(point);
	if (boxitem != BOXITEM(-1, -1))
	{
		GetOutputManager()->P2PSwitch(boxitem.iColumn+CSwitchHeadBar::m_sFirstVisiableItem, GetInputManager()->GetID(boxitem.iRow+CSwitchSourceBar::m_sFirstVisiableItem), TRUE, FALSE);
		
		Invalidate();
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CSwitchSelectBar::OnLButtonUp(UINT nFlags, CPoint point)
{

	CWnd::OnLButtonUp(nFlags, point);
}

void CSwitchSelectBar::OnMouseMove(UINT nFlags, CPoint point)
{
	OnHitTest(point);

	CWnd::OnMouseMove(nFlags, point);
}

void CSwitchSelectBar::OnHitTest(CPoint point)
{
	CRect rcCli;
	GetClientRect(&rcCli);

	CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

	CRect rcFunc;
	rcFunc = rcCli;
	rcFunc.left  = 5;
	rcFunc.right = pMargins->HeadBarMargin.iItemWidth*pMargins->HeadBarMargin.iMaxVisiableItemsOnHorz + 5; //pMargins->HeadBarMargin.iColumnWidthSec - 10;
	rcFunc.bottom = pMargins->SelectBarMargin.iItemHeight * pMargins->SourceBarMargin.iMaxVisiableItemsOnVert;

	BOXITEM PrevBox = m_FocusBox;

	if (!rcFunc.PtInRect(point))
	{
		m_FocusBox = BOXITEM(-1, -1);
	}
	else
	{
		int iCol = (point.x - rcFunc.left) / pMargins->SelectBarMargin.iItemWidth;
		int iRow = point.y / pMargins->SelectBarMargin.iItemHeight;

		CRect rcBox;
		rcBox.left  = iCol*pMargins->SelectBarMargin.iItemWidth + rcFunc.left;
		rcBox.top   = iRow*pMargins->SelectBarMargin.iItemHeight + rcFunc.top;
		rcBox.right = rcBox.left + pMargins->SelectBarMargin.iItemWidth;
		rcBox.bottom= rcBox.top + pMargins->SelectBarMargin.iItemHeight;

		rcBox.InflateRect(-(pMargins->SelectBarMargin.iItemWidth-pMargins->PageScale.iSelectBoxSize)/2, -(pMargins->SelectBarMargin.iItemHeight-pMargins->PageScale.iSelectBoxSize)/2);

		if (!PtInRect(rcBox, point))
		{
			m_FocusBox = BOXITEM(-1, -1);
		}
		else
		{
			m_FocusBox = BOXITEM(iCol, iRow);
		}
	}

	if (PrevBox != m_FocusBox)
	{
		Invalidate();

		SetTimer(TIMER_KILLFOCUS, 200, NULL);
	}

	CSwitchHeadBar* pHeadBar = (CSwitchHeadBar*)((CSwitchPage*)GetParent())->GetHeadBar();	
	pHeadBar->SetFocusItem(m_FocusBox.iColumn);

	CSwitchSourceBar* pSourceBar = (CSwitchSourceBar*)((CSwitchPage*)GetParent())->GetSourceBar();
	pSourceBar->SetFocuseItem(m_FocusBox.iRow);
		
}

BOXITEM CSwitchSelectBar::GetFocusBox(CPoint point)
{
	CRect rcCli;
	GetClientRect(&rcCli);

	CPageMargins* pMargins = (CPageMargins*)((CSwitchPage*)GetParent())->GetMargins();

	CRect rcFunc;
	rcFunc = rcCli;
	rcFunc.left  = 5;
	rcFunc.right = pMargins->HeadBarMargin.iItemWidth*pMargins->HeadBarMargin.iMaxVisiableItemsOnHorz + 5; //pMargins->HeadBarMargin.iColumnWidthSec - 10;
	rcFunc.bottom = pMargins->SelectBarMargin.iItemHeight * pMargins->SourceBarMargin.iMaxVisiableItemsOnVert;

	BOXITEM Boxitem;

	if (!rcFunc.PtInRect(point))
	{
		Boxitem = BOXITEM(-1, -1);
	}
	else
	{
		int iCol = (point.x - rcFunc.left) / pMargins->SelectBarMargin.iItemWidth;
		int iRow = point.y / pMargins->SelectBarMargin.iItemHeight;

		CRect rcBox;
		rcBox.left  = iCol*pMargins->SelectBarMargin.iItemWidth + rcFunc.left;
		rcBox.top   = iRow*pMargins->SelectBarMargin.iItemHeight + rcFunc.top;
		rcBox.right = rcBox.left + pMargins->SelectBarMargin.iItemWidth;
		rcBox.bottom= rcBox.top + pMargins->SelectBarMargin.iItemHeight;

		rcBox.InflateRect(-(pMargins->SelectBarMargin.iItemWidth-pMargins->PageScale.iSelectBoxSize)/2, -(pMargins->SelectBarMargin.iItemHeight-pMargins->PageScale.iSelectBoxSize)/2);

		if (!PtInRect(rcBox, point))
		{
			Boxitem = BOXITEM(-1, -1);
		}
		else
		{
			Boxitem = BOXITEM(iCol, iRow);
		}
	}

	return Boxitem;
}

void CSwitchSelectBar::SetFocusBox(const int iCol, const int iRow)
{
	if (m_FocusBox != BOXITEM(iCol, iRow))
	{
		m_FocusBox.iColumn = iCol;
		m_FocusBox.iRow = iRow;
		Invalidate();
	}	
}

void CSwitchSelectBar::SetFocusCol(const int iColumn)
{
	if (m_FocusBox.iColumn != iColumn)
	{
		m_FocusBox.iColumn = iColumn;
		Invalidate();
	}
}

void CSwitchSelectBar::SetFocusRow(const int iRow)
{
	if (m_FocusBox.iRow != iRow)
	{
		m_FocusBox.iRow = iRow;
		Invalidate();
	}
}

void CSwitchSelectBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CWnd::OnShowWindow(bShow, nStatus);

	SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
}
