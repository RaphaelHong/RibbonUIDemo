#pragma once

#define ADD_RESOLUTION  (0)
#define EDIT_RESOLUTION (1)
#define DEL_RESOLUTION  (2)

// CResolutionEditDlg �Ի���

class CResolutionEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CResolutionEditDlg)

public:
	CResolutionEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CResolutionEditDlg(const TIMING &Timing, int nMode, CWnd *pParent = NULL);
	virtual ~CResolutionEditDlg();

	// �Ի�������
	enum { IDD = IDD_DLG_TIMING_EDIT };

	//��ʾģʽ
	int m_nHActive;
	int m_nVActive;
	int m_nHz;

	//��ϸ�ֱ���
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
	BOOL m_bOrg; //�Ի���Ϊ��ʼ״̬, �˱���Ϊ��ֹ�༭��DodataExchange����
	int m_nMode; //Mode 0:��ӷֱ���ģʽ 1:�༭�ֱ���ģʽ
	void PreShowWindow();

	void UpdatePixClock();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
