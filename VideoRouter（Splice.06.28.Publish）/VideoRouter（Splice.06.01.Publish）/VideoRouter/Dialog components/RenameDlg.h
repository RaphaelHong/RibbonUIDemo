#pragma once


// CRenameDlg �Ի���

class CRenameDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameDlg)

public:
	CRenameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	CRenameDlg(CString strOrg, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRenameDlg();

// �Ի�������
	enum { IDD = IDD_DLG_RENAME };

	CString m_strOrgName;
	CPoint m_ptPopup;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
