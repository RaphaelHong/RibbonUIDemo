#pragma once
#include "..\ResolutionManager.h"

// CInputConfigDlg 对话框

class CInputConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputConfigDlg)

public:
	CInputConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInputConfigDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_PORT_CONFIG };

	CXTPComboBox m_CBoxHz;
	CXTPListCtrl m_listInput;
	CSliderCtrl m_SliderTiming;

	int m_nTimingIndex;

	CToolTipCtrl m_tips;
	BOOL SetTipText(UINT id, NMHDR * pTTTStruct, LRESULT * pRes) ;
	void SetTooltips();

	void InitInputList();
	void ShowSelectedTiming(int nTimingIndex);
	void ResetListItemColor();

private:
	CResolutionManager m_ResManager;
	void AddNewResolution(const TIMING &timing);
	void ModifyResolution(const TIMING &timing);
	void DeleteResolution(const TIMING &timing);
	int  GetFirstSelectedItemIndex();

	void InitChannelTiming();
	void ApplySetting();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBtnAll();
	afx_msg void OnBtnNone();
	afx_msg void OnBtnInvert();
	afx_msg void OnNMClickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnAddTiming();
	afx_msg void OnBnClickedBtnDelTiming();
	afx_msg void OnBnClickedBtnEditTiming();
	afx_msg void OnBnClickedBtnGetSlaveTiming();
	afx_msg void OnNMRClickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemChangedListOutput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnReset();
	afx_msg void OnBtnResetAll();
};
