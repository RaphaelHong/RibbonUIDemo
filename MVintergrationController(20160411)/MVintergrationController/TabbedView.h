#pragma once

////////////////////////////////////////////////////////////////////////////////////////
// CTabbedViewDoc 文档

class CTabbedViewDoc : public CDocument
{
	DECLARE_DYNCREATE(CTabbedViewDoc)

public:
	CTabbedViewDoc();
	virtual ~CTabbedViewDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
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
// CTabbedView 视图

class CTabbedView : public CView
{
	DECLARE_DYNCREATE(CTabbedView)

protected:
	CTabbedView();           // 动态创建所使用的受保护的构造函数
	virtual ~CTabbedView();

private:
	CTabbedViewDoc * GetDocument();
	CXTPTabControl m_wndTabControl;
	void UpdateDocTitle();

public:
	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon); //添加视图
	void DeleteAllViews();  //删除所有视图
	int GetViewCount();	 //获取当前所有视图数
	CView *GetViewByIndex(int nTabIndex); //获取指定视图指针
	int GetTabIndexByName(CString strTabName); //根据Tab标签页名称获取该视图的Tab项位置
	void SetActiveView(int nTabIndex); //激活指定视图

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
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


