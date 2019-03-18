#pragma once


// COsdStringDesignerDlg 对话框

class COsdStringDesignerDlg : public CDialog
{
	DECLARE_DYNAMIC(COsdStringDesignerDlg)

public:
	COsdStringDesignerDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COsdStringDesignerDlg();

// 对话框数据
	enum { IDD = IDD_DLG_OSD_DESIGNER };

	CMFCColorButton m_clrForegnd;
	CMFCColorButton m_clrBackgnd;

	CRichEditCtrl m_REditString;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMfcbtnFontdlg();
	afx_msg void OnBnClickedColorbtnForgndColor();
	afx_msg void OnBnClickedColorbtnBackgndColor();
	virtual BOOL OnInitDialog();
};
