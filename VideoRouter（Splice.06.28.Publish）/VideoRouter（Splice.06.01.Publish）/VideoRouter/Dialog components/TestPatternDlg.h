#pragma once

// CTestPatternDlg 对话框

class CTestPatternDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestPatternDlg)

public:
	CTestPatternDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestPatternDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_TEST_PATTERN };

	//CMFCColorButton m_clrbtnBackgrd;
	//CMFCColorButton m_clrbtnGridline;

	CComboBox m_cboxlineWid;
	CComboBox m_cboxHorzSpace;
	CComboBox m_cboxVertSpace;

	CTabCtrl m_Tab;
	CXTPListCtrl m_listInput;
	CXTPListCtrl m_listOutput;

	CMFCColorButton m_clrbtnBackgrd;
	CMFCColorButton m_clrbtnGridline;
	//CXTPColorPicker m_clrbtnBackgrd;
	//CXTPColorPicker m_clrbtnGridline;

	int m_nGridlineWid;
	int m_nGridHorzSpace;
	int m_nGridVertSpace;
	int m_nHasGrid;
	CPoint m_ptBegin;

private:
	void EnableGridEdit(BOOL bHasGrid);
	void OnVKreturn();

	BOOL m_bPreview; //背景预览
	void OnPreview();

	void InitTabCtrl();
	void InitListCtrl();
	void ResetListItemColor();

public:
	CSize m_BkgSize;
	void DrawGridLine(CDC *pDC);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSelChangeGridLineWid();
	afx_msg void OnSelChangeGridHorzSpace();
	afx_msg void OnSelChangeGridVertSpace();
public:
	afx_msg void OnBnClickedCheckHasGridPattern();
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBtnPreView();
	afx_msg void OnBtnAll();
	afx_msg void OnBtnNon();
	afx_msg void OnBtnInv();
	afx_msg void OnLvnItemchangedListPatternInput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListPatternOutput(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBtnTest();
	afx_msg void OnBtnCancelTest();
	afx_msg void OnBtnBkColor();
	afx_msg void OnBtnLineColor();
	afx_msg void OnTcnSelchangeTabPort(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckPrintPortIndex();
	afx_msg void OnBnClickedBtnPatternPrintOsd();
	afx_msg void OnBnClickedBtnPatternCancelOsd();
};
