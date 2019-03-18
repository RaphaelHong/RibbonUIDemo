#pragma once


// CHdmiTimingSetDlg �Ի���

class CHdmiTimingSetDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CHdmiTimingSetDlg)

public:
	CHdmiTimingSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHdmiTimingSetDlg();

// �Ի�������
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
	//���µ������Ƴߴ��λ��
	void ResizeAll();
	//�ؼ����
	void SetTheme(XTPControlTheme theme);
	//��ʼ��
	void Initialize();
	//��Ӿ�̬�ֱ���
	void AddTiming();
	//��ʾ��ϸ�ֱ���
	void ShowDetailTiming(pTIMING_STRUCT pTiming);
	//����ʱ����Ϣ
	void UpdatePixelClock();
	//�ֱ�����Ч���ж�
	bool ValidateTiming();
	//�Զ��ֱ����Ƿ��ԭ��������ظ�
	bool IsExistTiming();

public:
	//��ȡԤ����ֱ�����Ϣ
	pTIMING_STRUCT m_pTimings;
	CString *m_pszTiming;
	int m_nTotalTiming;

	//�Զ���ֱ���
	TIMING_STRUCT m_InTiming;
	TIMING_STRUCT m_OutTiming;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
