#pragma once
//#include "SceneListCtrl.h"
#define SCENE_MANAGER_SAVE  0 //保存场景
#define SCENE_MANAGER_LOAD  1 //加载场景
#define SCENE_MANAGER_EDIT  2 //编辑场景

// CSceneManagerDlg 对话框

class CSceneManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CSceneManagerDlg)

public:
	CSceneManagerDlg(CWnd* pParent = NULL);   // 标准构造函数
	CSceneManagerDlg(int nManagerType, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSceneManagerDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_SCENE_MANAGER };

	CButton m_checkQuickMenu;
	CImageList m_imglist16;
	CImageList m_imglist32;
	CComboBoxEx m_ExCBoxIcon;
	CXTPListCtrl m_ListScene;	
	CXTPHeaderCtrl header;
	CSpinButtonCtrl m_SpinID;
	CToolTipCtrl m_tip;

	int m_nSceneID;
	CString m_strSaveName;

	void InitSceneList();
	void UpdateListItem(int nFousedItem);
	void InitIconBox();
	int GetCursorItem();
	void ShowItemInfo(int nItem);
	void InitToolTip();

public:
	void SaveScene();
	void LoadScene();
	void EditScene();
	void GetUnusedSceneID();

private:
	int m_nManagerType; //0：保存场景 1：加载场景 2：编辑场景
	void PreShowWindow();
	void ShowFuncCtrls();
	void ResizeClientRect();
	void SetWindowTitle();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCboxexSceneIcon();
	afx_msg void OnEnChangedSceneNewName();
	afx_msg void OnLvnItemchangedListScene(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckQuickMenu();
	afx_msg void OnBnClickedBtnSceneDelete();
	afx_msg void OnBnClickedOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnSceneLoadsave();
	afx_msg void OnNMDblclkListScene(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnSceneLocalfile();
};
