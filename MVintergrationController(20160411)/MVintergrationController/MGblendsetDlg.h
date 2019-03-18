#pragma once
#include "NumberEdit.h"

#define LEFT_BLEND           0
#define TOP_BLEND            1
#define RIGHT_BLEND	         2
#define BOTTOM_BLEND         3

struct stBlendSettings
{
	int nBlendWidth;
	int nPureWhite;
	int nWhiteLvl;
	double fAlpha[2];
	double fP[2];
	double fGamma[2];
};
// CMGblendsetDlg 对话框

class CMGblendsetDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGblendsetDlg)

public:
	CMGblendsetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMGblendsetDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MG_BLEND };

	stBlendSettings m_stBlendSetting[4];
	void Initialize();

private:
	void SetOnBlending();
	void WriteBlendWidth();
	void WriteWhiteLevel();
	void SetAlphaGammaP(BOOL bDefault/* =FALSE */);
	void TestAlphaGammaP();
	void SetAdvancedBlendParam();
	void UpdateBlendParam(int nBlendSel);
	BOOL ValidateBlendWidth();

	inline void ShowHideBlendSelCtrls(int nBlendSel, int nShowHide);

private:
	CXTPButton m_GboxWhiteMode;
	CXTPButton m_GboxBlendPara;
	CXTPButton m_GboxBlendWid;

	CXTPButton m_CheckBlend[4];

	CXTPButton m_RadioBlend[4];

	CXTPButton m_RadioNonWhite;
	CXTPButton m_RadioPurWhite;

	CXTPButton m_CheckEnBlend;
	CXTPButton m_BtnSetBlendWid;
	CXTPButton m_BtnSetWhiteLvl;
	CXTPButton m_BtnDefParam;
	CXTPButton m_BtnTestParam;
	CXTPButton m_BtnAdvanced;

	CXTPEdit m_EditBlend[4];

	CXTPEdit m_EditWhiteLvl;
	CNumberEdit m_EditAlpha;
	CNumberEdit m_EditP;
	CNumberEdit m_EditGamma;

	CXTPSpinButtonCtrl m_SpinBlend[4];

	CSliderCtrl m_SliderAlpha;
	CSliderCtrl m_SliderP;
	CSliderCtrl m_SliderGamma;

	void SetControlTheme(XTPControlTheme theme);
	void ResizeIt();

	//控件关联值变量
	int m_nBlendwidth[4];   //四边融合带宽度
	int m_nPureWhite;       //融合模式选择
	int m_nWhiteLvl;        //纯白阀值
	int m_nEnableBlending;  //开启/关闭融合功能
	int m_nBlendSel;        //当前选择的融合带
	int m_nAlpha;
	int m_nP;
	int m_nGamma;

	CBrush m_bkBrush;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCheckBlendSel(UINT nID);
	afx_msg void OnRadioBlendSel(UINT nID);
	afx_msg void OnDeltaposSpinBlendWidth(UINT nID, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRadioPureWhiteSel(UINT nID);
	afx_msg void OnBnClickedBtnSetBlendWidth();
	afx_msg void OnBnClickedCheckEnableBlend();
	afx_msg void OnBnClickedBtnSetWhiteLevel();
	afx_msg void OnBnClickedBtnDefBlendParam();
	afx_msg void OnBnClickedBtnTestBlendPara();
	afx_msg void OnBnClickedBtnAdvancedParam();
	afx_msg void OnEnChangeEditAlpha();
	afx_msg void OnEnChangeEditP();
	afx_msg void OnEnChangeEditGamma();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
