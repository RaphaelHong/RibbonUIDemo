#pragma once


// CWaitingMsg 对话框

class CWaittingMsg : public CDialog
{
	DECLARE_DYNAMIC(CWaittingMsg)

public:
	CWaittingMsg(CWnd* pParent = NULL);   // 标准构造函数
	CWaittingMsg(CStringW strMsg, CWnd* pParent=NULL);
	virtual ~CWaittingMsg();

// 对话框数据
	enum { IDD = IDD_DLG_WAITING };

	void SetMessage(CStringW strMsg);
	void Reset();

private:
	CStringW m_strMsg;

	int m_iRotate;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
