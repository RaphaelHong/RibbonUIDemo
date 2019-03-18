#pragma once
#include "afxwin.h"
#include "BaseBoard.h"


// CMviewDlg �Ի���

class CMviewDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMviewDlg)

public:
	CMviewDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMviewDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MV };

	static int sMvObjCount;
	int m_nObjOder;

private:
	CBaseBoard m_boardInfo;
	TIMING_STRUCT m_CurTiming[3]; //���ڱ���ֱ��� Ĭ��/����1/����2

	TIMING_STRUCT *m_pDetailed_timing; //��ȡ��ȷ�ķֱ�����Ϣ����ָ��
	CString *m_pStrTiming; //��ȡ��ȷ�ķֱ�����������ָ��

	bool NVidia_Presented; //NViDIA �Կ�
	bool ATI_Presented; //ATI �Կ�

	int m_OSMajor,m_OSMinor;

	void ResetCombBox();
	void SetControlTheme(XTPControlTheme theme);
	BOOL AssertDevChanged() const;

	void SelectModes();   //���ģʽ����Ͽ�
	void SelectTimings(); //��ӷֱ��ʵ���Ͽ�
	void AddTimings(CXTPComboBox *pCbox, CString strMode); //����Ļ�ָ�ģʽ��ӷֱ���

	void GetCurrentSelectedTiming(CString *strName, int *nIndex, TIMING_STRUCT *pTiming, int iNum) const;
	BOOL IsWideTiming(TIMING_STRUCT *pTiming, int iNum) const;
	void SetWideScreenTimings(pTIMING_STRUCT pTiming);
	void GenWideTimingNVidiaXP(int h_act, int v_act, int h_total, int h_front, int h_synt, int v_total, int v_front, int vsynt, int hpol, int vpol, int pix_wid, int vHz, int pic_rate);
	void GenWideTimingNVidiaW7_Old(int h_act, int v_act, int h_total, int h_front, int h_synt, int v_total, int v_front, int vsynt, int hpol, int vpol, int pix_wid, int vHz, int pic_rate);
	void GenWideTimingNVidiaW7_New(int h_act, int v_act, int h_total, int h_front, int h_synt, int v_total, int v_front, int vsynt, int hpol, int vpol, int pix_wid, int vHz, int pic_rate);
	unsigned char* put_data(int val, unsigned char* p, int len);
	unsigned char* get_timingname(int h_act, int v_act,int vHz, unsigned char* pData, int len);
	void GetGPUModel();
	bool GetOSVersion(int &OSVerMajor,int &OSVerMinor);
	void SetWideScreenTimingsXP(unsigned char *pData,int len);
	void SetWideScreenTimingsW7_New(unsigned char *pData,int len);
	void SetWideScreenTimingsW7_Old(unsigned char *pData,int len);

public:
	void SetBoardInfo(CBaseBoard boardinfo);
	void Initialize(); //��ʼ����������

protected:
	void ResizeIt();
	CBrush m_bkBrush;
	void EnglishSupport();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CXTPButton m_btnRefresh;
	CXTPButton m_btnSetTiming;
	CXTPComboBox m_cmbDefLayout;
	CXTPComboBox m_cmbDefTiming;
	CXTPComboBox m_cmbSecLayout;
	CXTPComboBox m_cmbSecTiming;
	CXTPComboBox m_cmbThdLayout;
	CXTPComboBox m_cmbThdTiming;
	CXTPComboBox m_cmbAdapter;

	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBnClickedSetMvTiming();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnRefresh();
	afx_msg void OnCbnSelchangeCboxDefLayout();
	afx_msg void OnCbnSelchangeCboxSecLayout();
	afx_msg void OnCbnSelchangeCboxThdLayout();
};
