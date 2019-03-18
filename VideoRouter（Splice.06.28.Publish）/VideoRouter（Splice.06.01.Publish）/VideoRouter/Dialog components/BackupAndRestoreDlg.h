#pragma once


// CBackupAndRestoreDlg 对话框

class CBackupAndRestoreDlg : public CDialog
{
	DECLARE_DYNAMIC(CBackupAndRestoreDlg)

public:
	CBackupAndRestoreDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBackupAndRestoreDlg();

	int m_nOptDevParam;
	int m_nOptUserCust;
	int m_nOptSoftSet;

// 对话框数据
	enum { IDD = IDD_DLG_BACKUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckHardwareParam();
};
