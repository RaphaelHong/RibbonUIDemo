// ConnectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MVintergrationControllerDlg.h"
#include "ConnectDlg.h"

// 对话框渐透明弹起方式
#define POP_ECLIPSE 100
#define TIMER_POP_CONNECTDLG   1

// CConnectDlg 对话框

IMPLEMENT_DYNAMIC(CConnectDlg, CDialog)

CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
	, m_bExpanded(FALSE)
	, m_nAlpha(10)
{

}

CConnectDlg::~CConnectDlg()
{
}

void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_NET, m_btnNetConn);
	DDX_Control(pDX, IDC_BUTTON_COM, m_btnComConn);
	DDX_Control(pDX, IDC_BUTTON_USB, m_btnUSBConn);
	DDX_Control(pDX, IDC_BUTTON_ADVANCED_NET, m_btnAdvanced);
	DDX_Control(pDX, IDC_BUTTON_APPLAY_NET, m_btnApply);
	DDX_Control(pDX, IDC_BUTTON_DETECT_NET, m_btnDetect);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_COM, m_cmbComPort);
	DDX_Control(pDX, IDC_IPADDRESS_NEW_IP, m_ipaddress_ip);
	DDX_Control(pDX, IDC_IPADDRESS_NEW_MASK, m_ipaddress_mask);
	DDX_Control(pDX, IDC_IPADDRESS_NEW_GATEWAY, m_ipaddress_gateway);
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_NET, &CConnectDlg::OnBnClickedButtonNetConnect)
	ON_BN_CLICKED(IDC_BUTTON_COM, &CConnectDlg::OnBnClickedButtonComConnect)
	ON_BN_CLICKED(IDC_BUTTON_USB, &CConnectDlg::OnBnClickedButtonUSBConnect)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ADVANCED_NET, &CConnectDlg::OnBnClickedButtonAdvancedNet)
	ON_BN_CLICKED(IDC_BUTTON_DETECT_NET, &CConnectDlg::OnBnClickedBtnDetectNet)
	ON_BN_CLICKED(IDC_BUTTON_APPLAY_NET, &CConnectDlg::OnBnClickedBtnApplyNet)
	ON_WM_TIMER()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CConnectDlg 消息处理程序

BOOL CConnectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetCtrlTheme(xtpControlThemeResource);

	m_btnNetConn.SetIcon(CSize(16,16), IDI_NET_CONN);
	m_btnComConn.SetIcon(CSize(16,16), IDI_COM_CONN);
	m_btnUSBConn.SetIcon(CSize(16,16), IDI_USB_CONN);

	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000); 
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL; 
		//取得SetLayeredWindowAttributes函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes"); 
		if(fun)fun(this->GetSafeHwnd(),0, m_nAlpha,2); 
		FreeLibrary(hInst); 
	}

	//自动获取可用串口
	AutoComPortDetect();

	// 默认IP
	GetDlgItem(IDC_IPADDRESS_CUR_IP)->SetWindowText(_T("192.168.1.200"));

	//添加提示信息
	AddToolTips();

	//创建背景画刷
	//m_bkBrush.CreateSolidBrush(RGB(100, 150, 200));

	//支持英语
#ifdef ENGLISHVER
	EnglishSupport();
#endif

	InitButtonName();

	SetTimer(TIMER_POP_CONNECTDLG, POP_ECLIPSE, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CConnectDlg::InitButtonName()
{
#ifdef ENGLISHVER
	if (theApp.m_nConnType == NON_CONNECT)
	{
		GetDlgItem(IDC_BUTTON_NET)->SetWindowText(_T("Net Connect"));
		GetDlgItem(IDC_BUTTON_COM)->SetWindowText(_T("COM Connect"));
		GetDlgItem(IDC_BUTTON_USB)->SetWindowText(_T("USB Connect"));
	}
	else if (theApp.m_nConnType == NET_CONNECT)
	{
		GetDlgItem(IDC_BUTTON_NET)->SetWindowText(_T("Disconnect"));
		GetDlgItem(IDC_BUTTON_COM)->SetWindowText(_T("COM Connect"));
		GetDlgItem(IDC_BUTTON_USB)->SetWindowText(_T("USB Connect"));
	}
	else if (theApp.m_nConnType == COM_CONNECT)
	{
		GetDlgItem(IDC_BUTTON_NET)->SetWindowText(_T("Net Connect"));
		GetDlgItem(IDC_BUTTON_COM)->SetWindowText(_T("Disconnect"));
		GetDlgItem(IDC_BUTTON_USB)->SetWindowText(_T("USB Connect"));
	}
	else if (theApp.m_nConnType == USB_CONNECT)
	{
		GetDlgItem(IDC_BUTTON_NET)->SetWindowText(_T("Net Connect"));
		GetDlgItem(IDC_BUTTON_COM)->SetWindowText(_T("COM Connect"));
		GetDlgItem(IDC_BUTTON_USB)->SetWindowText(_T("Disconnect"));
	}
#else
	if (theApp.m_nConnType == NON_CONNECT)
	{
		GetDlgItem(IDC_BUTTON_NET)->SetWindowText(_T("网络连接"));
		GetDlgItem(IDC_BUTTON_COM)->SetWindowText(_T("串口连接"));
		GetDlgItem(IDC_BUTTON_USB)->SetWindowText(_T("USB连接"));
	}
	else if (theApp.m_nConnType == NET_CONNECT)
	{
		GetDlgItem(IDC_BUTTON_NET)->SetWindowText(_T("断开连接"));
		GetDlgItem(IDC_BUTTON_COM)->SetWindowText(_T("串口连接"));
		GetDlgItem(IDC_BUTTON_USB)->SetWindowText(_T("USB连接"));
	}
	else if (theApp.m_nConnType == COM_CONNECT)
	{
		GetDlgItem(IDC_BUTTON_NET)->SetWindowText(_T("网络连接"));
		GetDlgItem(IDC_BUTTON_COM)->SetWindowText(_T("断开连接"));
		GetDlgItem(IDC_BUTTON_USB)->SetWindowText(_T("USB连接"));
	}
	else if (theApp.m_nConnType == USB_CONNECT)
	{
		GetDlgItem(IDC_BUTTON_NET)->SetWindowText(_T("网络连接"));
		GetDlgItem(IDC_BUTTON_COM)->SetWindowText(_T("串口连接"));
		GetDlgItem(IDC_BUTTON_USB)->SetWindowText(_T("断开连接"));
	}
#endif

	m_btnNetConn.SetChecked(theApp.m_nConnType == NET_CONNECT);
	m_btnComConn.SetChecked(theApp.m_nConnType == COM_CONNECT);
	m_btnUSBConn.SetChecked(theApp.m_nConnType == USB_CONNECT);
}

void CConnectDlg::SetCtrlTheme(XTPControlTheme ctlTheme)
{
	m_btnAdvanced.SetTheme(ctlTheme);
	m_btnNetConn.SetTheme(ctlTheme);
	m_btnComConn.SetTheme(ctlTheme);
	m_btnUSBConn.SetTheme(ctlTheme);
	m_btnApply.SetTheme(ctlTheme);
	m_btnDetect.SetTheme(ctlTheme);
	m_btnCancel.SetTheme(ctlTheme);
	m_cmbComPort.SetTheme(ctlTheme);
}

void CConnectDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码

	CRect rcClient;
	GetWindowRect(&rcClient);

	CRect rcSplitter;
	GetDlgItem(IDC_SPLITTER_CONNECT)->GetWindowRect(&rcSplitter);

	SetWindowPos(&wndTop, rcClient.left, rcClient.top, rcClient.Width(), rcSplitter.bottom-rcClient.top, SWP_SHOWWINDOW);
}

void CConnectDlg::OnBnClickedButtonNetConnect()
{
	//如果当前为串口连接，先断开串口
	if (theApp.m_nConnType == COM_CONNECT) ComDisconnect();
	if (theApp.m_nConnType == USB_CONNECT) USBDisconnect();

	//连接或断开网络
	if (theApp.m_nConnType == NET_CONNECT) NetDisconnect();
	else
	{
		if (NetConnect())
		{
			//检测设备
			if (!DetectDevice())
			{
				NetDisconnect();
				MessageBox(_T("Can't detect our devices！"), _T("Device Detect"), MB_OK|MB_ICONSTOP);
			}
		}
	}

	//设置按钮名称
	InitButtonName();

	//更新主界面上的通信状态
	CMVintergrationControllerDlg *pDlg = (CMVintergrationControllerDlg*)AfxGetMainWnd();
	pDlg->SetConnectionStatus();
	pDlg->SetLightOn();
	pDlg->SetTimer(113, 500, NULL);

	if(theApp.m_nConnType != NON_CONNECT)	OnOK();
	else { pDlg->KillTimer(113); pDlg->SetLightOn(FALSE); }
}

void CConnectDlg::OnBnClickedButtonComConnect()
{
	//如果当前为网络连接，先断开网络
	if (theApp.m_nConnType == NET_CONNECT) NetDisconnect();
	if (theApp.m_nConnType == USB_CONNECT) USBDisconnect();

	//连接或断开串口
	if (theApp.m_nConnType == COM_CONNECT) ComDisconnect();
	else
	{
		if (ComConnect())
		{
			//检测设备
			if (!DetectDevice())
			{
				ComDisconnect();
				MessageBox(_T("Can't detect our devices！"), _T("Device Detect"), MB_OK|MB_ICONSTOP);
			}
		}
	}

	//设置按钮名称
	InitButtonName();

	//更新主界面上的通信状态
	CMVintergrationControllerDlg *pDlg = (CMVintergrationControllerDlg*)AfxGetMainWnd();
	pDlg->SetConnectionStatus();
	pDlg->SetLightOn();
	pDlg->SetTimer(113, 500, NULL);

	if(theApp.m_nConnType != NON_CONNECT)	OnOK();
	else { pDlg->KillTimer(113); pDlg->SetLightOn(FALSE); }
}


void CConnectDlg::OnBnClickedButtonUSBConnect()
{
	//如果当前为网络连接，先断开网络
	if (theApp.m_nConnType == NET_CONNECT) NetDisconnect();
	if (theApp.m_nConnType == COM_CONNECT) ComDisconnect();

	//连接或断开串口
	if (theApp.m_nConnType == USB_CONNECT) USBDisconnect();
	else
	{
		if (USBConnect())
		{
			//检测设备
			if (!DetectDevice())
			{
				USBDisconnect();
				MessageBox(_T("Can't detect our devices！"), _T("Device Detect"), MB_OK|MB_ICONSTOP);
			}
		}
	}

	//设置按钮名称
	InitButtonName();

	//更新主界面上的通信状态
	CMVintergrationControllerDlg *pDlg = (CMVintergrationControllerDlg*)AfxGetMainWnd();
	pDlg->SetConnectionStatus();
	pDlg->SetLightOn();
	pDlg->SetTimer(113, 500, NULL);

	if(theApp.m_nConnType != NON_CONNECT)	OnOK();
	else { pDlg->KillTimer(113); pDlg->SetLightOn(FALSE); }
}

BOOL CConnectDlg::NetConnect()
{
	UpdateData();

	CString strIP;
	GetDlgItem(IDC_IPADDRESS_CUR_IP)->GetWindowText(strIP);

	theApp.m_sock.m_ipaddr = strIP;

	if (!theApp.m_sock.OpenSocket())
	{
		MessageBox(_T("Can't connect to ethernet,check out the connectivity and IP address！"), _T("Net Connect"), MB_OK|MB_ICONSTOP);
	}
	else
		theApp.m_nConnType = NET_CONNECT;

	//设置指令模块通信接口
	theApp.SetCmdInterface(&theApp.m_sock, &theApp.m_Com, &theApp.m_USBport);

	return theApp.m_sock.b_Connect;
}

BOOL CConnectDlg::NetDisconnect()
{
	if (theApp.m_sock.b_Connect)
	{
		theApp.m_sock.CloseSocket();
		theApp.m_nConnType = NON_CONNECT;

		//设置指令模块通信接口
		theApp.SetCmdInterface(&theApp.m_sock, &theApp.m_Com, &theApp.m_USBport);
	}

	return TRUE;
}

BOOL CConnectDlg::ComConnect()
{
	UpdateData(TRUE);

	CString str;
	UINT nPort;
	GetDlgItem(IDC_COMBO_COM)->GetWindowText(str);
	str.Delete(0,3);
	nPort = _ttoi(str);
	nPort = nPort - 1;
	theApp.m_Com.m_nPort = nPort;

	if(! theApp.m_Com.OpenPort(nPort,115200))
	{
		str.Format(_T("Open serial port[%d] failed!"), theApp.m_Com.m_nPort+1);			
		MessageBox(str, _T("COM Connect"), MB_OK|MB_ICONSTOP);
	}
	else
		theApp.m_nConnType = COM_CONNECT;

	//设置指令模块通信接口
	theApp.SetCmdInterface(&theApp.m_sock, &theApp.m_Com, &theApp.m_USBport);

	return theApp.m_Com.m_bOpen;
}

BOOL CConnectDlg::ComDisconnect()
{
	if (theApp.m_Com.m_bOpen)
	{
		theApp.m_Com.ClosePort();
		theApp.m_nConnType = NON_CONNECT;

		//设置指令模块通信接口
		theApp.SetCmdInterface(&theApp.m_sock, &theApp.m_Com, &theApp.m_USBport);
	}

	return TRUE;
}

BOOL CConnectDlg::USBConnect()
{
	if (theApp.m_nConnType == USB_CONNECT && theApp.m_USBport.m_bOpen) return TRUE;

	if (!theApp.m_USBport.OpenPort())
		MessageBox(_T("Sorry, failed to open USB port！"), _T("Open USB port"), MB_ICONSTOP|MB_OK);
	else
		theApp.m_nConnType = USB_CONNECT;

	theApp.SetCmdInterface(&theApp.m_sock, &theApp.m_Com, &theApp.m_USBport);

	return theApp.m_USBport.m_bOpen;
}

BOOL CConnectDlg::USBDisconnect()
{
	if (theApp.m_USBport.m_bOpen)
	{
		theApp.m_USBport.ClosePort();
		theApp.m_nConnType = NON_CONNECT;

		theApp.SetCmdInterface(&theApp.m_sock, &theApp.m_Com, &theApp.m_USBport);
	}

	return TRUE;
}

BOOL CConnectDlg::DetectDevice()
{
	if (theApp.GetManufacture() != 1408) return FALSE;
	//if (theApp.GetDeviceID() != 80) return FALSE;

	//系统板卡的功能码 作用尚不明确
	//int nSysFuncId = theApp.GetBoardFuncCode();
	//if ((nSysFuncId&0xff000000) != 0x41000000) return FALSE; //甄别一体机的依据

	return TRUE;
}

// 获取设备所有可用串口
void CConnectDlg::GetAllComPorts(CString *pPortList)
{
	//int k=0;
	long   lReg; 
	HKEY   hKey; 
	DWORD   MaxValueLength; 
	DWORD   dwValueNumber; 

	lReg=RegOpenKeyExA(HKEY_LOCAL_MACHINE, 
		"HARDWARE\\DEVICEMAP\\SERIALCOMM", 
		0, 
		KEY_QUERY_VALUE, 
		&hKey);

	if(lReg!=ERROR_SUCCESS) //成功时返回ERROR_SUCCESS，
	{ 
		//AfxMessageBox(_T("Open   Registry   Error!\n")); 
		return;
	} 

	lReg=RegQueryInfoKeyA(hKey, 
		NULL, 
		NULL, 
		NULL, 
		NULL,
		NULL, 
		NULL, 
		&dwValueNumber, //返回和hKey关联的值
		&MaxValueLength, 
		NULL, 
		NULL, 
		NULL); 

	if(lReg!=ERROR_SUCCESS) //没有成功
	{ 
		//AfxMessageBox(_T("Getting   Info   Error!\n")); 
		return;
	} 

	LPSTR   pValueName,pCOMNumber; 
	DWORD   cchValueName,dwValueSize=6; 

	//printf("Number   of   SubKeys:%d\n",dwValueNumber); 

	for(int i=0; i < (int)dwValueNumber; i++) 
	{ 
		cchValueName=MaxValueLength+1; 
		dwValueSize=6; 
		pValueName=(LPSTR)VirtualAlloc(NULL,cchValueName,MEM_COMMIT,PAGE_READWRITE); 
		lReg=RegEnumValueA(hKey, 
			i, 
			pValueName, 
			&cchValueName, 
			NULL, 
			NULL, 
			NULL, 
			NULL); 

		if((lReg!=ERROR_SUCCESS)&&(lReg!=ERROR_NO_MORE_ITEMS)) 
		{ 
			//AfxMessageBox(_T("Enum   Registry   Error or No More Items!\n")); 
			CString slReg;
			slReg.Format(_T("%d"),lReg);
			//AfxMessageBox(slReg);
			return;
		} 

		pCOMNumber=(LPSTR)VirtualAlloc(NULL,6,MEM_COMMIT,PAGE_READWRITE); 
		lReg=RegQueryValueExA(hKey, 
			pValueName, 
			NULL, 
			NULL, 
			(LPBYTE)pCOMNumber, 
			&dwValueSize); 

		if(lReg!=ERROR_SUCCESS) 
		{ 
			//AfxMessageBox(_T("Can not get the name of the port"));
			return;
		} 

		CString str2(pCOMNumber);//,str3;//str1(pPortList),
		*pPortList += _T(",") + str2;                     

		VirtualFree(pValueName,0,MEM_RELEASE); 
		VirtualFree(pCOMNumber,0,MEM_RELEASE); 
	} 
}

// 自动检测可用串口
void CConnectDlg::AutoComPortDetect()
{
	CString strComm(_T(""));
	GetAllComPorts(&strComm);
	if (strComm.IsEmpty())
	{
		GetDlgItem(IDC_COMBO_COM)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_COM)->EnableWindow(FALSE);
		MessageBox(_T("No available COM ports！"), _T("Communication Configuration"), MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		CString resToken;
		int curPos = 0;
		resToken = strComm.Tokenize(_T(","), curPos);
		m_cmbComPort.Clear();
		while(resToken != _T(""))
		{
			m_cmbComPort.AddString(resToken);
			resToken = strComm.Tokenize(_T(","), curPos);
		}

		m_cmbComPort.SetCurSel(m_cmbComPort.GetCount()-1);
	}
}

HBRUSH CConnectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	if (nCtlColor == CTLCOLOR_DLG)
	{
		return theApp.m_DlgSetting.bkBrush;
		//return m_bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(theApp.m_DlgSetting.clrText);
		//pDC->SetTextColor(RGB(255,255,10));

		return theApp.m_DlgSetting.bkBrush;
		//return m_bkBrush;
	}
	return hbr;
}

void CConnectDlg::OnBnClickedButtonAdvancedNet()
{
	CRect rcClient;
	GetWindowRect(rcClient);

	if (m_bExpanded) //已展开窗口，点击收起
	{
		CRect rcSeparator;
		GetDlgItem(IDC_SPLITTER_CONNECT)->GetWindowRect(rcSeparator);

		SetWindowPos(&wndTop, rcClient.left, rcClient.top, rcClient.Width(), rcSeparator.bottom-rcClient.top, SWP_SHOWWINDOW);

		m_btnAdvanced.SetWindowText(_T("Advanced>>"));
	}
	else //未展开窗口， 点击展开
	{
		CRect rcSeparator;
		GetDlgItem(IDC_EDIT_MAC)->GetWindowRect(rcSeparator);

		SetWindowPos(&wndTop, rcClient.left, rcClient.top, rcClient.Width(), rcSeparator.bottom-rcClient.top+8, SWP_SHOWWINDOW);

		m_btnAdvanced.SetWindowText(_T("Advanced<<"));
	}

	m_bExpanded = !m_bExpanded;
}

void CConnectDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_POP_CONNECTDLG:
		{
			// 逐渐显示效果
			HINSTANCE hInst = LoadLibrary("User32.DLL"); 
			if(hInst) 
			{ 
				typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
				MYFUNC fun = NULL; 
				//取得SetLayeredWindowAttributes函数指针 
				fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes"); 

				m_nAlpha += 25;
				if(m_nAlpha > 255)
				{
					m_nAlpha = 255;
					if (fun) fun(this->m_hWnd, 0, m_nAlpha, 2);
					KillTimer(TIMER_POP_CONNECTDLG);
				}

				if (fun) fun(this->m_hWnd, 0, m_nAlpha, 2);

				FreeLibrary(hInst); 
			}
		}
		break;
	default: break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CConnectDlg::OnBnClickedBtnDetectNet()
{
	NetParams np;
	memset(&np, 0, sizeof(NetParams));

	if (theApp.GetNetParams(&np) != CMD_RTOK) return (void)MessageBox(_T("Failed to read network parameters！"),_T("Network Detect"), MB_OK|MB_ICONSTOP);

	CString strMac;
	strMac.Format(_T("%02X-%02X-%02X-%02X-%02X-%02X"), np.mac[0], np.mac[1], np.mac[2], np.mac[3], np.mac[4], np.mac[5] );
	m_ipaddress_ip.SetAddress((BYTE)np.ipaddr.sub0, (BYTE)np.ipaddr.sub1, (BYTE)np.ipaddr.sub2, (BYTE)np.ipaddr.sub3);
	m_ipaddress_mask.SetAddress((BYTE)np.mask.sub0, (BYTE)np.mask.sub1, (BYTE)np.mask.sub2, (BYTE)np.mask.sub3);
	m_ipaddress_gateway.SetAddress((BYTE)np.gateway.sub0, (BYTE)np.gateway.sub1, (BYTE)np.gateway.sub2, (BYTE)np.gateway.sub3);
	GetDlgItem(IDC_EDIT_MAC)->SetWindowText(strMac);
}

void CConnectDlg::OnBnClickedBtnApplyNet()
{
	NetParams np;
	
	if (!CollectNetworkParams(np)) return (void)MessageBox(_T("Illegal input,please retype and try again！"),_T("Apply New Settings"), MB_OK|MB_ICONWARNING);

	int nRet = theApp.SetNetParams(&np);

	if (nRet == -1) MessageBox(_T("Failure,communication failure or return an error！"), _T("Network Setting"), MB_OK|MB_ICONSTOP);
	else MessageBox(_T("Success,new configuration will take effect after the machine restarts！"), _T("Network Setting"), MB_OK|MB_ICONINFORMATION);
}

BOOL CConnectDlg::CollectNetworkParams(NetParams &np)
{
	UpdateData();

	if (m_ipaddress_ip.IsBlank()) { m_ipaddress_ip.SetFocus(); return FALSE; }
	if (m_ipaddress_mask.IsBlank()) { m_ipaddress_mask.SetFocus(); return FALSE;}
	if (m_ipaddress_gateway.IsBlank()) { m_ipaddress_gateway.SetFocus(); return FALSE;}

	memset(&np, 0, sizeof(np));

	m_ipaddress_ip.GetAddress((BYTE&)np.ipaddr.sub0, (BYTE&)np.ipaddr.sub1, (BYTE&)np.ipaddr.sub2, (BYTE&)np.ipaddr.sub3);
	m_ipaddress_mask.GetAddress((BYTE&)np.mask.sub0, (BYTE&)np.mask.sub1, (BYTE&)np.mask.sub2, (BYTE&)np.mask.sub3);
	m_ipaddress_gateway.GetAddress((BYTE&)np.gateway.sub0, (BYTE&)np.gateway.sub1, (BYTE&)np.gateway.sub2, (BYTE&)np.gateway.sub3);

	CString strMac;
	GetDlgItem(IDC_EDIT_MAC)->GetWindowText(strMac);
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
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MAC);
		pEdit->SetFocus();
		pEdit->SetSel(0, pEdit->GetWindowTextLength());

		return FALSE;
	}

	return TRUE;
}
BOOL CConnectDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tooltip.RelayEvent(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN
			||pMsg->wParam == VK_ESCAPE)
		{
			//防止回车或者Esc键导致的程序退出
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CConnectDlg::OnNcHitTest(CPoint point)
{
	UINT nHitTest = CDialog::OnNcHitTest(point);

	return nHitTest == HTCLIENT ? HTCAPTION : nHitTest;

	return CDialog::OnNcHitTest(point);
}


void CConnectDlg::EnglishSupport()
{
	SetWindowText(_T("Communication Configuration"));
	GetDlgItem(IDC_TEXT_IPADDR)->SetWindowText(_T("IP ADDR:"));
	GetDlgItem(IDC_TEXT_SERIALPORT)->SetWindowText(_T("COM PORT:"));
	GetDlgItem(IDC_TEXT_NETWORK_PARAM)->SetWindowText(_T("Network Parameters"));
	GetDlgItem(IDC_TEXT_NEW_IPADDR)->SetWindowText(_T("New IP Addr:"));
	GetDlgItem(IDC_TEXT_NEW_SUBNET)->SetWindowText(_T("New Subnet Addr:"));
	GetDlgItem(IDC_TEXT_NEW_GATEWAY)->SetWindowText(_T("New Gateway Addr:"));
	GetDlgItem(IDC_TEXT_NEW_MACADDR)->SetWindowText(_T("New Mac Addr:"));
	GetDlgItem(IDC_BUTTON_NET)->SetWindowText(_T("Net Connect"));
	GetDlgItem(IDC_BUTTON_COM)->SetWindowText(_T("COM Connect"));
	GetDlgItem(IDC_BUTTON_USB)->SetWindowText(_T("USB Connect"));
	GetDlgItem(IDC_BUTTON_ADVANCED_NET)->SetWindowText(_T("Advanced>>"));
	GetDlgItem(IDCANCEL)->SetWindowText(_T("Close"));
	GetDlgItem(IDC_BUTTON_DETECT_NET)->SetWindowText(_T("Detect"));
	GetDlgItem(IDC_BUTTON_APPLAY_NET)->SetWindowText(_T("Apply"));
}

void CConnectDlg::AddToolTips()
{
#ifdef ENGLISHVER
	m_tooltip.Create(this);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_COM), _T("Connect device via serial port"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_NET), _T("Connect device via network"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_USB), _T("Connect device via USB port"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_ADVANCED_NET), _T("Expand advanced network setting area"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_DETECT_NET), _T("Detect network parameters"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_APPLAY_NET), _T("Apply new network parameters"));
	m_tooltip.AddTool(GetDlgItem(IDC_IPADDRESS_CUR_IP), _T("Typing IP address here"));
	m_tooltip.AddTool(GetDlgItem(IDC_COMBO_COM), _T("Select the correct COM port"));
	m_tooltip.AddTool(GetDlgItem(IDC_IPADDRESS_NEW_IP), _T("Typing your new IP address here"));
	m_tooltip.AddTool(GetDlgItem(IDC_IPADDRESS_NEW_MASK), _T("Typing your new Subnet address here"));
	m_tooltip.AddTool(GetDlgItem(IDC_IPADDRESS_NEW_GATEWAY), _T("Typing your new Gateway address here"));
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_MAC), _T("Typing new MAC address(eg:5A-45-E5-C5-54-65)"));
	m_tooltip.Activate(TRUE);
	m_tooltip.SetTipBkColor(RGB(225,244,225));
	m_tooltip.SetTipTextColor(RGB(136,0,0));
#else
	m_tooltip.Create(this);
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_COM), _T("串口方式连接"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_NET), _T("网络方式连接"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_USB), _T("USB方式连接"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_ADVANCED_NET), _T("点击展开高级功能"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_DETECT_NET), _T("检测网络参数"));
	m_tooltip.AddTool(GetDlgItem(IDC_BUTTON_APPLAY_NET), _T("应用网络参数"));
	m_tooltip.AddTool(GetDlgItem(IDC_IPADDRESS_CUR_IP), _T("输入设备的IP地址"));
	m_tooltip.AddTool(GetDlgItem(IDC_COMBO_COM), _T("选择设备所在的串口"));
	m_tooltip.AddTool(GetDlgItem(IDC_IPADDRESS_NEW_IP), _T("输入新的IP地址"));
	m_tooltip.AddTool(GetDlgItem(IDC_IPADDRESS_NEW_MASK), _T("输入新的掩码"));
	m_tooltip.AddTool(GetDlgItem(IDC_IPADDRESS_NEW_GATEWAY), _T("输入新的网关地址"));
	m_tooltip.AddTool(GetDlgItem(IDC_EDIT_MAC), _T("输入MAC地址(示例:5A-45-E5-C5-54-65)"));
	m_tooltip.Activate(TRUE);
	m_tooltip.SetTipBkColor(RGB(225,244,225));
	m_tooltip.SetTipTextColor(RGB(136,0,0));
#endif
}