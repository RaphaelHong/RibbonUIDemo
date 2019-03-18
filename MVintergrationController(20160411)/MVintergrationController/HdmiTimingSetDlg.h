#pragma once


// CHdmiTimingSetDlg 对话框

class CHdmiTimingSetDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CHdmiTimingSetDlg)

public:
	CHdmiTimingSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHdmiTimingSetDlg();

// 对话框数据
	enum { IDD = IDD_DLG_HDMI_TIMING };

	CXTPButton m_GboxInput;
	CXTPButton m_GboxOutput;
	CXTPButton m_BtnCustomIn;
	CXTPButton m_BtnCustomOut;
	CXTPButton m_BtnSetInput;
	CXTPButton m_BtnSetOutput;
	CXTPComboBox m_CboxInputTiming;
	CXTPComboBox m_CboxHPol;
	CXTPComboBox m_CboxVPol;
	CXTPComboBox m_CboxOutputTiming;
	CXTPSpinButtonCtrl m_SpinHz;
	CEdit m_EditHz;

	CBrush m_bkBrush;
public:
	int m_nHact;
	int m_nVact;
	int m_nHfront;
	int m_nVfront;
	int m_nHsync;
	int m_nVsync;
	int m_nHtotal;
	int m_nVtotal;
	int m_nHpol;
	int m_nVpol;
	int m_nHz;

public:
	//重新调整控制尺寸和位置
	void ResizeAll();
	//控件风格
	void SetTheme(XTPControlTheme theme);
	//初始化
	void Initialize();
	//添加静态分辨率
	void AddTiming();
	//显示详细分辨率
	void ShowDetailTiming(pTIMING_STRUCT pTiming);
	//更新时钟信息
	void UpdatePixelClock();
	//分辨率有效性判断
	bool ValidateTiming();
	//自定分辨率是否和原定义的有重复
	bool IsExistTiming();

public:
	//获取预定义分辨率信息
	pTIMING_STRUCT m_pTimings;
	CString *m_pszTiming;
	int m_nTotalTiming;

	//自定义分辨率
	TIMING_STRUCT m_InTiming;
	TIMING_STRUCT m_OutTiming;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditHdmiHtotal();
	afx_msg void OnEnChangeEditHdmiVtotal();
	afx_msg void OnEnChangeEditHdmiHz();
	afx_msg void OnDeltaposSpinHdmiHz(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnCustomInTiming();
	afx_msg void OnBnClickedBtnCustomOutTiming();
	afx_msg void OnCbnSelchangeCboxHdmiInputtiming();
	afx_msg void OnCbnSelchangeCboxHdmiOutputtiming();
	afx_msg void OnBnClickedBtnHdmiSetInputtiming();
	afx_msg void OnBnClickedBtnHdmiSetOutputtiming();
};
