// RenameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "RenameDlg.h"
#include "afxdialogex.h"


// CRenameDlg �Ի���

IMPLEMENT_DYNAMIC(CRenameDlg, CDialog)

CRenameDlg::CRenameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRenameDlg::IDD, pParent)
{
	m_ptPopup = CPoint(-1, -1);
}

CRenameDlg::CRenameDlg(CStringA strOrg, CWnd* pParent /*=NULL*/)
	: CDialog(CRenameDlg::IDD, pParent)
{
	m_ptPopup = CPoint(-1, -1);
	m_strOrgName = strOrg;
}

CRenameDlg::~CRenameDlg()
{
}

void CRenameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRenameDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CRenameDlg::OnOK)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CRenameDlg ��Ϣ�������
BOOL CRenameDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_RENAME);

	pEdit->SetWindowText(m_strOrgName);
	pEdit->SetFocus();
	pEdit->SetSel(0, -1);

	return FALSE;
}

void CRenameDlg::OnOK()
{
	UpdateData();

	GetDlgItem(IDC_EDIT_RENAME)->GetWindowText(m_strOrgName);

	if(m_strOrgName.GetLength() > _MAX_SIGNAL_NAME)
	{
		return (void)MessageBox(_T("�ַ���������ӦС��128��Ӣ���ַ���"), _T("������"), MB_OK|MB_ICONERROR);
	}

	CDialog::OnOK();
}
void CRenameDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (m_ptPopup != CPoint(-1, -1))
	{
		CRect rcWnd;
		GetWindowRect(&rcWnd);

		SetWindowPos(NULL, m_ptPopup.x, m_ptPopup.y, rcWnd.Width(), rcWnd.Height(), SWP_SHOWWINDOW);
	}
}
