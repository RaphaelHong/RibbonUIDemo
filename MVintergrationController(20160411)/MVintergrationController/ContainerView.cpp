// MviewView.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "ContainerView.h"

// CMviewView

IMPLEMENT_DYNCREATE(CContainerView, CView)

CContainerView::CContainerView()
{
	m_eType  = Product_INVALID;

	m_pWndMG     = NULL;
	m_pWndMV     = NULL;
	m_pWndMVX    = NULL;
	m_pWndHDMI3D = NULL;
}

CContainerView::~CContainerView()
{
	if (m_pWndMG != NULL)
	{
		delete m_pWndMG;
		m_pWndMG = NULL;
	}

	if (m_pWndMV != NULL)
	{
		delete m_pWndMV;
		m_pWndMV = NULL;
	}

	if (m_pWndMVX != NULL)
	{
		delete m_pWndMVX;
		m_pWndMVX = NULL;
	}

	if (m_pWndHDMI3D != NULL)
	{
		delete m_pWndHDMI3D;
		m_pWndHDMI3D = NULL;
	}
}

BEGIN_MESSAGE_MAP(CContainerView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMviewView 绘图

void CContainerView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// CMviewView 诊断

#ifdef _DEBUG
void CContainerView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CContainerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMviewView 消息处理程序

int CContainerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	theApp.GlobleLock();
	m_eType = theApp.m_CurrentProductType;
	theApp.GlobleUnlock();

	if (m_eType == Product_MV)
	{
		if (m_pWndMV == NULL)
		{
			m_pWndMV = new CMviewDlg;
			m_pWndMV->Create(IDD_DLG_MV, this);
			m_pWndMV->MoveWindow(0,0,0,0);
			m_pWndMV->ShowWindow(SW_SHOW);
			m_pWndMV->SetFocus();
		}
	}
	else if (m_eType == Product_MG)
	{
		if (m_pWndMG == NULL)
		{
			m_pWndMG = new CMGeoMainDlg;
			m_pWndMG->Create(IDD_DLG_MG, this);
			m_pWndMG->MoveWindow(0,0,0,0);
			m_pWndMG->ShowWindow(SW_SHOW);
			m_pWndMG->SetFocus();
		}
	}
	else if (m_eType == Product_MVX)
	{
		if (m_pWndMVX == NULL)
		{
			m_pWndMVX = new CMvxSplitDlg;
			m_pWndMVX->Create(IDD_DLG_MVX_MAIN, this);
			m_pWndMVX->MoveWindow(0,0,0,0);
			m_pWndMVX->ShowWindow(SW_SHOW);
			m_pWndMVX->SetFocus();
		}
	}
	else if (m_eType == Product_HDMI_3D)
	{
		if (m_pWndHDMI3D == NULL)
		{
			m_pWndHDMI3D = new CHdmi3dDlg;
			m_pWndHDMI3D->Create(IDD_DLG_HDMI3D, this);
			m_pWndHDMI3D->MoveWindow(0,0,0,0);
			m_pWndHDMI3D->ShowWindow(SW_SHOW);
			m_pWndHDMI3D->SetFocus();
		}
	}

	return 0;
}

void CContainerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (m_eType == Product_MV)           m_pWndMV->MoveWindow(0,  0, cx, cy);
	else if (m_eType == Product_MG)      m_pWndMG->MoveWindow(0,  0, cx, cy);
	else if (m_eType == Product_MVX)     m_pWndMVX->MoveWindow(0, 0, cx, cy);
	else if (m_eType == Product_HDMI_3D) m_pWndHDMI3D->MoveWindow(0, 0, cx, cy);
}

BOOL CContainerView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}
