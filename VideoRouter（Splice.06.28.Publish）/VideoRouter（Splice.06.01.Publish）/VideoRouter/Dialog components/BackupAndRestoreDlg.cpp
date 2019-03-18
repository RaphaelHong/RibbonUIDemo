// BackupAndRestoreDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "BackupAndRestoreDlg.h"


// CBackupAndRestoreDlg �Ի���

IMPLEMENT_DYNAMIC(CBackupAndRestoreDlg, CDialog)

CBackupAndRestoreDlg::CBackupAndRestoreDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBackupAndRestoreDlg::IDD, pParent)
	, m_nOptDevParam(0)
	, m_nOptUserCust(1)
	, m_nOptSoftSet(0)
{

}

CBackupAndRestoreDlg::~CBackupAndRestoreDlg()
{
}

void CBackupAndRestoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_HARDWARE_PARAM, m_nOptDevParam);
	DDX_Check(pDX, IDC_CHECK_CUSTOM_PARAM, m_nOptUserCust);
	DDX_Check(pDX, IDC_CHECK_SOFTWARE_PARAM, m_nOptSoftSet);
}


BEGIN_MESSAGE_MAP(CBackupAndRestoreDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_HARDWARE_PARAM, &CBackupAndRestoreDlg::OnBnClickedCheckHardwareParam)
END_MESSAGE_MAP()


// CBackupAndRestoreDlg ��Ϣ�������

BOOL CBackupAndRestoreDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	if (CCommunicateManager::IsConnected())
	{
		m_nOptDevParam = 1;
		UpdateData(FALSE);
	}

	return TRUE;
}

void CBackupAndRestoreDlg::OnBnClickedCheckHardwareParam()
{
	UpdateData();

	if (m_nOptDevParam && !CCommunicateManager::IsConnected())
	{
		m_nOptDevParam = 0;
		UpdateData(FALSE);

		MessageBox(_T("ע�⣺�豸���߲��ܱ����豸������"), _T("������ָ�"), MB_OK|MB_ICONINFORMATION);
	}
}
