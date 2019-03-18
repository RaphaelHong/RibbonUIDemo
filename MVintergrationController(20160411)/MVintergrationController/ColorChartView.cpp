// ColorChartView.cpp : 实现文件
//

#include "stdafx.h"
#include "ColorChartView.h"

//*******************************************************
//g_cstBigCtrlNode[9] 记录粗调标准刻度值以及粗调控制点的位置
const int g_cstBigCtrlNode[BIG_CTRL_NODE] = { 
	0, 31, 63, 95, 127, 159, 191, 223, 255
};    

//*******************************************************
//g_cstTinyCtrlNode[32] 记录微调标准刻度值以及微调控制点的位置
const int g_cstTinyCtrlNode[TINY_CTRL_NODE] = {
	0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
	16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
};

const CString strBtnName[MAX_BUTTON_NUM] = {
	_T("R"),
	_T("G"),
	_T("B"),
	_T("W"),
	_T("复位"),
	_T("线性"),
	_T("区段"),
};

const COLORREF cstClrRed   = RGB(255,0,0);
const COLORREF cstClrGreen = RGB(0,255,0);
const COLORREF cstClrBlue  = RGB(0,0,255);
const COLORREF cstClrWhite = RGB(200, 240, 220);
const COLORREF cstClrBtnClicked = RGB(80, 80, 150);
const COLORREF cstClrBtnMouseOver = RGB(150,145,195);
const COLORREF cstClrBtnNormal = RGB(223,211, 240);

int m_colorSel = 0;       //当前颜色 0:红色 1:绿色 2:蓝色 3:纯白
#define RED   0
#define GREEN 1
#define BLUE  2
#define WHITE 3

BOOL stMyButton::DrawMyButton(CDC* pDC)
{
	CPen pen;//按钮外框的颜色
	CPen *pOldPen;

	//无标题的为R G B 纯白四纯色按钮
	pen.CreatePen(PS_SOLID, 1, nID <= eBtn_White ? (0xffffff-crBtnNormal) : RGB(0x10, 0x10, 0xf0)); //RGB(0xe8, 0xe0, 0xb0)
	pOldPen = pDC->SelectObject(&pen);

	//画按钮的外框，纯色按钮只有当前选中的颜色才画
	CRect rc = rcBtn;
	if (nID > eBtn_White)
	{
		pDC->Rectangle(rcBtn);
		rc.InflateRect(-1,-1,-1,-1);
	}
	else
	{	
		if (m_colorSel == nID)
		{
			pDC->Rectangle(rcBtn);
			rc.InflateRect(-1,-1,-1,-1);
		}
	}

	//按钮的颜色
	COLORREF btnColor;
	CString strName;

	btnColor = bAlwaysHighLight ? crBtnClicked : (bClicked ? crBtnClicked : (bMouseOver ? crBtnMouseOver : crBtnNormal));
	strName = strFirstName;

	pDC->FillSolidRect(rc, btnColor);

	//按钮的文本	
	pDC->SetTextColor(0xffffff-btnColor);
	pDC->SetBkMode(TRANSPARENT);

	CSize textSize;
	textSize = pDC->GetTextExtent(strName);

	pDC->TextOut(rc.left+int((rc.Width()-textSize.cx)*0.5), rc.top+int((rc.Height()-textSize.cy)*0.5), strName);

	//恢复DC
	pDC->SelectObject(pOldPen);

	return TRUE;
}

BOOL stMyButton::IsButtonClicked(CPoint ptCursor)
{
	if (bMouseOver)
	{
		return bClicked = PtInRect(rcBtn, ptCursor);
	}

	return FALSE;
}

BOOL stMyButton::IsMouseOverBtn(CPoint ptCursor)
{
	bMouseOver = PtInRect(rcBtn, ptCursor);
	if (!bMouseOver) bClicked = FALSE;

	return bMouseOver;
}
// CColorChartView

IMPLEMENT_DYNCREATE(CColorChartView, CView)

CColorChartView::CColorChartView()
{
	InitVariables();
}

CColorChartView::~CColorChartView()
{
	if (m_pColorMsgTipDlg != NULL)
	{
		delete m_pColorMsgTipDlg;
		m_pColorMsgTipDlg = NULL;
	}
}

BEGIN_MESSAGE_MAP(CColorChartView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void CColorChartView::InitVariables()
{
	m_pColorMsgTipDlg = NULL;

	m_bTPenable = FALSE;  //开启纯色测试

	m_nBlendOption = 0xff; //关闭消除功能

	//m_bLinkage = FALSE;  //默认非联动模式

	m_nChannel = 0;//默认当前为第一台投影机

	m_colorSel = RED;//默认当前颜色为红色

	m_nColorSection = 0;//默认微调色段号

	m_nCtrlPointIndex = 0;//默认当前第一个控制点
	m_bLBtnDownInCtrlPoint = FALSE;

	//初始化颜色
	for (int i=0; i<7; i++)
	{
		for (int j=0; j<4; j++)
		{
			m_ColorArray[i][j].ResetColor();
		}
	}
	//默认粗调模式
	m_emColorAjustType = eRoughType;

	//默认区域模式
	m_emColorAjustMode = eRegionMode;

	//初始化各按钮的状态
	InitButtons();

}

void CColorChartView::InitButtons()
{
	//所有按钮的共性初始特征
	//默认按钮状态
	for (int i=0; i< MAX_BUTTON_NUM; i++)
	{
		m_myButton[i].bClicked   = FALSE;
		m_myButton[i].bMouseOver = FALSE;
		m_myButton[i].bRealesed  = TRUE;
		m_myButton[i].bAlwaysHighLight = FALSE;

		m_myButton[i].nID        = i;

		//默认标题
		m_myButton[i].strFirstName = strBtnName[i];
	}

	m_myButton[eBtn_Linear].bAlwaysHighLight = m_emColorAjustMode == eLinearMode;
	m_myButton[eBtn_Region].bAlwaysHighLight = m_emColorAjustMode == eRegionMode;

	//按钮各状态所呈现的颜色
	m_myButton[eBtn_Red].crBtnClicked   = m_myButton[eBtn_Red].crBtnMouseOver   = m_myButton[eBtn_Red].crBtnNormal   = cstClrRed;
	m_myButton[eBtn_Green].crBtnClicked = m_myButton[eBtn_Green].crBtnMouseOver = m_myButton[eBtn_Green].crBtnNormal = cstClrGreen;
	m_myButton[eBtn_Blue].crBtnClicked  = m_myButton[eBtn_Blue].crBtnMouseOver  = m_myButton[eBtn_Blue].crBtnNormal  = cstClrBlue;
	m_myButton[eBtn_White].crBtnClicked = m_myButton[eBtn_White].crBtnMouseOver = m_myButton[eBtn_White].crBtnNormal = cstClrWhite;
	for (int i=eBtn_Reset; i<= eBtn_Region; i++)
	{
		m_myButton[i].crBtnClicked   = cstClrBtnClicked;
		m_myButton[i].crBtnMouseOver = cstClrBtnMouseOver;
		m_myButton[i].crBtnNormal    = cstClrBtnNormal;
	}
}


// CColorChartView 绘图

void CColorChartView::OnDraw(CDC* pDC)
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
	dcMem.FillSolidRect(0, 0, rcClient.right, rcClient.bottom, RGB(0xff, 0xff, 0xff)); 

	//画视图外框
	CPen pen;
	CPen *pOldPen;
	pen.CreatePen(PS_SOLID, 1, RGB(0xe8, 0xe0, 0xb0));
	pOldPen = dcMem.SelectObject(&pen);
	dcMem.Rectangle(&rcClient);
	dcMem.SelectObject(pOldPen);

	// 计算图表的位置
	CalcChartPos(&rcClient);

	// 画图表
	DrawChart(&dcMem);

	// 图表的标题
	DrawTitles(&dcMem);

	// 画按钮
	CalcButtonPos(&dcMem);
	for(int i=0; i<MAX_BUTTON_NUM; i++)
	{
		if(m_emColorAjustType == ePreciseType && (i== eBtn_Linear || i== eBtn_Region))
			continue;

		m_myButton[i].DrawMyButton(&dcMem); 
	}


	//最后画曲线
	DrawCurves(&dcMem);

	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
}

// CColorChartView 诊断

#ifdef _DEBUG
void CColorChartView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CColorChartView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

// CColorChartView 消息处理程序
BOOL CColorChartView::OnEraseBkgnd(CDC* pDC)
{
	//防止闪烁
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

void CColorChartView::CalcChartPos(CRect *pParentRect)
{
	int nBkWidth;
	nBkWidth = (pParentRect->Width() >= pParentRect->Height()) ? pParentRect->Height() : pParentRect->Width();
	nBkWidth -= 80;
	m_rcChart.left   = long((pParentRect->Width()-nBkWidth)*0.5);
	m_rcChart.right  = m_rcChart.left + nBkWidth;
	m_rcChart.top    = long((pParentRect->Height()-nBkWidth)*0.5);
	m_rcChart.bottom = m_rcChart.top + nBkWidth;
}

void CColorChartView::CalcButtonPos(CDC *pValidDC)
{
	CSize TextExt;
	TextExt = pValidDC->GetTextExtent(_T("复位"));
	//先计算各按钮的位置
	//红色按钮
	m_myButton[eBtn_Red].rcBtn    = CRect(m_rcChart.right, 
		m_rcChart.bottom + 8, 
		m_rcChart.right+20,
		m_rcChart.bottom+28);
	//绿色按钮
	m_myButton[eBtn_Green].rcBtn  = CRect(m_rcChart.right+20,
		m_rcChart.bottom + 8,
		m_rcChart.right+40,
		m_rcChart.bottom+28);
	//蓝色按钮
	m_myButton[eBtn_Blue].rcBtn   = CRect(m_rcChart.right+40,
		m_rcChart.bottom + 8, 
		m_rcChart.right+60,
		m_rcChart.bottom+28);
	//白色按钮
	m_myButton[eBtn_White].rcBtn= CRect(m_rcChart.right+60,
		m_rcChart.bottom+8,
		m_rcChart.right+80,
		m_rcChart.bottom+28);
	//复位按钮
	m_myButton[eBtn_Reset].rcBtn  = CRect(m_rcChart.left-int(TextExt.cx*0.5)-5,
		m_rcChart.top - 30,
		m_rcChart.left + int(TextExt.cx*0.5)+5,
		m_rcChart.top-30+TextExt.cy+2);
	//线性按钮
	m_myButton[eBtn_Linear].rcBtn = CRect(m_rcChart.right+15-int(TextExt.cx*0.5)+5,
		m_rcChart.top+50,
		m_rcChart.right+15+int(TextExt.cx*0.5)+15,
		m_rcChart.top+50+TextExt.cy+2);
	//区域按钮
	m_myButton[eBtn_Region].rcBtn = CRect(m_rcChart.right+15+int(TextExt.cx*0.5)+20,
		m_rcChart.top+50, 
		m_rcChart.right+15+int(TextExt.cx*0.5)+20+int(TextExt.cx*1.5),
		m_rcChart.top+50+TextExt.cy+2);
}

void CColorChartView::DrawButtons(CDC *pValidDC)
{/*
 CPen pen,pen1;
 CPen *pOldPen;
 pen.CreatePen(PS_SOLID, 1, RGB(0x10, 0x10, 0xf0));
 pen1.CreatePen(PS_SOLID, 1, RGB(0xe8, 0xe0, 0xb0));

 CSize TextExt;
 TextExt = pValidDC->GetTextExtent(_T("复位"));
 //先计算各按钮的位置
 m_rcResetBtn  = CRect(m_rcChart.left-int(TextExt.cx*0.5)-5, m_rcChart.top - 30, m_rcChart.left + int(TextExt.cx*0.5)+5, m_rcChart.top-30+TextExt.cy);
 m_rcRgbBtn[0] = CRect(m_rcChart.right,    m_rcChart.bottom + 8, m_rcChart.right+15, m_rcChart.bottom+23);
 m_rcRgbBtn[1] = CRect(m_rcChart.right+15, m_rcChart.bottom + 8, m_rcChart.right+30, m_rcChart.bottom+23);
 m_rcRgbBtn[2] = CRect(m_rcChart.right+30, m_rcChart.bottom + 8, m_rcChart.right+45, m_rcChart.bottom+23);
 m_rcTypeBtn   = CRect(m_rcChart.right+15-int(TextExt.cx*0.5)+5, m_rcChart.top, m_rcChart.right+15+int(TextExt.cx*0.5)+15, m_rcChart.top+TextExt.cy);
 m_rcLinearBtn = CRect(m_rcChart.right+15-int(TextExt.cx*0.5)+5, m_rcChart.top+50, m_rcChart.right+15+int(TextExt.cx*0.5)+15, m_rcChart.top+50+TextExt.cy);
 m_rcRegionBtn = CRect(m_rcChart.right+15+int(TextExt.cx*0.5)+20, m_rcChart.top+50, m_rcChart.right+15+int(TextExt.cx*0.5)+20+int(TextExt.cx*1.5), m_rcChart.top+50+TextExt.cy);
 m_rcLinkageBtn= CRect(m_rcChart.right+15-int(TextExt.cx*0.5)+5, m_rcChart.top+50+TextExt.cy+3, m_rcChart.right+15+int(TextExt.cx*0.5)+15, m_rcChart.top+50+TextExt.cy*2+3);

 COLORREF clrBtnClicked = RGB(80, 80, 150);
 COLORREF clrBtnReleased= RGB(223,211, 240);
 //绘按钮
 //R G B 按钮
 pOldPen = pValidDC->SelectObject(&pen1);
 pValidDC->Rectangle(m_rcRgbBtn[m_colorSel]);
 m_rcRgbBtn[m_colorSel].InflateRect(-1,-1,-1,-1);
 pValidDC->FillSolidRect(m_rcRgbBtn[0], RGB(255,0,0));
 pValidDC->FillSolidRect(m_rcRgbBtn[1], RGB(0,255,0));
 pValidDC->FillSolidRect(m_rcRgbBtn[2], RGB(0,0,255));

 //复位按钮
 pValidDC->SelectObject(&pen);
 pValidDC->Rectangle(m_rcResetBtn);
 m_rcResetBtn.InflateRect(-1,-1,-1,-1);
 pValidDC->FillSolidRect(m_rcResetBtn, clrBtnReleased);
 pValidDC->SetTextColor(0xffffff-clrBtnReleased);
 pValidDC->SetBkMode(TRANSPARENT);
 pValidDC->TextOut(m_rcResetBtn.left + 5, m_rcResetBtn.top, _T("复位"));
 pValidDC->SelectObject(pOldPen);

 //粗调微调按钮
 pValidDC->Rectangle(m_rcTypeBtn);
 m_rcTypeBtn.InflateRect(-1,-1,-1,-1);
 pValidDC->FillSolidRect(m_rcTypeBtn, clrBtnReleased);
 pValidDC->SetTextColor(0xffffff-clrBtnReleased);
 pValidDC->TextOut(m_rcTypeBtn.left+5, m_rcTypeBtn.top, _T("粗调"));

 if (m_emColorAjustType == eRoughType)
 {
 //线性按钮
 pValidDC->Rectangle(m_rcLinearBtn);
 m_rcLinearBtn.InflateRect(-1,-1,-1,-1);
 pValidDC->FillSolidRect(m_rcLinearBtn, clrBtnReleased);
 pValidDC->SetTextColor(0xffffff-clrBtnReleased);
 pValidDC->TextOut(m_rcLinearBtn.left+5, m_rcLinearBtn.top, _T("线性"));

 //区域按钮
 pValidDC->Rectangle(m_rcRegionBtn);
 m_rcRegionBtn.InflateRect(-1,-1,-1,-1);
 pValidDC->FillSolidRect(m_rcRegionBtn, clrBtnClicked);
 pValidDC->SetTextColor(0xffffff-clrBtnClicked);
 pValidDC->TextOut(m_rcRegionBtn.left+5, m_rcRegionBtn.top, _T("区域"));
 }

 //联动按钮
 pValidDC->Rectangle(m_rcLinkageBtn);
 m_rcLinkageBtn.InflateRect(-1,-1,-1,-1);
 pValidDC->FillSolidRect(m_rcLinkageBtn, clrBtnReleased);
 pValidDC->SetTextColor(0xffffff-clrBtnReleased);
 pValidDC->TextOut(m_rcLinkageBtn.left+5, m_rcLinkageBtn.top, _T("联动"));

 pValidDC->SelectObject(&pOldPen);
 */
}

void CColorChartView::DrawTitles(CDC *pValidDC)
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
		_T("Arial")));             // lpszFacename
	pOldFont = pValidDC->SelectObject(&titleFont);
	CSize textSize;
	CString strTitle;
	COLORREF colorTitle;
	strTitle   = m_colorSel==WHITE ? _T("白色校准表") : m_colorSel==RED ? _T("红色校准表") : m_colorSel == GREEN ? _T("绿色校准表") : _T("蓝色校准表") ;
	colorTitle = m_colorSel==WHITE ? RGB(100, 50, 80) : m_colorSel==RED ? RGB(255,  0,  0) : m_colorSel == GREEN ? RGB(0,  255,  0) : RGB(0,  0,  255) ;
	textSize = pValidDC->GetTextExtent(_T("红色校准表"));
	pValidDC->SetTextColor(colorTitle);
	pValidDC->TextOut(int((rcClient.Width()-textSize.cx)*0.5), rcClient.top+2, strTitle);

	pValidDC->SelectObject(pOldFont);
}

void CColorChartView::DrawChart(CDC *pValidDC)
{
	if (m_emColorAjustType == eRoughType){
		DrawRoughChart(pValidDC);   }
	else if (m_emColorAjustType == ePreciseType){
		DrawPreciseChart(pValidDC); }
}

void CColorChartView::DrawRoughChart(CDC *pValidDC)
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
	CPoint ptHorMark[BIG_CTRL_NODE], ptVerMark[BIG_CTRL_NODE];
	for(int i=0; i<BIG_CTRL_NODE; i++)
	{
		// 刻度字符
		strMark.Format(_T("%d"), g_cstBigCtrlNode[i]);

		// 纵向大刻度
		pValidDC->MoveTo(m_rcChart.left-8, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6));
		pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6));
		pValidDC->TextOut(m_rcChart.left-25, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6)-3, strMark);
		ptVerMark[i] = CPoint(m_rcChart.left, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6));

		// 横向大刻度
		pValidDC->MoveTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*1.6), m_rcChart.bottom);
		pValidDC->LineTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*1.6), m_rcChart.bottom+8);
		pValidDC->TextOut(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*1.6)-3, m_rcChart.bottom+10, strMark);
		ptHorMark[i] = CPoint(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*1.6), m_rcChart.bottom+8);

		if(i < BIG_CTRL_NODE-1)
		{
			if (i%2 == 1 && i>=1)
			{
				gridrect = CRect(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*1.6), m_rcChart.top, m_rcChart.left+10+Round(g_cstBigCtrlNode[i+1]*1.6), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
			}
			else if (i%2 ==0)
			{
				gridrect = CRect(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*1.6), m_rcChart.top, m_rcChart.left+10+Round(g_cstBigCtrlNode[i+1]*1.6), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(238,238,227));
			}

			// 画大刻度间的小刻度
			for (int j=1; j<5; j++)
			{
				// 纵向小刻度
				pValidDC->MoveTo(m_rcChart.left-5, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*1.6) - Round(32*1.6*j/5));
				pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*1.6) - Round(32*1.6*j/5));
				// 横向小刻度
				pValidDC->MoveTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*1.6)+Round(32*1.6*j/5), m_rcChart.bottom);
				pValidDC->LineTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*1.6)+Round(32*1.6*j/5), m_rcChart.bottom+5);
			}
		}
	}
	gridrect = CRect(m_rcChart.left+10+Round(g_cstBigCtrlNode[8]*1.6), m_rcChart.top, m_rcChart.right, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(238,238,227));


	// 绘出表格中的的格子
	pValidDC->SelectObject(&gridpen);
	for (int i=0; i<BIG_CTRL_NODE; i++)
	{
		if(i > 0)
		{
			pValidDC->MoveTo(m_rcChart.left+1, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6));
			pValidDC->LineTo(m_rcChart.right, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6));
		}
		pValidDC->MoveTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*1.6), m_rcChart.bottom-1);
		pValidDC->LineTo(m_rcChart.left+10+Round(g_cstBigCtrlNode[i]*1.6), m_rcChart.top);
	}

	// 横竖坐标的名称
	pValidDC->SelectObject(pOldpen);
	pValidDC->TextOut(m_rcChart.right+2, m_rcChart.bottom-3, _T("标准值"));
	pValidDC->TextOut(m_rcChart.left-20, m_rcChart.top-10, _T("校准值"));
	pValidDC->SelectObject(pOldFont);

}

void CColorChartView::DrawPreciseChart(CDC *pValidDC)
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
	CPoint ptHorMark[TINY_CTRL_NODE], ptVerMark[TINY_CTRL_NODE];
	int nStdSectionClr[32];
	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	m_ColorArray[m_nChannel][m_colorSel].GetStdSectionColor(nStdSectionClr, m_nColorSection);

	//int nMin = GetSectionMinValue(m_nColorSection);
	//m_nMinSecColor = nMin;

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
		pValidDC->MoveTo(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.bottom);
		pValidDC->LineTo(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.bottom+8);
		if (i%2 == 0) pValidDC->TextOut(m_rcChart.left+10+Round(i*1.6*8)-nMarkOffset, m_rcChart.bottom+9, strHorMark);
		else pValidDC->TextOut(m_rcChart.left+10+Round(i*1.6*8)-nMarkOffset, m_rcChart.bottom+14, strHorMark);
		ptHorMark[i] = CPoint(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.bottom+8);

		if(i < TINY_CTRL_NODE-1)
		{
			if (i%2 == 1 && i>=1)
			{
				gridrect = CRect(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.top, m_rcChart.left+10+Round((i+1)*1.6*8), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
			}
			else if (i%2 ==0)
			{
				gridrect = CRect(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.top, m_rcChart.left+10+Round((i+1)*1.6*8), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(238,238,227));
			}
		}
	}
	gridrect = CRect(m_rcChart.left+10+Round(31*1.6*8), m_rcChart.top, m_rcChart.right, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));

	//画纵轴
	for(int i=0; i<BIG_CTRL_NODE; i++)
	{
		// 刻度字符
		strVerMark.Format(_T("%d"), g_cstBigCtrlNode[i]);

		// 纵向大刻度
		pValidDC->MoveTo(m_rcChart.left-8, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6));
		pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6));
		pValidDC->TextOut(m_rcChart.left-25, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6)-3, strVerMark);
		ptVerMark[i] = CPoint(m_rcChart.left, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6));

		if(i < BIG_CTRL_NODE-1)
		{
			// 画大刻度间的小刻度
			for (int j=1; j<5; j++)
			{
				// 纵向小刻度
				pValidDC->MoveTo(m_rcChart.left-5, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*1.6) - Round(32*1.6*j/5));
				pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom-Round(g_cstBigCtrlNode[i]*1.6) - Round(32*1.6*j/5));
			}
		}
	}

	// 绘出表格中的的格子
	pValidDC->SelectObject(&gridpen);
	for (int i=0; i<TINY_CTRL_NODE; i++)
	{
		pValidDC->MoveTo(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.bottom-1);
		pValidDC->LineTo(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.top);
	}

	for (int i=0; i<BIG_CTRL_NODE; i++)
	{
		if(i > 0)
		{
			pValidDC->MoveTo(m_rcChart.left+1, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6));
			pValidDC->LineTo(m_rcChart.right, m_rcChart.bottom- Round(g_cstBigCtrlNode[i]*1.6));
		}
	}

	// 横竖坐标的名称
	pValidDC->SelectObject(pOldpen);
	pValidDC->TextOut(m_rcChart.right+2, m_rcChart.bottom-3, _T("标准值"));
	pValidDC->TextOut(m_rcChart.left-20, m_rcChart.top-10, _T("校准值"));
	pValidDC->SelectObject(pOldFont);
}
/*
void CColorChartView::DrawPreciseChart(CDC *pValidDC)
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
	CPoint ptHorMark[TINY_CTRL_NODE], ptVerMark[TINY_CTRL_NODE];
	int nStdSectionClr[32];
	m_ColorArray[m_nChannel][m_colorSel].GetStdSectionColor(nStdSectionClr, m_nColorSection);

	//int nMin = GetSectionMinValue(m_nColorSection);
	//m_nMinSecColor = nMin;

	CString strHorMark, strVerMark;
	int nVerMark = 0;
	int nMarkOffset = 0; //将刻度字符对准到刻度
	for(int i=0; i<TINY_CTRL_NODE; i++)
	{
		// 校准值偏移作为纵坐标
		nVerMark = nStdSectionClr[i] + (m_nMinSecColor - m_nColorSection*32);
		// 刻度字符
		strHorMark.Format(_T("%d"), nStdSectionClr[i]);
		strVerMark.Format(_T("%d"), nVerMark);

		if(nStdSectionClr[i] >=0 && nStdSectionClr[i]<10) nMarkOffset = 2;
		else if(nStdSectionClr[i]>=10 && nStdSectionClr[i]<100) nMarkOffset = 4;
		else nMarkOffset = 5;

		// 纵向刻度
		pValidDC->MoveTo(m_rcChart.left-8, m_rcChart.bottom- Round(i*1.6*8));
		pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom- Round(i*1.6*8));
		pValidDC->TextOut(m_rcChart.left-22, m_rcChart.bottom- Round(i*1.6*8)-4, strVerMark);
		ptVerMark[i] = CPoint(m_rcChart.left, m_rcChart.bottom- Round(i*1.6*8));

		// 横向刻度
		pValidDC->MoveTo(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.bottom);
		pValidDC->LineTo(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.bottom+8);
		if (i%2 == 0) pValidDC->TextOut(m_rcChart.left+10+Round(i*1.6*8)-nMarkOffset, m_rcChart.bottom+9, strHorMark);
		else pValidDC->TextOut(m_rcChart.left+10+Round(i*1.6*8)-nMarkOffset, m_rcChart.bottom+14, strHorMark);
		ptHorMark[i] = CPoint(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.bottom+8);

		if(i < TINY_CTRL_NODE-1)
		{
			if (i%2 == 1 && i>=1)
			{
				gridrect = CRect(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.top, m_rcChart.left+10+Round((i+1)*1.6*8), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));
			}
			else if (i%2 ==0)
			{
				gridrect = CRect(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.top, m_rcChart.left+10+Round((i+1)*1.6*8), m_rcChart.bottom-1);
				pValidDC->FillSolidRect(&gridrect, RGB(238,238,227));
			}
		}
	}
	gridrect = CRect(m_rcChart.left+10+Round(31*1.6*8), m_rcChart.top, m_rcChart.right, m_rcChart.bottom-1);
	pValidDC->FillSolidRect(&gridrect, RGB(249,249,246));


	// 绘出表格中的的格子
	pValidDC->SelectObject(&gridpen);
	for (int i=0; i<TINY_CTRL_NODE; i++)
	{
		if(i > 0)
		{
			pValidDC->MoveTo(m_rcChart.left+1, m_rcChart.bottom- Round(i*1.6*8));
			pValidDC->LineTo(m_rcChart.right, m_rcChart.bottom- Round(i*1.6*8));
		}
		pValidDC->MoveTo(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.bottom-1);
		pValidDC->LineTo(m_rcChart.left+10+Round(i*1.6*8), m_rcChart.top);
	}

	// 横竖坐标的名称
	pValidDC->SelectObject(pOldpen);
	pValidDC->TextOut(m_rcChart.right+2, m_rcChart.bottom-3, _T("标准值"));
	pValidDC->TextOut(m_rcChart.left-20, m_rcChart.top-10, _T("校准值"));
	pValidDC->SelectObject(pOldFont);
}
*/
void CColorChartView::DrawCurves(CDC *pValidDC)
{
	if (m_emColorAjustType == eRoughType){
		DrawRoughCurve(pValidDC); }
	else if (m_emColorAjustType == ePreciseType){
		DrawPreciseCurve(pValidDC);	}
}

void CColorChartView::DrawRoughCurve(CDC *pValidDC)
{
	CBrush br,brCtrl/*当前控制点的颜色*/,*pOldBr;
	CPen pen,*pOldpen;
	COLORREF colorCoodr;
	colorCoodr = m_colorSel==WHITE ? RGB(100, 50, 80) : m_colorSel==RED ? RGB(255,50,50) : m_colorSel == GREEN ? RGB(50,255,50) : RGB(50,50,255) ;
	br.CreateSolidBrush(colorCoodr);
	brCtrl.CreateSolidBrush(RGB(0,40,30));
	pen.CreatePen(PS_SOLID, 1, colorCoodr);
	pOldBr  = pValidDC->SelectObject(&br);
	pOldpen = pValidDC->SelectObject(&pen);

	int nDotColor = 0;
	int nNextDotColor = 0;
	//int nColorSel = m_colorSel == WHITE ? 0 : m_colorSel;
	//描颜色线
	for (int i=0; i<255; i++)
	{
		nDotColor     = m_ColorArray[m_nChannel][m_colorSel].DotColor(i);
		nNextDotColor = m_ColorArray[m_nChannel][m_colorSel].DotColor(i+1);
		pValidDC->MoveTo(m_rcChart.left + Round(i*1.6)     + 10, m_rcChart.bottom - Round(nDotColor*1.6));
		pValidDC->LineTo(m_rcChart.left + Round((i+1)*1.6) + 10, m_rcChart.bottom - Round(nNextDotColor*1.6));
	}

	//描控制点
	for(int i=0; i<BIG_CTRL_NODE; i++)
	{
		nDotColor = m_ColorArray[m_nChannel][m_colorSel].DotColor(i*32-1);

		//线性方式只画首尾两个控制点
		if(m_emColorAjustMode == eLinearMode)
		{
			if (i > 0 && i < BIG_CTRL_NODE-1)
			{
				continue;
			}
		}

		if (i == m_nCtrlPointIndex)
		{
			pValidDC->SelectObject(&brCtrl);
			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(i*32*1.6) - 5 + 10,
				m_rcChart.bottom - Round(nDotColor*1.6) - 5,
				m_rcChart.left   + Round(i*32*1.6) + 5 + 10,
				m_rcChart.bottom - Round(nDotColor*1.6) + 5));
		}
		else
		{
			pValidDC->SelectObject(&br);
			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(i*32*1.6) - 3 + 10,
				m_rcChart.bottom - Round(nDotColor*1.6) - 3,
				m_rcChart.left   + Round(i*32*1.6) + 3 + 10,
				m_rcChart.bottom - Round(nDotColor*1.6) + 3));
		}
	}

	pValidDC->SelectObject(pOldpen);
	pValidDC->SelectObject(pOldBr);
}

void CColorChartView::DrawPreciseCurve(CDC *pValidDC)
{
	CBrush br, brCtrl/*控制点的颜色*/, *pOldBr;
	CPen pen, *pOldpen;
	COLORREF colorCoodr;
	colorCoodr = m_colorSel==WHITE ? RGB(100, 50, 80) : m_colorSel==RED ? RGB(255,50,50) : m_colorSel == GREEN ? RGB(50,255,50) : RGB(50,50,255);
	br.CreateSolidBrush(colorCoodr);
	brCtrl.CreateSolidBrush(RGB(0, 40, 30));
	pen.CreatePen(PS_SOLID, 1, colorCoodr);
	pOldBr = pValidDC->SelectObject(&br);
	pOldpen = pValidDC->SelectObject(&pen);

	int nDotColor = 0;
	int nNextDotColor = 0;
	//int nColorSel = m_colorSel == WHITE ? 0 : m_colorSel;
	//描颜色线
	for (int i=0; i<TINY_CTRL_NODE; i++)
	{
		nDotColor     = m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nColorSection*32 +i);
		nNextDotColor = m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nColorSection*32 +i+1);

		//连接两个控制点
		if (i<TINY_CTRL_NODE-1)
		{
			pValidDC->MoveTo(m_rcChart.left + Round(i*1.6*8)     + 10, m_rcChart.bottom - Round(nDotColor*1.6));
			pValidDC->LineTo(m_rcChart.left + Round((i+1)*1.6*8) + 10, m_rcChart.bottom - Round(nNextDotColor*1.6));
		}

		//绘制控制点
		if(i == m_nCtrlPointIndex) //当前控制点
		{
			pValidDC->SelectObject(&brCtrl);
			pValidDC->Ellipse(CRect(m_rcChart.left + Round(i*1.6*8) + 10 -5,
				m_rcChart.bottom - Round(nDotColor*1.6)-5,
				m_rcChart.left   + Round(i*1.6*8) + 10 +5,
				m_rcChart.bottom - Round(nDotColor*1.6)+5));
		}
		else
		{
			pValidDC->SelectObject(&br);
			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(i*1.6*8) + 10 -3,
				m_rcChart.bottom - Round(nDotColor*1.6)-3,
				m_rcChart.left   + Round(i*1.6*8) + 10 +3,
				m_rcChart.bottom - Round(nDotColor*1.6)+3));
		}
	}

	pValidDC->SelectObject(pOldpen);
	pValidDC->SelectObject(pOldBr);
}

//void CColorChartView::DrawPreciseCurve(CDC *pValidDC)
//{
//	CBrush br, brCtrl/*控制点的颜色*/, *pOldBr;
//	CPen pen, *pOldpen;
//	COLORREF colorCoodr;
//	colorCoodr = m_colorSel==0 ? RGB(255,50,50) : m_colorSel == 1 ? RGB(50,255,50) : RGB(50,50,255);
//	br.CreateSolidBrush(colorCoodr);
//	brCtrl.CreateSolidBrush(RGB(0, 40, 30));
//	pen.CreatePen(PS_SOLID, 1, colorCoodr);
//	pOldBr = pValidDC->SelectObject(&br);
//	pOldpen = pValidDC->SelectObject(&pen);
//
//	//int nMin = GetSectionMinValue(m_nColorSection);
//
//	int nDotColor = 0;
//	int nNextDotColor = 0;
//	//描颜色线
//	for (int i=0; i<TINY_CTRL_NODE; i++)
//	{
//		nDotColor     = m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nColorSection*32 +i)   - m_nColorSection*32 - (m_nMinSecColor-m_nColorSection*32);
//		nNextDotColor = m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nColorSection*32 +i+1) - m_nColorSection*32 - (m_nMinSecColor-m_nColorSection*32);
//
//		//连接两个控制点
//		if (i<TINY_CTRL_NODE-1)
//		{
//			pValidDC->MoveTo(m_rcChart.left + Round(i*1.6*8)     + 10, m_rcChart.bottom - Round(nDotColor*1.6*8));
//			pValidDC->LineTo(m_rcChart.left + Round((i+1)*1.6*8) + 10, m_rcChart.bottom - Round(nNextDotColor*1.6*8));
//		}
//
//		//绘制控制点
//		if(i == m_nCtrlPointIndex) //当前控制点
//		{
//			pValidDC->SelectObject(&brCtrl);
//			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(i*1.6*8) + 10 -5,
//				m_rcChart.bottom - Round(nDotColor*1.6*8)-5,
//				m_rcChart.left   + Round(i*1.6*8) + 10 +5,
//				m_rcChart.bottom - Round(nDotColor*1.6*8)+5));
//		}
//		else
//		{
//			pValidDC->SelectObject(&br);
//			pValidDC->Ellipse(CRect(m_rcChart.left   + Round(i*1.6*8) + 10 -3,
//				m_rcChart.bottom - Round(nDotColor*1.6*8)-3,
//				m_rcChart.left   + Round(i*1.6*8) + 10 +3,
//				m_rcChart.bottom - Round(nDotColor*1.6*8)+3));
//		}
//	}
//
//	pValidDC->SelectObject(pOldpen);
//	pValidDC->SelectObject(pOldBr);
//}


void CColorChartView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	//鼠标左键是否在功能按钮上按下
	IsMyButtonClicked(point);

	//鼠标左键是否在色彩控制点上按下
	m_bLBtnDownInCtrlPoint = IsLBtnDownAtColorCtrlPoint(point);


		//色彩校正动作
	if (m_bLBtnDownInCtrlPoint)
	{
		CmdTestColor(m_nChannel); //写入测试数据 ，具体为背景测试还是信号源测试 根据m_bTPenable 来确定(设备驱动可根据m_bTPenable参数调整 )
	}

	CView::OnLButtonDown(nFlags, point);
}

void CColorChartView::IsMyButtonClicked(CPoint point)
{
	for (int i=0; i<MAX_BUTTON_NUM; i++)
	{
		if(m_emColorAjustType == ePreciseType && (i == eBtn_Linear || i== eBtn_Region))
		{
			continue;
		}

		m_myButton[i].IsButtonClicked(point);
		CDC *pDC = GetDC();
		m_myButton[i].DrawMyButton(pDC);
		ReleaseDC(pDC);
	}
}

BOOL CColorChartView::IsLBtnDownAtColorCtrlPoint(CPoint point)
{
	int nDotColor = 0;
	CRect rcCtrlPoint;
	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if (m_emColorAjustType == eRoughType)
	{
		//粗调模式 9个控制点
		for(int i=0; i<BIG_CTRL_NODE; i++)
		{
			nDotColor = m_ColorArray[m_nChannel][m_colorSel].DotColor(i*32-1);

			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*32*1.6) - 5 + 10,
				m_rcChart.bottom - Round(nDotColor*1.6) - 5,
				m_rcChart.left   + Round(i*32*1.6) + 5 + 10,
				m_rcChart.bottom - Round(nDotColor*1.6) + 5);

			if (PtInRect(rcCtrlPoint, point))
			{
				m_nCtrlPointIndex = i;
				InvalidateRect(CRect(m_rcChart.left, m_rcChart.top, m_rcChart.right, m_rcChart.bottom+5));

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
			nDotColor   = m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nColorSection*32 +i);
			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*1.6*8) + 10 -5,
				m_rcChart.bottom - Round(nDotColor*1.6)-5,
				m_rcChart.left   + Round(i*1.6*8) + 10 +5,
				m_rcChart.bottom - Round(nDotColor*1.6)+5);

			if (PtInRect(rcCtrlPoint, point))
			{
				m_nCtrlPointIndex = i;
				InvalidateRect(CRect(m_rcChart.left, m_rcChart.top, m_rcChart.right, m_rcChart.bottom+5));

				return TRUE;
			}
		}	
	}

	return FALSE;
}

void CColorChartView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	//鼠标左键弹起事件，是否在功能按钮上弹起
	for (int i=0; i<MAX_BUTTON_NUM; i++)
	{
		if(m_emColorAjustType == ePreciseType && (i == eBtn_Linear || i== eBtn_Region))
		{
			continue;
		}

		if(m_myButton[i].bClicked)
		{
			OnMyButtonClicked((emButtonID)i);

			m_myButton[i].bClicked = FALSE;

			CDC *pDC = GetDC();
			m_myButton[i].DrawMyButton(pDC);
			ReleaseDC(pDC);			
		}
	}	

	//重置控制变量
	m_bLBtnDownInCtrlPoint = FALSE;

	CView::OnLButtonUp(nFlags, point);
}

void CColorChartView::OnMouseMove(UINT nFlags, CPoint point)
{
	//鼠标在功能按钮上浮动事件
	BOOL oldstat, newstat;
	for (int i=0; i<MAX_BUTTON_NUM; i++)
	{
		//微调模式下，线性和区域按钮被隐藏，不响应鼠标消息
		if(m_emColorAjustType == ePreciseType && (i == eBtn_Linear || i== eBtn_Region))
		{
			continue;
		}
		
		oldstat = m_myButton[i].bMouseOver;
		newstat = m_myButton[i].IsMouseOverBtn(point);
		//防止鼠标移动导致按钮不断被重画
		if (oldstat != newstat)
		{
			CDC *pDC = GetDC();
			m_myButton[i].DrawMyButton(pDC);
			ReleaseDC(pDC);
		}
	}

	//鼠标在控制点上时显示提示信息
	if (PtInRect(m_rcChart, point))
	{
		CMsgTipArray msg;
		int nIndex = IsMouseOverColorCtrlPoint(point);
		if (nIndex != -1) 
		{
			GetMsgTipInfo(msg, point, nIndex);
			ShowMsgTip(TRUE, msg);
		}
		else
		{
			ShowMsgTip(FALSE, msg);
		}
	}

	//色彩校正动作
	if (m_bLBtnDownInCtrlPoint)
	{
		int nNewColorVal = Round(double(m_rcChart.bottom - point.y)/1.6);
		AjustColor(nNewColorVal);
	}

	CView::OnMouseMove(nFlags, point);
}

void CColorChartView::AjustColor(int nColorValue)
{
	//限制色彩范围0-255
	int nCurselColor = nColorValue;
	if ((nCurselColor < 0) || (nCurselColor > 255)) return;
			

	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if (m_emColorAjustType == eRoughType)
	{
		//粗调

		if (m_emColorAjustMode == eLinearMode)
		{
			//线性模式
			if(m_nCtrlPointIndex != 0 && m_nCtrlPointIndex != 8) 	return;

			int nNewStart = m_ColorArray[m_nChannel][m_colorSel].DotColor(0);
			int nNewEnd   = m_ColorArray[m_nChannel][m_colorSel].DotColor(255);

			if(m_nCtrlPointIndex == 0) nNewStart = nCurselColor;
			else nNewEnd =nCurselColor;

			for (int i=0; i<256; i++)
			{	
				m_ColorArray[m_nChannel][m_colorSel].DotColor(i) = nNewStart + Round(double(nNewEnd-nNewStart)*i/255);
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
				nNewStart = m_ColorArray[m_nChannel][m_colorSel].DotColor((m_nCtrlPointIndex-1)*32-1);
				nNewEnd   = nCurselColor;

				for (int i=0; i<32; i++)	{
					m_ColorArray[m_nChannel][m_colorSel].DotColor((m_nCtrlPointIndex-1)*32 + i) = nNewStart + Round((double(nNewEnd-nNewStart))*i/31);
				}				
			}				

			//上区间
			//最后一个控制键，没有上区间
			if(m_nCtrlPointIndex != BIG_CTRL_NODE-1)
			{
				nNewStart = nCurselColor;
				nNewEnd   = m_ColorArray[m_nChannel][m_colorSel].DotColor((m_nCtrlPointIndex+1)*32-1);

				for (int i=0; i<32; i++) {
					m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nCtrlPointIndex*32 + i) = nNewStart + Round((double(nNewEnd-nNewStart))*i/31);
				}
			}
		}
	}
	else
	{
		//微调
		int nNewColorValue = nCurselColor;

		m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nColorSection*32+m_nCtrlPointIndex) = nNewColorValue;

	}

	InvalidateRect(CRect(m_rcChart.left, m_rcChart.top, m_rcChart.right, m_rcChart.bottom+5));

	CmdWriteColor();  //写入RGB 256个数据， 纯白模式则写入256*3 RGB所有数据 
	//if (m_bTPenable)

	CmdTestColor(m_nChannel); //写入测试数据 ，具体为背景测试还是信号源测试 根据m_bTPenable 来确定(设备驱动可根据m_bTPenable参数调整 )
	/*else
		CmdWriteColor();*/
}


void CColorChartView::OnMyButtonClicked(emButtonID eID)
{
	switch(eID)
	{
	case eBtn_Red:
	case eBtn_Green:
	case eBtn_Blue:		
	case eBtn_White:
		m_colorSel = int(eID);
		Invalidate();
		break;		
	case eBtn_Reset:
		//复位
		ResetColor();
		break;
	case eBtn_Linear:
		//线性 曲线切换
		if(m_emColorAjustType == eRoughType)
		{
			if (m_emColorAjustMode == eLinearMode) break;
			else m_emColorAjustMode = eLinearMode;

			CDC *pDC = this->GetDC();

			m_myButton[eBtn_Linear].bAlwaysHighLight = m_emColorAjustMode == eLinearMode;
			m_myButton[eBtn_Region].bAlwaysHighLight = m_emColorAjustMode == eRegionMode;
			m_myButton[eBtn_Region].DrawMyButton(pDC);
			InvalidateRect(CRect(m_rcChart.left, m_rcChart.top, m_rcChart.right, m_rcChart.bottom+5));

			ReleaseDC(pDC);
		}
		break;
	case eBtn_Region:
		if(m_emColorAjustType == eRoughType)
		{
			if (m_emColorAjustMode == eRegionMode) break;
			else m_emColorAjustMode = eRegionMode;

			CDC *pDC = this->GetDC();

			m_myButton[eBtn_Linear].bAlwaysHighLight = m_emColorAjustMode == eLinearMode;
			m_myButton[eBtn_Region].bAlwaysHighLight = m_emColorAjustMode == eRegionMode;
			m_myButton[eBtn_Linear].DrawMyButton(pDC);
			InvalidateRect(CRect(m_rcChart.left, m_rcChart.top, m_rcChart.right, m_rcChart.bottom+5));

			ReleaseDC(pDC);
		}
		break;
	default:break;
	}
}

void CColorChartView::ResetColor()
{
	unsigned int nRet,nParam[10];

	if (m_emColorAjustType == eRoughType)//粗调
	{
		if (m_colorSel == WHITE)
		{
			for (int i=0 ;i< 4; i++)
				m_ColorArray[m_nChannel][i].ResetColor();
		}
		else
			m_ColorArray[m_nChannel][m_colorSel].ResetColor();
	}
	else  //细调
	{
		if (m_colorSel == WHITE)
		{
			for (int i=0; i<4; i++)
				m_ColorArray[m_nChannel][i].ResetSecColor(m_nColorSection);
		}
		else
			m_ColorArray[m_nChannel][m_colorSel].ResetSecColor(m_nColorSection);
	}

	if (m_colorSel == WHITE)
	{
		nParam[0] = 0x07;
		nRet = theApp.NormalASCII_RW("RSTC", nParam, 1, m_nChannel);
		if (nRet != 0x00)
		{
			AfxMessageBox(_T("复位失败！"));
		}
	}
	else
	{
		nParam[0] = m_colorSel;
		nRet = theApp.NormalASCII_RW("RSTC", nParam, 1, m_nChannel);
		if (nRet != 0x00)
		{
			AfxMessageBox(_T("复位失败！"));
		}
	}


	//CmdWriteColor();

	InvalidateRect(m_rcChart);
}

void CColorChartView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// 不在表格区域内不响应
	if (PtInRect(m_rcChart, point))
	{
		if (m_emColorAjustType == eRoughType)
		{
			m_nColorSection = GetColorSectionIndex(point);
		}

		m_emColorAjustType = (m_emColorAjustType == eRoughType) ? ePreciseType :eRoughType;

		//将当前控制点恢复到默认值
		m_nCtrlPointIndex = 0;

		Invalidate();
	}

	CView::OnLButtonDblClk(nFlags, point);
}

int CColorChartView::GetColorSectionIndex(CPoint point)
{
	CRect rcColorSection;
	int nSection = 0;

	for (int i=0; i<8; i++)
	{
		rcColorSection = CRect(m_rcChart.left+10+int(i*32*1.6),
			m_rcChart.top,
			m_rcChart.left+10+int((i+1)*32*1.6),
			m_rcChart.bottom);

		if (PtInRect(rcColorSection, point))
		{
			nSection = i;
			break;
		}
	}

	return nSection;
}

int CColorChartView::GetSectionMinValue(int nSectionIndex)
{
	int nMin, nMax;
	int nSecColor[32];
	m_ColorArray[m_nChannel][m_colorSel].GetSectionColor(nSecColor, nSectionIndex);

	FindMaxAndMinMethod4(nSecColor, 0, 31, nMax, nMin);

	return nMin;
}

void CColorChartView::CmdWriteColor()
{
	unsigned char nParam[800];
	unsigned int nRet;
	int tick = 0;

	if (m_colorSel == WHITE)
	{
		//纯白 红绿蓝三色都要写
		int nColorValue;
		
		tick = 0;
		for (int ncolor=0; ncolor<3; ncolor++)
		{
			//nParam[tick++] = ncolor;
			//第一个点 除数本为0 强制为1
			nColorValue = Round ( double(m_ColorArray[m_nChannel][WHITE].DotColor(0)) * (double(m_ColorArray[m_nChannel][ncolor].DotColor(0))/1) );
			nParam[tick++] = nColorValue > 255 ? 255 : nColorValue;

			for (int i=1; i<256; i++)
			{
				nColorValue = Round( double(m_ColorArray[m_nChannel][WHITE].DotColor(i)) * ( (double(m_ColorArray[m_nChannel][ncolor].DotColor(i))) / (double(i)) ) );
				nParam[tick++] = nColorValue > 255 ? 255 : nColorValue;
			}
			//SetColor((int *)&nParam[1], ncolor, m_nChannel);
			//nRet = theApp.NormalASCII_RW("WCRA", nParam, tick, m_nChannel);
		}		
		int return_back = theApp.BinCommand_RW(BINEXE_COLOR_WRITE|BINCODE_FCN, 0x07 , nParam, 256*3, m_nChannel);  //

		if(return_back&0x10000) //通信失败 
		{			
		}
	}
	else
	{
		//单色
		tick = 0;

		//nParam[tick++] = m_colorSel;
		for (int i=0; i<256; i++)
		{
			nParam[tick++] = m_ColorArray[m_nChannel][m_colorSel].DotColor(i);
		}

		int return_back = theApp.BinCommand_RW(BINEXE_COLOR_WRITE|BINCODE_FCN, 0x01<<m_colorSel , nParam, 256, m_nChannel);

		if(return_back&0x10000) //通信失败 
		{			
		}

	}
}

//< TCLR, R_Val, G_Val, B_Val , R_Tst, G_Tst, B_Tst, TP_Enable>
void CColorChartView::CmdTestColor(int nPort)
{
	int valueRGB[3]={0};  //当前测试点的RGB值
	int valueTst[3]={0};  //当前测试点的RGB索引位置
	int nDot = 0;

	//计算当前颜色点位置
	if (m_emColorAjustType == eRoughType)
		nDot = m_nCtrlPointIndex*32-1;                //粗调点
	else
		nDot = m_nColorSection*32+m_nCtrlPointIndex;  //微调点

	//获取当前控制点的颜色
	if (m_colorSel == WHITE)
	{
		//纯白各颜色要乘以当前色阶

		int nColorValue;		
		for(int i=0; i<3; i++)
		{
			//RGB值
			if (nDot <= 0)
				nColorValue = Round( double(m_ColorArray[nPort][WHITE].DotColor(0)) * (double(m_ColorArray[nPort][i].DotColor(0)) / 1) );
			else
				nColorValue = Round( double(m_ColorArray[nPort][WHITE].DotColor(nDot)) * (double(m_ColorArray[nPort][i].DotColor(nDot)) / (double(nDot)) ) );

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
		valueRGB[m_colorSel] = m_ColorArray[nPort][m_colorSel].DotColor(nDot);
		//测试点位置
		for (int nColor=0; nColor<3; nColor++)
		{
			if (nColor != m_colorSel)	{
				valueTst[nColor] = 256; //256
			}else {
				valueTst[nColor] = nDot > 0 ? nDot : 0;
			}
		}
	}

	unsigned int cmdpara_buff[7];
	unsigned int re_len;

	cmdpara_buff[0]  = valueRGB[0]; cmdpara_buff[1]  = valueRGB[1];	cmdpara_buff[2]  = valueRGB[2];	
	cmdpara_buff[3]  = valueTst[0]; cmdpara_buff[4]  = valueTst[1]; cmdpara_buff[5]  = valueTst[2];
	cmdpara_buff[6]  = (unsigned int)m_bTPenable;

	re_len = theApp.NormalASCII_RW("TCLR", cmdpara_buff, 7, nPort);   //< TCLR>  测试色差校正
}

void CColorChartView::CmdCancelColorTest()
{
	//< CCLR >  // clear color 取消色差校正测试
	unsigned int cmdpara_buff[4];
	unsigned int re_len;

	re_len = theApp.NormalASCII_RW("CCLR", cmdpara_buff, 0, m_nChannel);  //取消色差校正测试 
	if(re_len&0x10000)//通信失败 
	{
		theApp.Print_Comm_Error_Msg();//打印出错信息
		return;
	}
}

void CColorChartView::CmdSaveColor()
{
	unsigned int nParam[320];
	unsigned int nRet;
	int tick = 0;

	for (int ncolor=0; ncolor<3; ncolor++)
	{
		tick = 0;

		nParam[tick++] = ncolor;
		for (int i=0; i<256; i++)
		{
			nParam[tick++] = m_ColorArray[m_nChannel][ncolor].DotColor(i);
		}

		nRet = theApp.NormalASCII_RW("WCRA", nParam, tick, m_nChannel);

		Sleep(20);

		if (nRet != 0x00)
		{
			AfxMessageBox(_T("写入色差数据失败！"));
		}
	}

	tick = 0;
	for (int ncolor=0; ncolor<3; ncolor++)
	{
		for (int i=0; i<9; i++)
		{
			nParam[tick++] = m_ColorArray[m_nChannel][ncolor].DotColor(i*32-1);
		}
	}

	nParam[tick++] = 0x02; //表示第二代功能扩充 

	nRet = theApp.NormalASCII_RW("SCLR", nParam, tick, m_nChannel);
	if (nRet != 0x00)
	{
		AfxMessageBox(_T("保存色差数据失败！"));
	}

}

void CColorChartView::CmdCancelBlending()
{
	unsigned int nRet;
	unsigned int nCancelBlend = (unsigned int)m_nBlendOption;
	DEV_MSG_STRUCT *pDevMsg = (DEV_MSG_STRUCT*)&theApp.m_Dev_Msg;
	CString strMsg;

	for (int i=0; i<MAX_OUTPUT; i++)
	{
		if (pDevMsg->m_Output_Port[i] != 0xff)
		{
			nRet = theApp.NormalASCII_RW("CRBD", &nCancelBlend, 1, pDevMsg->m_Output_Port[i]);
			if (nRet != 0x00)
			{
				strMsg.Format(_T("关闭投影机%d融合带失败！"), i+1);
				AfxMessageBox(strMsg);
			}
		}
	}
	
}

void CColorChartView::SetColor(int *data, int nColorSel, int nChannel)
{
	//合法保护
	ASSERT(data);
	if (nColorSel > 2 || nColorSel < 0) return;
	if (nChannel < 0 || nChannel > 6) return;

	for (int i=0; i<256; i++)
	{
		m_ColorArray[nChannel][nColorSel].DotColor(i) = data[i];
	}
}

void CColorChartView::OnVKeyUpDown(UINT nVKCode)
{
	//计算色彩值
	int nDotColor;
	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if (m_emColorAjustType == eRoughType)
	{
		nDotColor = m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nCtrlPointIndex*32-1);
	}
	else
	{
		nDotColor = m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nColorSection*32 + m_nCtrlPointIndex);
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

void CColorChartView::ShowMsgTip(BOOL bShow, CMsgTipArray &arMsgTip)
{
	if(m_pColorMsgTipDlg == NULL)  
	{  
		m_pColorMsgTipDlg = new CMsgTipDlg();  
		m_pColorMsgTipDlg->Create(IDD_MSGTIPDLG, this);  
	} 
	if(!bShow)  
	{  
		if(m_pColorMsgTipDlg->IsWindowVisible()) m_pColorMsgTipDlg->ShowWindow(SW_HIDE);  
	}  
	else  
	{  
		CSize size = m_pColorMsgTipDlg->ShowMsgTip(arMsgTip);  
		if(size.cx == 0)  
			return;  
		CPoint pt;  
		GetCursorPos(&pt);  
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
		m_pColorMsgTipDlg->SetWindowPos(&wndTop,rc.left,rc.top,rc.Width(),rc.Height(),SWP_SHOWWINDOW|SWP_NOACTIVATE);  
		m_pColorMsgTipDlg->DrawMsgTip();  
	}  
}

void CColorChartView::GetMsgTipInfo(CMsgTipArray &arMsgTip, CPoint ptCursor, int nCtrlPtIndex)
{
	CString strInfo;
	MSG_TIP mt;
	COLORREF colorMsg;
	colorMsg = m_colorSel == RED ? RGB(255,0,0) : m_colorSel == GREEN ? RGB(0,255,0) : m_colorSel == BLUE ? RGB(0,0,255) : RGB(100, 50, 80);

	//提示框的标题
	mt.nInfoShowColor = -1;
	mt.sMsgItemName = _T("色差校正");
	mt.sMsgItemInfo = _T("色差校正");	
	arMsgTip.Add(mt);


	//提示条目一 颜色
	strInfo = m_colorSel == RED ? _T("红色") : m_colorSel == GREEN ? _T("绿色") : m_colorSel == BLUE ? _T("蓝色") : _T("纯白");
	mt.nInfoShowColor = colorMsg;
	mt.sMsgItemName = _T("颜色");
	mt.sMsgItemInfo = strInfo;
	arMsgTip.Add(mt);


	//提示条目二 校准值
	int nDotColor, nStdDotColor;
	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if(m_emColorAjustType == eRoughType)	{	nDotColor = m_ColorArray[m_nChannel][m_colorSel].DotColor(nCtrlPtIndex*32-1); }
	else	{	nDotColor = m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nColorSection*32+nCtrlPtIndex);	}
	strInfo.Format(_T("%d"), nDotColor);
	mt.nInfoShowColor = colorMsg;
	mt.sMsgItemName   = _T("校准值");
	mt.sMsgItemInfo   = strInfo;
	arMsgTip.Add(mt);


	//提示条目三 标准值
	if(m_emColorAjustType == eRoughType)	{	nStdDotColor = (nCtrlPtIndex*32-1) > 0 ? (nCtrlPtIndex*32-1) : 0 ; }
	else	{	nStdDotColor = m_nColorSection*32+nCtrlPtIndex;	}
	strInfo.Format(_T("%d"), nStdDotColor);
	mt.nInfoShowColor = colorMsg;
	mt.sMsgItemName   = _T("标准值");
	mt.sMsgItemInfo   = strInfo;
	arMsgTip.Add(mt);
}

int CColorChartView::IsMouseOverColorCtrlPoint(CPoint point)
{
	int nDotColor = 0;
	CRect rcCtrlPoint;
	int nCtrlIndex = 0;
	//int nColorSel = m_colorSel == WHITE ? RED : m_colorSel;
	if (m_emColorAjustType == eRoughType)
	{
		//粗调模式 9个控制点
		for(int i=0; i<BIG_CTRL_NODE; i++)
		{
			nDotColor = m_ColorArray[m_nChannel][m_colorSel].DotColor(i*32-1);

			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*32*1.6) - 3 + 10,
				m_rcChart.bottom - Round(nDotColor*1.6) - 3,
				m_rcChart.left   + Round(i*32*1.6) + 3 + 10,
				m_rcChart.bottom - Round(nDotColor*1.6) + 3);

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
			nDotColor   = m_ColorArray[m_nChannel][m_colorSel].DotColor(m_nColorSection*32 +i);
			rcCtrlPoint = CRect(m_rcChart.left   + Round(i*1.6*8) + 10 -3,
				m_rcChart.bottom - Round(nDotColor*1.6)-3,
				m_rcChart.left   + Round(i*1.6*8) + 10 +3,
				m_rcChart.bottom - Round(nDotColor*1.6)+3);

			if (PtInRect(rcCtrlPoint, point))
			{
				nCtrlIndex = i;

				return nCtrlIndex;
			}
		}	
	}


	return -1;
}