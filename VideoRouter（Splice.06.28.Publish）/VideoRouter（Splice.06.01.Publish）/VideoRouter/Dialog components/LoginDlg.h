#pragma once

#include "..\UserManager.h"

// CLoginDlg 对话框

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

	// 对话框数据
	enum { IDD = IDD_DLG_LOGIN };

	CComboBox m_cboxUser;

	void AddUsers();

public:
	CUserManager m_UserManager;

protected:
	CToolTipCtrl m_tips;
	void SetTooltips();

public:
	void SetWndRgn(CDC *pDC, UINT uBackBmpID, COLORREF clrTrans);

	CPoint m_BkPos; //背景图片播放的位置
	inline void CopyBitmap(CBitmap *pBmpDest, CBitmap *pBmpSrc, int nPos, CRect Rect, CWnd *pWnd);

	CDC *m_pMemDC;
	void DrawTitle(CDC *pValidDC);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
