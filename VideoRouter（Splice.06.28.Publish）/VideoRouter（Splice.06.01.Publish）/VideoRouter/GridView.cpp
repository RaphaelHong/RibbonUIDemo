// GridView.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "VideoRouterDoc.h"
#include "GridView.h"
#include "Dialog components/SpliceSettingDlg.h"
#include "Dialog components/WndProperty.h"


// 大屏背景区距视图边框的距离
const int iBorder = 20;

// CGridView

IMPLEMENT_DYNCREATE(CGridView, CScrollView)

CGridView::CGridView()
{
	//m_pMemDC = NULL;

	m_dScale = 1.0;
	
	m_sizeTotal = CSize(600, 400);
	m_sizePage = CSize(200, 200);
	m_sizeLine = CSize(20, 20);
	m_ptViewOrg = CPoint(0, 0);	

	m_rcPage = CRect(iBorder, iBorder, iBorder+600, iBorder+400);

	m_bFitWidth = TRUE;
	m_bFitHeight = TRUE;

	if (!LoadState())
	{
		m_Page.SetLayoutInfo(m_Layout);
		m_Page.SetPosition(m_rcPage);
		m_Page.ResetScreens();
		m_Page.InitScreenPos();
	}	

	m_MouseManager.SetOwner(&m_Page);
}

BOOL CGridView::LoadState()
{
	CSysConfManager SysConf;
	// 获取最近设置的屏幕组合数
	int iPhyHorz, iPhyVert;
	iPhyVert = iPhyHorz = 1;
	iPhyHorz = SysConf.GetInt(_T("LastSpliceModel"), _T("LastTryHorzScreenNumber"), iPhyHorz);
	iPhyVert = SysConf.GetInt(_T("LastSpliceModel"), _T("LastTryVertScreenNumber"), iPhyVert);

	// 获取最近保存的拼接信息
	CString strPath;
	strPath = GetGlobalManager()->GetAppPath() + _T("Config\\LastTrySpliceModel.lts");
	SysConf.SetConfigFilePath(strPath.GetBuffer());

	int iScreens = iPhyHorz*iPhyVert;
	long nFileLen = sizeof(SPLICELAYOUT) + sizeof(SCREENMAP)*iScreens;
	CMemoryAllocate<char> alloc(nFileLen);

	long nRead = nFileLen;
	
	if (!SysConf.LoadFromFile(NULL, alloc.GetAddress(), nRead, TRUE))
		return FALSE;

	if (nRead != nFileLen)
		return FALSE;

	pSCREENMAP pScrmap = new SCREENMAP[iScreens];
	memset(pScrmap, 0, sizeof(SCREENMAP)*iScreens);

	long lpos = 0;
	memcpy(&m_Layout, alloc.GetAddress()+lpos, sizeof(SPLICELAYOUT)); lpos += sizeof(SPLICELAYOUT);
	memcpy(pScrmap,   alloc.GetAddress()+lpos, sizeof(SCREENMAP)*iScreens); lpos += sizeof(SCREENMAP)*iScreens;

	m_Page.SetLayoutInfo(m_Layout);
	m_Page.ResetScreens(pScrmap, iScreens, FALSE);
	m_Page.SetPosition(m_rcPage);

	SAFE_DELETE_AR(pScrmap);

	return TRUE;
}

CGridView::~CGridView()
{
	//SAFE_DELETE(m_pMemDC);	
}


BEGIN_MESSAGE_MAP(CGridView, CScrollView)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_RIBBON_CMD_SPLIT_SET, &CGridView::OnMenuSpliceSet)
	ON_COMMAND(ID_RIBBON_CMD_WND_NEW, &CGridView::OnMenuNew)
	ON_COMMAND(ID_RIBBON_CMD_WND_CLOSE, &CGridView::OnMenuClose)
	ON_COMMAND(ID_RIBBON_CMD_WND_CLOSE_ALL, &CGridView::OnMenuCloseAll)
	ON_COMMAND(ID_RIBBON_CMD_WND_LOCKALL, &CGridView::OnMenuLock)
	ON_COMMAND(ID_RIBBON_CMD_WND_TOP, &CGridView::OnMenuMovetoTop)
	ON_COMMAND(ID_RIBBON_CMD_WND_BOTTOM, &CGridView::OnMenuMoveToBtm)
	ON_COMMAND(ID_RIBBON_CMD_WND_FRONT,&CGridView::OnMenuMoveFront)
	ON_COMMAND(ID_RIBBON_CMD_WND_BACK, &CGridView::OnMenuMoveBack)
	ON_COMMAND(ID_RIBBON_CMD_WND_PROPERTY, &CGridView::OnMenuProperty)
	ON_MESSAGE(WM_USER_SOURCECHANGED, &CGridView::OnSourceChange)
	ON_MESSAGE(WM_USER_OUTPORTCHANGED, &CGridView::OnOutPortChange)
	ON_MESSAGE(WM_USER_RECV_LAYOUT_INF, &CGridView::OnRecieveLayoutInf)
	ON_MESSAGE(WM_USER_RECV_SCENE_DATA, &CGridView::OnRecieveSceneInf)
	ON_MESSAGE(WM_USER_UNPACK_SCENE, &CGridView::OnUnpackSceneFile)
	ON_MESSAGE(WM_USER_PACKAGE_SCENE, &CGridView::OnPackageSceneFile)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_CMD_WND_LOCKALL, &CGridView::OnUpdateUICmdLock)
	ON_UPDATE_COMMAND_UI_RANGE(ID_RIBBON_CMD_WND_CLOSE, ID_RIBBON_CMD_WND_BACK, &CGridView::OnUpdateUICmdClose)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_RIBBON_CMD_SHOW_GRID, &CGridView::OnMenuShowGridLines)
	ON_UPDATE_COMMAND_UI(ID_RIBBON_CMD_SHOW_GRID, &CGridView::OnUpdateUICmdShowGridLines)
	ON_MESSAGE(WM_USER_ON_SOURCE_DROP, &CGridView::OnDropSource)
	ON_MESSAGE(WM_USER_ON_OUTPUT_DROP, &CGridView::OnDropOutput)
	ON_MESSAGE(WM_USER_RECV_SCREEN_INF, &CGridView::OnRecieveScreenInf)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()


// CGridView 绘图

void CGridView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	//CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	//sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, m_sizeTotal, m_sizePage, m_sizeLine);
}

//*************************************************
// 拼接视图绘画控制说明：
//     大屏背景按缩放比例缩放到绘制位图大小，
// 再由视图绘制在当前视图上。但由于在大屏背景位图周边
// 会保留一定的边框宽度，故绘图时需注意。
void CGridView::OnDraw(CDC* pDC)
{
	CVideoRouterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 获取视图窗口的坐标.
	CRect rcClient;
	GetClientRect(&rcClient);
	// 偏移后得到当前视图的坐标
	rcClient.OffsetRect(m_ptViewOrg);


	// 设置与设备环境中窗口的尺寸 （完整画布的大小）
	pDC->SetWindowExt(max(m_sizeTotal.cx, rcClient.Width()), max(m_sizeTotal.cy, rcClient.Height()));
	// 为设备环境设置视口大小（实际输出到屏幕时的尺寸，是从完整画布中截取出来的）
	pDC->SetViewportExt(rcClient.Width(), rcClient.Height());
	// 设置设备环境中窗口的起始坐标
	pDC->SetWindowOrg(-m_ptViewOrg);


	// 创建内存DC （一个临时绘画环境）
	CDC* pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);


	// 该临时环境的起始坐标应与实际设备环境一致
	pMemDC->SetViewportOrg(-m_ptViewOrg);


	// 为临时环境创建一副兼容位图
	CBitmap bmp, *pOldbmp;
	bmp.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	pOldbmp = pMemDC->SelectObject(&bmp);


	// 为位图填充背景色
	CBrush br(RGB(235, 240, 235));
	pMemDC->FillRect(rcClient, &br);


	// 开始真正的作画
	m_Page.DrawContent(pMemDC);


	// 将临时环境中的画片拷贝到实际输出环境中的正确位置
	pDC->BitBlt(0, 0, rcClient.Width(), rcClient.Height(), pMemDC, m_ptViewOrg.x, m_ptViewOrg.y, SRCCOPY);


	pMemDC->SelectObject(pOldbmp);
	SAFE_DELETE(pMemDC);
}

//void CGridView::OnDraw(CDC* pDC)
//{
//	CVideoRouterDoc* pDoc = GetDocument();
//	ASSERT_VALID(pDoc);
//	if (!pDoc)
//		return;
//
//	DWORD dwStart = GetTickCount();
//
//	// 客户区大小.
//	CRect rcClient;
//	GetClientRect(&rcClient);
//
//	CRgn clipRgn;
//	clipRgn.CreateRectRgn(rcClient.left, rcClient.top, rcClient.right,rcClient.bottom);
//	pDC->SelectClipRgn(&clipRgn);
//
//	// 最终图像尺寸.
//	CRect rcPaint;
//	rcPaint = CRect(0, 0, max(m_sizeTotal.cx, rcClient.Width()), max(m_sizeTotal.cy, rcClient.Height()));
//
//	// 开始绘窗口
//
//	// 创建一个内存DC
//
//	if (m_pMemDC == NULL)
//	{
//		m_pMemDC = new CDC;
//		m_pMemDC->CreateCompatibleDC(NULL);
//		CBitmap MemBmp;
//		MemBmp.CreateCompatibleBitmap(pDC, rcPaint.Width(), rcPaint.Height());
//		m_pMemDC->SelectObject(&MemBmp);
//
//		// 填充背景
//		CBrush br;
//		br.CreateSolidBrush(RGB(240, 240, 240));
//		m_pMemDC->FillRect(rcPaint, &br);
//
//		m_Page.DrawBk(m_pMemDC);
//	}
//
//	CDC* pCopyDC = new CDC;
//	pCopyDC->CreateCompatibleDC(NULL);
//	CBitmap CopyBmp, *pOldBmp;
//	CopyBmp.CreateCompatibleBitmap(pDC, rcPaint.Width(), rcPaint.Height());
//	pOldBmp = pCopyDC->SelectObject(&CopyBmp);
//
//	pCopyDC->BitBlt(0,0, rcPaint.Width(), rcPaint.Height(), m_pMemDC, 0, 0, SRCCOPY);
//
//	//画子屏幕
//	m_Page.DrawContent(pCopyDC);
//
//	pDC->BitBlt(0, 0, rcPaint.Width(), rcPaint.Height(), pCopyDC, 0, 0, SRCCOPY);
//	SAFE_DELETE(pCopyDC);
//
//	TRACE(_T("Once %d\n"), GetTickCount()-dwStart);
//}

// CGridView 诊断

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CGridView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif

CVideoRouterDoc* CGridView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVideoRouterDoc)));
	return (CVideoRouterDoc*)m_pDocument;
}
#endif //_DEBUG


// CGridView 消息处理程序

int CGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CDrawWndPage::m_sShowGridLines = theApp.GetInt("ShowGridLines", 1);

	DragAcceptFiles(TRUE);

	m_sizeTotal.cx = m_Layout.iPhyHorzPixel + iBorder*2;
	m_sizeTotal.cy = m_Layout.iPhyVertPixel + iBorder*2;

	SetTimer(1, 100, NULL);

	m_Page.SetOwner(this);

	return 0;
}

void CGridView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect rcClient;
	GetClientRect(rcClient);
	int nMinTrackPos = 0;
	int nMaxTrackPos = m_sizeTotal.cx - rcClient.Width();

	m_ptViewOrg = GetScrollPosition();

	switch (nSBCode)
	{
	case SB_THUMBTRACK: //按住滚动滑块滑动
	case SB_THUMBPOSITION: //鼠标点击在滑块上
		m_ptViewOrg.x = nPos;
		break;
	case SB_LINERIGHT: //向右滚动一行，鼠标点击在向右滚动的箭头上产生
		{
			m_ptViewOrg.x += m_sizeLine.cx;
			m_ptViewOrg.x = min(m_ptViewOrg.x, nMaxTrackPos);
		}
		break;
	case SB_PAGERIGHT: //向右滚动一页，鼠标点击在滑块右方的空白处产生
		{
			m_ptViewOrg.x += m_sizePage.cx;
			m_ptViewOrg.x = min(m_ptViewOrg.x, nMaxTrackPos);
		}
		break;
	case SB_RIGHT: //滚到最右
		{
			m_ptViewOrg.x = nMaxTrackPos;
		}
		break;
	case SB_LINELEFT: //向左滚动一行，鼠标点击在向左滚动的箭头上产生
		{
			m_ptViewOrg.x -= m_sizeLine.cx;
			m_ptViewOrg.x = max(m_ptViewOrg.x, nMinTrackPos);
		}
		break;
	case SB_PAGELEFT: //向左滚动一页，鼠标点击在滑块左方的空白处产生
		{
			m_ptViewOrg.x -= m_sizePage.cx;
			m_ptViewOrg.x = max(m_ptViewOrg.x, nMinTrackPos);
		}
		break;
	case SB_LEFT: //滚到最左
		{
			m_ptViewOrg.x = nMinTrackPos;
		}
		break;
	default:
		break;
	}


	SetScrollPos(SB_HORZ, m_ptViewOrg.x);
	Invalidate();

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGridView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect rcClient;
	GetClientRect(rcClient);
	int nMinTrackPos = 0;
	int nMaxTrackPos = m_sizeTotal.cy - rcClient.Height();

	m_ptViewOrg = GetScrollPosition();

	switch (nSBCode)
	{
	case SB_THUMBTRACK: //按住滚动滑块滑动
	case SB_THUMBPOSITION: //鼠标点击在滑块上
		m_ptViewOrg.y = nPos;
		break;
	case SB_LINEDOWN: //向右滚动一行，鼠标点击在向右滚动的箭头上产生
		{
			m_ptViewOrg.y += m_sizeLine.cy;
			m_ptViewOrg.y = min(m_ptViewOrg.y, nMaxTrackPos);
		}
		break;
	case SB_PAGEDOWN: //向右滚动一页，鼠标点击在滑块右方的空白处产生
		{
			m_ptViewOrg.y += m_sizePage.cy;
			m_ptViewOrg.y = min(m_ptViewOrg.y, nMaxTrackPos);
		}
		break;
	case SB_BOTTOM: //滚到最右
		{
			m_ptViewOrg.y = nMaxTrackPos;
		}
		break;
	case SB_LINEUP: //向左滚动一行，鼠标点击在向左滚动的箭头上产生
		{
			m_ptViewOrg.y -= m_sizeLine.cy;
			m_ptViewOrg.y = max(m_ptViewOrg.y, nMinTrackPos);
		}
		break;
	case SB_PAGEUP: //向左滚动一页，鼠标点击在滑块左方的空白处产生
		{
			m_ptViewOrg.y -= m_sizePage.cy;
			m_ptViewOrg.y = max(m_ptViewOrg.y, nMinTrackPos);
		}
		break;
	case SB_TOP: //滚到最左
		{
			m_ptViewOrg.y = nMinTrackPos;
		}
		break;
	default:
		break;
	}

	SetScrollPos(SB_VERT, m_ptViewOrg.y);
	Invalidate();

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CGridView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CScrollView::OnEraseBkgnd(pDC);
}

void CGridView::RedrawGraphic()
{
	//if (m_pMemDC)
	//{
	//	ReleaseDC(m_pMemDC);
	//	SAFE_DELETE(m_pMemDC);
	//}
	
	Invalidate();
}

BOOL CGridView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if ((::GetAsyncKeyState(VK_CONTROL) & 0x8000) == 0)
		OnMouseScroll(zDelta);
	else
		OnMouseZoom(zDelta);

	return TRUE;

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CGridView::OnMouseScroll(int zDelta)
{
	CRect rc;
	GetClientRect(&rc);
	if (rc.Height() > m_sizeTotal.cy)
	{
		//** Now the scrollbars were all hidden.
		return ;
	}

	int nNewPos = (-zDelta/120)*m_sizePage.cy + m_ptViewOrg.y;
	int nMinPos = 0;
	int nMaxPos = m_sizeTotal.cy - rc.Height();

	nNewPos = max(nNewPos, nMinPos);
	nNewPos = min(nNewPos, nMaxPos);

	m_ptViewOrg.y = nNewPos;

	SetScrollPos(SB_VERT, nNewPos);

	Invalidate();
}

void CGridView::OnMouseZoom(int zDelta)
{
	if ((m_dScale >= 1.0 && zDelta > 0) || (m_dScale<= 0.05 && zDelta < 0))
		return ;

	double dScale = .0;
	if (zDelta > 0) dScale = m_dScale*1.1;
	else dScale = m_dScale/1.1;

	if (dScale >= 1.0)
		dScale = 1.0;
	if (dScale < 0.05)
		dScale = 0.05;

	SetZoomLevel(dScale);

	GetMainFrame()->UpdateSliderZoomPos(dScale);
}

void CGridView::SetZoomLevel(double dZoomLvl)
{
	m_dScale = dZoomLvl;

	//锁定重绘
	CRedrawLock lock(this);

	//新的滚动位置
	CPoint ptNewScrollPos;
	
	//记录视图缩放前的滚动位置和视图大小
	m_ptViewOrg = GetScrollPosition();
	CSize sizeTotalOld = m_sizeTotal;

	CRect rcCli;
	GetClientRect(&rcCli);

	// 计算滚动大小
	int iLogHei = long(m_Layout.iPhyVertPixel*m_dScale);
	int iLogWid = long(m_Layout.iPhyHorzPixel*m_dScale);
	
	m_sizeTotal.cx = max(iLogWid + iBorder*2, rcCli.Width());
	m_sizeTotal.cy = max(iLogHei + iBorder*2, rcCli.Height());

	if (m_rcPage.Width() <= (rcCli.Width()-iBorder*2))
	{
		m_rcPage.left = long((rcCli.Width()-iLogWid)*0.5);
		m_rcPage.right = m_rcPage.left + iLogWid;
	}
	else
	{
		m_rcPage.left = iBorder;
		m_rcPage.right = m_rcPage.left + iLogWid;
	}
	
	if (m_rcPage.Height() <= (rcCli.Height()-iBorder*2))
	{
		m_rcPage.top = long((rcCli.Height()-iLogHei)*0.5);
		m_rcPage.bottom = m_rcPage.top + iLogHei;
	}
	else
	{
		m_rcPage.top = iBorder;
		m_rcPage.bottom = m_rcPage.top + iLogHei;
		m_bFitHeight = FALSE;
	}	

	//设置新的滚动尺寸
	SetScrollSizes(MM_TEXT, m_sizeTotal, m_sizePage, m_sizeLine);

	m_Page.SetPosition(m_rcPage);

	RedrawGraphic();

	m_ptViewOrg = GetScrollPosition();
}

void CGridView::SetProperZoom()
{
	CRect rcCli;
	GetClientRect(&rcCli);

	CRect rc = rcCli;
	rc.InflateRect(-iBorder, -iBorder);

	// 客户区与大屏宽度比
	double dHScale = rc.Width()*1.0/m_Layout.iPhyHorzPixel;
	// 客户区与大屏高度比
	double dVScale = rc.Height()*1.0/m_Layout.iPhyVertPixel;

	// 最合适的绘制比例
	double dProper = min(dHScale, dVScale);

	if (dProper >= 1.0)
	{
		// 此时客户区已完全能够容纳物理屏
		m_dScale = 1.0;
	}
	else
	{
		// 此时客户区已无法完全容纳物理屏
		m_dScale = dProper;
	}

	//计算逻辑背景区
	m_rcPage.left   = long((rcCli.Width()-m_Layout.iPhyHorzPixel*m_dScale)*0.5);
	m_rcPage.top    = long((rcCli.Height()-m_Layout.iPhyVertPixel*m_dScale)*0.5);
	m_rcPage.right  = m_rcPage.left + long(m_Layout.iPhyHorzPixel*m_dScale);
	m_rcPage.bottom = m_rcPage.top + long(m_Layout.iPhyVertPixel*m_dScale);

	// 滚动尺寸设为当前客户区大小
	m_sizeTotal = CSize(rcCli.Width(), rcCli.Height());	
	SetScrollSizes(MM_TEXT, m_sizeTotal, m_sizePage, m_sizeLine);

	m_Page.SetPosition(m_rcPage);

	GetMainFrame()->UpdateSliderZoomPos(m_dScale);
}

void CGridView::OnSize(UINT nType, int cx, int cy)
{
	SetScrollSizes(MM_TEXT, m_sizeTotal, m_sizePage, m_sizeLine);
	m_ptViewOrg = GetScrollPosition();

	CScrollView::OnSize(nType, cx, cy);

	SetZoomLevel(m_dScale);
}


void CGridView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		SetProperZoom();
		
		RedrawGraphic();

		KillTimer(nIDEvent);
	}

	CScrollView::OnTimer(nIDEvent);
}

void CGridView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();

	CPoint pt = point;
	pt.Offset(m_ptViewOrg);

	m_MouseManager.OnLBtnDown(pt);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CGridView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	CPoint pt = point;
	pt.Offset(m_ptViewOrg);

	m_MouseManager.OnLBtnUp(pt);

	CScrollView::OnLButtonUp(nFlags, point);
}

void CGridView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CPoint pt = point;
	pt.Offset(m_ptViewOrg);

	m_MouseManager.OnLBtnDbClick(pt);

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CGridView::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint pt = point;
	pt.Offset(m_ptViewOrg);

	m_MouseManager.OnMouseMove(pt);
	
	CScrollView::OnMouseMove(nFlags, point);
}

void CGridView::OnMenuSpliceSet()
{
	CSpliceSettingDlg DLG(&m_Layout, &m_Page, this);
	
	if (DLG.DoModal() == IDOK)
		RedrawGraphic();

	//m_Page.RemoveAllWnd();
	//m_Page.SetLayoutInfo(m_Layout);
	//m_Page.ResetScreens();

	//SetProperZoom(FALSE);

	//向底层设备发送背景布置信息
	//if (GetCmdManager()->CmdWCN1(SYSBOARD_PORT, eBoardTypeCtrl, m_Layout) == CMD_FAILURE)
	//{
	//	MessageBox(_T("更新拼接设置失败！"), _T("拼接设置"), MB_OK|MB_ICONERROR);
	//}
}

void CGridView::OnMenuNew()
{
	CRect rcCli;
	GetClientRect(rcCli);

	CPoint pt = rcCli.CenterPoint();
	pt.Offset(m_ptViewOrg);
	
	m_Page.NewWnd(pt);
}

void CGridView::OnMenuClose()
{
	CDrawWndObj* pObj = m_Page.GetActivedWnd();
	if (pObj == NULL)
		return;

	pObj->Shut(pObj->GetWndID());

	m_Page.RemoveWnd(pObj);
	Invalidate();
}

void CGridView::OnMenuCloseAll()
{
	CDrawWndObj::Shut(0);
	m_Page.RemoveAllWnd();
	Invalidate();
}

void CGridView::OnMenuLock()
{
	m_Page.LockAll();
	Invalidate();
}

void CGridView::OnMenuMoveFront()
{
	m_Page.MoveForward(m_Page.GetActivedWnd());
}

void CGridView::OnMenuMovetoTop()
{
	m_Page.MovetoTop(m_Page.GetActivedWnd());
}

void CGridView::OnMenuMoveBack()
{
	m_Page.MoveBackward(m_Page.GetActivedWnd());
}

void CGridView::OnMenuMoveToBtm()
{
	m_Page.MovetoBottom(m_Page.GetActivedWnd());
}

void CGridView::OnMenuProperty()
{
	CWndProperty DLG(&m_Page, m_Layout, this);
	DLG.DoModal();
}

void CGridView::OnMenuShowGridLines()
{
	CDrawWndPage::m_sShowGridLines = !CDrawWndPage::m_sShowGridLines;

	theApp.WriteInt(_T("ShowGridLines"), CDrawWndPage::m_sShowGridLines);

	Invalidate();
}

void CGridView::OnUpdateUICmdClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_Page.GetWndCount() > 0);	
}

void CGridView::OnUpdateUICmdShowGridLines(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CDrawWndPage::m_sShowGridLines);
}


LRESULT CGridView::OnSourceChange(WPARAM wParam, LPARAM lParam)
{
	int nInputCh = *((int*)wParam);
	m_Page.OnSourceChanged(nInputCh);

	return 0;
}

LRESULT CGridView::OnOutPortChange(WPARAM wParam, LPARAM lParam)
{
	//如果该消息收到的屏幕ID大于0 表示是从设备读取回来的屏幕位置信息
	//如果屏幕ID小于0表示是用户切换屏幕位置
	SCREENMAP scrmap;
	scrmap = *((pSCREENMAP)wParam);
	m_Page.OnOutPortChanged(scrmap);

	return 0;
}

void CGridView::OnUpdateUICmdLock(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(CDrawWndPage::m_sLockAll);
}

// 解析底层返回的拼接信息
LRESULT CGridView::OnRecieveLayoutInf(WPARAM wParam, LPARAM lParam)
{
	m_Layout = *((pSPLICELAYOUT)wParam);

	m_Page.RemoveAllWnd();
	m_Page.SetLayoutInfo(m_Layout);

	return 0;
}

// 解析底层返回的屏幕信息
LRESULT CGridView::OnRecieveScreenInf(WPARAM wParam, LPARAM lParam)
{
	pSCREENMAP pScrMap = (pSCREENMAP)wParam;
	int iScreens = *(int*)lParam;

	// 先计算背景大小
	CRect rcBkg = CDrawWndPage::GetBkgSizeAccordingtoScrInf(pScrMap, iScreens, TRUE);

	m_Layout.iPhyHorzPixel = rcBkg.Width();
	m_Layout.iPhyVertPixel = rcBkg.Height();

	m_Page.SetLayoutInfo(m_Layout);
	SetProperZoom();
	m_Page.ResetScreens(pScrMap, iScreens, TRUE);

	RedrawGraphic();

	return 0;
}

// 更新对齐线
void CGridView::UpdateAligningLine(const int iHorz, const int iVert)
{
	m_Layout.iLogHorz = iHorz;
	m_Layout.iLogVert = iVert;

	m_Page.UpdateAligningLine(iHorz, iVert);

	Invalidate();
}


// 解析底层返回的场景数据
LRESULT CGridView::OnRecieveSceneInf(WPARAM wParam, LPARAM lParam)
{
	UINT* pBuff = (UINT*)wParam;
	int   nLen  = *(int*)lParam;

	m_Page.ParseSceneBlocks(pBuff, nLen);

	return 0;
}

// 解析场景文件
LRESULT CGridView::OnUnpackSceneFile(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszFile = (LPCTSTR)wParam;

	m_Page.ParseFile(lpszFile);

	return 0;
}

// 打包场景文件
LRESULT CGridView::OnPackageSceneFile(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszFile = (LPCTSTR)wParam;

	m_Page.PackFile(lpszFile);

	return 0;
}

BOOL CGridView::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_UP) {
			CPageKeyboardManager keyboardManager(&m_Page);
			keyboardManager.OnArrowKeyUp();
		}
		else if (pMsg->wParam == VK_DOWN) {
			CPageKeyboardManager keyboardManager(&m_Page);
			keyboardManager.OnArrowKeyDown();
		}
		else if (pMsg->wParam == VK_LEFT) {
			CPageKeyboardManager keyboardManager(&m_Page);
			keyboardManager.OnArrowKeyLeft();
		}
		else if (pMsg->wParam == VK_RIGHT) {
			CPageKeyboardManager keyboardManager(&m_Page);
			keyboardManager.OnArrowKeyRight();
		}
	}

	return CScrollView::PreTranslateMessage(pMsg);
}

void CGridView::OnDropFiles(HDROP hDropInfo)
{
	CMemoryAllocate<char> alloc(_MAX_FNAME);

	// Get file count.
	UINT uFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	char* pExt = NULL;
	for (UINT i=0; i<uFiles; i++)
	{
		// Get file pathname.
		DragQueryFile(hDropInfo, i, alloc.GetAddress(), _MAX_FNAME);

		// Get extended file name.
		pExt = strrchr(alloc.GetAddress(), '.');

		if (pExt == NULL)
			continue;

		if (strstr(pExt, "scr") != NULL)
		{
			SendMessage(WM_USER_UNPACK_SCENE, WPARAM(alloc.GetAddress()), 0);
			break;
		}
	}

	pExt = NULL;
	
	DragFinish(hDropInfo);	

	CScrollView::OnDropFiles(hDropInfo);
}

// dropped a source signal.
LRESULT CGridView::OnDropSource(WPARAM wParam, LPARAM lParam)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	pt.Offset(m_ptViewOrg);

	int nSourcePort = *(int*)wParam;

	m_MouseManager.OnDropSource(nSourcePort, pt);

	return 0;
}
// dropped a output channel.
LRESULT CGridView::OnDropOutput(WPARAM wParam, LPARAM lParam)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	pt.Offset(m_ptViewOrg);

	int iOutPort = *(int*)wParam;

	m_MouseManager.OnDropOutput(iOutPort, pt);

	return 0;
}

void CGridView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CPoint pt = point;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	pt.Offset(m_ptViewOrg);

	m_MouseManager.OnContextMenu(pt);
}