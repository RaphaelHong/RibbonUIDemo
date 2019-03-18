// SceneManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "..\MainFrm.h"
#include "SceneManagerDlg.h"
#include "afxdialogex.h"


// CSceneManagerDlg 对话框

IMPLEMENT_DYNAMIC(CSceneManagerDlg, CDialog)

CSceneManagerDlg::CSceneManagerDlg(CWnd* pParent /*=NULL*/)
: CDialog(CSceneManagerDlg::IDD, pParent)
, m_nSceneID(1)
{

}

CSceneManagerDlg::CSceneManagerDlg(int nManagerType, CWnd* pParent /*=NULL*/)
: CDialog(CSceneManagerDlg::IDD, pParent)
, m_nSceneID(1)
{
	m_nManagerType = nManagerType;
}

CSceneManagerDlg::~CSceneManagerDlg()
{
}

void CSceneManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SCENE, m_ListScene);
	DDX_Control(pDX, IDC_CBOXEx_SCENE_ICON, m_ExCBoxIcon);
	DDX_Control(pDX, IDC_CHECK_QUICK_MENU, m_checkQuickMenu);
	DDX_Text(pDX, IDC_EDIT_SCENE_ID, m_nSceneID);
	DDV_MinMaxInt(pDX, m_nSceneID, 1, _MAX_SCENE_COUNT);
	DDX_Text(pDX, IDC_EDIT_SCENE_SAVENAME, m_strSaveName);
	DDX_Control(pDX, IDC_SPIN_SCENE_ID, m_SpinID);
}


BEGIN_MESSAGE_MAP(CSceneManagerDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_CBOXEx_SCENE_ICON, &CSceneManagerDlg::OnCbnSelchangeCboxexSceneIcon)
	ON_EN_CHANGE(IDC_EDIT_SCENE_NAME, OnEnChangedSceneNewName)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SCENE, &CSceneManagerDlg::OnLvnItemchangedListScene)
	ON_BN_CLICKED(IDC_CHECK_QUICK_MENU, &CSceneManagerDlg::OnBnClickedCheckQuickMenu)
	ON_BN_CLICKED(IDC_BTN_SCENE_DELETE, &CSceneManagerDlg::OnBnClickedBtnSceneDelete)
	ON_BN_CLICKED(IDOK, &CSceneManagerDlg::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SCENE, &CSceneManagerDlg::OnNMDblclkListScene)
	ON_BN_CLICKED(IDC_BTN_SCENE_LOCALFILE, &CSceneManagerDlg::OnBnClickedBtnSceneLocalfile)
END_MESSAGE_MAP()


// CSceneManagerDlg 消息处理程序


BOOL CSceneManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	//ID号微调范围
	m_SpinID.SetRange(1, _MAX_SCENE_COUNT);
	//窗体标题
	SetWindowTitle();
	//初始化图标下拉选择框，在列表数据初始化之前
	InitIconBox();
	//初始化场景列表
	InitSceneList();

	InitToolTip();

	return TRUE;
}

void CSceneManagerDlg::InitToolTip()
{
	m_tip.Create(this);
	if (SCENE_MANAGER_LOAD == m_nManagerType)
		m_tip.AddTool(GetDlgItem(IDC_BTN_SCENE_LOCALFILE), _T("可以在视图中按Ctrl+'O'打开场景！"));
	else if (SCENE_MANAGER_SAVE == m_nManagerType)
		m_tip.AddTool(GetDlgItem(IDC_BTN_SCENE_LOCALFILE), _T("可以在视图中按Ctrl+'S'保存场景！"));

	m_tip.SetDelayTime(200);
}

void CSceneManagerDlg::InitSceneList()
{
	DWORD dwStyle;
	dwStyle = m_ListScene.GetExtendedStyle(); 
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES;
	m_ListScene.SetExtendedStyle(dwStyle); 

	m_ListScene.EnableUserListColor(true);
	m_ListScene.EnableUserRowColor(true);
	m_ListScene.SetListBackColor(ListColorBackground);

	//场景管理对话框中的图标资源与RibbonBar所用的图标来源不一样
	HICON icon[9];
	icon[0] = AfxGetApp()->LoadIcon(IDI_ICON_BOARD);
	icon[1] = AfxGetApp()->LoadIcon(IDI_ICON_EXCEL);
	icon[2] = AfxGetApp()->LoadIcon(IDI_ICON_FILE);
	icon[3] = AfxGetApp()->LoadIcon(IDI_ICON_GRAPH);
	icon[4] = AfxGetApp()->LoadIcon(IDI_ICON_MEDIA);
	icon[5] = AfxGetApp()->LoadIcon(IDI_ICON_STRUCT);
	icon[6] = AfxGetApp()->LoadIcon(IDI_ICON_VISO);
	icon[7] = AfxGetApp()->LoadIcon(IDI_ICON_WINDOWS);
	icon[8] = AfxGetApp()->LoadIcon(IDI_ICON_WORD);

	m_imglist16.Create(16, 16, ILC_COLOR24, 9, 9);
	m_imglist16.Add(icon[0]);
	m_imglist16.Add(icon[1]);
	m_imglist16.Add(icon[2]);
	m_imglist16.Add(icon[3]);
	m_imglist16.Add(icon[4]);
	m_imglist16.Add(icon[5]);
	m_imglist16.Add(icon[6]);
	m_imglist16.Add(icon[7]);
	m_imglist16.Add(icon[8]);

	m_ListScene.InsertColumn(0, _T("场景号"), LVCFMT_LEFT, 100);
	m_ListScene.InsertColumn(1, _T("场景名称"), LVCFMT_LEFT, 100);
	m_ListScene.InsertColumn(2, _T("快捷菜单"), LVCFMT_LEFT, 80);

	m_ListScene.SetImageList(&m_imglist16, LVSIL_SMALL);

	UpdateListItem(0);
}

void CSceneManagerDlg::UpdateListItem(int nFousedItem)
{
	//先清除列表内容
	m_ListScene.DeleteAllItems();

	CSceneHeadInfo* pScene = NULL;
	int nCount = GetSceneManager()->GetCount();

	//重新添加列表内容
	LVITEM lvItem={0};
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;
	lvItem.pszText = new CHAR[_MAX_SCENE_NAME];
	lvItem.cchTextMax = _MAX_SCENE_NAME;
	char *pszText = NULL;
	int nScID = -1;
	for (int i=0; i<nCount; i++)
	{
		pScene = GetSceneManager()->GetAt(i);

		lvItem.iItem = i;
		lvItem.iImage = pScene->GetLargeImage();
		lvItem.iSubItem = 0;
		nScID = pScene->GetID();
		_itoa_s(nScID, lvItem.pszText, _MAX_SCENE_NAME, 10);
		m_ListScene.InsertItem(&lvItem);

		m_ListScene.SetItemText(i, 1, pScene->GetName());
		if (pScene->GetQuickMenuFlag())
			pszText = "√";
		else
			pszText = "";
		m_ListScene.SetItemText(i, 2, pszText);
	}

	delete [] lvItem.pszText;

	if (nCount > 0 && nFousedItem < nCount)
		m_ListScene.SetItemState(nFousedItem, LVIS_SELECTED, LVIS_SELECTED);
}

void CSceneManagerDlg::InitIconBox()
{
	LPSTR szText[9] = {
		"Board",
		"Excel",
		"File",
		"Graph",
		"Media",
		"Struct",
		"Viso",
		"Windows",
		"Word"
	};

	HICON icon[9];
	icon[0] = AfxGetApp()->LoadIcon(IDI_ICON_BOARD);
	icon[1] = AfxGetApp()->LoadIcon(IDI_ICON_EXCEL);
	icon[2] = AfxGetApp()->LoadIcon(IDI_ICON_FILE);
	icon[3] = AfxGetApp()->LoadIcon(IDI_ICON_GRAPH);
	icon[4] = AfxGetApp()->LoadIcon(IDI_ICON_MEDIA);
	icon[5] = AfxGetApp()->LoadIcon(IDI_ICON_STRUCT);
	icon[6] = AfxGetApp()->LoadIcon(IDI_ICON_VISO);
	icon[7] = AfxGetApp()->LoadIcon(IDI_ICON_WINDOWS);
	icon[8] = AfxGetApp()->LoadIcon(IDI_ICON_WORD);

	m_imglist32.Create(32,32,ILC_COLOR24, 9, 9);
	m_imglist32.Add(icon[0]);
	m_imglist32.Add(icon[1]);
	m_imglist32.Add(icon[2]);
	m_imglist32.Add(icon[3]);
	m_imglist32.Add(icon[4]);
	m_imglist32.Add(icon[5]);
	m_imglist32.Add(icon[6]);
	m_imglist32.Add(icon[7]);
	m_imglist32.Add(icon[8]); 


	m_ExCBoxIcon.SetImageList(&m_imglist32);

	COMBOBOXEXITEM ComboListItem;
	ComboListItem.mask=CBEIF_IMAGE|CBEIF_OVERLAY|CBEIF_SELECTEDIMAGE|CBEIF_TEXT;// （模式）
	for (int nItem = 0; nItem<9; nItem++) {
		ComboListItem.iItem = nItem;
		ComboListItem.iImage = nItem;
		ComboListItem.iSelectedImage = nItem;
		ComboListItem.iOverlay = nItem;
		ComboListItem.pszText = szText[nItem];
		m_ExCBoxIcon.InsertItem(&ComboListItem);
	}
}


void CSceneManagerDlg::SetWindowTitle()
{
	//CString strTitle;
	if (m_nManagerType == SCENE_MANAGER_LOAD)
	{
		//strTitle.LoadString(ID_VGA_PROPERTY);
		//SetWindowText(strTitle);
		SetWindowText(_T("场景加载"));
	}
	else if (m_nManagerType == SCENE_MANAGER_SAVE)
	{
		SetWindowText(_T("场景保存"));
	}
	else
		SetWindowText(_T("场景管理"));
}

void CSceneManagerDlg::OnCbnSelchangeCboxexSceneIcon()
{
	int nIcon = m_ExCBoxIcon.GetCurSel();

	int nItem = GetCursorItem();
	if (nItem == -1) 
		return;

	LVITEM lvItem = {0};
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;   // 文字、图片、状态
	lvItem.iItem = nItem;                            // 行号(第一行)
	lvItem.iSubItem = 0;                             // 子列号
	lvItem.pszText = new CHAR[_MAX_SCENE_NAME];
	lvItem.cchTextMax = _MAX_SCENE_NAME;
	m_ListScene.GetItem(&lvItem);

	lvItem.iImage = nIcon;
	m_ListScene.SetItem(&lvItem);

	delete [] lvItem.pszText;
}


void CSceneManagerDlg::OnEnChangedSceneNewName()
{
	UpdateData();
	LPTSTR pszText = new CHAR[_MAX_SCENE_NAME];
	GetDlgItemText(IDC_EDIT_SCENE_NAME, pszText, _MAX_SCENE_NAME);

	int nItem = GetCursorItem();
	if (nItem == -1)
	{
		delete [] pszText;
		return;
	}

	LVITEM lvItem = {0};
	lvItem.mask = LVIF_TEXT|LVIF_STATE;
	lvItem.iItem = nItem;
	lvItem.iSubItem = 1;
	lvItem.pszText = pszText;
	lvItem.cchTextMax = _MAX_SCENE_NAME;
	m_ListScene.SetItem(&lvItem);

	delete [] pszText;
}

void CSceneManagerDlg::OnLvnItemchangedListScene(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int nItem = GetCursorItem();

	ShowItemInfo(nItem);

	*pResult = 0;
}

void CSceneManagerDlg::ShowItemInfo(int nItem)
{
	if (nItem == -1)
	{
		m_ExCBoxIcon.SetCurSel(-1);
		SetDlgItemText(IDC_EDIT_SCENE_NAME, "");
		m_checkQuickMenu.SetCheck(0);
	}
	else
	{
		LVITEM lvItem = {0};

		lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;

		//获取第一列值
		lvItem.iItem = nItem;
		lvItem.iSubItem = 0;
		lvItem.pszText = new CHAR[_MAX_SCENE_NAME];
		lvItem.cchTextMax = _MAX_SCENE_NAME;
		m_ListScene.GetItem(&lvItem);
		int nImage = lvItem.iImage;
		m_ExCBoxIcon.SetCurSel(nImage);

		if (m_nManagerType == SCENE_MANAGER_LOAD)
			SetDlgItemText(IDC_EDIT_SCENE_ID, lvItem.pszText);

		//获取第二列值
		lvItem.mask = LVIF_TEXT|LVIF_STATE;
		lvItem.iSubItem = 1;
		m_ListScene.GetItem(&lvItem);
		SetDlgItemText(IDC_EDIT_SCENE_NAME, lvItem.pszText);
		if (m_nManagerType == SCENE_MANAGER_LOAD)
			SetDlgItemText(IDC_EDIT_SCENE_SAVENAME, lvItem.pszText);

		//获取第三列值
		lvItem.iSubItem = 2;
		m_ListScene.GetItem(&lvItem);

		if (strstr(lvItem.pszText, "√"))
			m_checkQuickMenu.SetCheck(1);
		else 
			m_checkQuickMenu.SetCheck(0);

		delete [] lvItem.pszText;
	}
}


int CSceneManagerDlg::GetCursorItem()
{
	if (m_ListScene.GetItemCount() == 0) return -1;

	int nItem = -1;

	for (int i=0; i<m_ListScene.GetItemCount(); i++) {
		if (m_ListScene.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED) {
			nItem = i;
			break;
		}
	}

	return nItem;
}

void CSceneManagerDlg::OnBnClickedCheckQuickMenu()
{
	UpdateData();

	int nCheck = m_checkQuickMenu.GetCheck();

	int nItem = GetCursorItem();
	if (nItem == -1) 
		return;

	if (nCheck) 
	{
		m_ListScene.SetItemText(nItem, 2, "√");
	}
	else
	{
		m_ListScene.SetItemText(nItem, 2, "");
	}
}


void CSceneManagerDlg::OnBnClickedBtnSceneDelete()
{
	int nItem = GetCursorItem();
	if (nItem == -1) 
		return;

	//获取删除项信息
	LVITEM lvi = {0};
	lvi.mask = LVIF_TEXT|LVIF_STATE|LVIF_IMAGE;
	lvi.pszText = new CHAR[_MAX_SCENE_NAME];
	lvi.cchTextMax = _MAX_SCENE_NAME;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	m_ListScene.GetItem(&lvi);

	//删除场景管理数组中该场景数据
	int nSceneID = atoi(lvi.pszText);
	GetSceneManager()->RemoveAt(nSceneID, TRUE);

	//更新加载场景按钮的下拉菜单
	GetMainFrame()->ArrangeSceneMenus();

	//删除列表控件中的该项
	m_ListScene.DeleteItem(nItem);
	m_ListScene.SetFocus();

	//将焦点设置到下一项，如果没有下一项则放到第一项
	int nNewCount = m_ListScene.GetItemCount();
	if (nItem < nNewCount)
	{
		m_ListScene.SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED);
	}
	else
	{
		if (nNewCount > 0)
		{
			m_ListScene.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED);
		}
	}

	delete [] lvi.pszText;
}


void CSceneManagerDlg::OnBnClickedOk()
{
	if (m_nManagerType == SCENE_MANAGER_SAVE)
	{
		//场景保存处理
		SaveScene();
	}
	else if (m_nManagerType == SCENE_MANAGER_LOAD)
	{
		//场景加载处理
		LoadScene();
	}
	else if (m_nManagerType == SCENE_MANAGER_EDIT)
	{
		//编辑场景属性
		EditScene();
	}
}


void CSceneManagerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	PreShowWindow();
}

void CSceneManagerDlg::PreShowWindow()
{
	ShowFuncCtrls();

	ResizeClientRect();

	//重命名加载保存按钮
	if (m_nManagerType == SCENE_MANAGER_SAVE)
	{
		//为用户预先选一个可用的场景ID
		GetUnusedSceneID();
		SetDlgItemText(IDOK, "保存");
	}
	else if (m_nManagerType == SCENE_MANAGER_LOAD)
		SetDlgItemText(IDOK, "读取");
	else if (m_nManagerType == SCENE_MANAGER_EDIT)
		SetDlgItemText(IDOK, "更新");
}

void CSceneManagerDlg::ShowFuncCtrls()
{
	BOOL bShow = m_nManagerType == SCENE_MANAGER_EDIT;

	GetDlgItem(IDC_TEXT_SCENE_ID)->ShowWindow(!bShow);
	GetDlgItem(IDC_LABEL_SCENE_SAVENAME)->ShowWindow(!bShow);
	GetDlgItem(IDC_EDIT_SCENE_ID)->ShowWindow(!bShow);
	GetDlgItem(IDC_SPIN_SCENE_ID)->ShowWindow(!bShow);
	GetDlgItem(IDC_EDIT_SCENE_SAVENAME)->ShowWindow(!bShow);
	//GetDlgItem(IDC_BTN_SCENE_LOADSAVE)->ShowWindow(!bShow);
	GetDlgItem(IDC_SEPARATOR_SCENE_MANAGER)->ShowWindow(!bShow);
	GetDlgItem(IDC_BTN_SCENE_LOCALFILE)->ShowWindow(!bShow);

	GetDlgItem(IDC_EDIT_SCENE_ID)->EnableWindow(m_nManagerType==SCENE_MANAGER_SAVE);
	GetDlgItem(IDC_EDIT_SCENE_SAVENAME)->EnableWindow(m_nManagerType==SCENE_MANAGER_SAVE);

	GetDlgItem(IDC_GBOX_SCENE_OPERATOR)->ShowWindow(bShow);
	GetDlgItem(IDC_LABEL_SCENE_ICON)->ShowWindow(bShow);
	GetDlgItem(IDC_LABEL_SCENE_NEWNAME)->ShowWindow(bShow);
	m_ExCBoxIcon.ShowWindow(bShow);
	GetDlgItem(IDC_EDIT_SCENE_NAME)->ShowWindow(bShow);
	GetDlgItem(IDC_CHECK_QUICK_MENU)->ShowWindow(bShow);
	GetDlgItem(IDC_BTN_SCENE_DELETE)->ShowWindow(bShow);	
}

void CSceneManagerDlg::ResizeClientRect()
{
	CWnd *pWnd = NULL;

	CRect rcCli, rcNewCli;
	GetWindowRect(&rcCli);

	CRect rcCtrl, rcGboxOp;
	GetDlgItem(IDC_GBOX_SCENE_OPERATOR)->GetWindowRect(&rcGboxOp);
	ScreenToClient(&rcGboxOp);

	rcNewCli = CRect(rcCli.left, rcCli.top, rcCli.right, rcCli.bottom-rcGboxOp.Height());

	int nVoffset = 0;
	if (m_nManagerType != SCENE_MANAGER_EDIT)
	{
		nVoffset  = rcGboxOp.Height();

		pWnd = GetDlgItem(IDC_TEXT_SCENE_ID);
		pWnd->GetWindowRect(&rcCtrl);
		ScreenToClient(&rcCtrl);
		pWnd->MoveWindow(rcCtrl.left, rcCtrl.top-nVoffset, rcCtrl.Width(), rcCtrl.Height());

		pWnd = GetDlgItem(IDC_LABEL_SCENE_SAVENAME);
		pWnd->GetWindowRect(&rcCtrl);
		ScreenToClient(&rcCtrl);
		pWnd->MoveWindow(rcCtrl.left, rcCtrl.top-nVoffset, rcCtrl.Width(), rcCtrl.Height());

		pWnd = GetDlgItem(IDC_EDIT_SCENE_ID);
		pWnd->GetWindowRect(&rcCtrl);
		ScreenToClient(&rcCtrl);
		pWnd->MoveWindow(rcCtrl.left, rcCtrl.top-nVoffset, rcCtrl.Width(), rcCtrl.Height());

		pWnd = GetDlgItem(IDC_SPIN_SCENE_ID);
		pWnd->GetWindowRect(&rcCtrl);
		ScreenToClient(&rcCtrl);
		pWnd->MoveWindow(rcCtrl.left, rcCtrl.top-nVoffset, rcCtrl.Width(), rcCtrl.Height());

		pWnd = GetDlgItem(IDC_EDIT_SCENE_SAVENAME);
		pWnd->GetWindowRect(&rcCtrl);
		ScreenToClient(&rcCtrl);
		pWnd->MoveWindow(rcCtrl.left, rcCtrl.top-nVoffset, rcCtrl.Width(), rcCtrl.Height());

		//pWnd = GetDlgItem(IDC_BTN_SCENE_LOADSAVE);
		//pWnd->GetWindowRect(&rcCtrl);
		//ScreenToClient(&rcCtrl);
		//pWnd->MoveWindow(rcCtrl.left, rcCtrl.top-nVoffset, rcCtrl.Width(), rcCtrl.Height());
	}
	else
	{
		nVoffset = rcGboxOp.Height()-18;
		rcNewCli.InflateRect(0, 0, 0, 18);
	}

	pWnd = GetDlgItem(IDC_SEPARATOR_SCENE_MANAGER);
	pWnd->GetWindowRect(&rcCtrl);
	ScreenToClient(&rcCtrl);
	pWnd->MoveWindow(rcCtrl.left, rcCtrl.top-nVoffset, rcCtrl.Width(), rcCtrl.Height());

	pWnd = GetDlgItem(IDC_BTN_SCENE_LOCALFILE);
	pWnd->GetWindowRect(&rcCtrl);
	ScreenToClient(&rcCtrl);
	pWnd->MoveWindow(rcCtrl.left, rcCtrl.top-nVoffset, rcCtrl.Width(), rcCtrl.Height());

	pWnd = GetDlgItem(IDOK);
	pWnd->GetWindowRect(&rcCtrl);
	ScreenToClient(&rcCtrl);
	pWnd->MoveWindow(rcCtrl.left, rcCtrl.top-nVoffset, rcCtrl.Width(), rcCtrl.Height());

	pWnd = GetDlgItem(IDCANCEL);
	pWnd->GetWindowRect(&rcCtrl);
	ScreenToClient(&rcCtrl);
	pWnd->MoveWindow(rcCtrl.left, rcCtrl.top-nVoffset, rcCtrl.Width(), rcCtrl.Height());

	SetWindowPos(NULL, rcNewCli.left, rcNewCli.top, rcNewCli.Width(), rcNewCli.Height(), SWP_SHOWWINDOW);
}


BOOL CSceneManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	m_tip.RelayEvent(pMsg);

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE
			||pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


// 保存场景
void CSceneManagerDlg::SaveScene()
{
	CWaitCursor wait;

	UpdateData();

	int nSaveID = m_nSceneID;
	CString strScene = m_strSaveName;

	//限制场景ID范围
	if (nSaveID > _MAX_SCENE_COUNT || nSaveID < 1)
	{
		MessageBox(_T("场景ID范围应在1-123之间！"), _T("场景保存"), MB_OK|MB_ICONWARNING);
		return ;
	}

	//场景名称不能为空
	if (strScene.IsEmpty())
	{
		MessageBox(_T("请输入场景名称！"), _T("场景保存"), MB_OK|MB_ICONWARNING);
		return ;
	}

	//获取命令结果
	int nRet = CMD_FAILURE;

	// 场景ID是否已被占用
	if (GetSceneManager()->FindItemByID(nSaveID) != NULL)
	{
		int nYesNo = MessageBox(_T("该场景ID已被使用，要覆盖此场景数据么？"), _T("场景保存"), MB_YESNO|MB_ICONWARNING);
		if (nYesNo != IDYES)
			return;

		// 发送场景保存命令
		nRet = GetCmdManager()->CmdSAVE(SYSBOARD_PORT, eBoardTypeCtrl, nSaveID);
	}
	else
	{
		// 发送场景保存命令
		nRet = GetCmdManager()->CmdSAVE(SYSBOARD_PORT, eBoardTypeCtrl, nSaveID);

		// 保存成功，更新场景管理器
		if (nRet == CMD_RTOK)
		{
			CSceneHeadInfo* pNewScene = new CSceneHeadInfo;
			pNewScene->GetID() = nSaveID;
			pNewScene->GetLargeImage() = nSaveID%9;
			pNewScene->GetSmallImage() = nSaveID%9;
			pNewScene->GetExcuteTime() = 5000;
			pNewScene->GetRemainTime() = 5000;
			pNewScene->SetName(strScene.GetBuffer());
			pNewScene->GetQuickMenuFlag() = FALSE;

			GetSceneManager()->AddItem(pNewScene);
		}
	}

	// 提示保存处理结果
	if (nRet != CMD_RTOK)
	{
		MessageBox(_T("err，保存场景失败！"), _T("场景保存"), MB_OK|MB_ICONSTOP);
		return;
	}
	else
		MessageBox(_T("场景已保存！"), _T("场景保存"), MB_OK|MB_ICONINFORMATION);

	//更新加载菜单下的子菜单项
	GetMainFrame()->ArrangeSceneMenus();

	//更新列表框
	UpdateListItem(0);
}

// 加载场景
void CSceneManagerDlg::LoadScene()
{
	if (GetSceneManager()->GetCount() == 0)
		return ;

	UpdateData();	

	GetMainFrame()->LoadScene(m_nSceneID);
}

// 编辑场景属性
void CSceneManagerDlg::EditScene()
{
	if (GetSceneManager()->GetCount() == 0)
		return ;

	//获取列表中各场景信息
	CString strSceneName;
	BOOL bQuickMenu = FALSE;
	int nSceneID = INVALID_VALUE;
	int nImage = INVALID_VALUE;	

	LVITEM lvi = {0};
	lvi.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;
	lvi.pszText = new CHAR[_MAX_SCENE_NAME];
	lvi.cchTextMax = _MAX_SCENE_NAME;

	for (int iItem=0; iItem<m_ListScene.GetItemCount(); iItem++)
	{		
		lvi.iItem = iItem;
		lvi.iSubItem = 0;
		m_ListScene.GetItem(&lvi);
		//场景ID
		nSceneID = atoi(lvi.pszText);

		//场景图标号
		nImage = lvi.iImage;

		//场景名
		lvi.iSubItem = 1;
		m_ListScene.GetItem(&lvi);
		strSceneName.Format(_T("%s"), lvi.pszText);

		//是否快捷菜单
		lvi.iSubItem = 2;
		m_ListScene.GetItem(&lvi);
		if (strstr(lvi.pszText, "√"))
			bQuickMenu = TRUE;
		else
			bQuickMenu = FALSE;

		//将列表项信息更新到场景配置文件
		CSceneHeadInfo* pScene = GetSceneManager()->FindItemByID(nSceneID);
		if (NULL == pScene)
			continue;

		pScene->GetLargeImage() = nImage;
		pScene->GetSmallImage() = nImage;
		pScene->GetQuickMenuFlag() = bQuickMenu;
		pScene->SetName(strSceneName.GetBuffer());
	}

	//更新到配置文件
	GetSceneManager()->WriteToFile(NULL);

	//更新加载菜单下的子菜单项
	GetMainFrame()->ArrangeSceneMenus();

	delete [] lvi.pszText;
}

//获取还未被占用的场景ID号
void CSceneManagerDlg::GetUnusedSceneID()
{
	m_nSceneID = GetSceneManager()->PrepareID();

	m_strSaveName.Format(_T("场景%d"), m_nSceneID);

	UpdateData(FALSE);
}

void CSceneManagerDlg::OnNMDblclkListScene(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int nItem = pNMItemActivate->iItem;
	if (-1 == nItem)
	{
		*pResult = 0;

		return;
	}

	if (SCENE_MANAGER_LOAD == m_nManagerType)
	{
		LoadScene();
	}
	else if (SCENE_MANAGER_SAVE == m_nManagerType)
	{
		LPTSTR szItem = new CHAR[_MAX_SCENE_NAME];
		m_ListScene.GetItemText(nItem, 0, szItem, 16);
		m_nSceneID = atoi(szItem);
		m_ListScene.GetItemText(nItem, 1, szItem, _MAX_SCENE_NAME);
		m_strSaveName = szItem;
		UpdateData(FALSE);

		SaveScene();

		delete [] szItem;
	}

	*pResult = 0;
}

void CSceneManagerDlg::OnBnClickedBtnSceneLocalfile()
{
	CSysConfManager sysconf;
	if ( SCENE_MANAGER_LOAD == m_nManagerType )
	{
		// 加载本地场景
		CFileDialog OpenFile(TRUE, _T(".scr"), _T("Splice"), OFN_HIDEREADONLY|OFN_READONLY, "scr File(*.scr)|*.scr|All Files (*.*)|*.*|");
		OpenFile.m_ofn.lpstrInitialDir = sysconf.GetDefaultSceneFilePath();
		if (OpenFile.DoModal() == IDOK)
		{
			GetSceneManager()->NotifyToUnpackFile(OpenFile.GetPathName());
		}
	}
	else if ( SCENE_MANAGER_SAVE == m_nManagerType )
	{
		// 保存到本地场景
		CFileDialog SaveFile(FALSE, _T(".scr"), _T("Splice"), OFN_HIDEREADONLY|OFN_READONLY, "scr File(*.scr)|*.scr|All Files (*.*)|*.*|");
		SaveFile.m_ofn.lpstrInitialDir = sysconf.GetDefaultSceneFilePath();
		if (SaveFile.DoModal() == IDOK)
		{
			GetSceneManager()->NotifyToPackageFile(SaveFile.GetPathName());
		}
	}
}
