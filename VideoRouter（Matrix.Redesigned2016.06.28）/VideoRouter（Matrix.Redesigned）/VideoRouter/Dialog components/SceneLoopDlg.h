#pragma once
#include "NumberEdit.h"

// CSceneLoopDlg 对话框

class CSceneLoopDlg : public CDialog
{
	DECLARE_DYNAMIC(CSceneLoopDlg)

public:
	CSceneLoopDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSceneLoopDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_SCENE_LOOP };

	CXTPListCtrl m_Listscene;
	CImageList m_imglist16;

	CSpinButtonCtrl m_spinH;
	CSpinButtonCtrl m_spinM;
	CSpinButtonCtrl m_spinS;

	CNumberEdit m_editHour;
	CNumberEdit m_editMinute;
	CNumberEdit m_editSecond;

	int m_nHour;
	int m_nMinute;
	int m_nSecond;

private:
	void InitListContent();
	void UpdateListItem(int nFocusedItem);
	int GetSelectItem() const;

	void SwapListItem(int nItemSrc, int nItemDest);
	void MoveItemTo(int nItem, int nPos);

	void UpdateLoopItem();

	void UpdateRemainTime();

	void EnableTimeEditor(BOOL bEnable);

	void UpdateExcuteTime();

	BOOL LoopRemind();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAll();
	afx_msg void OnBtnNon();
	afx_msg void OnBtnInv();
	afx_msg void OnBtnMoveItemUp();
	afx_msg void OnBtnMoveItemDown();
	afx_msg void OnBtnMoveItemTop();
	afx_msg void OnBtnMoveItemBottom();
	afx_msg void OnNMClickListItem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemChangeList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOK();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditHour();
	afx_msg void OnEnChangeEditMinute();
	afx_msg void OnEnChangeEditSecond();
	afx_msg void OnClose();
	afx_msg void OnCancel();
};
