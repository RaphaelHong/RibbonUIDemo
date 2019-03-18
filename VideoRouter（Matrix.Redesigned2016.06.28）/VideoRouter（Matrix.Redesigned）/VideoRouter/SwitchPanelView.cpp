
// SwitchPanelView.cpp : CSwitchPanelView ���ʵ��
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

#define TIMER_COUNTER        100  //��ʱ��ʱ��

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

// CSwitchPanelView ����/����

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

// CSwitchPanelView ����
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


// CSwitchPanelView ���

#ifdef _DEBUG
void CSwitchPanelView::AssertValid() const
{
	CView::AssertValid();
}

void CSwitchPanelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVideoRouterDoc* CSwitchPanelView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVideoRouterDoc)));
	return (CVideoRouterDoc*)m_pDocument;
}
#endif //_DEBUG


// CSwitchPanelView ��Ϣ�������

void CSwitchPanelView::OnSize(UINT nType, int cx, int cy)
{
	//���ڴ�С�仯����Ҫ���»�ȡ��ǰ��ͼ��ԭ��λ��
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
	case SB_THUMBTRACK: //��ס�������黬��
	case SB_THUMBPOSITION: //������ڻ�����
		break;
	case SB_LINERIGHT: //���ҹ���һ�У�����������ҹ����ļ�ͷ�ϲ���
		{
			m_ptViewOrg.x += m_sizeLine.cx;
			m_ptViewOrg.x = min(m_ptViewOrg.x, nMaxTrackPos);
		}
		break;
	case SB_PAGERIGHT: //���ҹ���һҳ��������ڻ����ҷ��Ŀհ״�����
		{
			m_ptViewOrg.x += m_sizePage.cx;
			m_ptViewOrg.x = min(m_ptViewOrg.x, nMaxTrackPos);
		}
		break;
	case SB_RIGHT: //��������
		{
			m_ptViewOrg.x = nMaxTrackPos;
		}
		break;
	case SB_LINELEFT: //�������һ�У����������������ļ�ͷ�ϲ���
		{
			m_ptViewOrg.x -= m_sizeLine.cx;
			m_ptViewOrg.x = max(m_ptViewOrg.x, nMinTrackPos);
		}
		break;
	case SB_PAGELEFT: //�������һҳ��������ڻ����󷽵Ŀհ״�����
		{
			m_ptViewOrg.x -= m_sizePage.cx;
			m_ptViewOrg.x = max(m_ptViewOrg.x, nMinTrackPos);
		}
		break;
	case SB_LEFT: //��������
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
	case SB_THUMBTRACK: //��ס�������黬��
	case SB_THUMBPOSITION: //������ڻ�����
		break;
	case SB_LINEDOWN: //���ҹ���һ�У�����������ҹ����ļ�ͷ�ϲ���
		{
			m_ptViewOrg.y += m_sizeLine.cy;
			m_ptViewOrg.y = min(m_ptViewOrg.y, nMaxTrackPos);
		}
		break;
	case SB_PAGEDOWN: //���ҹ���һҳ��������ڻ����ҷ��Ŀհ״�����
		{
			m_ptViewOrg.y += m_sizePage.cy;
			m_ptViewOrg.y = min(m_ptViewOrg.y, nMaxTrackPos);
		}
		break;
	case SB_BOTTOM: //��������
		{
			m_ptViewOrg.y = nMaxTrackPos;
		}
		break;
	case SB_LINEUP: //�������һ�У����������������ļ�ͷ�ϲ���
		{
			m_ptViewOrg.y -= m_sizeLine.cy;
			m_ptViewOrg.y = max(m_ptViewOrg.y, nMinTrackPos);
		}
		break;
	case SB_PAGEUP: //�������һҳ��������ڻ����󷽵Ŀհ״�����
		{
			m_ptViewOrg.y -= m_sizePage.cy;
			m_ptViewOrg.y = max(m_ptViewOrg.y, nMinTrackPos);
		}
		break;
	case SB_TOP: //��������
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
	// ��Ч�����ű���
	if (nLvl < eZLSmall || nLvl > eZLLarge)
		return;

	// ��ǰ��ͼ�Ѿ��Ǵ˱�����ʾ��
	if (nLvl == CSwitchPage::m_sZoomLvl)
		return;

	CSwitchPage::m_sZoomLvl = (emZoomLevel)nLvl;

	theApp.WriteInt("ZoomLevel", nLvl);

	// ������ͼ
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

// ���������ļ�
LRESULT CSwitchPanelView::OnUnpackSceneFile(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszFile = (LPCTSTR)wParam;

	m_page.ParseFile(lpszFile);

	return 0;
}

// ��������ļ�
LRESULT CSwitchPanelView::OnPackageSceneFile(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszFile = (LPCTSTR)wParam;

	m_page.PackFile(lpszFile);

	return 0;
}

// �����ײ㷵�صĳ�������
LRESULT CSwitchPanelView::OnRecieveSceneInf(WPARAM wParam, LPARAM lParam)
{
	UINT* pBuff = (UINT*)wParam;
	int   nLen  = *(int*)lParam;

	m_page.ParseSceneBlock(pBuff, nLen);

	return 0;
}