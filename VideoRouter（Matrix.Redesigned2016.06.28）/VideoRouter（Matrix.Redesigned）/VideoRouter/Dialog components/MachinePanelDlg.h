#pragma once

#define HORZ_CARD_NUM     2
#define VERT_CARD_NUM     16
#define TERMINALS_ONE_CARD  4

// CMachinePanelDlg �Ի���

class CMachinePanelDlg : public CDialog
{
	DECLARE_DYNAMIC(CMachinePanelDlg)

public:
	CMachinePanelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMachinePanelDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MACHINE_PANEL };

	CRect m_rcCard[HORZ_CARD_NUM][VERT_CARD_NUM];   //[H][V]
	CRect m_rcTerm[HORZ_CARD_NUM][VERT_CARD_NUM][TERMINALS_ONE_CARD];//[H][V][P]
	void CalcImagePosition();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
};
