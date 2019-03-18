#pragma once


// CWaitingMsg �Ի���

class CWaittingMsg : public CDialog
{
	DECLARE_DYNAMIC(CWaittingMsg)

public:
	CWaittingMsg(CWnd* pParent = NULL);   // ��׼���캯��
	CWaittingMsg(CStringW strMsg, CWnd* pParent=NULL);
	virtual ~CWaittingMsg();

// �Ի�������
	enum { IDD = IDD_DLG_WAITING };

	void SetMessage(CStringW strMsg);
	void Reset();

private:
	CStringW m_strMsg;

	int m_iRotate;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
