#pragma once

#define ADD_RESOLUTION  (0)
#define EDIT_RESOLUTION (1)
#define DEL_RESOLUTION  (2)

// CResolutionEditDlg 对话框

class CResolutionEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CResolutionEditDlg)

public:
	CResolutionEditDlg(CWnd* pParent = NULL);   // 标准构造函数
	CResolutionEditDlg(const TIMING &Timing, int nMode, CWnd *pParent = NULL);
	virtual ~CResolutionEditDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_TIMING_EDIT };

	//显示模式
	int m_nHActive;
	int m_nVActive;
	int m_nHz;

	//详细分辨率
	TIMING m_DetailTiming;

	CSpinButtonCtrl m_spHact;
	CSpinButtonCtrl m_spVact;
	CSpinButtonCtrl m_spHz;
	CSpinButtonCtrl m_spHact_detail;
	CSpinButtonCtrl m_spVact_detail;
	CSpinButtonCtrl m_spHfront_detail;
	CSpinButtonCtrl m_spVfront_detail;
	CSpinButtonCtrl m_spHsync_detail;
	CSpinButtonCtrl m_spVsync_detail;
	CSpinButtonCtrl m_spHtotal_detail;
	CSpinButtonCtrl m_spVtotal_detail;
	CSpinButtonCtrl m_spHz_detail;

public:
	BOOL m_bOrg; //对话框为初始状态, 此变量为防止编辑框DodataExchange出错
	int m_nMode; //Mode 0:添加分辨率模式 1:编辑分辨率模式
	void PreShowWindow();

	void UpdatePixClock();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEnChangedHActive();
	afx_msg void OnEnChangedVActive();
	afx_msg void OnEnChangedHz();
	afx_msg void OnEnChangedDetailHtotal();
	afx_msg void OnEnChangedDetailVtotal();
	afx_msg void OnOK();
	afx_msg void OnBnClickedBtnRestoreTiming();
};
