#pragma once


// CMvxBackgrdSettingDlg �Ի���

class CMvxBackgrdSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CMvxBackgrdSettingDlg)

public:
	CMvxBackgrdSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMvxBackgrdSettingDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MVX_BKGSETTING };

	CXTPComboBox m_CboxBkgTiming;
	CXTPComboBox m_CboxHPol;
	CXTPComboBox m_CboxVPol;
	CXTPComboBox m_CboxSplitMode;

	CXTPButton m_BtnCustomTiming;
	CXTPButton m_BtnSetBkgTiming;
	CXTPButton m_BtnSetCombiMode;
	CXTPButton m_BtnCancel;

	CXTPSpinButtonCtrl m_SpinFreshRate;

	CXTPEdit m_EditHz;

	//Timing refers
	int m_nHActive;
	int m_nHFront;
	int m_nHSync;
	int m_nHPol;
	int m_nHTotal;

	int m_nVActive;
	int m_nVFront;
	int m_nVSync;
	int m_nVPol;
	int m_nVTotal;

	int m_nHZ;

	//Combinations
	int m_nHPhyScreen;
	int m_nVPhyScreen;
	int m_nHLogScreen;
	int m_nVLogScreen;

private:
	CString *m_pStrTiming;
	TIMING_STRUCT *m_pDetailedTiming;
	BOOL m_bHasCustomizedTiming;   //�����Զ���ֱ���
	TIMING_STRUCT m_SelectedTiming;//��ǰѡ��ķֱ���
	TIMING_STRUCT m_CustomizedTiming; //�Զ���ֱ���

	void SetTheme(XTPControlTheme theme);
	void AddSplitMode();
	void AddTiming();
	const int AddTimingByModeName(const CString splitmode); //����ֵΪ���һ����ӵ�������ķֱ��ʵ����
	void ShowDetailedTiming();
	const int GetSelectedTimingIndex(); //��ȡ��ǰѡ��ֱ��ʵ����
	void UpdatePixClock();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeSplitMode();
	afx_msg void OnCbnSelchangeTiming();
	afx_msg void OnBtnClickedCustomTiming();
	afx_msg void OnBtnClickedSetBkgTiming();
	afx_msg void OnBtnClickedSetCombination();
	afx_msg void OnEnChangedEditHTotal();
	afx_msg void OnEnChangedEditVTotal();
	afx_msg void OnEnChangedEditHz();
	afx_msg void OnDeltaposSpinMvxBkgFreshrate(NMHDR *pNMHDR, LRESULT *pResult);
};
