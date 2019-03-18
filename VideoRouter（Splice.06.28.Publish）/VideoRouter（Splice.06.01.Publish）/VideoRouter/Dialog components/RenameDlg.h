#pragma once


// CRenameDlg 对话框

class CRenameDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameDlg)

public:
	CRenameDlg(CWnd* pParent = NULL);   // 标准构造函数
	CRenameDlg(CString strOrg, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRenameDlg();

// 对话框数据
	enum { IDD = IDD_DLG_RENAME };

	CString m_strOrgName;
	CPoint m_ptPopup;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual void OnOK();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
