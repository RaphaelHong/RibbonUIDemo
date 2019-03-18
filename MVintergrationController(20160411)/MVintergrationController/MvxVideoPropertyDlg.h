#pragma once


// CMvxVideoPropertyDlg 对话框

class CMvxVideoPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CMvxVideoPropertyDlg)

public:
	CMvxVideoPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMvxVideoPropertyDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MVX_VIDEOPROPERTY };

	CXTPComboBox m_CboxOutputTiming;
	CXTPComboBox m_CboxHPol;
	CXTPComboBox m_CboxVPol;
	
	CXTPButton m_BtnCustom;
	CXTPButton m_BtnSetTiming;
	CXTPButton m_BtnCancel;

	CXTPSpinButtonCtrl m_SpinHz;

	int m_nHActive;
	int m_nHTotal;
	int m_nHFront;
	int m_nHSync;
	int m_nHPol;
	int m_nVActive;
	int m_nVTotal;
	int m_nVFront;
	int m_nVSync;
	int m_nVPol;
	int m_nHz;

	void SetThemes(XTPControlTheme theme);

	void AddTimings();

	void ShowDetailedTiming();
	void UpdatePixelClock();

private:
	CString *m_pstrTiming;
	TIMING_STRUCT *m_pDetailedTiming;
	int m_nTotalTiming;

	TIMING_STRUCT m_SelectTiming;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelChangeTiming();
	afx_msg void OnEnChangedEditHTotal();
	afx_msg void OnEnChangedEditVTotal();
	afx_msg void OnEnChangedEditHz();
	afx_msg void OnDeltaPosSpinHZ(NMHDR *pNMHDR, LRESULT *pResult );
	afx_msg void OnBtnClickedSetOutTiming();
	afx_msg void OnBnClickedBtnMvxVideoCustom();
};
