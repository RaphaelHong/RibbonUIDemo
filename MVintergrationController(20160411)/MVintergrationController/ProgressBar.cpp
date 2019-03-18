// ProgressBar.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "ProgressBar.h"

// CProgressBar 对话框

IMPLEMENT_DYNAMIC(CProgressBar, CDialog)

CProgressBar::CProgressBar(CWnd* pParent/* =NULL*/, CString strText)
	: CDialog(CProgressBar::IDD, pParent)
	, m_nPos(0)
{
	m_strInfoText = strText;
}

CProgressBar::~CProgressBar()
{
}

void CProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}


BEGIN_MESSAGE_MAP(CProgressBar, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CProgressBar 消息处理程序

void CProgressBar::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if (m_strInfoText.IsEmpty())
	{
		//空的提示信息，隐藏下边的信息提示文本框
		CRect rcClient;
		GetWindowRect(&rcClient);

		CRect rcProgressCtrl;
		GetDlgItem(IDC_PROGRESS)->GetWindowRect(&rcProgressCtrl);
		SetWindowPos(&wndTop, rcClient.left, rcClient.top, rcClient.Width(), rcProgressCtrl.bottom-rcClient.top+2, SWP_SHOWWINDOW);

		//圆角对话框
		GetClientRect(&rcClient);

		CRgn wndRgn;
		wndRgn.CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right, rcClient.bottom, 5, 5);
		SetWindowRgn((HRGN)wndRgn, TRUE);
	}
	else
	{
		GetDlgItem(IDC_TEXT_INFO)->SetWindowText(m_strInfoText);
	}
}

BOOL CProgressBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rcDlg;
	GetClientRect(&rcDlg);

	//设置对话框具有阴影效果
	SetClassLong(this->m_hWnd, GCL_STYLE, GetClassLong(this->m_hWnd, GCL_STYLE) | 0x00020000); //CS_DROPSHADOW 0x00020000

	//圆角对话框
	CRgn wndRgn;
	wndRgn.CreateRoundRectRgn(rcDlg.left, rcDlg.top, rcDlg.right, rcDlg.bottom, 5, 5);
	SetWindowRgn((HRGN)wndRgn, TRUE);

	//初始化进度条
	m_progress.SetTheme(xtpControlThemeResource);
	m_progress.SetRange(0,100);
	m_progress.SetStep(1);

	//背景画刷
	m_bkBrush.CreateSolidBrush(RGB(66,204,250));

	//SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProgressBar::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		if (m_progress.GetPos() >= 100)
		{
			m_progress.SetPos(0);
		}

		m_progress.StepIt();
	}

	CDialog::OnTimer(nIDEvent);
}

HBRUSH CProgressBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		
		return m_bkBrush;
	}
	return hbr;
}
void CProgressBar::OnOK()
{
	//CDialog::OnOK();
}

void CProgressBar::OnCancel()
{
	//CDialog::OnCancel();
}
