#pragma once

typedef struct __st3Dmode
{
	CString str3Dname; //3D模式
	int     n3Dindex;  //索引号
}ST_3DMODE;

const ST_3DMODE st3Dmode[7] = {
	{_T("帧包模式(Frame Packing)"),       0},
	{_T("前后帧模式(Field Alternative)"), 1},
	{_T("行帧模式(Line Alternative)"),    2},
	{_T("左右格式(Side-by-Side(Full))"),  3},
	{_T("上下格式(Top-and-Bottom(Half))"),6},
	{_T("左右格式(Side-by-Side(Half))"),  8},
	{_T("直通模式(Bypass Mode)"),         0xff},
};

// CHdmi3dOptionDlg 对话框

class CHdmi3dOptionDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CHdmi3dOptionDlg)

public:
	CHdmi3dOptionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHdmi3dOptionDlg();

// 对话框数据
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

	//初始化
	void Initailize();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnHdmiSetInput3dmod();
	afx_msg void OnBnClickedBtnHdmiSetOutput3dmode();
	afx_msg void OnBnClickedBtnHdmiSetOctave();
	afx_msg void OnBnClickedBtnHdmiSwitchEye();
};
