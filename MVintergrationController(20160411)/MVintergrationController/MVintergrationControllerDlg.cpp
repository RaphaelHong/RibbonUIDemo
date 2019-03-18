// MVintergrationControllerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MVintergrationControllerDlg.h"
#include "ConnectDlg.h"
#include "ContainerView.h"

#define TIMER_POP_MAINWND      111  //主窗口渐现
#define TIMER_SHUT_MAINWND     112  //主窗口渐隐
#define TIMER_INDICATOR        113  //指示灯刷新
#define POP_ECLIPSE            100  //渐现时间间隔
#define SHUT_ECLIPSE           100  //渐隐时间间隔

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMVintergrationControllerDlg dialog

CMVintergrationControllerDlg::CMVintergrationControllerDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMVintergrationControllerDlg::IDD, pParent)
	, m_pTabbedView(NULL)
	, m_pTabbedDoc(NULL)
	, m_nAlpha(10)
	, m_bLightOn(FALSE)
	, m_bHideLeft(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//TCHAR szStylesPath[_MAX_PATH];

	//VERIFY(::GetModuleFileName(
	//	AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		

	//CString m_strStylesPath = szStylesPath;
	//int nIndex  = m_strStylesPath.ReverseFind(_T('\\'));
	//if (nIndex > 0) {
	//	m_strStylesPath = m_strStylesPath.Left(nIndex);
	//}
	//else {
	//	m_strStylesPath.Empty();
	//}
	//m_strStylesPath += _T("\\Styles\\");


	//XTPSkinManager()->AddColorFilter(new CXTPSkinManagerColorFilterColorize(80, 50, 10));

	//XTPSkinManager()->SetApplyOptions(XTPSkinManager()->GetApplyOptions() | xtpSkinApplyMetrics);
	//XTPSkinManager()->LoadSkin(m_strStylesPath + _T("Office2010.cjstyles"),_T("NormalBlack.ini"));
}

void CMVintergrationControllerDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_BOARD, m_treeState);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_TOOLBTN_CFG, m_btnToolCfg);
	DDX_Control(pDX, IDC_TOOLBTN_REFRESH, m_btnToolRefresh);
	DDX_Control(pDX, IDC_TOOLBTN_RESET, m_btnToolReset);
	DDX_Control(pDX, IDC_TOOLBTN_FOUR, m_btnToolUpdate);
	DDX_Control(pDX, IDC_TOOLBTN_FIVE, m_btnToolFive);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_btnConnectConfig);
	DDX_Control(pDX, IDC_BTN_HIDDEN_LEFTDLG, m_btnHidden);
}

BEGIN_MESSAGE_MAP(CMVintergrationControllerDlg, CXTPResizeDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CMVintergrationControllerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CMVintergrationControllerDlg::OnBnClickedButtonConnect)	
	ON_BN_CLICKED(IDC_TOOLBTN_CFG, &CMVintergrationControllerDlg::OnBnClickedToolbtnCfg)
	ON_BN_CLICKED(IDC_TOOLBTN_REFRESH, &CMVintergrationControllerDlg::OnBnClickedToolbtnRefresh)
	ON_BN_CLICKED(IDC_TOOLBTN_RESET, &CMVintergrationControllerDlg::OnBnClickedToolbtnReset)
	ON_BN_CLICKED(IDC_BTN_HIDDEN_LEFTDLG, &CMVintergrationControllerDlg::OnBnClickedHiddenLeftDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_BOARD, &CMVintergrationControllerDlg::OnNMDblclkTreeBoard)	
	ON_BN_CLICKED(IDC_TOOLBTN_FOUR, &CMVintergrationControllerDlg::OnBnClickedToolbtnUpdate)
	ON_BN_CLICKED(IDC_TOOLBTN_FIVE, &CMVintergrationControllerDlg::OnBnClickedToolbtnFive)
END_MESSAGE_MAP()


// CMVintergrationControllerDlg message handlers

BOOL CMVintergrationControllerDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);         // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

	// TODO: Add extra initialization here

	// 透明窗体效果
	//SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000); 
	//HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	//if(hInst) 
	//{ 
	//	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
	//	MYFUNC fun = NULL; 
	//	//取得SetLayeredWindowAttributes函数指针 
	//	fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes"); 
	//	if(fun)fun(this->GetSafeHwnd(),0,m_nAlpha,2); 
	//	FreeLibrary(hInst); 
	//}

	//SetTimer(TIMER_POP_MAINWND, POP_ECLIPSE, NULL);

	//获取程序最近的窗口设置
	LoadPlacement(_T("CMVintergrationControllerDlg"));

	//设置程序标题
	SetWindowText(_T("插卡式一体机控制端 - V1.1"));

	//初始化指示灯
	SetLightOn(FALSE);

	//初始化树形控件
	m_treeState.SetBKImage(IDB_BITMAP_TREEBK);
	InitTreeItem();

	//设置控件风格
	SetCtrTheme(xtpControlThemeResource);

	//设置需要重新调整大小的控件
	SetResize(IDOK, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
	SetResize(IDC_TREE_BOARD, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMLEFT);
	SetResize(IDC_BTN_HIDDEN_LEFTDLG, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);

	//为控制按钮加载图标
	SetIcons();

	//创建tabview
	CreateView();

	//创建背景画刷
	m_BkBrush.CreateSolidBrush(RGB(131,164,203));
	m_BkBitmap.LoadBitmap(IDB_BITMAP_BACK);

	//提示信息
	CreateToolTips();

#ifdef ENGLISHVER
	EnglishSupport();
#endif

	//窗口最近一次关闭的时候大小与静态窗口大小不一样，需重绘
	PostMessage(WM_SIZE);

	return TRUE;
}

void CMVintergrationControllerDlg::CreateToolTips()
{
	m_tipCtrl.Create(this);
#ifdef ENGLISHVER
	m_tipCtrl.AddTool(GetDlgItem(IDC_TREE_BOARD), _T("Double click item to active function view"));
#else
	m_tipCtrl.AddTool(GetDlgItem(IDC_TREE_BOARD), _T("双击列表项展开控制区"));
#endif	
	m_tipCtrl.SetTipBkColor(RGB(250,250,240));
	m_tipCtrl.SetTipTextColor(RGB(80,50,40));
	m_tipCtrl.SetDelayTime(200);
	m_tipCtrl.Activate(TRUE);
}

void CMVintergrationControllerDlg::SetIcons()
{
	m_btnToolCfg.SetIcon(CSize(16,16), IDI_ICON_CFG, IDI_ICON_CFG);
	m_btnToolRefresh.SetIcon(CSize(16,16), IDI_ICON_REFRESH, IDI_ICON_REFRESH);
	m_btnToolReset.SetIcon(CSize(16,16), IDI_ICON_RESET, IDI_ICON_RESETDOWN);
	m_btnToolUpdate.SetIcon(CSize(16,16), IDI_ICON_UPDATE, IDI_ICON_UPDATE);
	m_btnToolFive.SetIcon(CSize(16,16), IDI_ICON_RESETDOWN, IDI_ICON_RESETDOWN);
	m_btnConnectConfig.SetIcon(CSize(16,16), IDI_ICON_NET);
}

//初始化树形控件
void CMVintergrationControllerDlg::InitTreeItem()
{
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP_BOARDSTATE);

	m_imageList.Create(16,16,ILC_COLOR24|ILC_MASK, 3, 1);
	m_imageList.Add(&bitmap, RGB(0,255,0));
	m_treeState.SetImageList(&m_imageList, TVSIL_NORMAL);

	HTREEITEM hItemState = m_treeState.InsertItemState(_T("All Boards"), 2, 2);
	m_treeState.SetItemBold(hItemState);

	int i, nState = STATE_UNCHECKED;
	for(i = 0; i < 9; ++i )
	{
		m_treeState.InsertItemState(_T("Unknow device"), 0, 0, nState, hItemState);
	}

	m_treeState.SetTextColor(RGB(41,79,62));
	m_treeState.Expand(hItemState, TVE_EXPAND);
	m_treeState.SetFont(&XTPAuxData().font);
}

//设置控件风格
void CMVintergrationControllerDlg::SetCtrTheme(XTPControlTheme theme)
{
	m_btnOK.SetTheme(theme); 
	m_btnCancel.SetTheme(theme);
	m_btnToolCfg.SetTheme(theme);
	m_btnToolRefresh.SetTheme(theme);
	m_btnToolReset.SetTheme(theme);
	m_btnToolUpdate.SetTheme(theme);
	m_btnToolFive.SetTheme(theme);
	m_btnConnectConfig.SetTheme(theme);
	m_btnHidden.SetTheme(theme);

#ifdef ENGLISHVER
	m_btnConnectConfig.SetTooltip(_T("Communication configuration"));
	m_btnToolCfg.SetTooltip(_T("Board Settings"));
	m_btnToolRefresh.SetTooltip(_T("Re-detect"));
	m_btnHidden.SetTooltip(_T("Show/Hide left part"));
	m_btnToolUpdate.SetTooltip(_T("Update Device"));
#else
	m_btnConnectConfig.SetTooltip(_T("通信设置"));
	m_btnToolCfg.SetTooltip(_T("板卡设置"));
	m_btnToolRefresh.SetTooltip(_T("重新检测"));
	m_btnToolReset.SetTooltip(_T("复位板卡"));
	m_btnHidden.SetTooltip(_T("展开/隐藏左侧区域"));
	m_btnToolUpdate.SetTooltip(_T("固化设备数据"));
#endif
}

//创建标签页视图
BOOL CMVintergrationControllerDlg::CreateView()
{
	CRect rcView;
	GetDlgItem(IDC_PIC_FUNC)->GetWindowRect(&rcView);
	ScreenToClient(&rcView);
	rcView.InflateRect(-3, -6, -3, -3);

	CFrameWnd *pFrameWnd = (CFrameWnd*)this;
	m_pTabbedDoc = new CTabbedViewDoc;
	CCreateContext context;
	context.m_pCurrentDoc = (CDocument*)m_pTabbedDoc;
	context.m_pNewViewClass = RUNTIME_CLASS(CTabbedView);

	m_pTabbedView = (CTabbedView *)(pFrameWnd->CreateView(&context));
	ASSERT(m_pTabbedView);

	m_pTabbedView->MoveWindow(&rcView);

	return TRUE;
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMVintergrationControllerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//为对话框铺上图片背景
		
		CPaintDC dc(this);
		CRect rc;
		GetClientRect(&rc);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);

		BITMAP bitmap;
		m_BkBitmap.GetBitmap(&bitmap);
		CBitmap* pbmpPri = dcMem.SelectObject(&m_BkBitmap);

		dc.StretchBlt(0,0,rc.Width(), rc.Height(), &dcMem,0,0,bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

		dcMem.SelectObject(pbmpPri);

		dcMem.DeleteDC();

		//CXTPResizeDialog::OnPaint();
	}
}

void CMVintergrationControllerDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTPResizeDialog::OnSize(nType, cx, cy);

	//调整视图大小
	if (m_pTabbedView)
	{
		ExpandFuncArea(m_bHideLeft);
		ResizeView();
	}	
}

// 当窗口尺寸变化时，重新调整TabView大小
void CMVintergrationControllerDlg::ResizeView()
{
	if (m_pTabbedView)
	{
		CRect rcView;
		GetDlgItem(IDC_PIC_FUNC)->GetWindowRect(&rcView);
		ScreenToClient(&rcView);
		rcView.InflateRect(-3, -6, -3, -3);

		m_pTabbedView->MoveWindow(rcView, FALSE);

		RedrawWindow();
	}
}

void CMVintergrationControllerDlg::OnDestroy()
{
	SavePlacement(_T("CMVintergrationControllerDlg"));

	if (m_pTabbedDoc)
	{
		delete m_pTabbedDoc;
		m_pTabbedDoc = NULL;
	}

	CXTPResizeDialog::OnDestroy();
}

BOOL CMVintergrationControllerDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CXTPResizeDialog::OnEraseBkgnd(pDC);
}

HBRUSH CMVintergrationControllerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(250, 240, 50));

		return m_BkBrush;
	}

	return hbr;
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMVintergrationControllerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 双击TreeItem时，生成相应视图
void CMVintergrationControllerDlg::OnNMDblclkTreeBoard(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM htiSel = m_treeState.GetSelectedItem();
	if(!m_treeState.ItemHasChildren(htiSel) && htiSel != NULL)
	{	
		//无可用板卡
		if(m_BoardManager.GetCount() == 0) return; 

		//获取当前选择的板卡名称
		CString strItemText = m_treeState.GetItemText(htiSel);

		//无法识别的板卡
		if(strItemText == UNKNOW_DEV_NAME) return; 

		//获取当前板卡的序号
		int nItemIndex = GetTreeItemIndex(htiSel);
		if (nItemIndex == -1) return; //树形控件中找不到该条目
		 

		//全局变量标志当前应该生成哪种功能页面
		theApp.GlobleLock();
		theApp.m_CurrentProductType = m_BoardManager[nItemIndex].GetProductType();
		theApp.GlobleUnlock();

		int nTab = m_pTabbedView->GetTabIndexByName(strItemText);
		if (nTab == -1)
		{
			//当前没有该板卡的功能视图，创建新的视图
			CreateNewBoardView(theApp.m_CurrentProductType, nItemIndex);
		}
		else
		{
			//当前已有该板卡的功能视图，激活该视图
			m_pTabbedView->SetActiveView(nTab);
		}
	}

	*pResult = 0;
}

// 将读取到的板卡显示到树形控件中
void CMVintergrationControllerDlg::SetTreeItems()
{
	int nItemCount = m_BoardManager.GetCount();

	m_treeState.DeleteAllItems();

	HTREEITEM hItemRoot = m_treeState.InsertItemState(_T("All Boards"), 2, 2);
	m_treeState.SetItemBold(hItemRoot);

	int nState = STATE_UNCHECKED;
	if (nItemCount == 0)
	{
		m_treeState.InsertItemState(_T("No Board Exists"), 0, 0, nState, hItemRoot);
		m_treeState.SetTextColor(RGB(255,0,0));
	}
	else
	{
		for (int i=0; i<nItemCount; i++)
		{
			if (m_BoardManager[i].GetProductType() == Product_INVALID)
				m_treeState.InsertItemState(m_BoardManager[i].GetBoardName(), 0, 0, nState, hItemRoot);
			else
				m_treeState.InsertItemState(m_BoardManager[i].GetBoardName(), 1, 1, nState, hItemRoot);
		}
		m_treeState.SetTextColor(RGB(100, 150, 200));
	}

	m_treeState.Expand(hItemRoot, TVE_EXPAND);
	m_treeState.SetFont(&XTPAuxData().font);
}

int CMVintergrationControllerDlg::GetTreeItemIndex(HTREEITEM hti)
{
	int index = 0;
	HTREEITEM htiRoot = m_treeState.GetRootItem();
	HTREEITEM htiChild = m_treeState.GetChildItem(htiRoot);
	while(htiChild != NULL)
	{
		if (htiChild == hti) { return index; }

		htiChild = m_treeState.GetNextItem(htiChild, TVGN_NEXT);
		index++;
	}

	return -1;
}

void CMVintergrationControllerDlg::CreateNewBoardView(emProductType eboardtype, int nBoardIndex)
{
	if (theApp.m_CurrentProductType == Product_MV) 
	{   //多屏保功能视图 MVIEW系列
		
		if (!m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), m_BoardManager[nBoardIndex].GetBoardName(), 0))
			return (void)MessageBox(_T("创建视图失败！"), _T("创建新视图"), MB_OK|MB_ICONSTOP);

		m_pTabbedView->SetActiveView(m_pTabbedView->GetViewCount()-1);

		//初始化Mview板卡
		CContainerView *pView = (CContainerView*)(m_pTabbedView->GetViewByIndex(m_pTabbedView->GetViewCount()-1));
		pView->m_pWndMV->SetBoardInfo(m_BoardManager[nBoardIndex]);
		pView->m_pWndMV->Initialize();
	}
	else if (theApp.m_CurrentProductType == Product_MG)
	{   //几何校正模块MG300
		if (! m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), m_BoardManager[nBoardIndex].GetBoardName(), 1))
			return (void)MessageBox(_T("创建视图失败！"), _T("创建新视图"), MB_OK|MB_ICONSTOP);

		m_pTabbedView->SetActiveView(m_pTabbedView->GetViewCount()-1);

		//初始化MG300板卡
		CContainerView *pView = (CContainerView*)(m_pTabbedView->GetViewByIndex(m_pTabbedView->GetViewCount()-1));

		pView->m_pWndMG->SetBoardInfo(m_BoardManager[nBoardIndex]);
		pView->m_pWndMG->Initialize();
	}
	else if (theApp.m_CurrentProductType == Product_MVX)
	{
		if (!m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), m_BoardManager[nBoardIndex].GetBoardName(), 4))
			return (void)MessageBox(_T("创建视图失败！"), _T("创建新视图"), MB_OK|MB_ICONSTOP);

		m_pTabbedView->SetActiveView(m_pTabbedView->GetViewCount()-1);

		//初始化MVX303
		CContainerView *pView = (CContainerView*)(m_pTabbedView->GetViewByIndex(m_pTabbedView->GetViewCount()-1));

		pView->m_pWndMVX->SetBoardInfo(m_BoardManager[nBoardIndex]);
		pView->m_pWndMVX->Initialize();
	}
	else if (theApp.m_CurrentProductType == Product_HDMI_3D)
	{  //HDMI_3D板卡
		if (!m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), m_BoardManager[nBoardIndex].GetBoardName(), 6))
			return (void)MessageBox(_T("创建视图失败！"), _T("创建新视图"), MB_OK|MB_ICONSTOP);

		m_pTabbedView->SetActiveView(m_pTabbedView->GetViewCount()-1);

		//初始化HDMI_3D卡
		CContainerView *pView = (CContainerView*)(m_pTabbedView->GetViewByIndex(m_pTabbedView->GetViewCount()-1));

		pView->m_pWndHDMI3D->SetBoardInfo(m_BoardManager[nBoardIndex]);
		pView->m_pWndHDMI3D->Initialize();
	}
}

void CMVintergrationControllerDlg::OnBnClickedButtonConnect()
{
	CConnectDlg dlg;
	if (dlg.DoModal()==IDOK && theApp.m_nConnType != NON_CONNECT)
	{		
		MessageBox(_T("Device connected, data loading...！"), _T("Prompt"), MB_OK|MB_ICONINFORMATION);

		//清理所有功能页
		m_pTabbedView->DeleteAllViews();

		//获取所有板卡信息
		GetAllBoardInfo();

		//更新树形控件内容
		SetTreeItems();
	}
}

// 读取板卡信息
void CMVintergrationControllerDlg::GetAllBoardInfo()
{
	int nTotalBoards = 0;
	int nPortArr[64] = {0}; 

	//获取所有板卡及其端口号
	theApp.GetAllBoards(nTotalBoards, nPortArr);

	//清空板卡信息数组
	m_BoardManager.RemoveAll();

	if (nTotalBoards > 0)
	{
		CBaseBoard board;
		int nBoardID;    //暂存板卡ID
		int nFuncID;     //暂存板卡功能码
		int nExistNum=0; //相同类型板卡数量

		//获取每个板卡的ID，功能码
		for (int i=0; i<nTotalBoards; i++)
		{
			nExistNum = 0;
			if (nPortArr[i] != INVALID_PORT)
			{
				//该卡的通信端口号
				board.SetBoardPort(nPortArr[i]);

				//该板卡 ID
				nBoardID = theApp.GetDeviceID(nPortArr[i]);

				//该板卡 功能码
				nFuncID = theApp.GetBoardFuncCode(nPortArr[i]);

				board.SetBoardID(nBoardID);
				board.SetBoardFuncID(nFuncID);

				nExistNum = ExistSameProductType(nBoardID);
				if (nExistNum > 0) 
					board.SetExtendedName(nExistNum+1);

				m_BoardManager.Add(board);
			}
		}
	}
}

// 刷新通信状态
void CMVintergrationControllerDlg::SetConnectionStatus()
{
	CString strConnParam;
#ifdef ENGLISHVER
	if (theApp.m_nConnType == NON_CONNECT)
	{
		GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("Connection Mode：N/A"));
		GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(_T("Parameters：N/A"));
	}
	else if (theApp.m_nConnType == COM_CONNECT)
	{
		strConnParam.Format(_T("Serail port: COM%d"), theApp.m_Com.m_nPort+1);
		GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("Connection Mode: Serial Port"));
		GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(strConnParam);
	}
	else if (theApp.m_nConnType == NET_CONNECT)
	{
		strConnParam.Format(_T("IP Addr: %s"), theApp.m_sock.m_ipaddr);
		GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("Connection Mode: Network"));
		GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(strConnParam);
	}

	GetDlgItem(IDC_TEXT_CONNECT_STATE)->SetWindowText(_T("Communication State"));
#else
	if (theApp.m_nConnType == NON_CONNECT)
	{
		GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("通信方式：无"));
		GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(_T("通信参数：无"));
	}
	else if (theApp.m_nConnType == COM_CONNECT)
	{
		strConnParam.Format(_T("串口号: COM%d"), theApp.m_Com.m_nPort+1);
		GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("通信方式：已通过串口连接"));
		GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(strConnParam);
	}
	else if (theApp.m_nConnType == NET_CONNECT)
	{
		strConnParam.Format(_T("IP地址: %s"), theApp.m_sock.m_ipaddr);
		GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("通信方式：已通过网络连接"));
		GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(strConnParam);
	}

	GetDlgItem(IDC_TEXT_CONNECT_STATE)->SetWindowText(_T("通信状态"));
#endif
}

BOOL CMVintergrationControllerDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tipCtrl.RelayEvent(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN
			||pMsg->wParam == VK_ESCAPE)
		{
			//防止回车或者Esc键导致的程序退出
			return TRUE;
		}
	}

	return CXTPResizeDialog::PreTranslateMessage(pMsg);
}

// 存在多少相同类型板卡
int CMVintergrationControllerDlg::ExistSameProductType(int nBoardID)
{
	if (m_BoardManager.GetCount() == 0) return 0;

	int nExistNum = 0;
	for (int i=0; i<m_BoardManager.GetCount(); i++)
	{
		if (m_BoardManager[i].GetBoardID() == nBoardID)
		{
			nExistNum++;
		}
	}

	return nExistNum;
}

void CMVintergrationControllerDlg::OnClose()
{
	FriendlyShutDown();

	//CXTPResizeDialog::OnClose();
}

void CMVintergrationControllerDlg::OnBnClickedOk()
{
	FriendlyShutDown();

	//CXTPResizeDialog::OnOK();
}

void CMVintergrationControllerDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_POP_MAINWND:

		FadeInPopUpWindow();
		break;

	case TIMER_SHUT_MAINWND:
		
		FadeOutCloseWindow();
		break;

	case TIMER_INDICATOR:
		SetLightOn(!m_bLightOn);
		break;

	default:break;
	}
	
	CXTPResizeDialog::OnTimer(nIDEvent);
}

// 渐现式启动
void CMVintergrationControllerDlg::FadeInPopUpWindow()
{
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
			KillTimer(TIMER_POP_MAINWND);
		}

		if (fun) fun(this->m_hWnd, 0, m_nAlpha, 2);

		FreeLibrary(hInst); 
	}
}

// 渐隐式退出
void CMVintergrationControllerDlg::FadeOutCloseWindow()
{
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL; 
		//取得SetLayeredWindowAttributes函数指针 
		fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes"); 

		m_nAlpha -= 25;
		if(m_nAlpha < 1)
		{
			FreeLibrary(hInst); 
			KillTimer(TIMER_SHUT_MAINWND);
			PostMessage(WM_QUIT);
		}

		if (fun) fun(this->m_hWnd, 0, m_nAlpha, 2);

		FreeLibrary(hInst); 
	}
}

void CMVintergrationControllerDlg::OnBnClickedToolbtnCfg()
{

}

//刷新按钮
void CMVintergrationControllerDlg::OnBnClickedToolbtnRefresh()
{
	//刷新板卡
	if (m_BoardManager.IsEmpty()) return;

	CWaitCursor wait;

	CString strState, strRet;
	const CString strTmp = _T("已刷新！\r\n");
	const CString strErr = _T(" 没有生成控制页，双击此项可生成！\r\n");

	strState = _T("刷新完成！\r\n\r\n刷新结果：\r\n");

	CContainerView *pView = NULL;
	int nTreeIndex = 0;
	int nTabIndex = 0;
	BOOL bChecked = FALSE;
	HTREEITEM htiRoot = m_treeState.GetRootItem();
	HTREEITEM htiChild = m_treeState.GetChildItem(htiRoot);
	while (htiChild)
	{
		//该板卡是否选中
		bChecked = (STATE_CHECKED == m_treeState.GetItemCheck(htiChild));
		if (bChecked)
		{
			nTabIndex = m_pTabbedView->GetTabIndexByName(m_treeState.GetItemText(htiChild));
			pView = (CContainerView*)m_pTabbedView->GetViewByIndex(nTabIndex);

			if (pView)
			{
				if (m_BoardManager[nTreeIndex].GetProductType() == Product_MV)
				{//MV三屏宝初始化
					pView->m_pWndMV->Initialize();
				}
				else if (m_BoardManager[nTreeIndex].GetProductType() == Product_MG)
				{//MG300初始化
					pView->m_pWndMG->Initialize();
				}
				else if (m_BoardManager[nTreeIndex].GetProductType() == Product_MVX)
				{//MVX303开窗模块初始化
					pView->m_pWndMVX->Initialize();
				}
				else if (m_BoardManager[nTreeIndex].GetProductType() == Product_HDMI_3D)
				{//HDMI_3D板卡初始化
					pView->m_pWndHDMI3D->Initialize();
				}

				strRet = m_treeState.GetItemText(htiChild) + strTmp;
				strState += strRet;
			}
			else {
				strRet = m_treeState.GetItemText(htiChild) + strErr;
			    strState += strRet;
			}
		}

		htiChild = m_treeState.GetNextItem(htiChild, TVGN_NEXT);
		nTreeIndex ++;
	}

	if (strRet.IsEmpty())
	{
		strRet = _T("请在需要刷新的板卡前面打钩！");
		strState += strRet;
	}

	MessageBox(strState,_T("刷新"), MB_OK|MB_ICONINFORMATION);
}

//复位按钮
void CMVintergrationControllerDlg::OnBnClickedToolbtnReset()
{
	UpdateData();

	// 没有可用的板卡
	if (m_BoardManager.IsEmpty()) return;

	CWaitCursor wait;

	CString strResetState;
	CString strRep;
	const CString strSucc = _T("复位成功！\r\n");
	const CString strFail = _T("复位失败！\r\n");

	strResetState = _T("复位完成！\r\n\r\n复位结果：\r\n");

	//检查树形控件中的子项目是否被选中

	BOOL bCheck = FALSE;
	int nBoardIndex = 0;
	HTREEITEM htiRoot  = m_treeState.GetRootItem();
	HTREEITEM htiChild = m_treeState.GetChildItem(htiRoot);
	while(htiChild)
	{
		//该板卡是否选中
		bCheck = (STATE_CHECKED == m_treeState.GetItemCheck(htiChild));
		if (bCheck)
		{//复位命令
			if (theApp.ResetBoards(m_BoardManager[nBoardIndex].GetBoardPort()) == CMD_FAILURE)
				strRep.Format(m_treeState.GetItemText(htiChild)+strFail);
			else
				strRep.Format(m_treeState.GetItemText(htiChild)+strSucc);

			strResetState += strRep;
		}

		htiChild = m_treeState.GetNextItem(htiChild, TVGN_NEXT);
		nBoardIndex++;
	}

	if (strRep.IsEmpty())
	{
		//没有选中任何板卡
		strRep = _T("请在需要复位的板卡前打钩！");
		strResetState += strRep;
	}

	MessageBox(strResetState, _T("复位结果"), MB_OK|MB_ICONINFORMATION);
}

// 固化保存按钮
void CMVintergrationControllerDlg::OnBnClickedToolbtnUpdate()
{
	UpdateData();

	// 没有可用的板卡
	if (m_BoardManager.IsEmpty()) return;

	CWaitCursor wait;

	CString strUpdateState;
	CString strRep;
	const CString strSucc = _T("固化成功！\r\n");
	const CString strFail = _T("固化失败！\r\n");
	const CString strErr = _T(" 没有生成控制页，双击此项可生成！\r\n");

	strUpdateState = _T("固化完成！\r\n\r\n复位结果：\r\n");

	//检查树形控件中的子项目是否被选中

	BOOL bCheck = FALSE;
	BOOL bCmdSucc = FALSE;
	int nTreeIndex = 0;
	int nTabIndex = 0;
	HTREEITEM htiRoot  = m_treeState.GetRootItem();
	HTREEITEM htiChild = m_treeState.GetChildItem(htiRoot);
	while(htiChild)
	{
		bCmdSucc = FALSE;
		//该板卡是否选中
		bCheck = (STATE_CHECKED == m_treeState.GetItemCheck(htiChild));
		if (bCheck)
		{
			nTabIndex = m_pTabbedView->GetTabIndexByName(m_treeState.GetItemText(htiChild));
			CContainerView *pView = (CContainerView*)m_pTabbedView->GetViewByIndex(nTabIndex);
			if (pView)
			{
				if (m_BoardManager[nTreeIndex].GetProductType() == Product_MG)
				{//MG300的保存
					bCmdSucc = pView->m_pWndMG->SaveAll();
				}
				else if (m_BoardManager[nTreeIndex].GetProductType() == Product_MVX)
				{//MVX303开窗模块的保存
					bCmdSucc = pView->m_pWndMVX->SaveAll();
				}
				else if (m_BoardManager[nTreeIndex].GetProductType() == Product_HDMI_3D)
				{//HDMI_3D模块保存
					bCmdSucc = pView->m_pWndHDMI3D->SaveAll();
				}

				if (bCmdSucc)
					strRep.Format(m_treeState.GetItemText(htiChild) + strSucc);
				else
					strRep.Format(m_treeState.GetItemText(htiChild)+strFail);

				strUpdateState += strRep;
			}
			else {
				strRep = m_treeState.GetItemText(htiChild) + strErr;
				strUpdateState += strRep;
			}
		}

		htiChild = m_treeState.GetNextItem(htiChild, TVGN_NEXT);
		nTreeIndex ++;
	}

	if (strRep.IsEmpty())
	{
		//没有选中任何板卡
		strRep = _T("请在需要固化的板卡前打钩！");
		strUpdateState += strRep;
	}

	MessageBox(strUpdateState, _T("固化结果"), MB_OK|MB_ICONINFORMATION);
}


void CMVintergrationControllerDlg::OnBnClickedToolbtnFive()
{
#if _DEBUG
	theApp.m_CurrentProductType = Product_MV;
	m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), _T("MView1"), 0);

	theApp.m_CurrentProductType = Product_MG;
	m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), _T("MGeo1"), 1);

	theApp.ResumeThread(AJUST_THREAD);

	theApp.m_CurrentProductType = Product_MVX;
	m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), _T("MVX303"),  4);

	theApp.m_CurrentProductType = Product_HDMI_3D;
	m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), _T("HDMI_3D"), 6);
#endif
}


// 开关指示灯
void CMVintergrationControllerDlg::SetLightOn(BOOL bOnOFF)
{
	CStatic *pIndicator = (CStatic*)GetDlgItem(IDC_ONOFF_INDICATOR);

	if (bOnOFF)
		pIndicator->SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_LIGHTON)));
	else
		pIndicator->SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_LIGHTOFF)));

	m_bLightOn = bOnOFF;
}

void CMVintergrationControllerDlg::OnBnClickedHiddenLeftDlg()
{
	HideLeft(!m_bHideLeft);
}

// 隐藏/展开主窗口左边部分
void CMVintergrationControllerDlg::HideLeft(BOOL bHide/* =TRUE */)
{
	int nShowHide = bHide ? SW_HIDE : SW_SHOW;
	GetDlgItem(IDC_PIC_COMMUNICATE)->ShowWindow(nShowHide);
	GetDlgItem(IDC_TEXT_CONNECT_STATE)->ShowWindow(nShowHide);
	GetDlgItem(IDC_ONOFF_INDICATOR)->ShowWindow(nShowHide);
	GetDlgItem(IDC_SEPARATOR_MAIN)->ShowWindow(nShowHide);
	GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->ShowWindow(nShowHide);
	GetDlgItem(IDC_TEXT_CONNECT_PARAM)->ShowWindow(nShowHide);
	GetDlgItem(IDC_BUTTON_CONNECT)->ShowWindow(nShowHide);
	GetDlgItem(IDC_TOOLBTN_CFG)->ShowWindow(nShowHide);
	GetDlgItem(IDC_TOOLBTN_REFRESH)->ShowWindow(nShowHide);
	GetDlgItem(IDC_TOOLBTN_RESET)->ShowWindow(nShowHide);
	GetDlgItem(IDC_TOOLBTN_FOUR)->ShowWindow(nShowHide);
	GetDlgItem(IDC_TOOLBTN_FIVE)->ShowWindow(nShowHide);
	GetDlgItem(IDC_TREE_BOARD)->ShowWindow(nShowHide);

#ifdef ENGLISHVER
	m_btnHidden.SetWindowText(bHide ? _T("Show Left>>") : _T("Hide Left<<"));
#else
	m_btnHidden.SetWindowText(bHide ? _T("展开状态页>>") : _T("隐藏状态页<<"));
#endif

	ExpandFuncArea(bHide);
	ResizeView();

	m_bHideLeft = bHide;
}

// 伸缩功能区
void CMVintergrationControllerDlg::ExpandFuncArea(BOOL bExpand)
{
	CWnd *pWnd = GetDlgItem(IDC_PIC_FUNC);
	CRect rcDlg, rcPicView, rcPicComm, rcTree;
	GetDlgItem(IDC_PIC_COMMUNICATE)->GetWindowRect(rcPicComm);
	GetDlgItem(IDC_TREE_BOARD)->GetWindowRect(rcTree);
	this->GetWindowRect(rcDlg);

	if (bExpand) rcPicView = CRect(rcPicComm.left, rcPicComm.top, rcDlg.right-8, rcTree.bottom);
	else		 rcPicView = CRect(rcPicComm.right, rcPicComm.top, rcDlg.right-8, rcTree.bottom);
		
	ScreenToClient(&rcPicView);
	pWnd->MoveWindow(rcPicView);
}

// 鼠标点击在主窗体上也可以移动窗口
LRESULT CMVintergrationControllerDlg::OnNcHitTest(CPoint point)
{
	UINT nHitTest = CXTPResizeDialog::OnNcHitTest(point);

	return nHitTest== HTCLIENT ? HTCAPTION : nHitTest;

	return CXTPResizeDialog::OnNcHitTest(point);
}

// 英文支持
void CMVintergrationControllerDlg::EnglishSupport()
{
	SetWindowText(_T("Mview Integrate Controller-V1.0"));
	GetDlgItem(IDC_TEXT_CONNECT_STATE)->SetWindowText(_T("Communication State"));
	GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("Connection Mode:N/A"));
	GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(_T("Parameters:N/A"));
	GetDlgItem(IDC_BUTTON_CONNECT)->SetWindowText(_T("Communication Configuration"));
	GetDlgItem(IDC_BTN_HIDDEN_LEFTDLG)->SetWindowText(_T("Hide Left <<"));
	GetDlgItem(IDOK)->SetWindowText(_T("Quit"));
}

void CMVintergrationControllerDlg::FriendlyShutDown()
{
	CString strMsg, strTitle;
#ifdef ENGLISHVER
	strMsg = _T("This software will be shutdown, your settings already updated to the device?\r\n - Click「No」to back\r\n - Click「Yes」to quit");
	strTitle = _T("Quit Message");
#else
	strMsg = _T("提醒：程序即将关闭，你的设置是否已经保存到设备？\r\n\r\n - 忘记保存 请点「否」\r\n - 已经保存 请点「是」");
	strTitle = _T("退出提醒");
#endif
	if (MessageBox(strMsg, strTitle, MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		//SetTimer(TIMER_SHUT_MAINWND, SHUT_ECLIPSE, NULL);
		PostMessage(WM_QUIT);
	}
}
