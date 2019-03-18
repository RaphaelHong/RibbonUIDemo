// colorsetView.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGeoMainDlg.h"
#include "MGcolorsetDlg.h"
#include "MGcolorsetView.h"

#define BIG_CTRL_NODE  9
#define TINY_CTRL_NODE 32

//g_cstBigCtrlNode[9] 记录粗调标准刻度值以及粗调控制点的位置
const int g_cstBigCtrlNode[BIG_CTRL_NODE] = { 
	0, 31, 63, 95, 127, 159, 191, 223, 255
};

//g_cstTinyCtrlNode[32] 记录微调标准刻度值以及微调控制点的位置
const int g_cstTinyCtrlNode[TINY_CTRL_NODE] = {
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

// CcolorsetView

IMPLEMENT_DYNCREATE(CcolorsetView, CView)

CcolorsetView::CcolorsetView()
{
	InitMembers();
}

CcolorsetView::~CcolorsetView()
{
	if (m_pMsgTip)
	{
		m_pMsgTip->DestroyWindow();
		delete m_pMsgTip;
		m_pMsgTip = NULL;
	}
}

void CcolorsetView::InitMembers()
{
	m_pMsgTip = NULL;

	m_AjustType     = eRoughType;
	m_AjustMethod   = eLinearMode;
	m_eColor        = eRed;
	m_nColorSection = 5;
	m_nCtrlPointIndex = 0;

	m_bLBtnDownAtCtrlPoint = FALSE;
	m_nTestType = 0;
	
	for (int i=0; i<4; i++) m_ColorArray[i].ResetColor();
}

BEGIN_MESSAGE_MAP(CcolorsetView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CcolorsetView 绘图

void CcolorsetView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	
	//获取视图区大小
	CRect rcClient;
	GetClientRect(rcClient);

	//创建兼容DC
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	//创建空位图
	CBitmap *pOldbmp = dcMem.SelectObject(&MemBitmap); 
	//用指定颜色清除背景
	dcMem.FillSolidRect(0, 0, rcClient.right, rcClient.bottom, RGB(250,250,250)); 

	DrawChart(&dcMem);

	DrawTitle(&dcMem);

	DrawCurves(&dcMem);

	//pDC->TransparentBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, rcClient.Width(), rcClient.Height(), RGB(250,250,250));

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);

	dcMem.SelectObject(pOldbmp);
	ReleaseDC(&dcMem);
}

void CcolorsetView::DrawChart(CDC *pValidDC)
{
	if (m_AjustType == eRoughType)
		DrawRoughChart(pValidDC);
	else if (m_AjustType == ePreciseType)
		DrawPreciseChart(pValidDC);
}

void CcolorsetView::DrawRoughChart(CDC *pValidDC)
{
	CPen penBk,*pOldpen;
	penBk.CreatePen(PS_SOLID, 1, RGB(0x1a,0x1a,0x1a));
	pOldpen = pValidDC->SelectObject(&penBk);

	// 画图表的两条坐标轴
	pValidDC->MoveTo(CPoint(m_rcChart.left, m_rcChart.top));
	pValidDC->LineTo(CPoint(m_rcChart.left, m_rcChart.bottom+5));
	pValidDC->MoveTo(CPoint(m_rcChart.left-5, m_rcChart.bottom));
	pValidDC->LineTo(CPoint(m_rcChart.right, m_rcChart.bottom));

	// 刻度 （0,31,63,95...）
	CString strMark;
	CFont markFont,*pOldFont;
	VERIFY(markFont.CreateFont(
		10,                        // nHeight
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
	pOldFont = pValidDC->SelectObject(&markFont);
	pValidDC->SetBkMode(TRANSPARENT);
	pValidDC->SetTextColor(RGB(0,0,0));
	CPen  gridpen;
	CRect gridrect;
	gridpen.CreatePen(PS_SOLID, 1, RGB(220,220,197));

	// 画坐标轴刻度，相间的格子绘上不同的底色
	gridrect = CRect(m_rcChart.left+1, m_rcChart.top, m_rcChart.left+10, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
	//CPoint ptHorMark[BIG_CTRL_NODE], ptVerMark[BIG_CTRL_NODE];
	for(int i=0; i<BIG_CTRL_NODE; i++)
	{
		// 刻度字符
		strMark.Format(_T("%d"), g_cstBigCtrlNode[i]);

		// 纵向大刻度
		pValidDC->MoveTo(m_rcChart.left-8, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		pValidDC->TextOut(m_rcChart.left-25, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale)-3, strMark);
		//ptVerMark[i] = CPoint(m_rcChart.left, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));

		// 横向大刻度
		pValidDC->MoveTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.bottom);
		pValidDC->LineTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.bottom+8);
		pValidDC->TextOut(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale)-3, m_rcChart.bottom+10, strMark);
		//ptHorMark[i] = CPoint(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.bottom+8);

		if(i < BIG_CTRL_NODE-1)
		{
			// 相间颜色带
			if (i%2 == 1 && i>=1)
			{
				gridrect = CRect(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.top, m_rcChart.left+10+Round(g_cstBigCtrlNode[i+1]*m_Xscale), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
			}
			else if (i%2 ==0)
			{
				gridrect = CRect(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.top, m_rcChart.left+10+Round(g_cstBigCtrlNode[i+1]*m_Xscale), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(238,238,227));
			}

			// 画大刻度间的小刻度
			for (int j=1; j<5; j++)
			{
				// 纵向小刻度
				pValidDC->MoveTo(m_rcChart.left-5, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*m_Yscale) - Round(32*m_Yscale*j/5));
				pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*m_Yscale) - Round(32*m_Yscale*j/5));
				// 横向小刻度
				pValidDC->MoveTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale)+Round(32*m_Xscale*j/5), m_rcChart.bottom);
				pValidDC->LineTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale)+Round(32*m_Xscale*j/5), m_rcChart.bottom+5);
			}
		}
	}
	gridrect = CRect(m_rcChart.left+10+Round(g_cstBigCtrlNode[8]*m_Xscale), m_rcChart.top, m_rcChart.right, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(238,238,227));


	// 绘出表格中的的格子
	pValidDC->SelectObject(&gridpen);
	for (int i=0; i<BIG_CTRL_NODE; i++)
	{
		if(i > 0)
		{
			pValidDC->MoveTo(m_rcChart.left+1, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
			pValidDC->LineTo(m_rcChart.right, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		}
		pValidDC->MoveTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.bottom-1);
		pValidDC->LineTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*m_Xscale), m_rcChart.top);
	}

	// 横竖坐标的名称
	pValidDC->SelectObject(pOldpen);
	pValidDC->TextOut(m_rcChart.right+10, m_rcChart.bottom-3, _T("标准值"));
	pValidDC->TextOut(m_rcChart.left-20, m_rcChart.top-10, _T("校准值"));
	pValidDC->SelectObject(pOldFont);

}

void CcolorsetView::DrawPreciseChart(CDC *pValidDC)
{
	CPen penBk,*pOldpen;
	penBk.CreatePen(PS_SOLID, 1, RGB(0x1a,0x1a,0x1a));
	pOldpen = pValidDC->SelectObject(&penBk);

	// 画图表的两条坐标轴
	pValidDC->MoveTo(CPoint(m_rcChart.left,   m_rcChart.top));
	pValidDC->LineTo(CPoint(m_rcChart.left,   m_rcChart.bottom+5));
	pValidDC->MoveTo(CPoint(m_rcChart.left-5, m_rcChart.bottom));
	pValidDC->LineTo(CPoint(m_rcChart.right,  m_rcChart.bottom));

	// 刻度 (0,31,63,95...)
	CFont markFont,*pOldFont;
	VERIFY(markFont.CreateFont(
		10,                        // nHeight
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
	pOldFont = pValidDC->SelectObject(&markFont);
	pValidDC->SetBkMode(TRANSPARENT);
	pValidDC->SetTextColor(RGB(0,0,0));
	CPen  gridpen;
	CRect gridrect;
	gridpen.CreatePen(PS_SOLID, 1, RGB(220,220,197));

	// 画坐标轴刻度，相间的格子绘上不同的底色
	gridrect = CRect(m_rcChart.left+1, m_rcChart.top, m_rcChart.left+10, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
	int nStdSectionClr[32];
	m_ColorArray[m_eColor].GetStdSectionColor(nStdSectionClr, m_nColorSection);


	CString strHorMark, strVerMark;
	int nVerMark = 0;
	int nMarkOffset = 0; //将刻度字符对准到刻度

	//画横轴
	for(int i=0; i<TINY_CTRL_NODE; i++)
	{
		// 刻度字符
		strHorMark.Format(_T("%d"), nStdSectionClr[i]);

		if(nStdSectionClr[i] >=0 && nStdSectionClr[i]<10) nMarkOffset = 2;
		else if(nStdSectionClr[i]>=10 && nStdSectionClr[i]<100) nMarkOffset = 4;
		else nMarkOffset = 5;

		// 横向刻度
		pValidDC->MoveTo(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.bottom);
		pValidDC->LineTo(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.bottom+8);
		if (i%2 == 0) pValidDC->TextOut(m_rcChart.left+10+Round(i*m_Xscale*8)-nMarkOffset, m_rcChart.bottom+9, strHorMark);
		else pValidDC->TextOut(m_rcChart.left+10+Round(i*m_Xscale*8)-nMarkOffset, m_rcChart.bottom+14, strHorMark);

		if(i < TINY_CTRL_NODE-1)
		{
			if (i%2 == 1 && i>=1)
			{
				gridrect = CRect(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.top, m_rcChart.left+10+Round((i+1)*m_Xscale*8), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
			}
			else if (i%2 ==0)
			{
				gridrect = CRect(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.top, m_rcChart.left+10+Round((i+1)*m_Xscale*8), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(238,238,227));
			}
		}
	}
	gridrect = CRect(m_rcChart.left+10+Round(31*m_Xscale*8), m_rcChart.top, m_rcChart.right, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));

	//画纵轴
	for(int i=0; i<BIG_CTRL_NODE; i++)
	{
		// 刻度字符
		strVerMark.Format(_T("%d"), g_cstBigCtrlNode[i]);

		// 纵向大刻度
		pValidDC->MoveTo(m_rcChart.left-8, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		pValidDC->TextOut(m_rcChart.left-25, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale)-3, strVerMark);

		if(i < BIG_CTRL_NODE-1)
		{
			// 画大刻度间的小刻度
			for (int j=1; j<5; j++)
			{
				// 纵向小刻度
				pValidDC->MoveTo(m_rcChart.left-5, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*m_Yscale) - Round(32*m_Yscale*j/5));
				pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*m_Yscale) - Round(32*m_Yscale*j/5));
			}
		}
	}

	// 绘出表格中的的格子
	pValidDC->SelectObject(&gridpen);
	for (int i=0; i<TINY_CTRL_NODE; i++)
	{
		pValidDC->MoveTo(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.bottom-1);
		pValidDC->LineTo(m_rcChart.left+10+Round(i*m_Xscale*8), m_rcChart.top);
	}

	for (int i=0; i<BIG_CTRL_NODE; i++)
	{
		if(i > 0)
		{
			pValidDC->MoveTo(m_rcChart.left+1, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
			pValidDC->LineTo(m_rcChart.right, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*m_Yscale));
		}
	}

	// 横竖坐标的名称
	pValidDC->SelectObject(pOldpen);
	pValidDC->TextOut(m_rcChart.right+2, m_rcChart.bottom-3, _T("标准值"));
	pValidDC->TextOut(m_rcChart.left-20, m_rcChart.top-10, _T("校准值"));
	pValidDC->SelectObject(pOldFont);
}

void CcolorsetView::DrawTitle(CDC *pValidDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	CFont titleFont, *pOldFont;
	VERIFY(titleFont.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		TRUE,                      // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("楷体")));              // lpszFacename
	pOldFont = pValidDC->SelectObject(&titleFont);
	CSize textSize;
	CString strTitle;
	COLORREF colorTitle;
	strTitle   = m_eColor==eWhite ? _T("白色校准表") : m_eColor==eRed ? _T("红色校准表") : m_eColor == eGreen ? _T("绿色校准表") : _T("蓝色校准表") ;
	colorTitle = m_eColor==eWhite ? RGB(100, 50, 80) : m_eColor==eRed ? RGB(255, 40, 40) : m_eColor == eGreen ? RGB(10, 200, 40) : RGB(100, 100, 255) ;
	textSize = pValidDC->GetTextExtent(_T("红色校准表"));
	pValidDC->SetTextColor(colorTitle);
	pValidDC->TextOut(int((rcClient.Width()-textSize.cx)*0.5), rcClient.top+2, strTitle);

	pValidDC->SelectObject(pOldFont);
}

void CcolorsetView::DrawCurves(CDC *pValidDC)
{
	if (m_AjustType == eRoughType)
		DrawRoughCurve(pValidDC);
	else if (m_AjustType == ePreciseType)
		DrawPreciseCurve(pValidDC);
}

void CcolorsetView::DrawRoughCurve(CDC *pValidDC)
{
	CBrush br,brCtrl/*当前控制点的颜色*/,*pOldBr;
	CPen pen,*pOldpen;
	COLORREF colorCoodr;
	colorCoodr = m_eColor==eWhite ? RGB(200, 200, 200) : m_eColor==eRed ? RGB(255,50,50) : m_eColor == eGreen ? RGB(50,255,50) : RGB(50,50,255) ;
	br.CreateSolidBrush(colorCoodr);
	brCtrl.CreateSolidBrush(RGB(40,40,30));
	pen.CreatePen(PS_SOLID, 1, colorCoodr);
	pOldBr  = pValidDC->SelectObject(&br);
	pOldpen = pValidDC->SelectObject(&pen);

	int nDotColor = 0;
	int nNextDotColor = 0;

	//描颜色线
	for (int i=0; i<255; i++)
	{
		nDotColor     = m_ColorArray[m_eColor].DotColor(i);
		nNextDotColor = m_ColorArray[m_eColor].DotColor(i+1);
		pValidDC->MoveTo(m_rcChart.left + Round(i*m_Xscale)     + 10, m_rcChart.bottom - Round(nDotColor*m_Yscale));
		pValidDC->LineTo(m_rcChart.left + Round((i+1)*m_Xscale) + 10, m_rcChart.bottom - Round(nNextDotColor*m_Yscale));
	}

	//描控制点
	for(int i=0; i<BIG_CTRL_NODE; i++)
	{
		nDotColor = m_ColorArray[m_eColor].DotColor(i*32-1);

		//线性方式只画首尾两个控制点
		if(m_AjustMethod == eLinearMode)
		{
			if (i > 0 && i < BIG_CTRL_NODE-1)
			{
				continue;
			}
		}

		if (i == m_nCtrlPointIndex)
		{
			pValidDC->SelectObject(&brCtrl);
			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(g_cstBigCtrlNode[i]*m_Xscale) - 7 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) - 7,
				m_rcChart.left   + Round(g_cstBigCtrlNode[i]*m_Xscale) + 7 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) + 7));
		}
		else
		{
			pValidDC->SelectObject(&br);
			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(g_cstBigCtrlNode[i]*m_Xscale) - 5 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) - 5,
				m_rcChart.left   + Round(g_cstBigCtrlNode[i]*m_Xscale) + 5 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) + 5));
		}
	}

	pValidDC->SelectObject(pOldpen);
	pValidDC->SelectObject(pOldBr);
}

void CcolorsetView::DrawPreciseCurve(CDC *pValidDC)
{
	CBrush br, brCtrl/*控制点的颜色*/, *pOldBr;
	CPen pen, *pOldpen;
	COLORREF colorCoodr;
	colorCoodr = m_eColor==eWhite ? RGB(200, 200, 200) : m_eColor==eRed ? RGB(255,50,50) : m_eColor == eGreen ? RGB(50,255,50) : RGB(50,50,255);
	br.CreateSolidBrush(colorCoodr);
	brCtrl.CreateSolidBrush(RGB(40, 40, 30));
	pen.CreatePen(PS_SOLID, 1, colorCoodr);
	pOldBr = pValidDC->SelectObject(&br);
	pOldpen = pValidDC->SelectObject(&pen);

	int nDotColor = 0;
	int nNextDotColor = 0;
	//int nColorSel = m_eColor == WHITE ? 0 : m_eColor;
	//描颜色线
	for (int i=0; i<TINY_CTRL_NODE; i++)
	{
		nDotColor     = m_ColorArray[m_eColor].DotColor(m_nColorSection*32 +i);
		nNextDotColor = m_ColorArray[m_eColor].DotColor(m_nColorSection*32 +i+1);

		//连接两个控制点
		if (i<TINY_CTRL_NODE-1)
		{
			pValidDC->MoveTo(m_rcChart.left + Round(i*m_Xscale*8)     + 10, m_rcChart.bottom - Round(nDotColor*m_Yscale));
			pValidDC->LineTo(m_rcChart.left + Round((i+1)*m_Xscale*8) + 10, m_rcChart.bottom - Round(nNextDotColor*m_Yscale));
		}

		//绘制控制点
		if(i == m_nCtrlPointIndex) //当前控制点
		{
			pValidDC->SelectObject(&brCtrl);
			pValidDC->Ellipse(CRect(m_rcChart.left + Round(i*m_Xscale*8) + 10 -7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)-7,
				m_rcChart.left   + Round(i*m_Xscale*8) + 10 +7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)+7));
		}
		else
		{
			pValidDC->SelectObject(&br);
			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(i*m_Xscale*8) + 10 -5,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)-5,
				m_rcChart.left   + Round(i*m_Xscale*8) + 10 +5,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)+5));
		}
	}

	pValidDC->SelectObject(pOldpen);
	pValidDC->SelectObject(pOldBr);
}

// CcolorsetView 诊断

#ifdef _DEBUG
void CcolorsetView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CcolorsetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CcolorsetView 消息处理程序

BOOL CcolorsetView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

void CcolorsetView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);

	CalcChartPos(rcClient);
}

void CcolorsetView::CalcChartPos(CRect parentRc)
{
	int nChartWidth;
	nChartWidth = (parentRc.Width() >= parentRc.Height()) ? parentRc.Height() : parentRc.Width();
	nChartWidth -= 60;  //给表格周围腾出50像素的空余空间

	m_rcChart.left   = 30;  //long((parentRc.Width()-nChartWidth)*0.5);
	m_rcChart.right  = parentRc.Width()-35; //m_rcChart.left+nChartWidth;
	m_rcChart.top    = 30; //long((parentRc.Height()-nChartWidth)*0.5);
	m_rcChart.bottom = parentRc.Height()-30; //m_rcChart.top+nChartWidth;

	m_Xscale = double(m_rcChart.Width()-25) / 255.0;
	m_Yscale = double(m_rcChart.Height()-15) / 255.0;
}

void CcolorsetView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	m_bLBtnDownAtCtrlPoint = IsLBtnDownAtColorCtrlPoint(point);
	
	if (m_bLBtnDownAtCtrlPoint)
	{
		TestColor();
	}

	CView::OnLButtonDown(nFlags, point);
}

void CcolorsetView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	m_bLBtnDownAtCtrlPoint = FALSE;

	CView::OnLButtonUp(nFlags, point);
}

void CcolorsetView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// 不在表格区域内不响应
	if (PtInRect(m_rcChart, point))
	{
		if (m_AjustType == eRoughType)
		{
			m_nColorSection = GetColorSectionIndex(point);
		}

		m_AjustType = (m_AjustType == eRoughType) ? ePreciseType :eRoughType;

		//将当前控制点恢复到默认值
		m_nCtrlPointIndex = 0;

		Invalidate();
	}

	CView::OnLButtonDblClk(nFlags, point);
}

void CcolorsetView::OnMouseMove(UINT nFlags, CPoint point)
{
	//鼠标在控制点上时显示提示信息
	if (PtInRect(m_rcChart, point))
	{
		CMsgTipArray msg;
		int nIndex = IsMouseOverColorCtrlPoint(point);
		if (nIndex != -1) 
		{
			GetMsgTipInfo(msg, nIndex);
			ShowMsgTip(TRUE, msg);
		}
		else
		{
			ShowMsgTip(FALSE, msg);
		}
	}

	//色彩校正动作
	if (m_bLBtnDownAtCtrlPoint)
	{
		int nNewColorVal = Round(double(m_rcChart.bottom - point.y)/m_Yscale);
		AjustColor(nNewColorVal);
	}

	CView::OnMouseMove(nFlags, point);
}

void CcolorsetView::AjustColor(int nColorValue)
{
	//限制色彩范围0-255
	int nCurselColor = nColorValue;
	if (nCurselColor < 0) nCurselColor = 0;
	if (nCurselColor > 255) nCurselColor = 255;
			

	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if (m_AjustType == eRoughType)
	{
		//粗调

		if (m_AjustMethod == eLinearMode)
		{
			//线性模式
			if(m_nCtrlPointIndex != 0 && m_nCtrlPointIndex != 8) 	return;

			int nNewStart = m_ColorArray[m_eColor].DotColor(0);
			int nNewEnd   = m_ColorArray[m_eColor].DotColor(255);

			if(m_nCtrlPointIndex == 0) nNewStart = nCurselColor;
			else nNewEnd =nCurselColor;

			for (int i=0; i<256; i++)
			{	
				m_ColorArray[m_eColor].DotColor(i) = nNewStart + Round(double(nNewEnd-nNewStart)*i/255);
			}
		}
		else
		{
			//区段模式
			int nNewStart, nNewEnd;

			//下区间
			//第一个控制点没有下区间
			if (m_nCtrlPointIndex != 0)
			{
				nNewStart = m_ColorArray[m_eColor].DotColor((m_nCtrlPointIndex-1)*32-1);
				nNewEnd   = nCurselColor;

				for (int i=0; i<32; i++)	{
					m_ColorArray[m_eColor].DotColor((m_nCtrlPointIndex-1)*32 + i) = nNewStart + Round((double(nNewEnd-nNewStart))*i/31);
				}				
			}				

			//上区间
			//最后一个控制点，没有上区间
			if(m_nCtrlPointIndex != BIG_CTRL_NODE-1)
			{
				nNewStart = nCurselColor;
				nNewEnd   = m_ColorArray[m_eColor].DotColor((m_nCtrlPointIndex+1)*32-1);

				for (int i=0; i<32; i++) {
					m_ColorArray[m_eColor].DotColor(m_nCtrlPointIndex*32 + i) = nNewStart + Round((double(nNewEnd-nNewStart))*i/31);
				}
			}
		}
	}
	else
	{
		//微调
		int nNewColorValue = nCurselColor;

		m_ColorArray[m_eColor].DotColor(m_nColorSection*32+m_nCtrlPointIndex) = nNewColorValue;

	}

	Invalidate();

	WriteColor();  //写入RGB 256个数据， 纯白模式则写入256*3 RGB所有数据 

	TestColor(); //写入测试数据 ，具体为背景测试还是信号源测试 根据m_bTPenable 来确定(设备驱动可根据m_bTPenable参数调整 )
}


BOOL CcolorsetView::IsLBtnDownAtColorCtrlPoint(CPoint pt)
{
	int nDotColor = 0;
	CRect rcCtrlPoint;

	if (m_AjustType == eRoughType)
	{
		//粗调模式 9个控制点
		for(int i=0; i<BIG_CTRL_NODE; i++)
		{
			nDotColor = m_ColorArray[m_eColor].DotColor(i*32-1);

			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*32*m_Xscale) - 7 + 10,
				                m_rcChart.bottom - Round(nDotColor*m_Yscale) - 7,
				                m_rcChart.left   + Round(i*32*m_Xscale) + 7 + 10,
				                m_rcChart.bottom - Round(nDotColor*m_Yscale) + 7);

			if (PtInRect(rcCtrlPoint, pt))
			{
				m_nCtrlPointIndex = i;
				Invalidate();

				return TRUE;
			}
		}
	}
	else
	{
		//微调模式 32个控制点
		//int nMin = GetSectionMinValue(m_nColorSection);

		for (int i=0; i<TINY_CTRL_NODE; i++)
		{
			nDotColor   = m_ColorArray[m_eColor].DotColor(m_nColorSection*32 +i);
			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*m_Xscale*8) + 10 -7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)-7,
				m_rcChart.left   + Round(i*m_Xscale*8) + 10 +7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)+7);

			if (PtInRect(rcCtrlPoint, pt))
			{
				m_nCtrlPointIndex = i;
				Invalidate();

				return TRUE;
			}
		}	
	}

	return FALSE;
}

int CcolorsetView::GetColorSectionIndex(CPoint point)
{
	CRect rcColorSection;
	int nSection = 0;

	for (int i=0; i<8; i++)
	{
		rcColorSection = CRect(m_rcChart.left+10+int(i*32*m_Xscale),
			m_rcChart.top,
			m_rcChart.left+10+int((i+1)*32*m_Xscale),
			m_rcChart.bottom);

		if (PtInRect(rcColorSection, point))
		{
			nSection = i;
			break;
		}
	}

	return nSection;
}

void CcolorsetView::TestColor()
{
	// 开启测试图像
	//if (m_nTestType == 0) return;

	int valueRGB[3]={0};  //当前测试点的RGB值
	int valueTst[3]={0};  //当前测试点的RGB索引位置
	int nDot = 0;

	//计算当前颜色点位置
	if (m_AjustType == eRoughType)
		nDot = m_nCtrlPointIndex*32-1;                //粗调点
	else
		nDot = m_nColorSection*32+m_nCtrlPointIndex;  //微调点

	//获取当前控制点的颜色
	if (m_eColor == eWhite)
	{
		//纯白各颜色要乘以当前色阶

		int nColorValue;		
		for(int i=0; i<3; i++)
		{
			//RGB值
			if (nDot <= 0)
				nColorValue = Round( double(m_ColorArray[eWhite].DotColor(0)) * (double(m_ColorArray[i].DotColor(0)) / 1) );
			else
				nColorValue = Round( double(m_ColorArray[eWhite].DotColor(nDot)) * (double(m_ColorArray[i].DotColor(nDot)) / (double(nDot)) ) );

			nColorValue  = nColorValue > 255 ? 255 : nColorValue;
			valueRGB[i] = nColorValue;

			//测试点位置
			valueTst[i] = (nDot > 0) ? nDot : 0;
		}
	}
	else
	{
		//单色
		//RGB值
		valueRGB[m_eColor] = m_ColorArray[m_eColor].DotColor(nDot);
		//测试点位置
		for (int nColor=0; nColor<3; nColor++)
		{
			if (nColor != m_eColor)	{
				valueTst[nColor] = 256; //256
			}else {
				valueTst[nColor] = nDot > 0 ? nDot : 0;
			}
		}
	}

	ColorTestParams ctp;

	ctp.nRvalue   = valueRGB[0];
	ctp.nGvalue   = valueRGB[1];
	ctp.nBvalue   = valueRGB[2];	
	ctp.nRtestpos  = valueTst[0];
	ctp.nGtestpos  = valueTst[1];
	ctp.nBtestpos  = valueTst[2];
	ctp.nTPenable  = (UINT)m_nTestType;

	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)(((CMGcolorsetDlg*)GetParent())->GetOwner());

	theApp.MG_ColorTest(&ctp, pDlg->m_boardInfo.GetBoardPort());
}

void CcolorsetView::CancelTest()
{
	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)(((CMGcolorsetDlg*)GetParent())->GetOwner());
	theApp.MG_CancelColorTest(pDlg->m_boardInfo.GetBoardPort());
}

void CcolorsetView::WriteColor()
{
	UCHAR nParam[800] = {0};
	int tick = 0;
	UINT nRet;

	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)(((CMGcolorsetDlg*)GetParent())->GetOwner());
	int nPort = pOwner->m_boardInfo.GetBoardPort();

	if (m_eColor == eWhite)
	{
		//纯白 红绿蓝三色都要写
		int nColorValue;
		tick = 0;

		for (int ncolor=0; ncolor<3; ncolor++)
		{			
			nParam[tick++] = ncolor;
			//第一个点 除数本为0 强制为1
			nColorValue = Round ( double(m_ColorArray[eWhite].DotColor(0)) * (double(m_ColorArray[ncolor].DotColor(0))/1) );
			nParam[tick++] = nColorValue > 255 ? 255 : nColorValue;

			for (int i=1; i<256; i++)
			{
				nColorValue = Round( double(m_ColorArray[eWhite].DotColor(i)) * ( (double(m_ColorArray[ncolor].DotColor(i))) / (double(i)) ) );
				nParam[tick++] = nColorValue > 255 ? 255 : nColorValue;
			}
	
			//nRet = theApp.WriteColor(nParam, 256, ncolor, nPort);
		}

		nRet = theApp.MG_WriteColor(nParam, tick, 0x07, nPort);
	}
	else
	{
		//单色
		tick = 0;
		for (int i=0; i<256; i++)	{
			nParam[tick++] = m_ColorArray[m_eColor].DotColor(i);
		}

		//nRet = theApp.WriteColor(nParam, 256, m_eColor, nPort);
		nRet = theApp.MG_WriteColor(nParam, tick, 0x01<<m_eColor, nPort);
	}
}

void CcolorsetView::ResetColor()
{
	CMGeoMainDlg *pDlg = (CMGeoMainDlg*)(((CMGcolorsetDlg*)GetParent())->GetOwner());

	unsigned int nRet,nParam[10];
	nParam[0] = m_eColor == eWhite ? 0x07 : m_eColor;
	nRet = theApp.MG_ResetColor(nParam[0], pDlg->m_boardInfo.GetBoardPort());

	if (nRet != 0x00)
	{
		MessageBox(_T("复位失败，通信异常或者指令返回错误！"), _T("色差复位"), MB_OK|MB_ICONSTOP);
	}
	else
	{
		if (m_AjustType == eRoughType)//粗调
		{
			if (m_eColor == eWhite)
			{
				for (int i=0 ;i< 4; i++)
					m_ColorArray[i].ResetColor();
			}
			else
				m_ColorArray[m_eColor].ResetColor();
		}
		else  //细调
		{
			if (m_eColor == eWhite)
			{
				for (int i=0; i<4; i++)
					m_ColorArray[i].ResetSecColor(m_nColorSection);
			}
			else
				m_ColorArray[m_eColor].ResetSecColor(m_nColorSection);
		}

		Invalidate();

		MessageBox(_T("复位成功！"), _T("色差复位"), MB_OK|MB_ICONINFORMATION);
	}	
}

void CcolorsetView::OnVKeyUpDown(UINT nVKCode)
{
	//计算色彩值
	int nDotColor;
	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if (m_AjustType == eRoughType)
	{
		nDotColor = m_ColorArray[m_eColor].DotColor(m_nCtrlPointIndex*32-1);
	}
	else
	{
		nDotColor = m_ColorArray[m_eColor].DotColor(m_nColorSection*32 + m_nCtrlPointIndex);
	}

	if (nVKCode == VK_UP)
	{
		//按了↑方向键
		nDotColor ++;
	}
	else if (nVKCode == VK_DOWN)
	{
		//按了↓方向键
		nDotColor --;
	}

	AjustColor(nDotColor);

}

void CcolorsetView::SetColor(int *data, int nColorSel)
{	
	if (nColorSel > 3 || nColorSel < 0) return;

	if (nColorSel == 3)
	{//纯白
		for (int i=0; i<256; i++)	{
			m_ColorArray[nColorSel].DotColor(i) = Round((m_ColorArray[0].DotColor(i) + m_ColorArray[1].DotColor(i) + m_ColorArray[2].DotColor(i))/3.0);
		}
	}
	else
	{
		ASSERT(data);
		for (int i=0; i<256; i++)	{
			m_ColorArray[nColorSel].DotColor(i) = data[i];
		}
	}

}

void CcolorsetView::ShowMsgTip(BOOL bShow, CMsgTipArray &arMsgTip, BOOL bDockAtCursor, CPoint ptDockAt)
{
	if(m_pMsgTip == NULL)  
	{  
		m_pMsgTip = new CMsgTipDlg();  
		m_pMsgTip->Create(IDD_MSGTIPDLG, this);  
	} 
	if(!bShow)  
	{  
		if(m_pMsgTip->IsWindowVisible()) m_pMsgTip->ShowWindow(SW_HIDE);  
	}  
	else  
	{  
		CSize size = m_pMsgTip->ShowMsgTip(arMsgTip);  
		if(size.cx == 0)  
			return;

		CPoint pt; 
		if (bDockAtCursor) GetCursorPos(&pt); //提示栏紧随鼠标位置
		else 	pt = ptDockAt;  //指定提示栏位置

		CRect rc(pt,size);  
		rc.OffsetRect(15,20);  
		CRect mRc;  
		GetWindowRect(&mRc);  
		if(rc.bottom > mRc.bottom)  
			rc.OffsetRect(0,mRc.bottom-rc.bottom-5);  
		if(rc.right > mRc.right)  
		{
			rc.OffsetRect(-rc.Width()-30,0);
		}
		m_pMsgTip->SetWindowPos(&wndTop, rc.left, rc.top, rc.Width(),rc.Height(),SWP_SHOWWINDOW | SWP_NOACTIVATE);  
		m_pMsgTip->DrawMsgTip();  
		//m_pMsgTip->ShowWindow(SW_SHOW);
	}  
}

void CcolorsetView::GetMsgTipInfo(CMsgTipArray &arMsgTip, int nCtrlPtIndex)
{
	CString strInfo;
	MSG_TIP mt;
	COLORREF colorMsg;
	colorMsg =  m_eColor== eRed ? RGB(255,0,0) : m_eColor == eGreen ? RGB(0,255,0) : m_eColor == eBlue ? RGB(0,0,255) : RGB(100, 50, 80);

	//提示框的标题
	mt.nInfoShowColor = -1;
	mt.sMsgItemName = _T("色差校正");
	mt.sMsgItemInfo = _T("色差校正");	
	arMsgTip.Add(mt);


	//提示条目一 颜色
	strInfo = m_eColor == eRed ? _T("红色") : m_eColor == eGreen ? _T("绿色") : m_eColor == eBlue ? _T("蓝色") : _T("纯白");
	mt.nInfoShowColor = colorMsg;
	mt.sMsgItemName = _T("颜色");
	mt.sMsgItemInfo = strInfo;
	arMsgTip.Add(mt);


	//提示条目二 校准值
	int nDotColor, nStdDotColor;
	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if(m_AjustType == eRoughType)	{	nDotColor = m_ColorArray[m_eColor].DotColor(nCtrlPtIndex*32-1); }
	else	{	nDotColor = m_ColorArray[m_eColor].DotColor(m_nColorSection*32+nCtrlPtIndex);	}
	strInfo.Format(_T("%d"), nDotColor);
	mt.nInfoShowColor = colorMsg;
	mt.sMsgItemName   = _T("校准值");
	mt.sMsgItemInfo   = strInfo;
	arMsgTip.Add(mt);


	//提示条目三 标准值
	if(m_AjustType == eRoughType)	{	nStdDotColor = (nCtrlPtIndex*32-1) > 0 ? (nCtrlPtIndex*32-1) : 0 ; }
	else	{	nStdDotColor = m_nColorSection*32+nCtrlPtIndex;	}
	strInfo.Format(_T("%d"), nStdDotColor);
	mt.nInfoShowColor = colorMsg;
	mt.sMsgItemName   = _T("标准值");
	mt.sMsgItemInfo   = strInfo;
	arMsgTip.Add(mt);
}

int CcolorsetView::IsMouseOverColorCtrlPoint(CPoint point)
{
	int nDotColor = 0;
	CRect rcCtrlPoint;
	int nCtrlIndex = 0;

	if (m_AjustType == eRoughType)
	{
		//粗调模式 9个控制点
		for(int i=0; i<BIG_CTRL_NODE; i++)
		{
			nDotColor = m_ColorArray[m_eColor].DotColor(i*32-1);

			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*32*m_Xscale) - 7 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) - 7,
				m_rcChart.left   + Round(i*32*m_Xscale) + 7 + 10,
				m_rcChart.bottom - Round(nDotColor*m_Yscale) + 7);

			if (PtInRect(rcCtrlPoint, point))
			{
				nCtrlIndex = i;

				return nCtrlIndex;
			}
		}
	}
	else
	{
		//微调模式 32个控制点
		for (int i=0; i<TINY_CTRL_NODE; i++)
		{
			nDotColor   = m_ColorArray[m_eColor].DotColor(m_nColorSection*32 +i);
			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*m_Xscale*8) + 10 -7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)-7,
				m_rcChart.left   + Round(i*m_Xscale*8) + 10 +7,
				m_rcChart.bottom - Round(nDotColor*m_Yscale)+7);

			if (PtInRect(rcCtrlPoint, point))
			{
				nCtrlIndex = i;

				return nCtrlIndex;
			}
		}	
	}


	return -1;
}