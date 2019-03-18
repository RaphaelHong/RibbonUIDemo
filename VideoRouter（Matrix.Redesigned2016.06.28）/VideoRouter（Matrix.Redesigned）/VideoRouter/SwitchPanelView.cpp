
// SwitchPanelView.cpp : CSwitchPanelView 类的实现
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "VideoRouterDoc.h"
//#include "VideoRouterView.h"
#include "MainFrm.h"

#include "SwitchPanelView.h"
#include "Dialog components/OsdStringDesignerDlg.h"
#include "Dialog components/RenameDlg.h"
#include "Dialog components/SourcePropertyDlg.h"
#include "ResolutionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_COUNTER        100  //计时定时器

// CSwitchPanelView

IMPLEMENT_DYNCREATE(CSwitchPanelView, CView)

BEGIN_MESSAGE_MAP(CSwitchPanelView, CView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//ON_WM_MOUSEWHEEL()
	//ON_WM_HSCROLL()
	//ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_USER_UNPACK_SCENE, &CSwitchPanelView::OnUnpackSceneFile)
	ON_MESSAGE(WM_USER_PACKAGE_SCENE, &CSwitchPanelView::OnPackageSceneFile)
	ON_MESSAGE(WM_USER_RECV_SCENE_DATA, &CSwitchPanelView::OnRecieveSceneInf)
END_MESSAGE_MAP()

// CSwitchPanelView 构造/析构

CSwitchPanelView::CSwitchPanelView()
{
	m_sizeTotal = CSize(600, 400);
	m_sizePage = CSize(100, 100);
	m_sizeLine = CSize(25, 25);
	m_ptViewOrg = CPoint(0, 0);	
}

CSwitchPanelView::~CSwitchPanelView()
{
}


BOOL CSwitchPanelView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CView::PreCreateWindow(cs);
}

// CSwitchPanelView 绘制
void CSwitchPanelView::OnDraw(CDC* pDC)
{
	CVideoRouterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rcCli;
	GetClientRect(&rcCli);

	Graphics gp(pDC->m_hDC);

	LinearGradientBrush bkbr(Point(rcCli.top, rcCli.left), Point(rcCli.right, rcCli.bottom), Color(255, 120, 150, 223), Color(255, 180, 245, 255));
	gp.FillRectangle(&bkbr, rcCli.left, rcCli.top, rcCli.Width(), rcCli.Height());

	//Image* img;
	//if (ImageFromIDResource(IDB_PNG_PAGELEFT_CK, "png", img))
	//{
	//	Rect rcImg;
	//	rcImg.X = 500;
	//	rcImg.Y = 500;
	//	rcImg.Width = img->GetWidth();
	//	rcImg.Height = img->GetHeight();

	//	Image* img2 = img->Clone();
	//	img2->RotateFlip(Rotate180FlipX);
	//	gp.DrawImage(img2, rcImg);
	//}
}

void CSwitchPanelView::OnRButtonUp(UINT nFlags, CPoint point)
{

}

void CSwitchPanelView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CSwitchPanelView 诊断

#ifdef _DEBUG
void CSwitchPanelView::AssertValid() const
{
	CView::AssertValid();
}

void CSwitchPanelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVideoRouterDoc* CSwitchPanelView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVideoRouterDoc)));
	return (CVideoRouterDoc*)m_pDocument;
}
#endif //_DEBUG


// CSwitchPanelView 消息处理程序

void CSwitchPanelView::OnSize(UINT nType, int cx, int cy)
{
	//窗口大小变化后，需要重新获取当前视图的原点位置
#if ENABLE_SCROLLVIEW
	SetScrollSizes(MM_TEXT, CSize(cx, cy), m_sizePage, m_sizeLine);
	m_ptViewOrg = GetScrollPosition();
#endif

	if (m_page.GetSafeHwnd())
	{
		m_page.SetPageRect(CRect(0, 0, cx, cy));
		m_page.MoveWindow(CRect(0, 0, cx, cy));
		m_page.RecalLayout();
	}


	CView::OnSize(nType, cx, cy);	
}

BOOL CSwitchPanelView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

void CSwitchPanelView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);
}

void CSwitchPanelView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);
}


void CSwitchPanelView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);
}



void CSwitchPanelView::OnTimer(UINT_PTR nIDEvent)
{
	CView::OnTimer(nIDEvent);
}

#if ENABLE_SCROLLVIEW
void CSwitchPanelView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, m_sizeTotal, m_sizePage, m_sizeLine);
}
#endif

BOOL CSwitchPanelView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//return TRUE;

	//CRect rc;
	//GetClientRect(&rc);
	//if (rc.Height() > m_sizeTotal.cy)
	//{
	//	//** Now the scrollbars were all hidden.
	//	return TRUE;
	//}

	//int nNewPos = (-zDelta/120)*m_sizePage.cy + m_ptViewOrg.y;
	//int nMinPos = 0;
	//int nMaxPos = m_sizeTotal.cy - rc.Height();

	//nNewPos = max(nNewPos, nMinPos);
	//nNewPos = min(nNewPos, nMaxPos);

	//m_ptViewOrg.y = nNewPos;

	//SetScrollPos(SB_VERT, nNewPos);

	//Invalidate();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

#if ENABLE_SCROLLVIEW
void CSwitchPanelView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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

	Invalidate();

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSwitchPanelView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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

	Invalidate();

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

#endif

BOOL CSwitchPanelView::PreTranslateMessage(MSG* pMsg)
{
	//if (pMsg->message == WM_KEYDOWN)
	//{
	//	//** Scroll view by virtual key.
	//	if (   pMsg->wParam == VK_UP
	//		|| pMsg->wParam == VK_DOWN
	//		|| pMsg->wParam == VK_LEFT
	//		|| pMsg->wParam == VK_RIGHT 
	//		|| pMsg->wParam == VK_HOME
	//		|| pMsg->wParam == VK_END
	//		|| pMsg->wParam == VK_PRIOR
	//		|| pMsg->wParam == VK_NEXT)
	//	{
	//		//OnVKScroll(pMsg->wParam);
	//	}
	//}

	return CView::PreTranslateMessage(pMsg);
}

int CSwitchPanelView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CSwitchPage::m_sZoomLvl = emZoomLevel(theApp.GetInt("ZoomLevel", 3));
	m_page.SetPageRect(CRect(0, 0, 1024, 768));
	m_page.SetMargins();
	m_page.Create(NULL, "", WS_CHILD|WS_VISIBLE, CRect(0, 0, 1024, 768), this, ID_WND_SWITCHPAGE);

	return 0;
}

void CSwitchPanelView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CView::OnShowWindow(bShow, nStatus);
}

void CSwitchPanelView::SetZoomLevel(int nLvl)
{
	// 无效的缩放比例
	if (nLvl < eZLSmall || nLvl > eZLLarge)
		return;

	// 当前视图已经是此比例显示了
	if (nLvl == CSwitchPage::m_sZoomLvl)
		return;

	CSwitchPage::m_sZoomLvl = (emZoomLevel)nLvl;

	theApp.WriteInt("ZoomLevel", nLvl);

	// 重置视图
	ResetPage();
}

void CSwitchPanelView::ResetPage()
{
	if (m_page.GetSafeHwnd())
	{
		m_page.RecalLayout();
		m_page.RedrawWindow();
	}
}

void CSwitchPanelView::RefreshComplete()
{
	if (m_page.GetSafeHwnd())
	{
		m_page.RefreshAll();
	}
}

// 解析场景文件
LRESULT CSwitchPanelView::OnUnpackSceneFile(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszFile = (LPCTSTR)wParam;

	m_page.ParseFile(lpszFile);

	return 0;
}

// 打包场景文件
LRESULT CSwitchPanelView::OnPackageSceneFile(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszFile = (LPCTSTR)wParam;

	m_page.PackFile(lpszFile);

	return 0;
}

// 解析底层返回的场景数据
LRESULT CSwitchPanelView::OnRecieveSceneInf(WPARAM wParam, LPARAM lParam)
{
	UINT* pBuff = (UINT*)wParam;
	int   nLen  = *(int*)lParam;

	m_page.ParseSceneBlock(pBuff, nLen);

	return 0;
}