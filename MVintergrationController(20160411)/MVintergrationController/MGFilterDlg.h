#pragma once
#include "afxcmn.h"


// CMGblurDlg 对话框

class CMGFilterDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGFilterDlg)

public:
	CMGFilterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMGFilterDlg();

// 对话框数据
	enum { IDD = IDD_DLG_MG_FILTER };

	void Initialize();

private:
	CXTPEdit m_EditRadius;
	CXTPSpinButtonCtrl m_SpinRadius;
	CSliderCtrl m_SliderBlur;

	void ResizeIt();

	CBrush m_bkBrush;
	COLORREF m_clrPaint;
	CBitmap m_srcBitmap;
	CBitmap m_BluredBitmap;
	int m_nFilterRadius;

	CWinThread *m_pThreadGaussFilter;
	DWORD GaussFilterProcess(CBitmap &srcBitmap);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDeltaposSpinBlurRadius(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
