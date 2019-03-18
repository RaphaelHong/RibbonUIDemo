// MVintergrationControllerDlg.h : header file
//
#pragma once

#include "SelectTreeCtrl.h"
#include "StateTreeCtrl.h"
#include "TabbedView.h"
#include "BaseBoard.h"
#include "ProgressBar.h"


// CMVintergrationControllerDlg dialog
class CMVintergrationControllerDlg : public CXTPResizeDialog
{
// Construction
public:
	CMVintergrationControllerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	enum { IDD = IDD_DLG_MAINFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CImageList m_imageList;
	CBrush m_BkBrush;
	CBitmap m_BkBitmap;

	CXTPButton m_btnOK;
	CXTPButton m_btnCancel;
	CXTPButton m_btnToolCfg;
	CXTPButton m_btnToolRefresh;
	CXTPButton m_btnToolReset;
	CXTPButton m_btnToolUpdate;
	CXTPButton m_btnToolFive;
	CXTPButton m_btnConnectConfig;
	CXTPButton m_btnHidden;

	void InitTreeItem(); //初始化树形控件
	void SetTreeItems(); //将读取到的板卡显示到树形控件中
	void SetCtrTheme(XTPControlTheme theme);
	void SetIcons();
	void CreateToolTips();

public:
	CArray <CBaseBoard, CBaseBoard&> m_BoardManager; //板卡信息列表
	void GetAllBoardInfo();                 //从设备读取所有板卡信息(板卡数、端口号、功能码等)
	int ExistSameProductType(int nBoardID); //返回相同型号板卡个数

	CWinThread m_AjustThread;

public:
	CStateTreeCtrl m_treeState;

	CTabbedView *m_pTabbedView;
	CTabbedViewDoc *m_pTabbedDoc;

	BOOL CreateView();                    //创建TabbedView
	void SetConnectionStatus();           //更新连接状态
	void SetLightOn(BOOL bOnOFF=TRUE);    //设置指示灯
	int GetTreeItemIndex(HTREEITEM hti);  //获取树形控件某个子项的序号

	CToolTipCtrl m_tipCtrl;

private:
	BOOL m_bLightOn;  //通信指示灯
	BOOL m_bHideLeft; //隐藏左边区域

	void HideLeft(BOOL bHide=TRUE); //隐藏主对话框左边区域
	void ExpandFuncArea(BOOL bExpand=FALSE);    //动态缩放右边功能区
	void ResizeView();

	void EnglishSupport();      //英文版本

	void FadeInPopUpWindow();   //渐现式启动
	void FadeOutCloseWindow();  //渐隐式退出

	void FriendlyShutDown();  //关闭程序提醒用户保存数据

	void CreateNewBoardView(emProductType eboardtype, int nBoardIndex);//创建新的板卡控制窗口

// Implementation
protected:
	HICON m_hIcon;
	int m_nAlpha; //透明度

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMDblclkTreeBoard(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedToolbtnCfg();
	afx_msg void OnBnClickedToolbtnRefresh();
	afx_msg void OnBnClickedToolbtnReset();
	afx_msg void OnBnClickedHiddenLeftDlg();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedToolbtnUpdate();
	afx_msg void OnBnClickedToolbtnFive();
};
