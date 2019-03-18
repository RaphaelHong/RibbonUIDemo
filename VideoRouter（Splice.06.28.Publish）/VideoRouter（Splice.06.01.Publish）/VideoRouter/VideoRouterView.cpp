// MainView.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "VideoRouterDoc.h"
#include "VideoRouterView.h"
//#include "QuickSwitchView.h"

#define IDC_MAINTABCONTRL 100
// CVideoRouterView

IMPLEMENT_DYNCREATE(CVideoRouterView, CView)

CVideoRouterView::CVideoRouterView()
{

}

CVideoRouterView::~CVideoRouterView()
{
}

BEGIN_MESSAGE_MAP(CVideoRouterView, CView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAINTABCONTRL, OnSelectedChanged)
END_MESSAGE_MAP()


// CVideoRouterView 绘图

void CVideoRouterView::OnDraw(CDC* pDC)
{

}


// CVideoRouterView 诊断

#ifdef _DEBUG
void CVideoRouterView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CVideoRouterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif

CVideoRouterDoc* CVideoRouterView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVideoRouterDoc)));

	return (CVideoRouterDoc*)m_pDocument;
}

#endif //_DEBUG


// CVideoRouterView 消息处理程序

BOOL CVideoRouterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_HSCROLL;
	cs.style &= ~WS_VSCROLL;

	return CView::PreCreateWindow(cs);
}

int CVideoRouterView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	m_wndTabControl.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_MAINTABCONTRL);
	//m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003);
	m_wndTabControl.GetPaintManager()->m_bHotTracking = TRUE;
	m_wndTabControl.GetPaintManager()->m_bShowIcons = TRUE;
	//m_wndTabControl.GetPaintManager()->DisableLunaColors(FALSE);
	//m_wndTabControl.GetPaintManager()->SetOneNoteColors(TRUE);
	m_wndTabControl.GetPaintManager()->SetPosition(xtpTabPositionBottom);
	m_wndTabControl.GetImageManager()->SetIcons(IDB_BUTTONS, NULL, 0, CSize(16, 16), xtpImageNormal);

	//AddView(RUNTIME_CLASS(CQuickSwitchView), _T("快速切换器"), 0);
	//AddView(RUNTIME_CLASS(CQuickSwitchView), _T("Panel Model"), 1);

	return 0;
}

void CVideoRouterView::UpdateDocTitle()
{
	GetDocument()->UpdateFrameCounts();
}

BOOL CVideoRouterView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
{
	CCreateContext contextT;
	contextT.m_pCurrentDoc     = GetDocument();
	contextT.m_pNewViewClass   = pViewClass;
	contextT.m_pNewDocTemplate = GetDocument()->GetDocTemplate();

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
		{
			AfxThrowMemoryException();
		}
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" );
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

		DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	dwStyle &= ~WS_BORDER;

	int nTab = m_wndTabControl.GetItemCount();

	// Create with the right size (wrong position)
	CRect rect(0,0,0,0);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, &m_wndTabControl, (AFX_IDW_PANE_FIRST + nTab), &contextT))
	{
		TRACE0( "Warning: couldn't create client tab for view.\n" );
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}
	m_wndTabControl.InsertItem(nTab, lpszTitle, pWnd->GetSafeHwnd(), nIcon);

	pWnd->SetOwner(this);

	return TRUE;

}
BOOL CVideoRouterView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

void CVideoRouterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_wndTabControl.GetSafeHwnd())
	{
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
	}
}

void CVideoRouterView::OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);
	*pResult = 0;

	UpdateDocTitle();

	CFrameWnd* pFrame = GetParentFrame();
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
	ASSERT_KINDOF(CView, pView);

	pFrame->SetActiveView(pView);
}

int CVideoRouterView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CVideoRouterView::SetViewCaption(int iItem, CString strCaption)
{
	int nItemCount = m_wndTabControl.GetItemCount();
	if (iItem >= nItemCount || iItem < 0)
		return;

	m_wndTabControl.GetItem(iItem)->SetCaption(strCaption);
}

int CVideoRouterView::GetViewCount() const
{
	return m_wndTabControl.GetItemCount();
}

CView *CVideoRouterView::GetView(int nTabItem) const
{
	int nItemCount = m_wndTabControl.GetItemCount();
	if (nTabItem >= nItemCount || nTabItem < 0)
		return NULL;

	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(nTabItem)->GetHandle()));
	ASSERT_KINDOF(CView, pView);

	return pView;

}