// TabbedView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "TabbedView.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// CTabbedViewDoc cpp
///////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNCREATE(CTabbedViewDoc, CDocument)

CTabbedViewDoc::CTabbedViewDoc()
{
}

BOOL CTabbedViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CTabbedViewDoc::~CTabbedViewDoc()
{
}


BEGIN_MESSAGE_MAP(CTabbedViewDoc, CDocument)
END_MESSAGE_MAP()


// CTabbedViewDoc ���

#ifdef _DEBUG
void CTabbedViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CTabbedViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CTabbedViewDoc ���л�

void CTabbedViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}
#endif


// CTabbedViewDoc ����


///////////////////////////////////////////////////////////////////////////////////////////////
// CSafeView
///////////////////////////////////////////////////////////////////////////////////////////////
class CSafeView : public CView
{
	DECLARE_DYNCREATE(CSafeView)

protected:
	CSafeView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSafeView();

public:
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////
// SafeView.cpp : ʵ���ļ�

// CSafeView

IMPLEMENT_DYNCREATE(CSafeView, CView)

CSafeView::CSafeView()
{

}

CSafeView::~CSafeView()
{
}

BEGIN_MESSAGE_MAP(CSafeView, CView)
END_MESSAGE_MAP()


// CSafeView ��ͼ

void CSafeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// CSafeView ���

#ifdef _DEBUG
void CSafeView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSafeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSafeView ��Ϣ�������


///////////////////////////////////////////////////////////////////////////////////////////////
// CTabbedView
///////////////////////////////////////////////////////////////////////////////////////////////

CSafeView *pSafeView = NULL; //��Tabҳ��Ϊ0��ʱ�򣬽�������ӽ���ָʾ������ͼ
#define IDC_MAIN_TABCONTROL 100

// CTabbedView

IMPLEMENT_DYNCREATE(CTabbedView, CView)

CTabbedView::CTabbedView()
{

}

CTabbedView::~CTabbedView()
{
}

BEGIN_MESSAGE_MAP(CTabbedView, CView)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TABCONTROL, OnSelectedChanged)
END_MESSAGE_MAP()


// CTabbedView ��ͼ

void CTabbedView::OnDraw(CDC* pDC)
{
}


// CTabbedView ���

#ifdef _DEBUG
void CTabbedView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTabbedView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

CTabbedViewDoc* CTabbedView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTabbedViewDoc)));
	return (CTabbedViewDoc*)m_pDocument;
}

// CTabbedView ��Ϣ�������

BOOL CTabbedView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (!CView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	return TRUE;

	return CView::PreCreateWindow(cs);
}

int CTabbedView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_wndTabControl.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_MAIN_TABCONTROL);
	m_wndTabControl.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003);
	m_wndTabControl.GetPaintManager()->DisableLunaColors(FALSE);
	m_wndTabControl.GetPaintManager()->SetOneNoteColors(TRUE);
	m_wndTabControl.GetPaintManager()->m_bHotTracking = TRUE;
	m_wndTabControl.GetPaintManager()->m_bShowIcons = TRUE;	
	m_wndTabControl.GetPaintManager()->SetPosition(xtpTabPositionBottom);	
	m_wndTabControl.GetPaintManager()->m_bBoldSelected = TRUE;
	m_wndTabControl.GetImageManager()->SetIcons(IDB_TAB_ICON, NULL, 0, CSize(16, 16), xtpImageNormal);

	CFrameWnd *pFrame = (CFrameWnd*)AfxGetMainWnd();
	CCreateContext ctxt;
	ctxt.m_pNewViewClass = RUNTIME_CLASS(CSafeView);
	pSafeView = (CSafeView *)pFrame->CreateView(&ctxt);
	ASSERT(pSafeView);

	return 0;
}

BOOL CTabbedView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

void CTabbedView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CView::OnPaint()
}

void CTabbedView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_wndTabControl.GetSafeHwnd())
	{
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
	}
}

BOOL CTabbedView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
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

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW ;
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

int CTabbedView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}


void CTabbedView::OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);
	*pResult = 0;

	SetActiveView(m_wndTabControl.GetCurSel());
}

void CTabbedView::SetActiveView(int nTabIndex)
{
	UpdateDocTitle();

	m_wndTabControl.SetCurSel(nTabIndex);

	CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(nTabIndex)->GetHandle()));
	ASSERT_KINDOF(CView, pView);

	pFrame->SetActiveView(pView);
}

void CTabbedView::UpdateDocTitle()
{
	GetDocument()->UpdateFrameCounts();
}

void CTabbedView::DeleteAllViews()
{
	CFrameWnd *pFrame = (CFrameWnd*)AfxGetMainWnd();
	pFrame->SetActiveView(pSafeView);

	CXTPTabManagerItem *pTabItem;
	CWnd *pWnd;
	for (int i=0; i<m_wndTabControl.GetItemCount(); i++) 
	{
		pTabItem = m_wndTabControl.GetItem(i);
		pWnd = FromHandle(pTabItem->GetHandle());
		if (pWnd != NULL) pWnd->DestroyWindow();

		DeleteObject(pWnd);

	}

	m_wndTabControl.DeleteAllItems();
	UpdateDocTitle();
}

int CTabbedView::GetViewCount()
{
	CString strItemText = m_wndTabControl.GetItemCaption(m_wndTabControl.GetItem(0));
	return m_wndTabControl.GetItemCount();
}

int CTabbedView::GetTabIndexByName(CString strTabName)
{
	int TabIndex = -1;
	for (int i=0; i<m_wndTabControl.GetItemCount(); i++)
	{
		if (m_wndTabControl.GetItemCaption(m_wndTabControl.GetItem(i)) == strTabName)
		{
			TabIndex = i;
			break;
		}
	}

	return TabIndex;
}

CView *CTabbedView::GetViewByIndex(int nTabIndex)
{
	if ((nTabIndex >= m_wndTabControl.GetItemCount()) || (nTabIndex < 0))  return NULL;

	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(nTabIndex)->GetHandle()));
	return pView;
}