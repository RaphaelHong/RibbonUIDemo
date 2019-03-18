#pragma once
#include "MGcolorsetView.h"

// CMGcolorsetDlg 对话框

class CMGcolorsetDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGcolorsetDlg)

public:
	CMGcolorsetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMGcolorsetDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MG_COLOR };

	CcolorsetView *m_pColorView;
	void Initialize();

private:
	CXTPButton m_GboxTestMode;
	CXTPButton m_RadioSrcTest;
	CXTPButton m_RadioImgTest;

	CXTPButton m_GboxBlendOpt;
	CXTPButton m_RadioCancelB;
	CXTPButton m_RadioBlackB;
	CXTPButton m_RadioResumeB;

	CXTPButton m_GboxColorSel;
	CXTPButton m_RadioRed;
	CXTPButton m_RadioGreen;
	CXTPButton m_RadioBlue;
	CXTPButton m_RadioWhite;

	CXTPButton m_GboxMethod;
	CXTPButton m_RadioLinear;
	CXTPButton m_RadioRegion;

	CXTPButton m_btnClrReset;

	int m_nBlendOpt;

	CBrush m_redBrush;
	CBrush m_greenBrush;
	CBrush m_blueBrush;
	CBrush m_whiteBrush;
	CBrush m_bkBrush;

	void SetControlTheme(XTPControlTheme theme);
	void ResizeIt();
	void CreateView();
	void SetColor(int *data, int nColorSel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRadioTestType(UINT nID);
	afx_msg void OnRadioBlendOpt(UINT nID);
	afx_msg void OnRadioAjustMethod(UINT nID);
	afx_msg void OnRadioColorSel(UINT nID);
	afx_msg void OnBnClickedReset();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
