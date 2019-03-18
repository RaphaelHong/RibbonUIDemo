#pragma once
#include "afxcmn.h"


// CMGblurDlg �Ի���

class CMGFilterDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGFilterDlg)

public:
	CMGFilterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMGFilterDlg();

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
