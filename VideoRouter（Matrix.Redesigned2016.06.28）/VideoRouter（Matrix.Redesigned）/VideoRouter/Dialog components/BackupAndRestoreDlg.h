#pragma once


// CBackupAndRestoreDlg �Ի���

class CBackupAndRestoreDlg : public CDialog
{
	DECLARE_DYNAMIC(CBackupAndRestoreDlg)

public:
	CBackupAndRestoreDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBackupAndRestoreDlg();

	int m_nOptDevParam;
	int m_nOptUserCust;
	int m_nOptSoftSet;

// �Ի�������
	enum { IDD = IDD_DLG_BACKUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckHardwareParam();
};
