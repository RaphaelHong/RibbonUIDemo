#pragma once
#include "MviewDlg.h"
#include "MGeoMainDlg.h"
#include "MvxSplitDlg.h"
#include "Hdmi3dDlg.h"

// CMviewView 视图

class CContainerView : public CView
{
	DECLARE_DYNCREATE(CContainerView)

protected:
	CContainerView();           // 动态创建所使用的受保护的构造函数
	virtual ~CContainerView();

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	emProductType m_eType;
	CMviewDlg *m_pWndMV;
	CMGeoMainDlg *m_pWndMG;
	CMvxSplitDlg *m_pWndMVX;
	CHdmi3dDlg *m_pWndHDMI3D;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


