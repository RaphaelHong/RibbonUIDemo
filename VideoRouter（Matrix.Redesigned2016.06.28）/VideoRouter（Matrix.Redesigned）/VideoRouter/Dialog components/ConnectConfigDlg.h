#pragma once
#include "afxwin.h"

// CConnectConfigDlg 对话框

class CConnectConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CConnectConfigDlg)

public:
	CConnectConfigDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConnectConfigDlg();

	// 对话框数据
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

	//设置连接按钮的标题
	void SetBtnFaceName();

	//检测设备
	BOOL DetectDevice();

	//获取用户输入的网络参数
	BOOL CollectNetworkParams(NETPARAMS &np);
private:
	//扩展伸缩网络参数框
	BOOL m_bExpanded;
	void ExpandDlg();
	inline void ShowHideControls(BOOL bShowHide);
	//自动检测串口
	//void GetAllCOMPort(CString *pPortList);
	void AutoCOMPortDetect();
	//初始化搜索主机列表
	void HostListInit();
	//添加提示框
	void InitToolTipCtrl();
	//初始化界面参数
	void InitContent();

	void GetAllDomainHost();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
