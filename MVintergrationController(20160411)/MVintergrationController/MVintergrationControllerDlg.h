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

	void InitTreeItem(); //��ʼ�����οؼ�
	void SetTreeItems(); //����ȡ���İ忨��ʾ�����οؼ���
	void SetCtrTheme(XTPControlTheme theme);
	void SetIcons();
	void CreateToolTips();

public:
	CArray <CBaseBoard, CBaseBoard&> m_BoardManager; //�忨��Ϣ�б�
	void GetAllBoardInfo();                 //���豸��ȡ���а忨��Ϣ(�忨�����˿ںš��������)
	int ExistSameProductType(int nBoardID); //������ͬ�ͺŰ忨����

	CWinThread m_AjustThread;

public:
	CStateTreeCtrl m_treeState;

	CTabbedView *m_pTabbedView;
	CTabbedViewDoc *m_pTabbedDoc;

	BOOL CreateView();                    //����TabbedView
	void SetConnectionStatus();           //��������״̬
	void SetLightOn(BOOL bOnOFF=TRUE);    //����ָʾ��
	int GetTreeItemIndex(HTREEITEM hti);  //��ȡ���οؼ�ĳ����������

	CToolTipCtrl m_tipCtrl;

private:
	BOOL m_bLightOn;  //ͨ��ָʾ��
	BOOL m_bHideLeft; //�����������

	void HideLeft(BOOL bHide=TRUE); //�������Ի����������
	void ExpandFuncArea(BOOL bExpand=FALSE);    //��̬�����ұ߹�����
	void ResizeView();

	void EnglishSupport();      //Ӣ�İ汾

	void FadeInPopUpWindow();   //����ʽ����
	void FadeOutCloseWindow();  //����ʽ�˳�

	void FriendlyShutDown();  //�رճ��������û���������

	void CreateNewBoardView(emProductType eboardtype, int nBoardIndex);//�����µİ忨���ƴ���

// Implementation
protected:
	HICON m_hIcon;
	int m_nAlpha; //͸����

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
