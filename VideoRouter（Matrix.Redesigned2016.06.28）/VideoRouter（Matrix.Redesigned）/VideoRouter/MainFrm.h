// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// ("Fluent UI")，该示例仅作为参考资料提供， 
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有 (C) Microsoft Corporation
// 保留所有权利。

// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "Resource.h"
#include "MyRibbonControls.h"
#include "WaittingMsg.h"

//The Category Index of the Ribbon Bar
#define RIBBON_CATEGORY_MAIN    0   //主功能面
#define RIBBON_CATEGORY_MANAGER 1   //管理

//The Panel Index of Category Main
#define RIBBON_PANEL_MODE       0
#define RIBBON_PANEL_CONNECT    1   //通信
#define RIBBON_PANEL_SCENE      2   //场景
#define RIBBON_PANEL_INPUT      3   //输入
#define RIBBON_PANEL_OUTPUT     4   //输出
#define RIBBON_PANEL_STATUS     5   //状态

//The Panel Index of Category Manager
#define RIBBON_PANEL_BACKUP     0   //备份与还原
#define RIBBON_PANEL_POWER      1   //电源
#define RIBBON_PANEL_USER       2   //权限
#define RIBBON_PANEL_LANG       3   //语言



class CMainFrame : public CFrameWndEx
{

protected: // 仅从序列化创建
	DECLARE_DYNCREATE(CMainFrame)
	CMainFrame();

// 属性
protected:
	//CSplitterWnd m_wndSplitter;
	int m_nTimeoutCount;

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // 控件条嵌入成员
	CMyRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	//COutlookBar       m_wndNavigationBar;
	//CMFCShellTreeCtrl m_wndTree;
	//CCalendarBar      m_wndCalendar;
	CMFCCaptionBar    m_wndCaptionBar;

	//重新设置场景有关的菜单项
	void ArrangeSceneMenus();

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	void SetRibbonBar();
	void SetRibbonAppButton();
	//BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMFCShellTreeCtrl& tree, CCalendarBar& calendar, int nInitialWidth);
	BOOL CreateCaptionBar();
	BOOL CreateStatusBar();

public:
	CWaittingMsg *m_pWaittingDlg;
	CWaittingMsg m_WaitDlg;
	void ShowWaittingDlg(BOOL bShow, CStringW strMsg);
	void WaitOver(CStringW strMsg);

public:
	afx_msg void OnCmdConnect();
	afx_msg void OnCmdLoadScene();
	afx_msg void OnCmdSaveScene();
	afx_msg void OnCmdRefreshScene();
	afx_msg void OnCmdLoopScene();
	afx_msg void OnCmdManaScene();
	afx_msg void OnCmdInputSetup();
	afx_msg void OnCmdOutputSetup();
	afx_msg void OnCmdOutputTest();
	afx_msg void OnCmdSysBackup();
	afx_msg void OnCmdSysRestore();
	afx_msg void OnCmdPowerControl();
	afx_msg void OnCmdUserManage();
	afx_msg void OnCmdLanguage();
	afx_msg void OnCmdRangeSceneID(UINT nID);
	afx_msg void OnClose();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnCmdMatrixMode();
	afx_msg void OnUpdateCmdRangeScene(CCmdUI *pCmdUI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSliderZoom();
	afx_msg LRESULT OnTransactionComplete(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCmdUserChange();
	afx_msg void OnCmdReset();
	afx_msg void OnUpdateCmdBackup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCmdRestore(CCmdUI* pCmdUI);
};


