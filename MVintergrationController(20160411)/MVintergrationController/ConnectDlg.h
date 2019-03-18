#pragma once


// CConnectDlg 对话框

class CConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConnectDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CONNECT };

private:
	BOOL m_bExpanded;
	CXTPButton m_btnNetConn;
	CXTPButton m_btnComConn;
	CXTPButton m_btnUSBConn;
	CXTPButton m_btnAdvanced;
	CXTPButton m_btnApply;
	CXTPButton m_btnDetect;
	CXTPButton m_btnCancel;
	CXTPComboBox m_cmbComPort;

	CIPAddressCtrl m_ipaddress_ip;
	CIPAddressCtrl m_ipaddress_mask;
	CIPAddressCtrl m_ipaddress_gateway;

	CToolTipCtrl m_tooltip;

	int m_nAlpha;

	void InitButtonName();
	void SetCtrlTheme(XTPControlTheme ctlTheme);

	//CBrush m_bkBrush;

	void AddToolTips();
	void EnglishSupport();

private:
	BOOL NetConnect();
	BOOL NetDisconnect();
	BOOL ComConnect();
	BOOL ComDisconnect();
	BOOL USBConnect();
	BOOL USBDisconnect();
	BOOL DetectDevice();

	void AutoComPortDetect(); //自动检测可用串口

	void GetAllComPorts(CString *pPortList);/*查询本机所有可用串口信息*/

	BOOL CollectNetworkParams(NetParams &np);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonNetConnect();
	afx_msg void OnBnClickedButtonComConnect();
	afx_msg void OnBnClickedButtonUSBConnect();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonAdvancedNet();
	afx_msg void OnBnClickedBtnDetectNet();
	afx_msg void OnBnClickedBtnApplyNet();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
