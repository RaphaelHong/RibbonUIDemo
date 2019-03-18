// ConnectConfigDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../VideoRouter.h"
#include "../MainFrm.h"
#include "../CommunicateManager.h"
#include "ConnectConfigDlg.h"


using namespace std;

// CConnectConfigDlg 对话框

IMPLEMENT_DYNAMIC(CConnectConfigDlg, CDialog)

CConnectConfigDlg::CConnectConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectConfigDlg::IDD, pParent)
	, m_bExpanded(TRUE)
	, m_nRadioConn(0)
{

}

CConnectConfigDlg::~CConnectConfigDlg()
{
}

void CConnectConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IP_ADDR, m_ipCtrlCurIP);
	DDX_Control(pDX, IDC_IP_NEW_IPADDR, m_ipCtrlNewIP);
	DDX_Control(pDX, IDC_IP_NEW_MASKADDR, m_ipCtrlNewMask);
	DDX_Control(pDX, IDC_IP_NEW_GATEWAYADDR, m_ipCtrlNewGateway);
	DDX_Control(pDX, IDC_CBOX_COMPORT, m_cboxCOM);
	DDX_Radio(pDX, IDC_RADIO_NET_SEL, m_nRadioConn);
	DDX_Control(pDX, IDC_LIST_HOST_ADDR, m_ListHost);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_btnConn);
}


BEGIN_MESSAGE_MAP(CConnectConfigDlg, CDialog)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BTN_ADVANCE_NETCFG, &CConnectConfigDlg::OnBnClickedBtnAdvanceNetcfg)
	ON_BN_CLICKED(IDC_BTN_HOST_SEARCH, &CConnectConfigDlg::OnBnClickedBtnHostSearch)
	ON_BN_CLICKED(IDC_BTN_NET_DETECT, &CConnectConfigDlg::OnBnClickedBtnNetDetect)
	ON_BN_CLICKED(IDC_BTN_NET_APPLY, &CConnectConfigDlg::OnBnClickedBtnNetApply)
	ON_BN_CLICKED(IDOK, &CConnectConfigDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CConnectConfigDlg::OnBnClickedBtnConnect)
	ON_COMMAND_RANGE(IDC_RADIO_NET_SEL, IDC_RADIO_COM_SEL, &CConnectConfigDlg::OnRadioConnType)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_HOST_ADDR, &CConnectConfigDlg::OnNMDblclkListHostAddr)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CConnectConfigDlg 消息处理程序
BOOL CConnectConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	//默认的连接状态
	SetBtnFaceName();
	//默认界面参数
	InitContent();
	//自动检测串口
	AutoCOMPortDetect();
	//初始化主机列表控件
	HostListInit();
	//添加提示框
	InitToolTipCtrl();
		
	return FALSE; 
}

LRESULT CConnectConfigDlg::OnNcHitTest(CPoint point)
{
	UINT nHitTest = CDialog::OnNcHitTest(point);

	return nHitTest = (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;

	return CDialog::OnNcHitTest(point);
}


void CConnectConfigDlg::ExpandDlg()
{
	CRect rc, rcNew;
	GetWindowRect(&rc);
	rcNew = rc;

	CRect rcBtnSearch, rcBtnCan, rcGBoxSetting, rcGBoxNew;
	GetDlgItem(IDC_BTN_HOST_SEARCH)->GetWindowRect(&rcBtnSearch);
	GetDlgItem(IDCANCEL)->GetWindowRect(&rcBtnCan);
	GetDlgItem(IDC_GBOX_CONNECT_SETTING)->GetWindowRect(&rcGBoxSetting);
	GetDlgItem(IDC_GBOX_NEW_NETCONFIG)->GetWindowRect(&rcGBoxNew);
	ScreenToClient(&rcBtnCan);
	ScreenToClient(&rcGBoxSetting);

	if (m_bExpanded)
	{
		rcNew.bottom = rc.bottom-rcGBoxNew.Height();
		rcBtnCan.OffsetRect(0, -rcGBoxNew.Height());
		rcGBoxSetting.bottom = rcGBoxSetting.bottom - rcGBoxNew.Height();
		GetDlgItem(IDC_GBOX_CONNECT_SETTING)->MoveWindow(rcGBoxSetting);
		GetDlgItem(IDCANCEL)->MoveWindow(rcBtnCan);
		SetWindowPos(NULL, rcNew.left, rcNew.top, rcNew.Width(), rcNew.Height(), SWP_SHOWWINDOW);

		GetDlgItem(IDC_BTN_ADVANCE_NETCFG)->SetWindowText(_T("高级>>"));
	}
	else
	{
		rcNew.bottom = rc.bottom+rcGBoxNew.Height();
		rcBtnCan.OffsetRect(0, rcGBoxNew.Height());
		rcGBoxSetting.bottom = rcGBoxSetting.bottom+rcGBoxNew.Height();
		GetDlgItem(IDC_GBOX_CONNECT_SETTING)->MoveWindow(rcGBoxSetting);
		GetDlgItem(IDCANCEL)->MoveWindow(rcBtnCan);
		SetWindowPos(NULL, rcNew.left, rcNew.top, rcNew.Width(), rcNew.Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_BTN_ADVANCE_NETCFG)->SetWindowText(_T("高级<<"));
	}

	m_bExpanded = !m_bExpanded;
	ShowHideControls(m_bExpanded);
}

inline void CConnectConfigDlg::ShowHideControls(BOOL bShowHide)
{
	int nShowHide = bShowHide ? SW_SHOW : SW_HIDE;

	GetDlgItem(IDC_LABEL_NEW_IP)->ShowWindow(nShowHide);
	GetDlgItem(IDC_LABEL_NEW_MASK)->ShowWindow(nShowHide);
	GetDlgItem(IDC_LABEL_NEW_GATEWAY)->ShowWindow(nShowHide);
	GetDlgItem(IDC_LABEL_NEW_MAC)->ShowWindow(nShowHide);
	GetDlgItem(IDC_IP_NEW_IPADDR)->ShowWindow(nShowHide);
	GetDlgItem(IDC_IP_NEW_MASKADDR)->ShowWindow(nShowHide);
	GetDlgItem(IDC_IP_NEW_GATEWAYADDR)->ShowWindow(nShowHide);
	GetDlgItem(IDC_EDIT_MAC_ADDR)->ShowWindow(nShowHide);
	GetDlgItem(IDC_BTN_NET_DETECT)->ShowWindow(nShowHide);
	GetDlgItem(IDC_BTN_NET_APPLY)->ShowWindow(nShowHide);
	GetDlgItem(IDC_GBOX_NEW_NETCONFIG)->ShowWindow(nShowHide);
}

void CConnectConfigDlg::OnBnClickedBtnAdvanceNetcfg()
{
	ExpandDlg();
}

void CConnectConfigDlg::AutoCOMPortDetect()
{
	CString strCOM(_T(""));
    GetCmdManager()->GetAllCOMPort(&strCOM);//	GetAllCOMPort(&strCOM);

	if (strCOM.IsEmpty())
	{
		m_nRadioConn = 0;
		GetDlgItem(IDC_RADIO_COM_SEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_CBOX_COMPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LABEL_DEV_COM)->SetWindowText(_T("（无可用串口！）"));
		UpdateData(FALSE);
	}
	else
	{
		CString resToken;
		int curPos = 0;
		resToken = strCOM.Tokenize(_T(","), curPos);
		m_cboxCOM.Clear();
		while(resToken != _T(""))
		{
			m_cboxCOM.AddString(resToken);
			resToken = strCOM.Tokenize(_T(","), curPos);
		}

		m_cboxCOM.SetCurSel(m_cboxCOM.GetCount()-1);
	}
}


void CConnectConfigDlg::InitContent()
{
	CSysConfManager sysconf;
	CString strDefAddr;
	strDefAddr.Format(_T("%s"), sysconf.GetDefaultIPAddress());
	if (strDefAddr.IsEmpty())
		strDefAddr = _T("192.168.1.200");

	m_ipCtrlCurIP.SetWindowText(strDefAddr);
	m_ipCtrlNewIP.SetWindowText(_T("192.168.1.200"));
	m_ipCtrlNewMask.SetWindowText(_T("255.255.255.0"));
	m_ipCtrlNewGateway.SetWindowText(_T("192.168.1.1"));
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MAC_ADDR);
	pEdit->SetWindowText(_T("eg：00-78-DC-E6-78-48"));
	pEdit->SetFocus();
	pEdit->SetSel(0, -1);
}

void CConnectConfigDlg::HostListInit()
{
	m_ListHost.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP);

	int nColunm = 0;
	m_ListHost.InsertColumn(nColunm++, _T("序号"), LVCFMT_LEFT, 50);
	m_ListHost.InsertColumn(nColunm++, _T("IP地址"), LVCFMT_LEFT, 100);
	m_ListHost.InsertColumn(nColunm++, _T("子网掩码"), LVCFMT_LEFT, 100);
	m_ListHost.InsertColumn(nColunm++, _T("MAC地址"), LVCFMT_LEFT, 120);
	m_ListHost.InsertColumn(nColunm++, _T("说明"), LVCFMT_LEFT, 100);
}

void CConnectConfigDlg::InitToolTipCtrl()
{
	m_tipCtrl.Create(this);
	m_tipCtrl.AddTool(&m_ListHost, _T("双击一项将该IP选中到IP地址栏！"));
	m_tipCtrl.AddTool(GetDlgItem(IDC_BTN_HOST_SEARCH), _T("点击搜索域内所有主机"));
	m_tipCtrl.AddTool(GetDlgItem(IDC_BTN_ADVANCE_NETCFG), _T("点击展开/收缩网络设置区"));
	m_tipCtrl.AddTool(GetDlgItem(IDC_BTN_NET_APPLY), _T("点击设置新的网络地址"));
	m_tipCtrl.AddTool(GetDlgItem(IDC_BTN_NET_DETECT), _T("点击获取设备网络地址"));
	m_tipCtrl.SetTipBkColor(RGB(250, 250, 240));
	m_tipCtrl.SetTipTextColor(RGB(80, 50, 40));
	m_tipCtrl.Activate(TRUE);
	m_tipCtrl.SetDelayTime(200);	
}


BOOL CConnectConfigDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tipCtrl.RelayEvent(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CConnectConfigDlg::OnBnClickedBtnHostSearch()
{	
	GetAllDomainHost();
}

void CConnectConfigDlg::GetAllDomainHost()
{
	CWaitCursor wait;

	//获取所有本公司设备IP
	IPMAC hosts[254];
	int iHost = 0;

	GetCmdManager()->GetDomainHosts(hosts, iHost);

	m_ListHost.SetRedraw(FALSE);

	//清除列表内容
	m_ListHost.DeleteAllItems();

	CString  strMac, strDest, strItem; 
	PBYTE pmac;
	int nItem = 0;
	
	//根据连接后返回的字符串判断是否是本公司的设备
	for (int i=0; i<iHost; i++)
	{
		strDest.Format(_T("%s"), hosts[i].szIp);
		pmac=(PBYTE)hosts[i].szMac;  
		strMac.Format(_T("%02x-%02x-%02x-%02x-%02x-%02x"),pmac[0],pmac[1],pmac[2],pmac[3],pmac[4],pmac[5]);
		strMac.MakeUpper();

		strItem.Format(_T("%d"), nItem);
		m_ListHost.InsertItem(LVIF_TEXT, nItem, strItem, 0, LVIS_SELECTED, 0, 0);
		m_ListHost.SetItemText(nItem, 1, strDest);
		m_ListHost.SetItemText(nItem, 2, _T("255.255.255.0"));
		m_ListHost.SetItemText(nItem, 3, strMac);
		nItem++;
	}

	m_ListHost.SetRedraw(TRUE);

	AfxMessageBox(_T("搜索完成！"), MB_OK|MB_ICONINFORMATION);
}


void CConnectConfigDlg::OnBnClickedBtnNetDetect()
{
	if (!CCommunicateManager::IsConnected())
	{
		MSG_NO_CONNECTION;
		return;
	}

	NETPARAMS np = {0};

	if (GetCmdManager()->CmdRNET(SYSBOARD_PORT, eBoardTypeCtrl, np) != CMD_RTOK)
	{
		MessageBox(_T("错误，读取网络配置失败！"), _T("网络检测"), MB_OK|MB_ICONERROR);
		return;
	}

	CString strMac;
	strMac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"), np.mac[0], np.mac[1], np.mac[2], np.mac[3], np.mac[4], np.mac[5] );
	m_ipCtrlNewIP.SetAddress((BYTE)np.ipaddr.sub0, (BYTE)np.ipaddr.sub1, (BYTE)np.ipaddr.sub2, (BYTE)np.ipaddr.sub3);
	m_ipCtrlNewMask.SetAddress((BYTE)np.mask.sub0, (BYTE)np.mask.sub1, (BYTE)np.mask.sub2, (BYTE)np.mask.sub3);
	m_ipCtrlNewGateway.SetAddress((BYTE)np.gateway.sub0, (BYTE)np.gateway.sub1, (BYTE)np.gateway.sub2, (BYTE)np.gateway.sub3);
	GetDlgItem(IDC_EDIT_MAC_ADDR)->SetWindowText(strMac);

	MessageBox(_T("读取网络配置成功！"), _T("网络检测"), MB_OK|MB_ICONINFORMATION);
}


void CConnectConfigDlg::OnBnClickedBtnNetApply()
{
	if (! CCommunicateManager::IsConnected())
	{
		MSG_NO_CONNECTION;
		return;
	}

	UpdateData();

	NETPARAMS np = {0};
	if (!CollectNetworkParams(np))
	{
		MessageBox(_T("输入非法，请重新输入！"),_T("网络设置"), MB_OK|MB_ICONSTOP);
		return;
	}

	if (GetCmdManager()->CmdWNET(SYSBOARD_PORT, eBoardTypeCtrl, np) != CMD_RTOK)
		MessageBox(_T("设置失败！"), _T("网络设置"), MB_OK|MB_ICONERROR);
	else
		MessageBox(_T("设置成功，设备重启后生效！"), _T("网络设置"), MB_OK|MB_ICONINFORMATION);
}

BOOL CConnectConfigDlg::CollectNetworkParams(NETPARAMS &np)
{
	UpdateData();

	if (m_ipCtrlNewIP.IsBlank()) { m_ipCtrlNewIP.SetFocus(); return FALSE; }
	if (m_ipCtrlNewMask.IsBlank()) { m_ipCtrlNewMask.SetFocus(); return FALSE;}
	if (m_ipCtrlNewGateway.IsBlank()) { m_ipCtrlNewGateway.SetFocus(); return FALSE;}

	memset(&np, 0, sizeof(np));

	m_ipCtrlNewIP.GetAddress((BYTE&)np.ipaddr.sub0, (BYTE&)np.ipaddr.sub1, (BYTE&)np.ipaddr.sub2, (BYTE&)np.ipaddr.sub3);
	m_ipCtrlNewMask.GetAddress((BYTE&)np.mask.sub0, (BYTE&)np.mask.sub1, (BYTE&)np.mask.sub2, (BYTE&)np.mask.sub3);
	m_ipCtrlNewGateway.GetAddress((BYTE&)np.gateway.sub0, (BYTE&)np.gateway.sub1, (BYTE&)np.gateway.sub2, (BYTE&)np.gateway.sub3);

	CString strMac;
	GetDlgItem(IDC_EDIT_MAC_ADDR)->GetWindowText(strMac);
	strMac.MakeUpper();

	//MAC地址
	int nTick=0;
	CString strtmp;
	int tokpos=0;
	strtmp = strMac.Tokenize(_T("-"), tokpos);
	while(!strtmp.IsEmpty())
	{
		np.mac[nTick++] = atoi(strtmp);
		strtmp = strMac.Tokenize(_T("-"), tokpos);
	}
	if (nTick != 6)
	{
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MAC_ADDR);
		pEdit->SetFocus();
		pEdit->SetSel(0, pEdit->GetWindowTextLength());

		return FALSE;
	}

	return TRUE;
}


void CConnectConfigDlg::OnRadioConnType(UINT nID)
{
	UpdateData();
}

void CConnectConfigDlg::OnBnClickedBtnConnect()
{
	if (! CCommunicateManager::IsConnected())
		ConnectToDevice();
	else
		DisconnectDivice();

	//刷新连接按钮的标题
	SetBtnFaceName();

	//已连接成功，关闭对话框
	CMFCRibbonLabel *pLabel = (CMFCRibbonLabel *)GetMainFrame()->m_wndStatusBar.GetElement(0);
	if (CCommunicateManager::IsConnected())
	{
		pLabel->SetText(_T("已连接设备"));
		OnOK();
	}
	else
	{
		pLabel->SetText(_T("未连接设备！"));
	}

	GetMainFrame()->m_wndStatusBar.ForceRecalcLayout();
}

void CConnectConfigDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}

void CConnectConfigDlg::ConnectToDevice()
{
	if (m_nRadioConn == 0)
		NetConnect();
	else if (m_nRadioConn == 1)
		ComConnect();

	if (CCommunicateManager::IsConnected())
	{
		if (DetectDevice())
		{
			CString strDevIP;
			m_ipCtrlCurIP.GetWindowText(strDevIP);
			CSysConfManager sysconfig;
			sysconfig.SetDefaultIPAddress(strDevIP);
			MessageBox(_T("已连接到设备，点击【确定】开始初始化参数！"), _T("连接设备"), MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			MessageBox(_T("无法检测到设备，已断开连接！"), _T("连接设备"), MB_OK|MB_ICONSTOP);

			if (m_nRadioConn == 0)
				NetDisconnect();
			else if (m_nRadioConn == 1)
				ComDisconnect();
		}
	}
	else
	{
		MessageBox(_T("失败，无法连接设备！请检查通信参数和链路连接！"), _T("连接设备"), MB_OK|MB_ICONSTOP);
	}
}

void CConnectConfigDlg::DisconnectDivice()
{
	GetCmdManager()->Close();

	if (! CCommunicateManager::IsConnected())
	{
		MessageBox(_T("已断开设备连接！"), _T("断开连接"), MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(_T("失败，未知的原因导致无法断开连接！"), _T("断开连接"), MB_OK|MB_ICONERROR);
	}
}

void CConnectConfigDlg::NetConnect()
{
 	UpdateData();
 
 	CString strDevIP;
 	m_ipCtrlCurIP.GetWindowText(strDevIP);
 
	GetCmdManager()->OpenSocket(strDevIP);
}

void CConnectConfigDlg::NetDisconnect()
{
 	GetCmdManager()->Close();
}

void CConnectConfigDlg::ComConnect()
{
 	UpdateData(TRUE);
 
 	CString str;
 	UINT nPort;
 	GetDlgItem(IDC_CBOX_COMPORT)->GetWindowText(str);
 	str.Delete(0,3);
 	nPort = _ttoi(str);
 	nPort = nPort - 1;
 	
	GetCmdManager()->OpenSerial(nPort, 115200);
}

void CConnectConfigDlg::ComDisconnect()
{
	GetCmdManager()->Close();
}

BOOL CConnectConfigDlg::DetectDevice()
{
	//厂商版本号
 	if (GetCmdManager()->CmdRMVR(SYSBOARD_PORT, eBoardTypeCtrl) != 1408)
 	{
 		return FALSE;
 	}
 
 	//机器ID号
 	//if (GetCmdManager()->CmdRDID(SYSBOARD_PORT, eBoardTypeCtrl) != SYSBOARD_PORT)
 	//{
 	//	return FALSE;
 	//}

	//机器功能码
	int nDevCode = GetCmdManager()->CmdRDFN(SYSBOARD_PORT, eBoardTypeCtrl);

	if ((nDevCode & 0xFF000000) != 0xB0000000)
		return FALSE;
	else
		return TRUE;

	return TRUE;
}

void CConnectConfigDlg::SetBtnFaceName()
{
	if (! CCommunicateManager::IsConnected())
		m_btnConn.SetWindowText(_T("连接"));
	else
		m_btnConn.SetWindowText(_T("断开连接"));

	m_btnConn.SetChecked(CCommunicateManager::IsConnected());
}

void CConnectConfigDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	ExpandDlg();
}


void CConnectConfigDlg::OnNMDblclkListHostAddr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;

	if (nItem >= 0)
	{
		LVITEM lvi = {0};
		lvi.mask = LVIF_TEXT | LVIF_STATE;
		lvi.iItem = nItem;
		lvi.iSubItem = 1;
		lvi.pszText = new TCHAR[256];
		lvi.cchTextMax = 256;

		m_ListHost.GetItem(&lvi);

		SetDlgItemText(IDC_IP_ADDR, lvi.pszText);
		SetDlgItemText(IDC_IP_NEW_IPADDR, lvi.pszText);

		lvi.iSubItem = 2;
		m_ListHost.GetItem(&lvi);
		SetDlgItemText(IDC_IP_NEW_MASKADDR, lvi.pszText);

		lvi.iSubItem = 3;
		m_ListHost.GetItem(&lvi);
		SetDlgItemText(IDC_EDIT_MAC_ADDR, lvi.pszText);

		delete [] lvi.pszText;
	}

	*pResult = 0;
}



//获取工作组内所有主机主机名，IP地址和MAC地址
//CString GetMAC(CString IP,int & rs)     // 取IP地址对应的MAC地址
//{
//	unsigned   char   mac[6];
//	ULONG   MacLen=6;
//	ULONG   DestIP=inet_addr(IP);
//	rs=SendARP(DestIP,(ULONG)NULL,(PULONG)mac,(PULONG)&MacLen);
//	CString MACs=_T("");
//	if (rs==0)
//	{
//		char  buf[32];
//		sprintf_s(buf,"%02X-%02X-%02X-%02X-%02X-%02X",
//			(unsigned int)mac[0],
//			(unsigned int)mac[1],
//			(unsigned int)mac[2],
//			(unsigned int)mac[3],
//			(unsigned int)mac[4],
//			(unsigned int)mac[5]);
//		MACs=buf;
//	}
//	return(MACs);
//}

//void CConnectConfigDlg::GetNameAndIp()  
//{  
//	CWaitCursor wait;
//
//	struct   hostent   *host;  
//	struct   in_addr   *ptr;  
//
//	DWORD   dwScope = RESOURCE_CONTEXT;  
//	NETRESOURCE   *NetResource = NULL;  
//	HANDLE   hEnum;  
//	WNetOpenEnum(dwScope, NULL, NULL, NULL, &hEnum);  
//
//	WSADATA   wsaData;  
//	WSAStartup(MAKEWORD(1, 1), &wsaData);  
//
//	if (hEnum)  
//	{  
//		DWORD   Count = 0xFFFFFFFF;  
//		DWORD   BufferSize = 10240;  
//		LPVOID   Buffer = new char[10240];  
//		WNetEnumResource(hEnum, &Count, Buffer, &BufferSize);  
//		NetResource = (NETRESOURCE*)Buffer;
//
//		char *pchar = new char[256];
//
//		char szHostName[200];  
//
//		int nItem = 0;
//
//		for (unsigned int i = 0; i < BufferSize / sizeof(NETRESOURCE); i++, NetResource++)  
//		{  
//			if (NetResource->dwUsage == RESOURCEUSAGE_CONTAINER && NetResource->dwType == RESOURCETYPE_ANY)  
//			{  
//				if (NetResource->lpRemoteName)  
//				{  
//					CString   strFullName = NetResource->lpRemoteName;  
//					if (0 == strFullName.Left(2).Compare(_T("\\\\")))  
//						strFullName = strFullName.Right(strFullName.GetLength() - 2);  
//
//					gethostname(szHostName, strlen(szHostName));  
//					USES_CONVERSION;
//					memset(pchar, 0, 256);
//					_tcsncpy_s(pchar, 256, strFullName.GetBuffer(), strFullName.GetLength());
//					strFullName.ReleaseBuffer();
//					//char *pchar = T2A(strFullName);  
//					host = gethostbyname(pchar);  
//
//					if (host == NULL)   continue;  
//					ptr = (struct in_addr *)   host->h_addr_list[0];  
//
//					CString str = _T("");  
//					for (int n = 0; n<4; n++)  
//					{  					
//						if (n > 0)  
//						{  
//							str += _T(".");  
//						}  
//						int value = (unsigned int)((unsigned char*)host->h_addr_list[0])[n];  
//						CString p;
//						p.Format(_T("%d"), value);  
//						str += p;
//					}  
//
//					int nRes;
//					CString strMac = GetMAC(str, nRes);
//					CString strItem;
//					strItem.Format(_T("%d"), nItem);
//					m_ListHost.InsertItem(LVIF_TEXT, nItem, strItem, 0, LVIS_SELECTED, 0, 0);
//					m_ListHost.SetItemText(nItem, 1, host->h_name);
//					m_ListHost.SetItemText(nItem, 2, str);
//					m_ListHost.SetItemText(nItem, 3, strMac);
//
//					nItem++;
//				}  
//			}  
//		}  
//		delete   Buffer;
//		delete   pchar;
//		WNetCloseEnum(hEnum);
//	}  
//	WSACleanup(); 
//}