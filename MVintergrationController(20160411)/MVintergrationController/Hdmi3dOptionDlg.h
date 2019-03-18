#pragma once

typedef struct __st3Dmode
{
	CString str3Dname; //3Dģʽ
	int     n3Dindex;  //������
}ST_3DMODE;

const ST_3DMODE st3Dmode[7] = {
	{_T("֡��ģʽ(Frame Packing)"),       0},
	{_T("ǰ��֡ģʽ(Field Alternative)"), 1},
	{_T("��֡ģʽ(Line Alternative)"),    2},
	{_T("���Ҹ�ʽ(Side-by-Side(Full))"),  3},
	{_T("���¸�ʽ(Top-and-Bottom(Half))"),6},
	{_T("���Ҹ�ʽ(Side-by-Side(Half))"),  8},
	{_T("ֱͨģʽ(Bypass Mode)"),         0xff},
};

// CHdmi3dOptionDlg �Ի���

class CHdmi3dOptionDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CHdmi3dOptionDlg)

public:
	CHdmi3dOptionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHdmi3dOptionDlg();

// �Ի�������
	enum { IDD = IDD_DLG_HDMI_3DSETTING };

	CXTPButton m_GboxIn3Dmode;
	CXTPButton m_GboxOut3Dmode;
	CXTPButton m_GboxOctave;
	CXTPButton m_GboxEyeSwitch;
	CXTPButton m_BtnSetIn3D;
	CXTPButton m_BtnSetOut3D;
	CXTPButton m_BtnSetOctave;
	CXTPButton m_BtnSwitchEye;
	CXTPComboBox m_CboxIn3Dmode;
	CXTPComboBox m_CboxOut3Dmode;
	CXTPComboBox m_CboxOctave;
	CXTPComboBox m_CboxEye;

	CBrush m_bkBrush;

public:
	void ResizeAll();
	void SetTheme(XTPControlTheme theme);

	//��ʼ��
	void Initailize();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnHdmiSetInput3dmod();
	afx_msg void OnBnClickedBtnHdmiSetOutput3dmode();
	afx_msg void OnBnClickedBtnHdmiSetOctave();
	afx_msg void OnBnClickedBtnHdmiSwitchEye();
};
