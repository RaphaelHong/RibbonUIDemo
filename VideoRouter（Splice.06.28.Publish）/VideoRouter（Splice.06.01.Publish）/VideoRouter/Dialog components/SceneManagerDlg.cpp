// SceneManagerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "..\MainFrm.h"
#include "SceneManagerDlg.h"
#include "afxdialogex.h"


// CSceneManagerDlg �Ի���

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


// CSceneManagerDlg ��Ϣ�������


BOOL CSceneManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	//ID��΢����Χ
	m_SpinID.SetRange(1, _MAX_SCENE_COUNT);
	//�������
	SetWindowTitle();
	//��ʼ��ͼ������ѡ������б����ݳ�ʼ��֮ǰ
	InitIconBox();
	//��ʼ�������б�
	InitSceneList();

	InitToolTip();

	return TRUE;
}

void CSceneManagerDlg::InitToolTip()
{
	m_tip.Create(this);
	if (SCENE_MANAGER_LOAD == m_nManagerType)
		m_tip.AddTool(GetDlgItem(IDC_BTN_SCENE_LOCALFILE), _T("��������ͼ�а�Ctrl+'O'�򿪳�����"));
	else if (SCENE_MANAGER_SAVE == m_nManagerType)
		m_tip.AddTool(GetDlgItem(IDC_BTN_SCENE_LOCALFILE), _T("��������ͼ�а�Ctrl+'S'���泡����"));

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

	//��������Ի����е�ͼ����Դ��RibbonBar���õ�ͼ����Դ��һ��
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

	m_ListScene.InsertColumn(0, _T("������"), LVCFMT_LEFT, 100);
	m_ListScene.InsertColumn(1, _T("��������"), LVCFMT_LEFT, 100);
	m_ListScene.InsertColumn(2, _T("��ݲ˵�"), LVCFMT_LEFT, 80);

	m_ListScene.SetImageList(&m_imglist16, LVSIL_SMALL);

	UpdateListItem(0);
}

void CSceneManagerDlg::UpdateListItem(int nFousedItem)
{
	//������б�����
	m_ListScene.DeleteAllItems();

	CSceneHeadInfo* pScene = NULL;
	int nCount = GetSceneManager()->GetCount();

	//��������б�����
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
			pszText = "��";
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
	ComboListItem.mask=CBEIF_IMAGE|CBEIF_OVERLAY|CBEIF_SELECTEDIMAGE|CBEIF_TEXT;// ��ģʽ��
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
		SetWindowText(_T("��������"));
	}
	else if (m_nManagerType == SCENE_MANAGER_SAVE)
	{
		SetWindowText(_T("��������"));
	}
	else
		SetWindowText(_T("��������"));
}

void CSceneManagerDlg::OnCbnSelchangeCboxexSceneIcon()
{
	int nIcon = m_ExCBoxIcon.GetCurSel();

	int nItem = GetCursorItem();
	if (nItem == -1) 
		return;

	LVITEM lvItem = {0};
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;   // ���֡�ͼƬ��״̬
	lvItem.iItem = nItem;                            // �к�(��һ��)
	lvItem.iSubItem = 0;                             // ���к�
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

		//��ȡ��һ��ֵ
		lvItem.iItem = nItem;
		lvItem.iSubItem = 0;
		lvItem.pszText = new CHAR[_MAX_SCENE_NAME];
		lvItem.cchTextMax = _MAX_SCENE_NAME;
		m_ListScene.GetItem(&lvItem);
		int nImage = lvItem.iImage;
		m_ExCBoxIcon.SetCurSel(nImage);

		if (m_nManagerType == SCENE_MANAGER_LOAD)
			SetDlgItemText(IDC_EDIT_SCENE_ID, lvItem.pszText);

		//��ȡ�ڶ���ֵ
		lvItem.mask = LVIF_TEXT|LVIF_STATE;
		lvItem.iSubItem = 1;
		m_ListScene.GetItem(&lvItem);
		SetDlgItemText(IDC_EDIT_SCENE_NAME, lvItem.pszText);
		if (m_nManagerType == SCENE_MANAGER_LOAD)
			SetDlgItemText(IDC_EDIT_SCENE_SAVENAME, lvItem.pszText);

		//��ȡ������ֵ
		lvItem.iSubItem = 2;
		m_ListScene.GetItem(&lvItem);

		if (strstr(lvItem.pszText, "��"))
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
		m_ListScene.SetItemText(nItem, 2, "��");
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

	//��ȡɾ������Ϣ
	LVITEM lvi = {0};
	lvi.mask = LVIF_TEXT|LVIF_STATE|LVIF_IMAGE;
	lvi.pszText = new CHAR[_MAX_SCENE_NAME];
	lvi.cchTextMax = _MAX_SCENE_NAME;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	m_ListScene.GetItem(&lvi);

	//ɾ���������������иó�������
	int nSceneID = atoi(lvi.pszText);
	GetSceneManager()->RemoveAt(nSceneID, TRUE);

	//���¼��س�����ť�������˵�
	GetMainFrame()->ArrangeSceneMenus();

	//ɾ���б�ؼ��еĸ���
	m_ListScene.DeleteItem(nItem);
	m_ListScene.SetFocus();

	//���������õ���һ����û����һ����ŵ���һ��
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
		//�������洦��
		SaveScene();
	}
	else if (m_nManagerType == SCENE_MANAGER_LOAD)
	{
		//�������ش���
		LoadScene();
	}
	else if (m_nManagerType == SCENE_MANAGER_EDIT)
	{
		//�༭��������
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

	//���������ر��水ť
	if (m_nManagerType == SCENE_MANAGER_SAVE)
	{
		//Ϊ�û�Ԥ��ѡһ�����õĳ���ID
		GetUnusedSceneID();
		SetDlgItemText(IDOK, "����");
	}
	else if (m_nManagerType == SCENE_MANAGER_LOAD)
		SetDlgItemText(IDOK, "��ȡ");
	else if (m_nManagerType == SCENE_MANAGER_EDIT)
		SetDlgItemText(IDOK, "����");
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


// ���泡��
void CSceneManagerDlg::SaveScene()
{
	CWaitCursor wait;

	UpdateData();

	int nSaveID = m_nSceneID;
	CString strScene = m_strSaveName;

	//���Ƴ���ID��Χ
	if (nSaveID > _MAX_SCENE_COUNT || nSaveID < 1)
	{
		MessageBox(_T("����ID��ΧӦ��1-123֮�䣡"), _T("��������"), MB_OK|MB_ICONWARNING);
		return ;
	}

	//�������Ʋ���Ϊ��
	if (strScene.IsEmpty())
	{
		MessageBox(_T("�����볡�����ƣ�"), _T("��������"), MB_OK|MB_ICONWARNING);
		return ;
	}

	//��ȡ������
	int nRet = CMD_FAILURE;

	// ����ID�Ƿ��ѱ�ռ��
	if (GetSceneManager()->FindItemByID(nSaveID) != NULL)
	{
		int nYesNo = MessageBox(_T("�ó���ID�ѱ�ʹ�ã�Ҫ���Ǵ˳�������ô��"), _T("��������"), MB_YESNO|MB_ICONWARNING);
		if (nYesNo != IDYES)
			return;

		// ���ͳ�����������
		nRet = GetCmdManager()->CmdSAVE(SYSBOARD_PORT, eBoardTypeCtrl, nSaveID);
	}
	else
	{
		// ���ͳ�����������
		nRet = GetCmdManager()->CmdSAVE(SYSBOARD_PORT, eBoardTypeCtrl, nSaveID);

		// ����ɹ������³���������
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

	// ��ʾ���洦����
	if (nRet != CMD_RTOK)
	{
		MessageBox(_T("err�����泡��ʧ�ܣ�"), _T("��������"), MB_OK|MB_ICONSTOP);
		return;
	}
	else
		MessageBox(_T("�����ѱ��棡"), _T("��������"), MB_OK|MB_ICONINFORMATION);

	//���¼��ز˵��µ��Ӳ˵���
	GetMainFrame()->ArrangeSceneMenus();

	//�����б��
	UpdateListItem(0);
}

// ���س���
void CSceneManagerDlg::LoadScene()
{
	if (GetSceneManager()->GetCount() == 0)
		return ;

	UpdateData();	

	GetMainFrame()->LoadScene(m_nSceneID);
}

// �༭��������
void CSceneManagerDlg::EditScene()
{
	if (GetSceneManager()->GetCount() == 0)
		return ;

	//��ȡ�б��и�������Ϣ
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
		//����ID
		nSceneID = atoi(lvi.pszText);

		//����ͼ���
		nImage = lvi.iImage;

		//������
		lvi.iSubItem = 1;
		m_ListScene.GetItem(&lvi);
		strSceneName.Format(_T("%s"), lvi.pszText);

		//�Ƿ��ݲ˵�
		lvi.iSubItem = 2;
		m_ListScene.GetItem(&lvi);
		if (strstr(lvi.pszText, "��"))
			bQuickMenu = TRUE;
		else
			bQuickMenu = FALSE;

		//���б�����Ϣ���µ����������ļ�
		CSceneHeadInfo* pScene = GetSceneManager()->FindItemByID(nSceneID);
		if (NULL == pScene)
			continue;

		pScene->GetLargeImage() = nImage;
		pScene->GetSmallImage() = nImage;
		pScene->GetQuickMenuFlag() = bQuickMenu;
		pScene->SetName(strSceneName.GetBuffer());
	}

	//���µ������ļ�
	GetSceneManager()->WriteToFile(NULL);

	//���¼��ز˵��µ��Ӳ˵���
	GetMainFrame()->ArrangeSceneMenus();

	delete [] lvi.pszText;
}

//��ȡ��δ��ռ�õĳ���ID��
void CSceneManagerDlg::GetUnusedSceneID()
{
	m_nSceneID = GetSceneManager()->PrepareID();

	m_strSaveName.Format(_T("����%d"), m_nSceneID);

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
		// ���ر��س���
		CFileDialog OpenFile(TRUE, _T(".scr"), _T("Splice"), OFN_HIDEREADONLY|OFN_READONLY, "scr File(*.scr)|*.scr|All Files (*.*)|*.*|");
		OpenFile.m_ofn.lpstrInitialDir = sysconf.GetDefaultSceneFilePath();
		if (OpenFile.DoModal() == IDOK)
		{
			GetSceneManager()->NotifyToUnpackFile(OpenFile.GetPathName());
		}
	}
	else if ( SCENE_MANAGER_SAVE == m_nManagerType )
	{
		// ���浽���س���
		CFileDialog SaveFile(FALSE, _T(".scr"), _T("Splice"), OFN_HIDEREADONLY|OFN_READONLY, "scr File(*.scr)|*.scr|All Files (*.*)|*.*|");
		SaveFile.m_ofn.lpstrInitialDir = sysconf.GetDefaultSceneFilePath();
		if (SaveFile.DoModal() == IDOK)
		{
			GetSceneManager()->NotifyToPackageFile(SaveFile.GetPathName());
		}
	}
}
