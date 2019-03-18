#pragma once

#define XTP_BUTTON_SETTIMING  123
// CMGtimingsetDlg 对话框

class CMGtimingsetDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGtimingsetDlg)

public:
	CMGtimingsetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMGtimingsetDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MG_TIMING };

	CXTPButton m_btnSet;
	CXTPComboBox m_cmbMode;
	CXTPComboBox m_cmbTiming;
	CXTPEdit m_editTimingInfo;

	CXTPPropertyGrid m_wndPropertyGrid;

	TIMING_STRUCT m_curTiming;

	CString *m_pstrTming;
	TIMING_STRUCT *m_pDetailedTiming;
	int m_nTotalTiming;

	void Initialize();

private:
	void SetControlsTheme(XTPControlTheme theme);
	int SetTiming(int nIndex);
	void AddTimingToList();

	CBrush m_bkBrush;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);
};
