#pragma once

////////////////////////////////////////////////////////////////////////////////////////
// CTabbedViewDoc �ĵ�

class CTabbedViewDoc : public CDocument
{
	DECLARE_DYNCREATE(CTabbedViewDoc)

public:
	CTabbedViewDoc();
	virtual ~CTabbedViewDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};



//////////////////////////////////////////////////////////////////////////////////////////
// CTabbedView ��ͼ

class CTabbedView : public CView
{
	DECLARE_DYNCREATE(CTabbedView)

protected:
	CTabbedView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CTabbedView();

private:
	CTabbedViewDoc * GetDocument();
	CXTPTabControl m_wndTabControl;
	void UpdateDocTitle();

public:
	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon); //�����ͼ
	void DeleteAllViews();  //ɾ��������ͼ
	int GetViewCount();	 //��ȡ��ǰ������ͼ��
	CView *GetViewByIndex(int nTabIndex); //��ȡָ����ͼָ��
	int GetTabIndexByName(CString strTabName); //����Tab��ǩҳ���ƻ�ȡ����ͼ��Tab��λ��
	void SetActiveView(int nTabIndex); //����ָ����ͼ

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
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);
};


