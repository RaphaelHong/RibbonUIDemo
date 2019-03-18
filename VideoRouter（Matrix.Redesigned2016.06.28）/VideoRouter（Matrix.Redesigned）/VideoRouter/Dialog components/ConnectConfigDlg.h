#pragma once
#include "afxwin.h"

// CConnectConfigDlg �Ի���

class CConnectConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectConfigDlg)

public:
	CConnectConfigDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConnectConfigDlg();

	// �Ի�������
	enum { IDD = IDD_DLG_COMMUNICATE_CFG };

	CIPAddressCtrl m_ipCtrlCurIP;
	CIPAddressCtrl m_ipCtrlNewIP;
	CIPAddressCtrl m_ipCtrlNewMask;
	CIPAddressCtrl m_ipCtrlNewGateway;
	CXTPComboBox m_cboxCOM;
	CXTPListCtrl m_ListHost;
	CToolTipCtrl m_tipCtrl;

	CXTPButton m_btnConn;

public:
	int m_nRadioConn;

	void ConnectToDevice();
	void DisconnectDivice();

	void NetConnect();
	void NetDisconnect();
	void ComConnect();
	void ComDisconnect();

	//�������Ӱ�ť�ı���
	void SetBtnFaceName();

	//����豸
	BOOL DetectDevice();

	//��ȡ�û�������������
	BOOL CollectNetworkParams(NETPARAMS &np);
private:
	//��չ�������������
	BOOL m_bExpanded;
	void ExpandDlg();
	inline void ShowHideControls(BOOL bShowHide);
	//�Զ���⴮��
	//void GetAllCOMPort(CString *pPortList);
	void AutoCOMPortDetect();
	//��ʼ�����������б�
	void HostListInit();
	//�����ʾ��
	void InitToolTipCtrl();
	//��ʼ���������
	void InitContent();

	void GetAllDomainHost();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedBtnAdvanceNetcfg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnHostSearch();
	afx_msg void OnBnClickedBtnNetDetect();
	afx_msg void OnBnClickedBtnNetApply();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnRadioConnType(UINT nID);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnNMDblclkListHostAddr(NMHDR *pNMHDR, LRESULT *pResult);
};
