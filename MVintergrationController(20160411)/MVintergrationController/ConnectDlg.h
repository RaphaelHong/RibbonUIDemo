#pragma once


// CConnectDlg �Ի���

class CConnectDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectDlg)

public:
	CConnectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConnectDlg();

// �Ի�������
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

	void AutoComPortDetect(); //�Զ������ô���

	void GetAllComPorts(CString *pPortList);/*��ѯ�������п��ô�����Ϣ*/

	BOOL CollectNetworkParams(NetParams &np);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
