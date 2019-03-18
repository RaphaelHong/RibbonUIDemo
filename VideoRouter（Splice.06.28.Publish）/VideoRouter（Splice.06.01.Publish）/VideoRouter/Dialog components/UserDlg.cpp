// UserDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "UserDlg.h"


// CUserDlg �Ի���

IMPLEMENT_DYNAMIC(CUserDlg, CDialog)

CUserDlg::CUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUserDlg::IDD, pParent)
{
	m_opt = opt_non;
}

CUserDlg::~CUserDlg()
{
}

void CUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER, m_UserList);
	DDX_Control(pDX, IDC_BTN_ADD_USER, m_btnAdd);
	DDX_Control(pDX, IDC_BTN_EDIT_USER, m_btnMod);
	DDX_Control(pDX, IDC_BTN_SAVE_USER, m_btnSav);
	DDX_Control(pDX, IDC_BTN_DEL_USER, m_btnDel);
	DDX_Control(pDX, IDC_CBOX_USER_AUTHORITY, m_cboxAuthority);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_USER_PWD, m_strUserPwd);
	DDX_Text(pDX, IDC_EDIT_USER_PWD_REPEAT, m_strUserPwdRept);
}


BEGIN_MESSAGE_MAP(CUserDlg, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_USER, &CUserDlg::OnLvnItemchangedListUser)
	ON_BN_CLICKED(IDC_BTN_ADD_USER, &CUserDlg::OnBtnAddUser)
	ON_BN_CLICKED(IDC_BTN_EDIT_USER, &CUserDlg::OnBtnModUser)
	ON_BN_CLICKED(IDC_BTN_SAVE_USER, &CUserDlg::OnBtnSavUser)
	ON_BN_CLICKED(IDC_BTN_DEL_USER, &CUserDlg::OnBtnDelUser)
	ON_CBN_SELCHANGE(IDC_CBOX_USER_AUTHORITY, &CUserDlg::OnCbnSelchangeCboxUserAuthority)
END_MESSAGE_MAP()


// CUserDlg ��Ϣ�������

BOOL CUserDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SUPERADMIN)), FALSE);

	m_btnAdd.SetImage(IDB_PNG_USER_ADD16);
	m_btnMod.SetImage(IDB_PNG_USER_EDIT16);
	m_btnSav.SetImage(IDB_PNG_USER_ACCEPT16);
	m_btnDel.SetImage(IDB_PNG_USER_REMOVE16);

	InitUserList();

	EnableEditUserInfo(FALSE);

	return TRUE;
}

void CUserDlg::InitUserList()
{
	DWORD dwStyle;
	dwStyle = m_UserList.GetExtendedStyle(); 
	dwStyle = dwStyle|LVS_EX_FULLROWSELECT|LVS_EX_SUBITEMIMAGES|LVS_EX_GRIDLINES;
	m_UserList.SetExtendedStyle(dwStyle);


	HICON icon[2];
	icon[0] = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_ADMIN));
	icon[1] = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_GENERAL_USER));

	m_imglistUser.Create(16,16,ILC_COLOR24, 2, 2);
	m_imglistUser.Add(icon[0]);
	m_imglistUser.Add(icon[1]);

	m_UserList.SetImageList(&m_imglistUser, LVSIL_SMALL);
	m_UserList.InsertColumn(0, _T("ͼ��"), LVCFMT_LEFT, 50);
	m_UserList.InsertColumn(1, _T("�û���"), LVCFMT_LEFT, 100);
	m_UserList.InsertColumn(2, _T("Ȩ��"), LVCFMT_CENTER, 100);

	LVITEM lvi = {0};
	lvi.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;
	//lvi.pszText = new char[_MAX_UNAME];
	//lvi.cchTextMax = _MAX_UNAME;

	pUserData pUser=NULL;

	for (int i=0; i<m_userManager.GetUserCount(); i++)
	{
		pUser = m_userManager.GetUser(i);
		if (pUser == NULL)
			continue;

		lvi.iItem = i;
		lvi.iImage = pUser->eAuthority == eAdministrator ? 0 : 1;
		lvi.iSubItem = 0;
		lvi.pszText = "";
		m_UserList.InsertItem(&lvi);

		m_UserList.SetItemText(i, 1, pUser->szUserName);
		m_UserList.SetItemText(i, 2, pUser->eAuthority == eAdministrator ? "����Ա" : "��ͨ�û�");
	}

	if (m_userManager.GetUserCount() > 0)
		GetUserInfo(0);
}

void CUserDlg::OnLvnItemchangedListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	int iItem = GetCursorItem();

	GetUserInfo(iItem);

	*pResult = 0;
}

int CUserDlg::GetCursorItem()
{
	for (int i=0; i<m_UserList.GetItemCount(); i++)
	{
		if (m_UserList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			return i;
		}
	}

	return -1;
}

void CUserDlg::GetUserInfo(int iItem)
{
	if (iItem < 0)
	{
		DescribeAuthority(eAuthorityErr);
	}
	else
	{
		//��ʾ��ǰѡ����û���Ϣ
		pUserData pUser = m_userManager.GetUser(iItem);
		if (pUser != NULL)
		{
			GetDlgItem(IDC_EDIT_USER_NAME)->SetWindowText(pUser->szUserName);
			GetDlgItem(IDC_EDIT_USER_PWD)->SetWindowText(pUser->szUserPwd);
			GetDlgItem(IDC_EDIT_USER_PWD_REPEAT)->SetWindowText(pUser->szUserPwd);
			m_cboxAuthority.SetCurSel(pUser->eAuthority == eAdministrator ? 0 : 1);
			DescribeAuthority(pUser->eAuthority);

			GetDlgItem(IDC_BTN_DEL_USER)->EnableWindow(pUser->eAuthority != eAdministrator);
		}
	}

	GetDlgItem(IDC_BTN_EDIT_USER)->EnableWindow(iItem >= 0);
	GetDlgItem(IDC_BTN_SAVE_USER)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BTN_DEL_USER)->EnableWindow(iItem >= 0);
	if (iItem < 0)
		GetDlgItem(IDC_BTN_DEL_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_ADD_USER)->EnableWindow(TRUE);
}

void CUserDlg::DescribeAuthority(emUserAuthority eUA)
{
	if (eUA == eAdministrator)
	{
		GetDlgItem(IDC_LABEL_SUMMERY)->SetWindowText("����Ա�û���\n\nӵ�����в���Ȩ�ޡ�");
	}
	else if (eUA == eGeneral)
	{
		GetDlgItem(IDC_LABEL_SUMMERY)->SetWindowText("��ͨ�û���\n\nӵ���ź��л����������õ�Ȩ�ޡ�");
	}
	else if (eUA == eAuthorityErr)
	{
		GetDlgItem(IDC_LABEL_SUMMERY)->SetWindowText("�û�Ȩ��˵����");
	}
}

void CUserDlg::EnableEditUserInfo(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_USER_PWD)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_USER_PWD_REPEAT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CBOX_USER_AUTHORITY)->EnableWindow(bEnable);
}

void CUserDlg::ClearEditUserInfo()
{
	GetDlgItem(IDC_EDIT_USER_NAME)->SetWindowText(NULL);
	GetDlgItem(IDC_EDIT_USER_PWD)->SetWindowText(NULL);
	GetDlgItem(IDC_EDIT_USER_PWD_REPEAT)->SetWindowText(NULL);
	GetDlgItem(IDC_CBOX_USER_AUTHORITY)->SetWindowText(NULL);
}

void CUserDlg::OnBtnAddUser()
{
	if (m_opt == opt_add)
	{
		GetDlgItem(IDC_BTN_ADD_USER)->SetWindowText(_T("���(&A)"));
		m_opt = opt_non;

		GetDlgItem(IDC_BTN_EDIT_USER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SAVE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DEL_USER)->EnableWindow(TRUE);

		EnableEditUserInfo(FALSE);

		GetUserInfo(GetCursorItem());
	}
	else
	{
		ClearEditUserInfo();
		EnableEditUserInfo(TRUE);
		// ֻ����һ������Ա�û�
		m_cboxAuthority.SetCurSel(1);
		DescribeAuthority(eGeneral);


		GetDlgItem(IDC_BTN_ADD_USER)->SetWindowText(_T("ȡ��(&C)"));
		//GetDlgItem(IDC_BTN_ADD_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_EDIT_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVE_USER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DEL_USER)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_USER_NAME)->SetFocus();

		m_opt = opt_add;
	}	
}

void CUserDlg::OnBtnModUser()
{
	if (m_opt == opt_mod)
	{
		m_opt = opt_non;

		GetDlgItem(IDC_BTN_SAVE_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DEL_USER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_ADD_USER)->EnableWindow(TRUE);

		GetDlgItem(IDC_BTN_EDIT_USER)->SetWindowText(_T("�༭(&E)"));

		EnableEditUserInfo(FALSE);
		GetDlgItem(IDC_LIST_USER)->EnableWindow(TRUE);

		GetUserInfo(GetCursorItem());
	}
	else
	{
		EnableEditUserInfo(TRUE);

		// �������޸��û���Ȩ��
		GetDlgItem(IDC_CBOX_USER_AUTHORITY)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_USER)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_SAVE_USER)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DEL_USER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_ADD_USER)->EnableWindow(FALSE);

		GetDlgItem(IDC_BTN_EDIT_USER)->SetWindowText(_T("ȡ��(&C)"));

		GetDlgItem(IDC_EDIT_USER_NAME)->SetFocus();

		m_opt = opt_mod;
	}

}

void CUserDlg::OnBtnSavUser()
{
	// ������
	if (!CheckUserInf())
		return;

	// �������
	if (m_opt == opt_add)
		SaveNewUserInfo();
	else if (m_opt == opt_mod)
		SaveModifiedInfo();

	m_opt = opt_non;

	// �������
	EnableEditUserInfo(FALSE);

	GetDlgItem(IDC_LIST_USER)->EnableWindow(TRUE);

	GetDlgItem(IDC_BTN_EDIT_USER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_SAVE_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BTN_DEL_USER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BTN_ADD_USER)->EnableWindow(TRUE);
}

void CUserDlg::OnBtnDelUser()
{
	CString strMsgTite(_T("ɾ���û�"));
	int iItem = GetCursorItem();
	if (iItem < 0)
	{
		return (void)MessageBox(_T("��ѡ����Ҫɾ�����û���"), strMsgTite, MB_OK|MB_ICONWARNING);
	}

	pUserData pUser = m_userManager.GetUser(iItem);
	if (pUser == NULL)
	{
		return (void)MessageBox(_T("Sorry, ��ȡ�û���Ϣʧ�ܣ�"), strMsgTite, MB_OK|MB_ICONSTOP);
	}

	if (pUser->eAuthority == eAdministrator)
	{
		return (void)MessageBox(_T("��ֹɾ��Ψһ�Ĺ���Ա�û���"), strMsgTite, MB_OK|MB_ICONWARNING);
	}

	CString strName, strMsg;
	strName.Format(_T("%s"), pUser->szUserName);
	strMsg.Format(_T("ȷ��Ҫɾ���û� [ %s ] ��"), pUser->szUserName);
	if (MessageBox(strMsg, strMsgTite, MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;

	// ������ɾ��
	m_userManager.RemoveAt(iItem);

	m_UserList.DeleteItem(iItem);

	if (m_UserList.GetItemCount() > 0)
	{
		if (iItem > m_UserList.GetItemCount()-1)
		{
			m_UserList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
			GetUserInfo(0);
		}
		else
		{
			m_UserList.SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED);
			GetUserInfo(iItem);
		}
	}

	strMsg.Format(_T("�û�%s��ɾ����"), strName);
	MessageBox(strMsg, strMsgTite, MB_OK|MB_ICONINFORMATION);
}

void CUserDlg::OnCbnSelchangeCboxUserAuthority()
{
	int nCursel = m_cboxAuthority.GetCurSel();
	if (nCursel == CB_ERR)
	{
		DescribeAuthority(eAuthorityErr);
		return;
	}

	if (nCursel == 0)
	{
		DescribeAuthority(eAdministrator);
	}
	else if (nCursel == 1)
	{
		DescribeAuthority(eGeneral);
	}
}


BOOL CUserDlg::CheckUserInf()
{
	CString strName, strPwd, strPwdRpt, strMsg, strMsgTitle;
	CEdit *pEditName, *pEditPwd, *pEditPwdRpt;

	pEditName = (CEdit*)GetDlgItem(IDC_EDIT_USER_NAME);
	pEditPwd  = (CEdit*)GetDlgItem(IDC_EDIT_USER_PWD);
	pEditPwdRpt = (CEdit*)GetDlgItem(IDC_EDIT_USER_PWD_REPEAT);

	pEditName->GetWindowText(strName);
	pEditPwd->GetWindowText(strPwd);
	pEditPwdRpt->GetWindowText(strPwdRpt);

	strMsgTitle = _T("�û���Ϣ���");

	if (strName.IsEmpty())
	{
		MessageBox(_T("�û�������Ϊ�գ�"), strMsgTitle, MB_OK|MB_ICONSTOP);

		pEditName->SetFocus();
		pEditName->SetSel(0, pEditName->GetWindowTextLength());
		return FALSE;
	}

	if (strName.GetLength() > _MAX_UNAME)
	{
		MessageBox(_T("�û���̫������������128�������ַ�����256�������ַ����ڣ�"), strMsgTitle, MB_ICONSTOP|MB_OK);
		
		pEditName->SetFocus();
		pEditName->SetSel(0, pEditName->GetWindowTextLength());
		return FALSE;
	}

	if (strPwd.GetLength() > 20)
	{
		MessageBox(_T("���볤��̫����20λ���ڣ�"), strMsgTitle, MB_OK|MB_ICONSTOP);

		pEditPwd->SetFocus();
		pEditPwd->SetSel(0, pEditPwd->GetWindowTextLength());
		return FALSE;
	}

	if (!PasswordCharLmt(strPwd))
	{
		MessageBox(_T("�������зǷ��ַ���"), strMsgTitle, MB_OK|MB_ICONSTOP);

		pEditPwd->SetFocus();
		pEditPwd->SetSel(0, pEditPwd->GetWindowTextLength());
		return FALSE;
	}

	if (strPwdRpt.GetLength() > 20)
	{
		MessageBox(_T("���볤��̫����20λ���ڣ�"), strMsgTitle, MB_OK|MB_ICONSTOP);

		pEditPwdRpt->SetFocus();
		pEditPwdRpt->SetSel(0, pEditPwd->GetWindowTextLength());
		return FALSE;
	}

	if (!PasswordCharLmt(strPwdRpt))
	{
		MessageBox(_T("�������зǷ��ַ���"), strMsgTitle, MB_OK|MB_ICONSTOP);

		pEditPwdRpt->SetFocus();
		pEditPwdRpt->SetSel(0, pEditPwdRpt->GetWindowTextLength());
		return FALSE;
	}

	if (strPwd != strPwdRpt)
	{
		MessageBox(_T("������������벻һ����"), strMsgTitle, MB_OK|MB_ICONSTOP);

		pEditPwdRpt->SetFocus();
		pEditPwdRpt->SetSel(0, pEditPwdRpt->GetWindowTextLength());
		return FALSE;
	}

	if (m_cboxAuthority.GetCurSel() == CB_ERR)
	{
		MessageBox(_T("��ѡ���û�Ȩ�ޣ�"), strMsgTitle, MB_ICONSTOP|MB_OK);

		return FALSE;
	}

	if (m_opt == opt_add && m_userManager.FindDefaultAdmin() != -1)
	{
		if (m_cboxAuthority.GetCurSel() == 0)
		{
			MessageBox(_T("����Ա�˻���Ψһ�˻����벻Ҫ�ظ���ӣ�"), strMsgTitle, MB_ICONSTOP|MB_OK);

			return FALSE;
		}
	}
	

	return TRUE;
}

BOOL CUserDlg::PasswordCharLmt(const CString &strPassword)
{
	for (int i=0; i<strPassword.GetLength(); i++)
	{
		if ( (strPassword[i] <= 'z' && strPassword[i] >= 'a')
		  || (strPassword[i] <= '9' && strPassword[i] >= '0')
		  || (strPassword[i] == '_'))
		  continue;
		else
			return FALSE;
	}

	return TRUE;
}

void CUserDlg::SaveModifiedInfo()
{
	int nItem = GetCursorItem();
	if (nItem < 0)
		return;

	pUserData pUser = m_userManager.GetUser(nItem);
	if (pUser == NULL)
		return;

	CString strName, strPwd, strPwdRpt;
	GetDlgItem(IDC_EDIT_USER_NAME)->GetWindowText(strName);
	GetDlgItem(IDC_EDIT_USER_PWD)->GetWindowText(strPwd);
	GetDlgItem(IDC_EDIT_USER_PWD_REPEAT)->GetWindowText(strPwdRpt);

	emUserAuthority eAu;
	if (m_cboxAuthority.GetCurSel() == 0)
		eAu = eAdministrator;
	else if (m_cboxAuthority.GetCurSel() == 1)
		eAu = eGeneral;

	m_userManager.ModifyUser(pUser->nUserID, strName.GetBuffer(), strPwd.GetBuffer(), eAu);

	// �����б�
	LVITEM lvItem = {0};
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;   // ���֡�ͼƬ��״̬
	lvItem.iItem = nItem;                            // �к�(��һ��)
	lvItem.iSubItem = 0;                             // ���к�
	lvItem.pszText = new CHAR[_MAX_UNAME];
	lvItem.cchTextMax = _MAX_UNAME;
	m_UserList.GetItem(&lvItem);

	lvItem.iImage = eAu==eAdministrator ? 0 : 1;
	m_UserList.SetItem(&lvItem);

	delete [] lvItem.pszText;

	m_UserList.SetItemText(nItem, 1, strName);
	m_UserList.SetItemText(nItem, 2, pUser->eAuthority == eAdministrator ? "����Ա" : "��ͨ�û�");

	GetDlgItem(IDC_BTN_EDIT_USER)->SetWindowText(_T("�༭(&E)"));
}

void CUserDlg::SaveNewUserInfo()
{
	CString strName, strPwd, strPwdRpt;
	GetDlgItem(IDC_EDIT_USER_NAME)->GetWindowText(strName);
	GetDlgItem(IDC_EDIT_USER_PWD)->GetWindowText(strPwd);
	GetDlgItem(IDC_EDIT_USER_PWD_REPEAT)->GetWindowText(strPwdRpt);

	emUserAuthority eAu;
	if (m_cboxAuthority.GetCurSel() == 0)
		eAu = eAdministrator;
	else if (m_cboxAuthority.GetCurSel() == 1)
		eAu = eGeneral;

	// ������û�
	UserData user;
	ZeroMemory(&user, sizeof(UserData));
	user.nUserID = m_userManager.PrepareUserID();
	strncpy_s(user.szUserName, _MAX_UNAME, strName.GetBuffer(), strName.GetLength());
	strncpy_s(user.szUserPwd, _MAX_PASSWORD, strPwd.GetBuffer(), strPwd.GetLength());
	user.eAuthority = eAu;
	m_userManager.AddUser(user);	

	// �����б�
	int nItem = GetCursorItem();
	if (nItem != -1)
	{
		m_UserList.SetItemState(nItem, ~LVIS_SELECTED, LVIS_SELECTED);
	}

	nItem = m_UserList.GetItemCount();

	LVITEM lvItem = {0};
	lvItem.mask = LVIF_IMAGE|LVIF_TEXT|LVIF_STATE;   // ���֡�ͼƬ��״̬
	lvItem.iItem = nItem;                            // �к�(��һ��)
	lvItem.iSubItem = 0;                             // ���к�
	lvItem.pszText = NULL;

	lvItem.iImage = eAu == eAdministrator ? 0 : 1;
	
	m_UserList.InsertItem(&lvItem);
	
	delete [] lvItem.pszText;

	m_UserList.SetItemText(nItem, 1, strName);
	m_UserList.SetItemText(nItem, 2, eAu == eAdministrator ? "����Ա" : "��ͨ�û�");

	m_UserList.SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	GetDlgItem(IDC_BTN_ADD_USER)->SetWindowText(_T("���(&A)"));
}

BOOL CUserDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
