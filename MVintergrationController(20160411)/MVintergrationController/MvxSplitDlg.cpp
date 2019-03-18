// MvxSplitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MvxSplitDlg.h"
#include "MvxWndPropertyDlg.h"
#include "MvxBackgrdSettingDlg.h"
#include "MvxVideoPropertyDlg.h"

COLORREF WINDOWBACKGROUNDCOLORDEF[]=
{
	RGB(0x63,0x5d,0xbb),
	RGB(0xaf,0x5d,0x22),
	RGB(0x88,0x22,0x55),
	RGB(0xcc,0x99,0x33),
	RGB(0xcc,0x33,0x33),
	RGB(0x33,0xcc,0x33),
	RGB(0x33,0xcc,0xcc),
	RGB(0x33,0x33,0xcc),
	RGB(0x66,0x33,0xcc),
	RGB(0xcc,0x33,0x99),
};

int CMvxSplitDlg::stcMvxObjCount = 0;
// CMvxSplitDlg 对话框

IMPLEMENT_DYNAMIC(CMvxSplitDlg, CXTPResizeDialog)

CMvxSplitDlg::CMvxSplitDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMvxSplitDlg::IDD, pParent)
{
	//对象计数
	m_nObjOder = stcMvxObjCount++;

	//功能按钮初始化
	for (int i=0; i<MAX_BTN; i++)	{	m_xBtn[i].nID = i;	}

	m_xBtn[0].strSurfaceName = _T("背景属性");
	m_xBtn[1].strSurfaceName = _T("视频属性");
	m_xBtn[2].strSurfaceName = _T("窗口属性");
	m_xBtn[3].strSurfaceName = _T("场景管理");
	m_xBtn[4].strSurfaceName = _T("快捷菜单");

	//默认背景分辨率
	m_nPhyHorz       = 1024;
	m_nPhyVert       = 768;
	memset(&m_BkgTiming, 0, sizeof(TIMING_STRUCT));

	//默认网格
	m_nHorzLines     = 3;
	m_nVertLines     = 2;

	//控制量初始化
	m_bLBtnDown      = FALSE;
	m_bLBtnDownInWnd = FALSE;
	m_nFocusID       = ErrWinID;
	m_nNewWndID      = ErrWinID;
	m_bNewCreated    = FALSE;

	m_eResizeDir     = eResize_Invalid;
	m_eMoveDir       = eNoMove;
}

CMvxSplitDlg::~CMvxSplitDlg()
{
	stcMvxObjCount--;
}

void CMvxSplitDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMvxSplitDlg, CXTPResizeDialog)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_SAVE_SCENE_FILE, OnSaveSceneFile)
	ON_COMMAND(ID_READ_SCENE_FILE, OnReadSceneFile)
	ON_COMMAND_RANGE(ID_WRITE_DEV_SCENE1, ID_WRITE_DEV_SCENE8, OnWriteDevScene)
	ON_COMMAND_RANGE(ID_LOAD_DEV_SCENE1, ID_LOAD_DEV_SCENE8, OnLoadDevScene)
	ON_COMMAND_RANGE(ID_MVX_OPEN_WINDOW, ID_MVX_SAME_SIZE, OnWindowOption)
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CMvxSplitDlg 消息处理程序

BOOL CMvxSplitDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	SetResize(IDC_PIC_MVX, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);

	CalcBtnRect();

	CalcAnchorPoint();

	m_bkBrush.CreateSolidBrush(RGB(50,180,240));

	return TRUE;
}

void CMvxSplitDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	PaintBottomPicture();

	PaintTopPicture();
}

//*************************************************************************************
//                 绘制底下的图片 --- 功能按钮区
//*************************************************************************************
void CMvxSplitDlg::PaintBottomPicture()
{
	//获取绘图DC
	CDC *pDC = GetDlgItem(IDC_PIC_MVX)->GetDC();

	//创建内存DC
	CDC *pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);

	//用空位图作为背景
	CBitmap MemBmp, *pOldBmp;
	MemBmp.CreateCompatibleBitmap(pDC, m_rcBtnPic.Width(), m_rcBtnPic.Height());
	pOldBmp = pMemDC->SelectObject(&MemBmp);

	//为背景着色
	pMemDC->FillSolidRect(0 , 0, m_rcBtnPic.Width(), m_rcBtnPic.Height(), RGB(193,243,133));

	//画功能按钮
	DrawCustomBtn(pMemDC);

	//将内存DC拷贝到绘图DC上
	pDC->BitBlt(0, 0, m_rcBtnPic.Width(), m_rcBtnPic.Height(), pMemDC, 0, 0, SRCCOPY);

	//资源释放
	pMemDC->SelectObject(pOldBmp);

	pMemDC->DeleteDC();
	delete pMemDC;
	pMemDC = NULL;

	ReleaseDC(pDC);
}

//*************************************************************************************
//                 绘制顶上的图片 --- 主开窗区
//*************************************************************************************
void CMvxSplitDlg::PaintTopPicture()
{
	//获取绘图DC
	CDC *pDC = GetDlgItem(IDC_PIC_MVX_CHART)->GetDC();

	//创建内存DC
	CDC *pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);

	//选取空位图
	CBitmap MemBmp, *pOldBmp;
	MemBmp.CreateCompatibleBitmap(pDC, m_rcChart.Width(), m_rcChart.Height());
	pOldBmp = pMemDC->SelectObject(&MemBmp);

	//为背景着色
	pMemDC->FillSolidRect(0, 0, m_rcChart.Width(), m_rcChart.Height(), RGB(50,30,60));//RGB(0xe6, 0xe8, 0xfa)

	//网格线
	DrawGridLine(pMemDC);

	//分辨率
	DrawResolution(pMemDC);

	//窗体
	ReflashWindow(pMemDC);

	//拷贝内存DC到绘图DC
	pDC->BitBlt(0, 0, m_rcChart.Width(), m_rcChart.Height(), pMemDC, 0, 0, SRCCOPY);

	//释放资源
	pMemDC->SelectObject(pOldBmp);

	pMemDC->DeleteDC();
	delete pMemDC;
	pMemDC = NULL;

	ReleaseDC(pDC);
}

void CMvxSplitDlg::DrawCustomBtn(CDC *pValidDC)
{
	CFont markFont, boldFont, *pOldFont;
	VERIFY(markFont.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_REGULAR,                // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Ms Sans Serif")));     // lpszFacename

	VERIFY(boldFont.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Ms Sans Serif")));     // lpszFacename

	pOldFont = pValidDC->SelectObject(&markFont);

	CPen bluepen, *pOldPen;
	bluepen.CreatePen(PS_SOLID, 2, RGB(50, 220, 240));
	pOldPen = pValidDC->SelectObject(&bluepen);

	CSize textExt;

	pValidDC->SetBkMode(TRANSPARENT);
	for (int i=0 ; i<MAX_BTN; i++)
	{
		pValidDC->FillSolidRect(m_xBtn[i].rcWnd, m_xBtn[i].bClicked ? RGB(50,150,50) : m_xBtn[i].bFocused ? RGB(120, 200, 150) : RGB(180, 250, 200));
		pValidDC->FrameRect(m_xBtn[i].rcWnd, NULL);

		if (m_xBtn[i].bFocused) pValidDC->SelectObject(&boldFont);
		else pValidDC->SelectObject(&markFont);

		textExt = pValidDC->GetTextExtent(m_xBtn[i].strSurfaceName);
		pValidDC->SetTextColor(RGB(20,50,40));

		pValidDC->TextOut(m_xBtn[i].rcWnd.left + Round((m_xBtn[i].rcWnd.Width()-textExt.cx)*0.5),
			              m_xBtn[i].rcWnd.top + Round((m_xBtn[i].rcWnd.Height()-textExt.cy)*0.5),
						  m_xBtn[i].strSurfaceName);
	}

	pValidDC->SelectObject(pOldPen);
	pValidDC->SelectObject(pOldFont);
}

void CMvxSplitDlg::DrawGridLine(CDC *pValidDC)
{
	CPen bluepen, dotpen, *pOldPen;
	bluepen.CreatePen(PS_SOLID, 1, RGB(255, 255, 100));
	dotpen.CreatePen(PS_DOT, 1, RGB(63, 245, 180));

	pOldPen = pValidDC->SelectObject(&bluepen);
	pValidDC->MoveTo(m_rcChart.left, m_rcChart.top);
	pValidDC->LineTo(m_rcChart.right-1,m_rcChart.top);
	pValidDC->LineTo(m_rcChart.right-1,m_rcChart.bottom-1);
	pValidDC->LineTo(m_rcChart.left, m_rcChart.bottom-1);
	pValidDC->LineTo(m_rcChart.left, m_rcChart.top);

	pValidDC->SelectObject(&dotpen);
	for (int i=1; i<m_nHorzLines; i++)
	{
		pValidDC->MoveTo(i*m_rcChart.Width()/m_nHorzLines, m_rcChart.top);
		pValidDC->LineTo(i*m_rcChart.Width()/m_nHorzLines, m_rcChart.bottom);
	}

	for (int i=1; i<m_nVertLines; i++)
	{
		pValidDC->MoveTo(m_rcChart.left,  i*m_rcChart.Height()/m_nVertLines);
		pValidDC->LineTo(m_rcChart.right, i*m_rcChart.Height()/m_nVertLines);
	}

	pValidDC->SelectObject(pOldPen);
}


void CMvxSplitDlg::DrawResolution(CDC *pValidDC)
{
	CFont markFont, *pOldFont;
	VERIFY(markFont.CreateFont(
		40,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		TRUE,                      // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Ms Sans Serif")));     // lpszFacename

	pOldFont = pValidDC->SelectObject(&markFont);

	CString strRes;
	strRes.Format(_T("%dx%d @ %dHz"), m_nPhyHorz, m_nPhyVert, m_BkgTiming.m_Hz);

	CSize txtExt;
	txtExt = pValidDC->GetTextExtent(strRes);

	CPoint ptMark;
	ptMark.x = m_rcChart.CenterPoint().x - int(txtExt.cx*0.5);
	ptMark.y = m_rcChart.CenterPoint().y - int(txtExt.cy*0.5);
	ptMark.Offset(-m_rcChart.left, -m_rcChart.top);

	pValidDC->SetBkMode(TRANSPARENT);
	pValidDC->SetTextColor(RGB(255,255,200));
	pValidDC->TextOut(ptMark.x, ptMark.y, strRes);

	ptMark.Offset(1,1);
	pValidDC->SetTextColor(RGB(178,145,240));
	pValidDC->TextOut(ptMark.x, ptMark.y, strRes);

	pValidDC->SelectObject(pOldFont);
}

void CMvxSplitDlg::ReflashWindow(CDC *pDC)
{
	int nCount = m_ArrWinInfo.GetCount();

	for (int i=0; i<MAX_WIN; i++)
	{
		for (int j=0; j<nCount; j++)
		{
			if (m_ArrWinInfo[j].iZorder == i)
			{
				DrawWindow(pDC, j);
			}
		}
	}
}

void CMvxSplitDlg::DrawWindow(CDC *pValidDC, int nIndex)
{
	CRect rcWnd = m_ArrWinInfo[nIndex].rcLog;
	int nWndID  = m_ArrWinInfo[nIndex].WinId;

	//创建一个和窗口大小一样的背景位图
	CBitmap MemBmp;
	MemBmp.CreateCompatibleBitmap(pValidDC, rcWnd.Width(), rcWnd.Height());

	//创建一个绘图DC
	CDC *pDC = new CDC;
	pDC->CreateCompatibleDC(NULL);
	pDC->SelectObject(MemBmp);
	pDC->SetBkMode(TRANSPARENT);

	//标题画刷
	CBrush BrushBlue(RGB(101,211,246));
	CBrush BrushGray(RGB(90, 170, 90));
	CBrush* pOldBrush;
	if(m_ArrWinInfo[nIndex].WinId == m_nFocusID)
		pOldBrush = pDC->SelectObject(&BrushBlue);
	else
		pOldBrush = pDC->SelectObject(&BrushGray);

	CRect rcTitle, rcButton;

	//【标题栏】
	rcTitle.left   = 0;
	rcTitle.top    = 0;
	rcTitle.right  = rcWnd.Width();
	rcTitle.bottom = 21;
	m_ArrWinInfo[nIndex].rcTitle = rcTitle;
	m_ArrWinInfo[nIndex].rcTitle.OffsetRect(rcWnd.left, rcWnd.top);

	pDC->PatBlt(0, 0, rcWnd.Width(), 21, PATCOPY);
	pDC->SelectObject(pOldBrush);

	//【窗口框架】
	CPen YellowPen(PS_SOLID, 2, RGB(250, 240, 78));
	CPen *pOldPen = pDC->SelectObject(&YellowPen);

	pDC->MoveTo(0,0);
	pDC->LineTo(0,rcWnd.Height()-1);
	pDC->LineTo(rcWnd.Width()-1,rcWnd.Height()-1);
	pDC->LineTo(rcWnd.Width()-1,0);
	pDC->LineTo(0,0);

	pDC->SelectObject(pOldPen);

	//准备按钮位图DC
	CBitmap *pBmp = new CBitmap;
	CBitmap *pOldBmp;
	CDC *pImageDC = new CDC;
	pImageDC->CreateCompatibleDC(pDC);

	//【关闭按钮】
	rcButton.left  = rcTitle.right-22;
	rcButton.top   = rcTitle.top+2;
	rcButton.right = rcTitle.right - 3;
	rcButton.bottom= rcTitle.top+21;
	m_ArrWinInfo[nIndex].rcBtnExit = rcButton;
	m_ArrWinInfo[nIndex].rcBtnExit.OffsetRect(rcWnd.left, rcWnd.top);

	pBmp->LoadBitmap(IDB_BITMAP_EXIT_NORM);
	pOldBmp = pImageDC->SelectObject(pBmp);
	pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), pImageDC, 0, 0, SRCCOPY);

	pImageDC->SelectObject(pOldBmp);
	pBmp->DeleteObject();

	//【恢复按钮】
	rcButton.OffsetRect(-19, 0);
	m_ArrWinInfo[nIndex].rcBtnRestore = rcButton;
	m_ArrWinInfo[nIndex].rcBtnRestore.OffsetRect(rcWnd.left, rcWnd.top);

	pBmp->LoadBitmap(IDB_BITMAP_RESTORE_NORM);
	pOldBmp = pImageDC->SelectObject(pBmp);
	pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), pImageDC, 0, 0, SRCCOPY);

	pImageDC->SelectObject(pOldBmp);
	pBmp->DeleteObject();

	//【最大化按钮】
	rcButton.OffsetRect(-19, 0);
	m_ArrWinInfo[nIndex].rcBtnMax = rcButton;
	m_ArrWinInfo[nIndex].rcBtnMax.OffsetRect(rcWnd.left, rcWnd.top);

	pBmp->LoadBitmap(IDB_BITMAP_MAX_NORM);
	pOldBmp = pImageDC->SelectObject(pBmp);
	pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), pImageDC, 0, 0, SRCCOPY);

	pImageDC->SelectObject(pOldBmp);
	pBmp->DeleteObject();

	//【对齐按钮】
	rcButton.OffsetRect(-19, 0);
	m_ArrWinInfo[nIndex].rcBtnResize = rcButton;
	m_ArrWinInfo[nIndex].rcBtnResize.OffsetRect(rcWnd.left, rcWnd.top);

	pBmp->LoadBitmap(IDB_BITMAP_RESIZE_NORM);
	pOldBmp = pImageDC->SelectObject(pBmp);
	pDC->BitBlt(rcButton.left, rcButton.top, rcButton.Width(), rcButton.Height(), pImageDC, 0, 0, SRCCOPY);

	pImageDC->SelectObject(pOldBmp);
	pBmp->DeleteObject();

	//【主窗体背景色】
	CBrush bkBrush(WINDOWBACKGROUNDCOLORDEF[nWndID]);
	pOldBrush = pDC->SelectObject(&bkBrush);

	pDC->PatBlt(1, rcTitle.bottom, rcWnd.Width()-2, rcWnd.Height()-rcTitle.Height()-1, PATCOPY);
	pDC->SelectObject(pOldBrush);

	//准备文本绘制
	int OutX,OutY;
	if(rcWnd.left<0)	OutX = 0-rcWnd.left;
	else	OutX = 2;

	if(rcWnd.top<0)		OutY = 0-rcWnd.top;
	else	OutY = 2;

	CFont font; 
	font.CreatePointFont(80,_T("Arial")); 
	pDC->SelectObject(&font);

	CString str;
	//【标题文字】
	str.Format(_T("%s"), m_ArrWinInfo[nIndex].szTitle);
	pDC->TextOut(OutX+25, OutY, str);
	//【窗口标示】
	str.Format(_T("窗口标示: %d"), m_ArrWinInfo[nIndex].WinId);
	pDC->TextOut(OutX, OutY+35, str);
	//【叠放序号】
	str.Format(_T("叠放序号: %d"), m_ArrWinInfo[nIndex].iZorder);
	pDC->TextOut(OutX, OutY+55, str);
	//【信号源】
	str.Format(_T("信号源: %d"), m_ArrWinInfo[nIndex].iSource);
	pDC->TextOut(OutX, OutY+75, str);
	//【窗口位置】
	str.Format(_T("窗口位置: (%d, %d)"), m_ArrWinInfo[nIndex].rcPhy.left, m_ArrWinInfo[nIndex].rcPhy.top);
	pDC->TextOut(OutX, OutY+95, str);
	//【窗口大小】
	str.Format(_T("窗口大小: (%d x %d)"), m_ArrWinInfo[nIndex].rcPhy.Width(), m_ArrWinInfo[nIndex].rcPhy.Height());
	pDC->TextOut(OutX, OutY+115, str);
	//【LOGO】
	font.DeleteObject();
	font.CreatePointFont(240,_T("Ms Sans Serif"));
	pDC->SelectObject(&font);
	str.Format(_T("%d"), m_ArrWinInfo[nIndex].WinId+1);
	pDC->SetTextColor(RGB(128,128,128));
	pDC->TextOut(OutX+1, OutY, str);
	pDC->SetTextColor(RGB(254,210,234));
	pDC->TextOut(OutX+3, OutY+2, str);
		

	pValidDC->BitBlt(rcWnd.left, rcWnd.top, rcWnd.Width(), rcWnd.Height(), pDC, 0, 0, SRCCOPY);

	delete pBmp;
	
	ReleaseDC(pImageDC);
	delete pImageDC;

	ReleaseDC(pDC);
	delete pDC;
}

void CMvxSplitDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTPResizeDialog::OnSize(nType, cx, cy);

	if(GetDlgItem(IDC_PIC_MVX)->GetSafeHwnd())
	{
		CalcBtnRect();

		//计算开窗区位置
		ZoomPaintRect();

		//重新计算各个窗口的逻辑区域大小
		ZoomWindows();
	}
}

// 计算自绘按钮的坐标位置
void CMvxSplitDlg::CalcBtnRect()
{
	GetDlgItem(IDC_PIC_MVX)->GetWindowRect(&m_rcBtnPic);
	ScreenToClient(&m_rcBtnPic);
	
	CRect tmpRc = m_rcBtnPic;
	tmpRc.OffsetRect(-m_rcBtnPic.left, -m_rcBtnPic.top);
	
	int nStart = Round((tmpRc.Width()-MAX_BTN*BTN_WID)*0.5);
	//计算底部几个按钮的位置
	for (int i=0; i<MAX_BTN; i++)
	{
		m_xBtn[i].rcWnd.left   = nStart + i*BTN_WID;
		m_xBtn[i].rcWnd.top    = tmpRc.top;
		m_xBtn[i].rcWnd.right  = nStart + i*BTN_WID + BTN_WID;
		m_xBtn[i].rcWnd.bottom = tmpRc.bottom;
	}
}

//根据对话框的尺寸调整绘图区(开窗区)到合适的大小
void CMvxSplitDlg::ZoomPaintRect()
{
	CRect rcClient;
	GetWindowRect(&rcClient);
	ScreenToClient(&rcClient);

	int nXScale, nYScale, nScale;
	nXScale = rcClient.Width()*20 / m_nPhyHorz;
	nYScale = (rcClient.Height()-m_rcBtnPic.Height()-10)*20 / m_nPhyVert;

	nScale = min(nXScale, nYScale);

	m_rcChart = CRect(Round((rcClient.Width() - m_nPhyHorz*nScale/20)*0.5),
                      Round((rcClient.Height()+m_rcBtnPic.Height()+10-m_nPhyVert*nScale/20)*0.5),
                      Round((rcClient.Width() + m_nPhyHorz*nScale/20)*0.5),
                      Round((rcClient.Height()+m_rcBtnPic.Height()+10+m_nPhyVert*nScale/20)*0.5));

	GetDlgItem(IDC_PIC_MVX_CHART)->MoveWindow(m_rcChart);
	//ScreenToClient(&m_rcChart);
	m_rcPaint = m_rcChart;
	m_rcChart.OffsetRect(-m_rcChart.left, -m_rcChart.top);

	m_nLogHorz = m_rcChart.Width();
	m_nLogVert = m_rcChart.Height();
}

//由于当前画布的大小改变，需要重新计算窗口在画布上的大小
void CMvxSplitDlg::ZoomWindows()
{
	int nCount = m_ArrWinInfo.GetCount();
	
	for (int i=0; i<nCount; i++)
	{
		m_ArrWinInfo[i].rcLog = m_ArrWinInfo[i].rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);
	}
}


BOOL CMvxSplitDlg::OnEraseBkgnd(CDC* pDC)
{
	return CXTPResizeDialog::OnEraseBkgnd(pDC);
}

HBRUSH CMvxSplitDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return (HBRUSH)m_bkBrush;
	}

	return hbr;
}

void CMvxSplitDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	m_bLBtnDown = TRUE;

	//鼠标是否在自绘按钮上
	int nID = IsMouseOverCtrls(point);
	if (nID != -1)	{
		InvalidateRect(m_rcBtnPic);
		return CXTPResizeDialog::OnLButtonDown(nFlags, point);
	}

	point.Offset(-m_rcPaint.left, -m_rcPaint.top);
	m_ptLBtnDown = point;

	//鼠标不在绘图区
	if (!PtInRect(m_rcChart, point))
		return CXTPResizeDialog::OnLButtonDown(nFlags, point);

	//鼠标是否点击在窗口上
	m_bLBtnDownInWnd = IsMouseBtnDownInWnd(point);

	//若鼠标未点击在任何一个窗口上，则判断是否有新开窗口的可能
	if (m_bLBtnDownInWnd)
		HitTest(point, eLBtnDown);
	else
		m_nNewWndID = PrepareNewWinID();

	CXTPResizeDialog::OnLButtonDown(nFlags, point);
}

void CMvxSplitDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	//鼠标是否在自绘按钮上
	int nID = IsMouseOverCtrls(point);
	if (nID != -1)
	{
		m_xBtn[nID].bClicked = FALSE;
		InvalidateRect(m_rcBtnPic);

		//自定按钮消息响应处理
		OnCustomBtnClicked(nID);

		//重置控制变量
		ResetVariables();

		return CXTPResizeDialog::OnLButtonUp(nFlags, point);
	}

	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	if (m_bLBtnDownInWnd)
	{// 处理窗口上的鼠标弹起消息
		DefLBtnUpProc(point);

		int nIndex = GetWndIndex(m_nFocusID);
		if (nIndex != ErrIndex)
		{
		//Send Cmd
		if ((m_eResizeDir | eNoResize) == eNoResize)
			theApp.MVX_MoveTheWindow(m_nFocusID, 
                                 m_ArrWinInfo[nIndex].rcPhy, 
                                 m_ArrWinInfo[nIndex].iSource, 
                                 m_ArrWinInfo[nIndex].iSource, 
                                 m_boardInfo.GetBoardPort());
		else
			theApp.MVX_ResizeWindow(m_nFocusID, 
                                 m_ArrWinInfo[nIndex].rcPhy, 
                                 m_ArrWinInfo[nIndex].iSource, 
                                 m_ArrWinInfo[nIndex].iSource, 
                                 m_boardInfo.GetBoardPort());
		}
	}
	else 
	{// 新开窗口处理
		if ( (m_nNewWndID != ErrWinID )
		  && (abs(point.x - m_ptLBtnDown.x) > 50 || abs(point.y - m_ptLBtnDown.y) > 50) )
		{
			CMvxWndPropertyDlg PropertyDlg(TRUE, this);
			if (PropertyDlg.DoModal() != IDOK)
				DeleteWindow(m_nFocusID);
		}
		else {// 防止其他鼠标弹起事件导致的关闭窗口
			if (m_bNewCreated) DeleteWindow(m_nFocusID); 
		}
	}

	//重置控制变量
	ResetVariables();

	CXTPResizeDialog::OnLButtonUp(nFlags, point);
}

// 重置中间控制量
void CMvxSplitDlg::ResetVariables()
{
	m_nNewWndID = ErrWinID;
	m_bLBtnDown = FALSE;
	m_bLBtnDownInWnd = FALSE;

	m_bNewCreated = FALSE;

	m_eResizeDir = eResize_Invalid;
}

void CMvxSplitDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	//鼠标是否在自绘按钮上
	int nID = IsMouseOverCtrls(point);
	if (nID != -1)
	{
		InvalidateRect(m_rcBtnPic);
		return CXTPResizeDialog::OnMouseMove(nFlags, point);
	}

	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	//窗口控制
	if (m_bLBtnDown)
	{		
		if (m_bLBtnDownInWnd)
		{	//移动窗口或者调节窗口大小
			OnMoveNResize(point);

			m_ptLBtnDown = point;
		}
		else
		{	//新开窗口
			if (m_nNewWndID != ErrWinID)
				DynamicCreate(point);
		}
	}
	else
		HitTest(point, eMouseMove);

	CXTPResizeDialog::OnMouseMove(nFlags, point);
}

// 动态创建新的窗口
void CMvxSplitDlg::DynamicCreate(CPoint ptCursor)
{
	CRect rcLogical;
	rcLogical = CRect(min(ptCursor.x, m_ptLBtnDown.x),
		min(ptCursor.y, m_ptLBtnDown.y),
		max(ptCursor.x, m_ptLBtnDown.x),
		max(ptCursor.y, m_ptLBtnDown.y));

	CRect rcPhysical;
	rcPhysical = rcLogical.MulDiv(m_nPhyHorz, m_nLogHorz);

	CString strName;
	strName.Format(_T("SourceChannel[%d]"), m_boardInfo.GetBoardPort()+1);

	SetWndInfo(m_nNewWndID, m_boardInfo.GetBoardPort()+1, rcLogical, rcPhysical, strName);

	m_bNewCreated = TRUE;
}

// 调整窗口尺寸或者位置
void CMvxSplitDlg::OnMoveNResize(CPoint ptCursor)
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	//窗口物理与逻辑尺寸比
	double HScale = double(m_nPhyHorz)/double(m_nLogHorz);
	double VScale = double(m_nPhyVert)/double(m_nLogVert);

	CRect rcLog = m_ArrWinInfo[nIndex].rcLog;
	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;

	int nMinSize = 20;

	// 无任何操作
	if (m_eResizeDir == eResize_Invalid) return;

	if ((m_eResizeDir & eResize_L) == eResize_L)
	{ //调整左边框
		if ((ptCursor.x + nMinSize) < rcLog.right)
			rcPhy.left = Round(ptCursor.x*HScale);
		else
			rcPhy.left = rcPhy.right-Round(nMinSize*HScale);
	}
	if ((m_eResizeDir & eResize_T) == eResize_T)
	{ //调整上边框
		if ((ptCursor.y + nMinSize) < rcLog.bottom)
			rcPhy.top = Round(ptCursor.y*VScale);
		else
			rcPhy.top = rcPhy.bottom-Round(nMinSize*VScale);
	}
	if ((m_eResizeDir & eResize_R) == eResize_R)
	{ //调整右边框
		if ((ptCursor.x - nMinSize) > rcLog.left)
			rcPhy.right = Round(ptCursor.x*HScale);
		else
			rcPhy.right = rcPhy.left + Round(nMinSize*HScale);
	}
	if ((m_eResizeDir & eResize_B) == eResize_B)
	{ //调整下边框
		if ((ptCursor.y - nMinSize) > rcLog.top)
			rcPhy.bottom = Round(ptCursor.y*VScale);
		else
			rcPhy.bottom = rcPhy.top + Round(nMinSize*VScale);
	}
	if ((m_eResizeDir | eNoResize) == eNoResize)
	{ //移动窗口
		rcPhy.OffsetRect(Round((ptCursor.x-m_ptLBtnDown.x)*HScale), Round((ptCursor.y-m_ptLBtnDown.y)*VScale));
	}

	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);
}

// 鼠标左键弹起消息处理 主要处理窗口上的功能键
void CMvxSplitDlg::DefLBtnUpProc(CPoint ptCursor)
{
	int nCount = m_ArrWinInfo.GetCount();
	if (nCount == 0) return;

	CRect rcLog;
	for (int iZoder=MAX_WIN-1; iZoder>=0; iZoder--)
	{
		for (int i=0; i<nCount; i++)
		{
			rcLog = m_ArrWinInfo[i].rcLog;
			if ((m_ArrWinInfo[i].iZorder == iZoder) && (PtInRect(rcLog, ptCursor)))
			{
				if (PtInRect(m_ArrWinInfo[i].rcBtnExit, ptCursor))
				{ //点击退出按钮
					MyCloseWindow();
				}
				else if (PtInRect(m_ArrWinInfo[i].rcBtnMax, ptCursor))
				{ //点击最大化按钮
					MyMaximizeWindow();
				}
				else if (PtInRect(m_ArrWinInfo[i].rcBtnResize, ptCursor))
				{ //点击对齐按钮
					MyAligningWindow();
				}
				else if (PtInRect(m_ArrWinInfo[i].rcBtnRestore, ptCursor))
				{ //点击还原按钮
					MyRestoreWindow();
				}

				return; //终结循环
			}
		}
	}
}

// 鼠标是否停留在自定义按钮上
int CMvxSplitDlg::IsMouseOverCtrls(CPoint ptCursor)
{
	ResetBtnState();

	if (PtInRect(m_rcBtnPic, ptCursor))
	{
		CRect tmpRc;

		for (int i=0; i<MAX_BTN; i++)
		{
			tmpRc = m_xBtn[i].rcWnd;
			tmpRc.OffsetRect(m_rcBtnPic.left, m_rcBtnPic.top);

			if (PtInRect(tmpRc, ptCursor))
			{
				m_xBtn[i].bFocused = TRUE;
				m_xBtn[i].bClicked = m_bLBtnDown;

				return i;
			}
		}
	}

	// 鼠标没有经过按钮区 恢复按钮到正常状态
	InvalidateRect(m_rcBtnPic);

	return -1;
}

void CMvxSplitDlg::ResetBtnState()
{
	for (int i=0; i<MAX_BTN; i++)
	{
		m_xBtn[i].bClicked = m_xBtn[i].bFocused = FALSE;
	}
}

// 鼠标左键是否点击窗口
BOOL CMvxSplitDlg::IsMouseBtnDownInWnd(CPoint ptCursor)
{
	int nCount = m_ArrWinInfo.GetCount();
	if (nCount == 0) return FALSE;

	for (int iZoder=MAX_WIN-1; iZoder>=0; iZoder--) {
		for (int i=0; i<nCount; i++)   {
			if ((m_ArrWinInfo[i].iZorder == iZoder) && (PtInRect(m_ArrWinInfo[i].rcLog, ptCursor)))
			{
				m_nFocusID = m_ArrWinInfo[i].WinId;
				return TRUE;
			}
		}
	}

	return FALSE;
}

// 鼠标追踪
void CMvxSplitDlg::HitTest(CPoint ptCursor, emMouseAct nMouseAct)
{
	int nCount = m_ArrWinInfo.GetCount();
	if (nCount == 0) return;
	
	CRect rcLog;
	CRect rcL, rcTL, rcT, rcTR, rcR, rcBR, rcB, rcBL;
	for (int iZoder=MAX_WIN-1; iZoder>=0; iZoder--)	{
		for (int i=0; i<nCount; i++)
		{
			rcLog = m_ArrWinInfo[i].rcLog;
			if ((m_ArrWinInfo[i].iZorder == iZoder) && PtInRect(rcLog, ptCursor))
			{				
				rcL  = CRect(rcLog.left,    rcLog.top+6,   rcLog.left+6,  rcLog.bottom-6);
				rcTL = CRect(rcLog.left,    rcLog.top,     rcLog.left+6,  rcLog.top+6);
				rcT  = CRect(rcLog.left+6,  rcLog.top,     rcLog.right-6, rcLog.top+6);
				rcTR = CRect(rcLog.right-6, rcLog.top,     rcLog.right,   rcLog.top+6);
				rcR  = CRect(rcLog.right-6, rcLog.top+6,   rcLog.right,   rcLog.bottom-6);
				rcBR = CRect(rcLog.right-6, rcLog.bottom-6,rcLog.right,   rcLog.bottom);
				rcB  = CRect(rcLog.left+6,  rcLog.bottom-6,rcLog.right-6, rcLog.bottom);
				rcBL = CRect(rcLog.left,    rcLog.bottom-6,rcLog.left+6,  rcLog.bottom);

				if (PtInRect(rcL, ptCursor)) {
					m_eResizeDir = eResize_L;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE))); }
				else if (PtInRect(rcTL, ptCursor)) {
					m_eResizeDir = eResize_TL;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))); }
				else if (PtInRect(rcT, ptCursor)) {
					m_eResizeDir = eResize_T;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); }
				else if (PtInRect(rcTR, ptCursor)) {
					m_eResizeDir = eResize_TR;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); }
				else if (PtInRect(rcR, ptCursor)) {
					m_eResizeDir = eResize_R;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE))); }
				else if (PtInRect(rcBR, ptCursor)) {
					m_eResizeDir = eResize_BR;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))); }
				else if (PtInRect(rcB, ptCursor)) {
					m_eResizeDir = eResize_B;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS))); }
				else if (PtInRect(rcBL, ptCursor)) {
					m_eResizeDir = eResize_BL;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW))); }
				else {
					m_eResizeDir = eNoResize;
					SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW))); }

				//不是左键点击触发
				if (nMouseAct != eLBtnDown) m_eResizeDir = eResize_Invalid;

				return; //终结循环
			}
		}
	}
}

// 计算网格对齐点坐标
void CMvxSplitDlg::CalcAnchorPoint()
{
	m_ArrAnchorPt.RemoveAll();

	CPoint ptAnchor;

	for (int i=0; i<m_nHorzLines; i++)	{
		for (int j=0; j<m_nVertLines; j++)	{
			ptAnchor.x = i*m_nPhyHorz/m_nHorzLines;
			ptAnchor.y = j*m_nPhyVert/m_nVertLines;
			m_ArrAnchorPt.Add(ptAnchor);
		}
	}
}

//获取新开窗口的ID
int CMvxSplitDlg::PrepareNewWinID() const
{
	int nCount = m_ArrWinInfo.GetCount();
	if (nCount == MAX_WIN) return ErrWinID;

	bool bFound = false;

	//寻找未使用的ID
	for (int i=0; i<MAX_WIN; i++) 
	{
		bFound = false;

		for (int j=0; j<nCount; j++)
		{
			if (m_ArrWinInfo[j].WinId == i)
			{
				//已有窗口ID等于i的窗口
				bFound = true;
				break;
			}
		}

		if (!bFound) return i;
	}

	return ErrWinID;
}

//获取窗口在数组中的位置
int CMvxSplitDlg::GetWndIndex(const int nIDtoFind) const
{
	for (int i=0; i<m_ArrWinInfo.GetCount(); i++)
	{
		if (m_ArrWinInfo[i].WinId == nIDtoFind)
		{
			return i;
		}
	}

	return ErrIndex;
}

//添加新的窗口信息到数组
void CMvxSplitDlg::AddWindow(int nNewID, int nSourceID, CRect rcLog, CRect rcPhy, CString strName)
{
	WindowInfo winfo;

	winfo.WinId     = nNewID;
	winfo.iSource   = nSourceID;
	winfo.iZorder   = m_ArrWinInfo.GetCount();
	winfo.rcLog     = rcLog;
	winfo.rcLogLast = rcLog;
	winfo.rcPhy     = rcPhy;
	winfo.rcPhyLast = rcPhy;
	strncpy_s(winfo.szTitle, strName.GetBuffer(), strName.GetLength());
	strName.ReleaseBuffer();

	m_ArrWinInfo.Add(winfo);

	m_nFocusID = nNewID;
}

//删除窗口信息
void CMvxSplitDlg::DeleteWindow(const int nWinID)
{
	int nIndex = GetWndIndex(nWinID);
	if (nIndex == ErrIndex) return;

	m_ArrWinInfo.RemoveAt(nIndex);

	if (m_ArrWinInfo.GetCount() == 0) m_nFocusID = ErrWinID;
	else
	{
		for (int i=MAX_WIN-1; i>=0; i--)	{
			for (int j=0; j<m_ArrWinInfo.GetCount(); j++)	{
				if (m_ArrWinInfo[j].WinId == i)
				{
					m_nFocusID = i;
					return;
				}
			}
		}
	}
}

//设置窗口信息
void CMvxSplitDlg::SetWndInfo(int nWinID, int nSourceID, CRect rcLog, CRect rcPhy, CString strName)
{
	int nIndex = GetWndIndex(nWinID);
	if (nIndex == ErrIndex)
		AddWindow(nWinID, nSourceID, rcLog, rcPhy, strName);
	else
	{
		m_ArrWinInfo[nIndex].iSource   = nSourceID;
		m_ArrWinInfo[nIndex].rcLog     = rcLog;
		m_ArrWinInfo[nIndex].rcPhy     = rcPhy;
		m_ArrWinInfo[nIndex].rcPhyLast = rcPhy;
		strncpy_s(m_ArrWinInfo[nIndex].szTitle, strName.GetBuffer(), strName.GetLength());
		strName.ReleaseBuffer();
	}
}

//设置窗口的Z顺序
void CMvxSplitDlg::SetWndZorder(int nWinID, emZorderAct eZoderAct)
{
	int nCount = m_ArrWinInfo.GetCount();
	int nIndex = GetWndIndex(nWinID);
	if (nIndex == ErrIndex) return;
	switch(eZoderAct)
	{
	case eStepFront:
		{//向屏幕外移动
			if (m_ArrWinInfo[nIndex].iZorder == nCount-1) return;//窗口已在最顶层
			for (int j=0;j<nCount; j++)
			{
				if (m_ArrWinInfo[j].iZorder == m_ArrWinInfo[nIndex].iZorder+1)
				{
					SwapWndZorder(m_ArrWinInfo[nIndex], m_ArrWinInfo[j]);
					break;
				}
			}
		}
		break;
	case eStepBack:
		{//向屏幕内移动
			if (m_ArrWinInfo[nIndex].iZorder == 0) return;//窗口已在最底层
			for (int j=0;j<nCount; j++)
			{
				if (m_ArrWinInfo[j].iZorder == m_ArrWinInfo[nIndex].iZorder-1)
				{
					SwapWndZorder(m_ArrWinInfo[nIndex], m_ArrWinInfo[j]);
					break;
				}
			}
		}
		break;
	case eStepTop:
		{//置顶显示
			if (m_ArrWinInfo[nIndex].iZorder == nCount-1) return;//窗口已在最顶层
			for (int j=0;j<nCount; j++)
			{
				if (m_ArrWinInfo[j].iZorder == nCount-1)
				{
					SwapWndZorder(m_ArrWinInfo[nIndex], m_ArrWinInfo[j]);
					break;
				}
			}
		}
		break;
	case eStepBottom:
		{//置底显示
			if (m_ArrWinInfo[nIndex].iZorder == 0) return;//窗口已在最底层
			for (int j=0;j<nCount; j++)
			{
				if (m_ArrWinInfo[j].iZorder == 0)
				{
					SwapWndZorder(m_ArrWinInfo[nIndex], m_ArrWinInfo[j]);
					break;
				}
			}
		}
		break;
	default:break;
	}
}

//交换两个窗口的叠放顺序
inline void CMvxSplitDlg::SwapWndZorder(WindowInfo &WinInfoA, WindowInfo &WinInfoB)
{
	int iTmpZorder;
	iTmpZorder = WinInfoA.iZorder;
	WinInfoA.iZorder = WinInfoB.iZorder;
	WinInfoB.iZorder = iTmpZorder;
}

// 自定义按钮点击事件处理
void CMvxSplitDlg::OnCustomBtnClicked(UINT nID)
{
	if (nID == 0)
		OnBkgndProperty();
	else if (nID == 1)
		OnVedioProperty();
	else if (nID == 2)
		OnWndProperty();
	else if (nID == 3)
		OnSceneManage();
	else if (nID == 4)
		OnShotCut();

}

void CMvxSplitDlg::OnBkgndProperty()
{
	//弹出背景属性对话框，设置背景属性
	CMvxBackgrdSettingDlg BackgrdSetting(this);
	BackgrdSetting.m_nHLogScreen = m_nHorzLines;
	BackgrdSetting.m_nVLogScreen = m_nVertLines;
	BackgrdSetting.DoModal();
}

void CMvxSplitDlg::OnVedioProperty()
{
	//弹出视频属性对话框，主要设置输出
	CMvxVideoPropertyDlg VideoProperty(this);
	VideoProperty.DoModal();
}

void CMvxSplitDlg::OnWndProperty()
{
	//弹出窗口信息对话框
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CMvxWndPropertyDlg PropertyDlg(FALSE, this);

	PropertyDlg.DoModal();
}

void CMvxSplitDlg::OnSceneManage()
{
	//弹出场景管理菜单
	CPoint point;
	GetCursorPos(&point);

	theApp.MvTrackPopupMenu(point, IDR_MENU_MVX_SCENE_MANAGE, this);
}

void CMvxSplitDlg::OnShotCut()
{
	//弹出快捷菜单
	CPoint point;
	GetCursorPos(&point);

	theApp.MvTrackPopupMenu(point, IDR_MENU_MVX_WND_OPTION, this);
}

void CMvxSplitDlg::MyCloseWindow()
{
	//DeleteWindow 函数会改变m_nFocusID的值
	theApp.MVX_ShutdownWindow(m_nFocusID, m_boardInfo.GetBoardPort());

	//将该窗口的Zorder顺序置顶，利于叠放顺序管理
	SetWndZorder(m_nFocusID, eStepTop);

	//删除该窗口在数组中的值
	DeleteWindow(m_nFocusID);
}

void CMvxSplitDlg::MyMaximizeWindow()
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;

	rcPhy.left   = 0;
	rcPhy.top    = 0;
	rcPhy.right  = m_nPhyHorz;
	rcPhy.bottom = m_nPhyVert;

	//新的尺寸与上次设置的大小一样
	if (rcPhy == m_ArrWinInfo[nIndex].rcPhyLast) return;

	m_ArrWinInfo[nIndex].rcPhyLast = m_ArrWinInfo[nIndex].rcPhy;

	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);

	//Send Cmd
	theApp.MVX_ResizeWindow(m_nFocusID, 
                        rcPhy,
                        m_ArrWinInfo[nIndex].iSource,
                        m_ArrWinInfo[nIndex].iSource,
                        m_boardInfo.GetBoardPort());
}

void CMvxSplitDlg::MyAligningWindow()
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;

	int nLeft   = 0;
	int nTop    = 0;
	int nRight  = m_nPhyHorz;
	int nBottom = m_nPhyVert;

	for (int i=0; i<m_ArrAnchorPt.GetCount(); i++)
	{
		// 左对齐线
		if ((m_ArrAnchorPt[i].x <= rcPhy.left) && (m_ArrAnchorPt[i].x >= nLeft))
			nLeft = m_ArrAnchorPt[i].x;
		// 上对齐线
		if ((m_ArrAnchorPt[i].y <= rcPhy.top) && (m_ArrAnchorPt[i].y >= nTop))
			nTop = m_ArrAnchorPt[i].y;
		// 右对齐线
		if ((m_ArrAnchorPt[i].x >= rcPhy.right) && (m_ArrAnchorPt[i].x <= nRight))
			nRight = m_ArrAnchorPt[i].x;
		// 下对齐线
		if ((m_ArrAnchorPt[i].y >= rcPhy.bottom) && (m_ArrAnchorPt[i].y <= nBottom))
			nBottom = m_ArrAnchorPt[i].y;
	}

	rcPhy = CRect(nLeft, nTop, nRight, nBottom);

	//新的尺寸与上次设置的大小一样
	if (m_ArrWinInfo[nIndex].rcPhyLast == rcPhy) return;

	m_ArrWinInfo[nIndex].rcPhyLast = m_ArrWinInfo[nIndex].rcPhy;	
	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);

	//Send Cmd
	theApp.MVX_ResizeWindow(m_nFocusID, 
                        rcPhy,
                        m_ArrWinInfo[nIndex].iSource,
                        m_ArrWinInfo[nIndex].iSource,
                        m_boardInfo.GetBoardPort());
}

void CMvxSplitDlg::MyRestoreWindow()
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhyLast;

	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);

	//Send Cmd
	theApp.MVX_ResizeWindow(m_nFocusID, 
                        rcPhy,
                        m_ArrWinInfo[nIndex].iSource,
                        m_ArrWinInfo[nIndex].iSource,
                        m_boardInfo.GetBoardPort());
}

void CMvxSplitDlg::Initialize()
{
	UINT Params[128] = {0};
	int Port = m_boardInfo.GetBoardPort();
	//读取背景信号
	if (theApp.MVX_ReadBackgrdInfo(Params, Port) == CMD_FAILURE)
		return (void)MessageBox(_T("初始化失败！未能读取背景信号！"), _T("MVX303初始化"), MB_OK|MB_ICONSTOP);

	if (Params[0] != 0x01)
		return (void)MessageBox(_T("初始化失败！无输入信号，请确定输入端口是否接上信号！"), _T("MVX303初始化"), MB_OK|MB_ICONWARNING);

	//背景信号的分辨率信息
	m_BkgTiming.m_HTotal  = Params[1];
	m_BkgTiming.m_HActive = Params[2];
	m_BkgTiming.m_HFPorch = Params[3];
	m_BkgTiming.m_HSyncWid= Params[4];
	m_BkgTiming.m_HPol    = Params[5];
	m_BkgTiming.m_HBlank  = m_BkgTiming.m_HTotal - m_BkgTiming.m_HActive;

	m_BkgTiming.m_VTotal  = Params[6];
	m_BkgTiming.m_VActive = Params[7];
	m_BkgTiming.m_VFPorch = Params[8];
	m_BkgTiming.m_VSyncWid= Params[9];
	m_BkgTiming.m_VPol    = Params[10];
	m_BkgTiming.m_VBlank  = m_BkgTiming.m_VTotal - m_BkgTiming.m_VActive;

	m_BkgTiming.m_Hz      = Params[11];

	m_nPhyHorz            = m_BkgTiming.m_HActive;
	m_nPhyVert            = m_BkgTiming.m_VActive;

	//逻辑子屏幕的行和列
	m_nHorzLines          = Params[16];
	m_nVertLines          = Params[17];

	//重新计算开窗区大小
	CalcAnchorPoint();
	ZoomPaintRect();

	//读取场景信息
	InitScenes();
}

void CMvxSplitDlg::InitScenes()
{
	//读取当前场景
	LoadDevScene(0xff);

	//调整窗口的逻辑大小
	ZoomWindows();

	//重绘
	InvalidateRect(m_rcChart);
}

void CMvxSplitDlg::LoadDevScene(int nSceneID)
{
	//读取场景数据
	UINT Params[256] = {0};
	int Port = m_boardInfo.GetBoardPort();

	if (theApp.MVX_CallWindows(Params, nSceneID, Port) == CMD_FAILURE)
		return (void)MessageBox(_T("加载场景失败！未能读取场景数据！"), _T("场景加载"), MB_OK|MB_ICONSTOP);

	//清理窗口信息数组
	m_ArrWinInfo.RemoveAll();

	int nNextPos = 0;    //下一个窗口信息的起始位置

	//窗口信息
	WindowInfo WinInfo;
	memset(&WinInfo, 0, sizeof(WindowInfo));

	int nSrcID  = Port;  //信号源
	int nWinID;          //窗口ID  
	CRect rcPhy;         //窗口物理尺寸
	CRect rcLog;         //窗口在绘图区的尺寸
	CString strName;
	strName.Format(_T("SourceChannel[%d]"), nSrcID+1);

	//更新场景到界面
	while(Params[nNextPos] != 0) //窗口有效性标志位 0x01表示有效 0xff表示无效 0x00表示结束
	{
		if (Params[nNextPos] == 0xff) { nNextPos += 9; continue; }

		nWinID = Params[nNextPos+1];
		if (nWinID >= MAX_WIN)	{ nNextPos += 9; continue; }

		rcPhy.left   = Params[nNextPos+5];
		rcPhy.top    = Params[nNextPos+6];
		rcPhy.right  = Params[nNextPos+7];
		rcPhy.bottom = Params[nNextPos+8];

		rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);

		SetWndInfo(nWinID, nSrcID, rcLog, rcPhy, strName);

		nNextPos += 9;
	}
}

void CMvxSplitDlg::WriteDevScene(int nSceneID)
{
	UINT nParam[256];
	memset(nParam, 0, sizeof(nParam));

	int nTick = 0;
	nParam[nTick++] = nSceneID;  // num 场景号 默认场景一 对应的场景号为0

	int nCount = m_ArrWinInfo.GetCount();
	for (int i=0; i< nCount; i++)
	{
		nParam[nTick++] = 1;                           //win_valid 窗口有效性
		nParam[nTick++] = m_ArrWinInfo[i].WinId;       //win_ID
		nParam[nTick++] = m_ArrWinInfo[i].iSource;     //win_sourceID
		nParam[nTick++] = m_ArrWinInfo[i].iZorder;     //win_lvl
		nParam[nTick++] = 0;                           //win_angle

		nParam[nTick++] = m_ArrWinInfo[i].rcPhy.left;  //x0 图像的左上角
		nParam[nTick++] = m_ArrWinInfo[i].rcPhy.top;   //y0 图像的左上角

		nParam[nTick++] = m_ArrWinInfo[i].rcPhy.right;  //x1 图像的右下角
		nParam[nTick++] = m_ArrWinInfo[i].rcPhy.bottom; //y1 图像的右下角
	}

	int nRet = theApp.MVX_SaveWindows(nParam, nTick, nSceneID, m_boardInfo.GetBoardPort());
	if (nRet == -1)
		MessageBox(_T("对不起，保存场景失败！"), _T("保存场景"), MB_OK|MB_ICONERROR);		
	else
		MessageBox(_T("保存场景成功！"), _T("保存场景"), MB_OK|MB_ICONINFORMATION);
}

void CMvxSplitDlg::OnSaveSceneFile()
{
	//选择存储文件路径
	CFileDialog FileDlg(FALSE, NULL, "Screen1.srf", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Screen Files (*.srf)|*.srf|All Files (*.*)|*.*||");
	if (FileDlg.DoModal() == IDOK)
	{
		//获取路径
		CString strFilPath = FileDlg.GetPathName();

		//创建场景文件并写入数据
		CStdioFile File;
		if (!File.Open(strFilPath, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			return (void)MessageBox(_T("创建文件失败！"),_T("场景保存"), MB_OK|MB_ICONSTOP);

		int nCount = m_ArrWinInfo.GetCount();

		for (int i=0; i<nCount; i++)
		{
			File.Write(&m_ArrWinInfo[i], sizeof(WindowInfo));
		}

		File.Close();

		MessageBox(_T("保存场景文件成功！"), _T("场景保存"), MB_OK|MB_ICONINFORMATION);
	}
}

void CMvxSplitDlg::OnReadSceneFile()
{
	//打开场景数据文件
	CFileDialog FileDlg(TRUE, _T("srf"), "Screen1", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "Screen Files (*.srf)|*.srf|All Files (*.*)|*.*||");
	if (FileDlg.DoModal() == IDOK)
	{
		//获取文件路径
		CString strFilePath = FileDlg.GetPathName();

		CStdioFile File;
		if (!File.Open(strFilePath, CFile::modeRead))
			return (void)MessageBox(_T("打开文件失败！"),_T("场景加载"), MB_OK|MB_ICONSTOP);

		//清理所有窗口信息
		m_ArrWinInfo.RemoveAll();
		WindowInfo WinInfo;
		memset(&WinInfo, 0, sizeof(WindowInfo));

		//读取文件数据
		char buff[512] = {0};
		while(File.Read(buff, sizeof(WindowInfo)))
		{
			memcpy(&WinInfo, buff, sizeof(WindowInfo));
			m_ArrWinInfo.Add(WinInfo);
			memset(&WinInfo, 0, sizeof(WindowInfo));
		}
		File.Close();

		//根据当前画布大小调整窗口的逻辑尺寸
		ZoomWindows();

		//重绘
		InvalidateRect(m_rcChart);

		MessageBox(_T("读取场景成功！"), _T("场景加载"), MB_OK|MB_ICONINFORMATION);
	}
}

void CMvxSplitDlg::OnWriteDevScene(UINT nID)
{
	int nSceneID = nID-ID_WRITE_DEV_SCENE1;

	WriteDevScene(nSceneID);
}

void CMvxSplitDlg::OnLoadDevScene(UINT nID)
{
	int nSceneID = nID-ID_LOAD_DEV_SCENE1;

	//读取场景数据
	LoadDevScene(nSceneID);

	//根据当前绘图区的大小重新调整各窗口的逻辑尺寸
	ZoomWindows();

	//重绘
	InvalidateRect(m_rcChart);
}

void CMvxSplitDlg::OnWindowOption(UINT nID)
{
	switch(nID)
	{
	case ID_MVX_OPEN_WINDOW:
		//新建菜单
		OnNewWindow();
		break;
	case ID_MVX_CLOSE_WINDOW:
		//关闭菜单
		OnCloseWindow();
		break;
	case ID_ZORDER_TOPMOST:
	case ID_ZORDER_BOTTOMMOST:
	case ID_ZORDER_MOVEFRONT:
	case ID_ZORDER_MOVEBACK:
		//设置Zorder
		{
			emZorderAct ZorderAct;
			ZorderAct = emZorderAct(nID-ID_ZORDER_TOPMOST+1);

			SetWndZorder(m_nFocusID, ZorderAct);
		}
		break;
	case ID_MVX_SAME_WID:
		//等宽菜单
		OnSameSize(0x01);
		break;
	case ID_MVX_SAME_HGT:
		//等高菜单
		OnSameSize(0x10);
		break;
	case ID_MVX_SAME_SIZE:
		//等大小菜单
		OnSameSize(0x11);
		break;
	default:break;
	}
}


void CMvxSplitDlg::OnNewWindow()
{
	//申请不到新的合法ID
	int nNewID = PrepareNewWinID();
	if (nNewID == ErrWinID) return;

	CString strName;
	CRect rcPhy, rcLog;
	int nSource = m_boardInfo.GetBoardPort();
	strName.Format(_T("SourceChannel[%d]"), nSource+1);

	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex)
	{// 当前没有窗口 使用默认尺寸新建窗口
		//宽度为背景宽度的1/3，高度为背景高度的1/2
		rcPhy.left   = m_nPhyHorz/3;
		rcPhy.top    = m_nPhyVert/4;
		rcPhy.right  = m_nPhyHorz/3*2;
		rcPhy.bottom = m_nPhyVert/4*3;
	}
	else
	{// 使用焦点窗口的尺寸新建窗口
		rcPhy = m_ArrWinInfo[nIndex].rcPhy;
		rcPhy.OffsetRect(m_nPhyHorz/2 - rcPhy.CenterPoint().x, m_nPhyVert/2 - rcPhy.CenterPoint().y);
	}

	rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);

	SetWndInfo(nNewID, nSource, rcLog, rcPhy, strName);

	InvalidateRect(m_rcChart);

	//确认新建窗口
	CMvxWndPropertyDlg dlg(TRUE, this);
	if (dlg.DoModal() != IDOK)
		DeleteWindow(nNewID);
}

void CMvxSplitDlg::OnCloseWindow()
{
	MyCloseWindow();
}

//使所有窗口的尺寸与当前窗口的一致
void CMvxSplitDlg::OnSameSize(int ntype)
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;
	CRect rcTmp;

	int nCount = m_ArrWinInfo.GetCount();
	for (int i=0; i<nCount; i++)
	{
		rcTmp = m_ArrWinInfo[i].rcPhy;

		//宽度相同
		if (ntype & 0x01) rcTmp.right = rcTmp.left + rcPhy.Width();
		//高度相同
		if (ntype & 0x10) rcTmp.bottom = rcTmp.top + rcPhy.Height();

		m_ArrWinInfo[i].rcPhy = rcTmp;
		m_ArrWinInfo[i].rcPhyLast = rcTmp;
		m_ArrWinInfo[i].rcLog = rcTmp.MulDiv(m_nLogHorz, m_nPhyHorz);
		m_ArrWinInfo[i].rcLogLast = m_ArrWinInfo[i].rcLog;

		theApp.MVX_ResizeWindow(m_ArrWinInfo[i].WinId,
                            rcTmp,
                            m_ArrWinInfo[i].iSource,
                            m_ArrWinInfo[i].iSource,
                            m_boardInfo.GetBoardPort());
	}

	//重绘
	InvalidateRect(m_rcChart);
}

void CMvxSplitDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	point.Offset(-m_rcPaint.left, -m_rcPaint.top);

	//右键窗口弹出菜单
	if (IsMouseBtnDownInWnd(point))
	{
		CPoint ptCursor;
		GetCursorPos(&ptCursor);

		theApp.MvTrackPopupMenu(ptCursor, IDR_MENU_MVX_WND_OPTION, this);
	}

	CXTPResizeDialog::OnRButtonUp(nFlags, point);
}

BOOL CMvxSplitDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam >= VK_LEFT) && (pMsg->wParam <= VK_DOWN))
		{
			//LShift + Arrow Key 放大窗口
			if (::GetAsyncKeyState(VK_LSHIFT) & 0x8000)
				OnVKresizeWindow(pMsg->wParam, TRUE);
			//RShift + Arrow Key 缩小窗口
			else if (::GetAsyncKeyState(VK_RSHIFT) & 0x8000)
				OnVKresizeWindow(pMsg->wParam, FALSE);
			//方向键移动窗口
			else
				OnVKmoveWindow(pMsg->wParam);

			return TRUE;
		}
		else if (pMsg->wParam == VK_TAB)
		{
			//快捷键切换窗口
			if (::GetAsyncKeyState(VK_CONTROL) & 0x8000)
				OnTabChangedWindow();
		}
	}

	return CXTPResizeDialog::PreTranslateMessage(pMsg);
}

//方向键移动窗口
void CMvxSplitDlg::OnVKmoveWindow(UINT nVKcode)
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;

	rcPhy.OffsetRect((nVKcode==VK_LEFT) ? -1 : ((nVKcode==VK_RIGHT) ? 1 : 0),
                     (nVKcode==VK_UP) ? -1 : ((nVKcode==VK_DOWN) ? 1 : 0));

	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcPhyLast = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);
	m_ArrWinInfo[nIndex].rcLogLast = m_ArrWinInfo[nIndex].rcLog;

	theApp.MVX_MoveTheWindow(m_nFocusID, 
                         rcPhy,
                         m_ArrWinInfo[nIndex].iSource,
                         m_ArrWinInfo[nIndex].iSource,
                         m_boardInfo.GetBoardPort());

	InvalidateRect(m_rcChart);
}

//快捷键调整窗口大小
void CMvxSplitDlg::OnVKresizeWindow(UINT nVKcode, BOOL bInflate)
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	CRect rcPhy = m_ArrWinInfo[nIndex].rcPhy;

	if (bInflate)  //放大
		rcPhy.InflateRect(nVKcode==VK_LEFT ? 1 : 0,
                          nVKcode == VK_UP ? 1 : 0,
                          nVKcode == VK_RIGHT ? 1 : 0,
                          nVKcode == VK_DOWN ? 1 : 0);
	else  //缩小
		rcPhy.InflateRect(nVKcode==VK_LEFT ? -1 : 0,
                          nVKcode == VK_UP ? -1 : 0,
                          nVKcode == VK_RIGHT ? -1 : 0,
                          nVKcode == VK_DOWN ? -1 : 0);

	m_ArrWinInfo[nIndex].rcPhy = rcPhy;
	m_ArrWinInfo[nIndex].rcPhyLast = rcPhy;
	m_ArrWinInfo[nIndex].rcLog = rcPhy.MulDiv(m_nLogHorz, m_nPhyHorz);
	m_ArrWinInfo[nIndex].rcLogLast = m_ArrWinInfo[nIndex].rcLog;

	theApp.MVX_ResizeWindow(m_nFocusID, 
                         rcPhy,
                         m_ArrWinInfo[nIndex].iSource,
                         m_ArrWinInfo[nIndex].iSource,
                         m_boardInfo.GetBoardPort());

	InvalidateRect(m_rcChart);
}

//Ctrl + Tab 切换窗口
void CMvxSplitDlg::OnTabChangedWindow()
{
	int nIndex = GetWndIndex(m_nFocusID);
	if (nIndex == ErrIndex) return;

	int nTabedID = (m_nFocusID+1)%MAX_WIN;

	if (GetWndIndex(nTabedID) == ErrIndex)
		return;

	m_nFocusID = nTabedID;

	InvalidateRect(m_rcChart);
}

//保存设置
BOOL CMvxSplitDlg::SaveAll()
{
	WriteDevScene(0);

	Sleep(50);

	if (theApp.SaveAll(m_boardInfo.GetBoardPort()) == CMD_FAILURE)
	{
		MessageBox(_T("错误！固化命令失败！"), _T("固化参数"), MB_OK|MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}