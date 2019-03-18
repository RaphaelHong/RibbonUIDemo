#pragma once


// CProgressBar �Ի���

class CProgressBar : public CDialog
{
	DECLARE_DYNAMIC(CProgressBar)

public:
	CProgressBar(CWnd* pParent = NULL, CString strText = _T(""));   // ��׼���캯��
	virtual ~CProgressBar();

// �Ի�������
	enum { IDD = IDD_DLG_PROGRESSBAR };

	CXTPProgressCtrl m_progress;
	void SetShowText(CString strText) { m_strInfoText = strText; }

	void InitProgressBar() { m_progress.SetPos(0); }


private:
	CString m_strInfoText;
	int m_nPos;

	CBrush m_bkBrush;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual void OnOK();
	virtual void OnCancel();
};
