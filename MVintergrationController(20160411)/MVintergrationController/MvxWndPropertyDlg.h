#pragma once


// CMvxWndPropertyDlg 对话框

class CMvxWndPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CMvxWndPropertyDlg)

public:
	CMvxWndPropertyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMvxWndPropertyDlg();

	CMvxWndPropertyDlg(BOOL bNewModify, CWnd* pParent = NULL);   // 标准构造函数

// 对话框数据
	enum { IDD = IDD_DLG_MVX_WNDPROPERTY };

	int m_nWinPosX;
	int m_nWinPosY;
	int m_nWinSizeX;
	int m_nWinSizeY;
	CString m_strWinName;

	CXTPComboBox m_CBoxWndIndex;
	CXTPButton m_BtnOK;
	CXTPButton m_BtnCancel;

	void SetTheme(XTPControlTheme theme);

	//CBrush m_bkBrush;

public:
	BOOL m_bNewModify; /*TRUE: new a window, FALSE: modify a window*/
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelChangedWnd();
};
