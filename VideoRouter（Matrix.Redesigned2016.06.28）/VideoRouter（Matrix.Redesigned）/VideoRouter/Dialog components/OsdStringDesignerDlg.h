#pragma once


// COsdStringDesignerDlg �Ի���

class COsdStringDesignerDlg : public CDialog
{
	DECLARE_DYNAMIC(COsdStringDesignerDlg)

public:
	COsdStringDesignerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COsdStringDesignerDlg();

// �Ի�������
	enum { IDD = IDD_DLG_OSD_DESIGNER };

	CMFCColorButton m_clrForegnd;
	CMFCColorButton m_clrBackgnd;

	CRichEditCtrl m_REditString;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMfcbtnFontdlg();
	afx_msg void OnBnClickedColorbtnForgndColor();
	afx_msg void OnBnClickedColorbtnBackgndColor();
	virtual BOOL OnInitDialog();
};
