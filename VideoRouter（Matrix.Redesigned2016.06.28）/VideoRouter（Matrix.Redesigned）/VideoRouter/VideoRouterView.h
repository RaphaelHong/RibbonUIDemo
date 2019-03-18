#pragma once

class CVideoRouterDoc;
// CVideoRouterView ��ͼ

class CVideoRouterView : public CView
{
	DECLARE_DYNCREATE(CVideoRouterView)

protected:
	CVideoRouterView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CVideoRouterView();

	// Attributes
public:
	CVideoRouterDoc* GetDocument() const;

	CXTPTabControl m_wndTabControl;

	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon);
	void UpdateDocTitle();
	int GetViewCount() const;
	void SetViewCaption(int iItem, CString strCaption);
	CView *GetView(int nTabItem) const;

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
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};

#ifndef _DEBUG  // VedioRouterView.cpp �еĵ��԰汾
inline CVideoRouterDoc* CVideoRouterView::GetDocument() const
{ return reinterpret_cast<CVideoRouterDoc*>(m_pDocument); }
#endif