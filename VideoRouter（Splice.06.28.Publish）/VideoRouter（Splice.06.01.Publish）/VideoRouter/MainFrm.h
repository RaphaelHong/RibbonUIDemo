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
//#include "DragTreeCtrl.h"
#include "Dialog components/MMListCtrl.h"
#include "Dialog components/MMTreeCtrl.h"
#include "WaittingMsg.h"

//The Category Index of the Ribbon Bar
#define RIBBON_CATEGORY_MAIN    0   //主功能面
#define RIBBON_CATEGORY_SET     1   //管理
#define RIBBON_CATEGORY_SYS     2   //系统设置

//The Panel Index of Category Main
//#define RIBBON_PANEL_MODE       0
//#define RIBBON_PANEL_CONNECT    1   //通信
//#define RIBBON_PANEL_SCENE      2   //场景
//#define RIBBON_PANEL_INPUT      3   //输入
//#define RIBBON_PANEL_OUTPUT     4   //输出
//#define RIBBON_PANEL_STATUS     5   //状态

//The Panel Index of Category Manager
//#define RIBBON_PANEL_BACKUP     0   //备份与还原
//#define RIBBON_PANEL_POWER      1   //电源
//#define RIBBON_PANEL_USER       2   //权限
//#define RIBBON_PANEL_LANG       3   //语言



class CMainFrame : public CFrameWndEx
{

protected: // 仅从序列化创建
	DECLARE_DYNCREATE(CMainFrame)
	CMainFrame();

// 属性
protected:
	//CSplitterWnd m_wndSplitter;
	CString m_strStylesPath;
	int m_nTimeCounter;

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // 控件条嵌入成员
	CMFCRibbonBar      m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar   m_wndStatusBar;
	COutlookBar       m_wndNavigationBar;
	//CMFCShellTreeCtrl m_wndTree;
	CMFCCaptionBar    m_wndCaptionBar;

	CMMTreeCtrl m_wndTreeSource;
	CMMTreeCtrl m_wndTreeScreen;
	CMMListCtrl m_wndListScene;
	CImageList m_imgTree;

	//重新设置场景有关的菜单项
	void ArrangeSceneMenus();

	// 树形控件的图标
	void InitImageList(); 
	// 重置信号源项
	void ResetSourceTreeItem(); 
	// 刷新信号源状态显示
	void ResetSourceState();
	// 重置输出口项
	void ResetScreenTreeItem(); 

	// 更新缩放信息
	void UpdateSliderZoomPos(double dZoomLvl);

public:
	void LoadScene(int nID); // 加载场景

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnViewTaskBar();
	afx_msg void OnUpdateViewTaskBar(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	void InitializeRibbonBar();
	void ResetRibbonBar();
	void SetRibbonAppButton();
	BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, CMMTreeCtrl& treesrc, CMMTreeCtrl& treescn,CMMListCtrl& list, int nInitialWidth);
	BOOL CreateCaptionBar();
	BOOL CreateStatusBar();

	CXTPTaskPanelGroup* CreateToolboxGroup(UINT nID, LPCTSTR lpGroupName);
	void ResetToolboxItems();

	int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);

	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;

public:
	CWaittingMsg m_WaitDlg;
	void ShowWaittingDlg(BOOL bShow, CStringW strMsg);
	void WaitOver(CStringW strMsg);

public:
	afx_msg void OnMenuConnect();
	afx_msg void OnMenuLoadScene();
	afx_msg void OnMenuSaveScene();
	afx_msg void OnMenuRefreshScene();
	afx_msg void OnMenuLoopScene();
	afx_msg void OnMenuManaScene();
	afx_msg void OnMenuInputSetup();
	afx_msg void OnMenuOutputSetup();
	afx_msg void OnMenuImageTest();
	afx_msg void OnMenuSysBackup();
	afx_msg void OnMenuSysRestore();
	afx_msg void OnMenuPowerControl();
	afx_msg void OnMenuUserManage();
	afx_msg void OnMenuLanguage();
	afx_msg void OnMenuRangeSceneID(UINT nID);
	afx_msg void OnClose();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnUpdateCmdLoop(CCmdUI *pCmdUI);
	afx_msg void OnCaptionBarOption();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCmdBtnZoom();
	afx_msg void OnCmdSliderZoom();
	afx_msg void OnCmdUserChange();
	afx_msg void OnCmdReset();
	afx_msg LRESULT OnTransactionComplete(WPARAM wParam, LPARAM lParam);
};


