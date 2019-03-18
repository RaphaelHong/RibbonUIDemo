// MVintergrationControllerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MVintergrationControllerDlg.h"
#include "ConnectDlg.h"
#include "ContainerView.h"

#define TIMER_POP_MAINWND      111  //�����ڽ���
#define TIMER_SHUT_MAINWND     112  //�����ڽ���
#define TIMER_INDICATOR        113  //ָʾ��ˢ��
#define POP_ECLIPSE            100  //����ʱ����
#define SHUT_ECLIPSE           100  //����ʱ����

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

	// ͸������Ч��
	//SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000); 
	//HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	//if(hInst) 
	//{ 
	//	typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
	//	MYFUNC fun = NULL; 
	//	//ȡ��SetLayeredWindowAttributes����ָ�� 
	//	fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes"); 
	//	if(fun)fun(this->GetSafeHwnd(),0,m_nAlpha,2); 
	//	FreeLibrary(hInst); 
	//}

	//SetTimer(TIMER_POP_MAINWND, POP_ECLIPSE, NULL);

	//��ȡ��������Ĵ�������
	LoadPlacement(_T("CMVintergrationControllerDlg"));

	//���ó������
	SetWindowText(_T("�忨ʽһ������ƶ� - V1.1"));

	//��ʼ��ָʾ��
	SetLightOn(FALSE);

	//��ʼ�����οؼ�
	m_treeState.SetBKImage(IDB_BITMAP_TREEBK);
	InitTreeItem();

	//���ÿؼ����
	SetCtrTheme(xtpControlThemeResource);

	//������Ҫ���µ�����С�Ŀؼ�
	SetResize(IDOK, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);
	SetResize(IDC_TREE_BOARD, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMLEFT);
	SetResize(IDC_BTN_HIDDEN_LEFTDLG, XTP_ANCHOR_BOTTOMRIGHT, XTP_ANCHOR_BOTTOMRIGHT);

	//Ϊ���ư�ť����ͼ��
	SetIcons();

	//����tabview
	CreateView();

	//����������ˢ
	m_BkBrush.CreateSolidBrush(RGB(131,164,203));
	m_BkBitmap.LoadBitmap(IDB_BITMAP_BACK);

	//��ʾ��Ϣ
	CreateToolTips();

#ifdef ENGLISHVER
	EnglishSupport();
#endif

	//�������һ�ιرյ�ʱ���С�뾲̬���ڴ�С��һ�������ػ�
	PostMessage(WM_SIZE);

	return TRUE;
}

void CMVintergrationControllerDlg::CreateToolTips()
{
	m_tipCtrl.Create(this);
#ifdef ENGLISHVER
	m_tipCtrl.AddTool(GetDlgItem(IDC_TREE_BOARD), _T("Double click item to active function view"));
#else
	m_tipCtrl.AddTool(GetDlgItem(IDC_TREE_BOARD), _T("˫���б���չ��������"));
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

//��ʼ�����οؼ�
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

//���ÿؼ����
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
	m_btnConnectConfig.SetTooltip(_T("ͨ������"));
	m_btnToolCfg.SetTooltip(_T("�忨����"));
	m_btnToolRefresh.SetTooltip(_T("���¼��"));
	m_btnToolReset.SetTooltip(_T("��λ�忨"));
	m_btnHidden.SetTooltip(_T("չ��/�����������"));
	m_btnToolUpdate.SetTooltip(_T("�̻��豸����"));
#endif
}

//������ǩҳ��ͼ
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
		//Ϊ�Ի�������ͼƬ����
		
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

	//������ͼ��С
	if (m_pTabbedView)
	{
		ExpandFuncArea(m_bHideLeft);
		ResizeView();
	}	
}

// �����ڳߴ�仯ʱ�����µ���TabView��С
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

// ˫��TreeItemʱ��������Ӧ��ͼ
void CMVintergrationControllerDlg::OnNMDblclkTreeBoard(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM htiSel = m_treeState.GetSelectedItem();
	if(!m_treeState.ItemHasChildren(htiSel) && htiSel != NULL)
	{	
		//�޿��ð忨
		if(m_BoardManager.GetCount() == 0) return; 

		//��ȡ��ǰѡ��İ忨����
		CString strItemText = m_treeState.GetItemText(htiSel);

		//�޷�ʶ��İ忨
		if(strItemText == UNKNOW_DEV_NAME) return; 

		//��ȡ��ǰ�忨�����
		int nItemIndex = GetTreeItemIndex(htiSel);
		if (nItemIndex == -1) return; //���οؼ����Ҳ�������Ŀ
		 

		//ȫ�ֱ�����־��ǰӦ���������ֹ���ҳ��
		theApp.GlobleLock();
		theApp.m_CurrentProductType = m_BoardManager[nItemIndex].GetProductType();
		theApp.GlobleUnlock();

		int nTab = m_pTabbedView->GetTabIndexByName(strItemText);
		if (nTab == -1)
		{
			//��ǰû�иð忨�Ĺ�����ͼ�������µ���ͼ
			CreateNewBoardView(theApp.m_CurrentProductType, nItemIndex);
		}
		else
		{
			//��ǰ���иð忨�Ĺ�����ͼ���������ͼ
			m_pTabbedView->SetActiveView(nTab);
		}
	}

	*pResult = 0;
}

// ����ȡ���İ忨��ʾ�����οؼ���
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
	{   //������������ͼ MVIEWϵ��
		
		if (!m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), m_BoardManager[nBoardIndex].GetBoardName(), 0))
			return (void)MessageBox(_T("������ͼʧ�ܣ�"), _T("��������ͼ"), MB_OK|MB_ICONSTOP);

		m_pTabbedView->SetActiveView(m_pTabbedView->GetViewCount()-1);

		//��ʼ��Mview�忨
		CContainerView *pView = (CContainerView*)(m_pTabbedView->GetViewByIndex(m_pTabbedView->GetViewCount()-1));
		pView->m_pWndMV->SetBoardInfo(m_BoardManager[nBoardIndex]);
		pView->m_pWndMV->Initialize();
	}
	else if (theApp.m_CurrentProductType == Product_MG)
	{   //����У��ģ��MG300
		if (! m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), m_BoardManager[nBoardIndex].GetBoardName(), 1))
			return (void)MessageBox(_T("������ͼʧ�ܣ�"), _T("��������ͼ"), MB_OK|MB_ICONSTOP);

		m_pTabbedView->SetActiveView(m_pTabbedView->GetViewCount()-1);

		//��ʼ��MG300�忨
		CContainerView *pView = (CContainerView*)(m_pTabbedView->GetViewByIndex(m_pTabbedView->GetViewCount()-1));

		pView->m_pWndMG->SetBoardInfo(m_BoardManager[nBoardIndex]);
		pView->m_pWndMG->Initialize();
	}
	else if (theApp.m_CurrentProductType == Product_MVX)
	{
		if (!m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), m_BoardManager[nBoardIndex].GetBoardName(), 4))
			return (void)MessageBox(_T("������ͼʧ�ܣ�"), _T("��������ͼ"), MB_OK|MB_ICONSTOP);

		m_pTabbedView->SetActiveView(m_pTabbedView->GetViewCount()-1);

		//��ʼ��MVX303
		CContainerView *pView = (CContainerView*)(m_pTabbedView->GetViewByIndex(m_pTabbedView->GetViewCount()-1));

		pView->m_pWndMVX->SetBoardInfo(m_BoardManager[nBoardIndex]);
		pView->m_pWndMVX->Initialize();
	}
	else if (theApp.m_CurrentProductType == Product_HDMI_3D)
	{  //HDMI_3D�忨
		if (!m_pTabbedView->AddView(RUNTIME_CLASS(CContainerView), m_BoardManager[nBoardIndex].GetBoardName(), 6))
			return (void)MessageBox(_T("������ͼʧ�ܣ�"), _T("��������ͼ"), MB_OK|MB_ICONSTOP);

		m_pTabbedView->SetActiveView(m_pTabbedView->GetViewCount()-1);

		//��ʼ��HDMI_3D��
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
		MessageBox(_T("Device connected, data loading...��"), _T("Prompt"), MB_OK|MB_ICONINFORMATION);

		//�������й���ҳ
		m_pTabbedView->DeleteAllViews();

		//��ȡ���а忨��Ϣ
		GetAllBoardInfo();

		//�������οؼ�����
		SetTreeItems();
	}
}

// ��ȡ�忨��Ϣ
void CMVintergrationControllerDlg::GetAllBoardInfo()
{
	int nTotalBoards = 0;
	int nPortArr[64] = {0}; 

	//��ȡ���а忨����˿ں�
	theApp.GetAllBoards(nTotalBoards, nPortArr);

	//��հ忨��Ϣ����
	m_BoardManager.RemoveAll();

	if (nTotalBoards > 0)
	{
		CBaseBoard board;
		int nBoardID;    //�ݴ�忨ID
		int nFuncID;     //�ݴ�忨������
		int nExistNum=0; //��ͬ���Ͱ忨����

		//��ȡÿ���忨��ID��������
		for (int i=0; i<nTotalBoards; i++)
		{
			nExistNum = 0;
			if (nPortArr[i] != INVALID_PORT)
			{
				//�ÿ���ͨ�Ŷ˿ں�
				board.SetBoardPort(nPortArr[i]);

				//�ð忨 ID
				nBoardID = theApp.GetDeviceID(nPortArr[i]);

				//�ð忨 ������
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

// ˢ��ͨ��״̬
void CMVintergrationControllerDlg::SetConnectionStatus()
{
	CString strConnParam;
#ifdef ENGLISHVER
	if (theApp.m_nConnType == NON_CONNECT)
	{
		GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("Connection Mode��N/A"));
		GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(_T("Parameters��N/A"));
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
		GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("ͨ�ŷ�ʽ����"));
		GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(_T("ͨ�Ų�������"));
	}
	else if (theApp.m_nConnType == COM_CONNECT)
	{
		strConnParam.Format(_T("���ں�: COM%d"), theApp.m_Com.m_nPort+1);
		GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("ͨ�ŷ�ʽ����ͨ����������"));
		GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(strConnParam);
	}
	else if (theApp.m_nConnType == NET_CONNECT)
	{
		strConnParam.Format(_T("IP��ַ: %s"), theApp.m_sock.m_ipaddr);
		GetDlgItem(IDC_TEXT_COMMUNICATION_TYPE)->SetWindowText(_T("ͨ�ŷ�ʽ����ͨ����������"));
		GetDlgItem(IDC_TEXT_CONNECT_PARAM)->SetWindowText(strConnParam);
	}

	GetDlgItem(IDC_TEXT_CONNECT_STATE)->SetWindowText(_T("ͨ��״̬"));
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
			//��ֹ�س�����Esc�����µĳ����˳�
			return TRUE;
		}
	}

	return CXTPResizeDialog::PreTranslateMessage(pMsg);
}

// ���ڶ�����ͬ���Ͱ忨
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

// ����ʽ����
void CMVintergrationControllerDlg::FadeInPopUpWindow()
{
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL; 
		//ȡ��SetLayeredWindowAttributes����ָ�� 
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

// ����ʽ�˳�
void CMVintergrationControllerDlg::FadeOutCloseWindow()
{
	HINSTANCE hInst = LoadLibrary("User32.DLL"); 
	if(hInst) 
	{ 
		typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD); 
		MYFUNC fun = NULL; 
		//ȡ��SetLayeredWindowAttributes����ָ�� 
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

//ˢ�°�ť
void CMVintergrationControllerDlg::OnBnClickedToolbtnRefresh()
{
	//ˢ�°忨
	if (m_BoardManager.IsEmpty()) return;

	CWaitCursor wait;

	CString strState, strRet;
	const CString strTmp = _T("��ˢ�£�\r\n");
	const CString strErr = _T(" û�����ɿ���ҳ��˫����������ɣ�\r\n");

	strState = _T("ˢ����ɣ�\r\n\r\nˢ�½����\r\n");

	CContainerView *pView = NULL;
	int nTreeIndex = 0;
	int nTabIndex = 0;
	BOOL bChecked = FALSE;
	HTREEITEM htiRoot = m_treeState.GetRootItem();
	HTREEITEM htiChild = m_treeState.GetChildItem(htiRoot);
	while (htiChild)
	{
		//�ð忨�Ƿ�ѡ��
		bChecked = (STATE_CHECKED == m_treeState.GetItemCheck(htiChild));
		if (bChecked)
		{
			nTabIndex = m_pTabbedView->GetTabIndexByName(m_treeState.GetItemText(htiChild));
			pView = (CContainerView*)m_pTabbedView->GetViewByIndex(nTabIndex);

			if (pView)
			{
				if (m_BoardManager[nTreeIndex].GetProductType() == Product_MV)
				{//MV��������ʼ��
					pView->m_pWndMV->Initialize();
				}
				else if (m_BoardManager[nTreeIndex].GetProductType() == Product_MG)
				{//MG300��ʼ��
					pView->m_pWndMG->Initialize();
				}
				else if (m_BoardManager[nTreeIndex].GetProductType() == Product_MVX)
				{//MVX303����ģ���ʼ��
					pView->m_pWndMVX->Initialize();
				}
				else if (m_BoardManager[nTreeIndex].GetProductType() == Product_HDMI_3D)
				{//HDMI_3D�忨��ʼ��
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
		strRet = _T("������Ҫˢ�µİ忨ǰ��򹳣�");
		strState += strRet;
	}

	MessageBox(strState,_T("ˢ��"), MB_OK|MB_ICONINFORMATION);
}

//��λ��ť
void CMVintergrationControllerDlg::OnBnClickedToolbtnReset()
{
	UpdateData();

	// û�п��õİ忨
	if (m_BoardManager.IsEmpty()) return;

	CWaitCursor wait;

	CString strResetState;
	CString strRep;
	const CString strSucc = _T("��λ�ɹ���\r\n");
	const CString strFail = _T("��λʧ�ܣ�\r\n");

	strResetState = _T("��λ��ɣ�\r\n\r\n��λ�����\r\n");

	//������οؼ��е�����Ŀ�Ƿ�ѡ��

	BOOL bCheck = FALSE;
	int nBoardIndex = 0;
	HTREEITEM htiRoot  = m_treeState.GetRootItem();
	HTREEITEM htiChild = m_treeState.GetChildItem(htiRoot);
	while(htiChild)
	{
		//�ð忨�Ƿ�ѡ��
		bCheck = (STATE_CHECKED == m_treeState.GetItemCheck(htiChild));
		if (bCheck)
		{//��λ����
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
		//û��ѡ���κΰ忨
		strRep = _T("������Ҫ��λ�İ忨ǰ�򹳣�");
		strResetState += strRep;
	}

	MessageBox(strResetState, _T("��λ���"), MB_OK|MB_ICONINFORMATION);
}

// �̻����水ť
void CMVintergrationControllerDlg::OnBnClickedToolbtnUpdate()
{
	UpdateData();

	// û�п��õİ忨
	if (m_BoardManager.IsEmpty()) return;

	CWaitCursor wait;

	CString strUpdateState;
	CString strRep;
	const CString strSucc = _T("�̻��ɹ���\r\n");
	const CString strFail = _T("�̻�ʧ�ܣ�\r\n");
	const CString strErr = _T(" û�����ɿ���ҳ��˫����������ɣ�\r\n");

	strUpdateState = _T("�̻���ɣ�\r\n\r\n��λ�����\r\n");

	//������οؼ��е�����Ŀ�Ƿ�ѡ��

	BOOL bCheck = FALSE;
	BOOL bCmdSucc = FALSE;
	int nTreeIndex = 0;
	int nTabIndex = 0;
	HTREEITEM htiRoot  = m_treeState.GetRootItem();
	HTREEITEM htiChild = m_treeState.GetChildItem(htiRoot);
	while(htiChild)
	{
		bCmdSucc = FALSE;
		//�ð忨�Ƿ�ѡ��
		bCheck = (STATE_CHECKED == m_treeState.GetItemCheck(htiChild));
		if (bCheck)
		{
			nTabIndex = m_pTabbedView->GetTabIndexByName(m_treeState.GetItemText(htiChild));
			CContainerView *pView = (CContainerView*)m_pTabbedView->GetViewByIndex(nTabIndex);
			if (pView)
			{
				if (m_BoardManager[nTreeIndex].GetProductType() == Product_MG)
				{//MG300�ı���
					bCmdSucc = pView->m_pWndMG->SaveAll();
				}
				else if (m_BoardManager[nTreeIndex].GetProductType() == Product_MVX)
				{//MVX303����ģ��ı���
					bCmdSucc = pView->m_pWndMVX->SaveAll();
				}
				else if (m_BoardManager[nTreeIndex].GetProductType() == Product_HDMI_3D)
				{//HDMI_3Dģ�鱣��
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
		//û��ѡ���κΰ忨
		strRep = _T("������Ҫ�̻��İ忨ǰ�򹳣�");
		strUpdateState += strRep;
	}

	MessageBox(strUpdateState, _T("�̻����"), MB_OK|MB_ICONINFORMATION);
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


// ����ָʾ��
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

// ����/չ����������߲���
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
	m_btnHidden.SetWindowText(bHide ? _T("չ��״̬ҳ>>") : _T("����״̬ҳ<<"));
#endif

	ExpandFuncArea(bHide);
	ResizeView();

	m_bHideLeft = bHide;
}

// ����������
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

// ���������������Ҳ�����ƶ�����
LRESULT CMVintergrationControllerDlg::OnNcHitTest(CPoint point)
{
	UINT nHitTest = CXTPResizeDialog::OnNcHitTest(point);

	return nHitTest== HTCLIENT ? HTCAPTION : nHitTest;

	return CXTPResizeDialog::OnNcHitTest(point);
}

// Ӣ��֧��
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
	strMsg = _T("This software will be shutdown, your settings already updated to the device?\r\n - Click��No��to back\r\n - Click��Yes��to quit");
	strTitle = _T("Quit Message");
#else
	strMsg = _T("���ѣ����򼴽��رգ���������Ƿ��Ѿ����浽�豸��\r\n\r\n - ���Ǳ��� ��㡸��\r\n - �Ѿ����� ��㡸�ǡ�");
	strTitle = _T("�˳�����");
#endif
	if (MessageBox(strMsg, strTitle, MB_YESNO|MB_ICONQUESTION) == IDYES) 
	{
		//SetTimer(TIMER_SHUT_MAINWND, SHUT_ECLIPSE, NULL);
		PostMessage(WM_QUIT);
	}
}
