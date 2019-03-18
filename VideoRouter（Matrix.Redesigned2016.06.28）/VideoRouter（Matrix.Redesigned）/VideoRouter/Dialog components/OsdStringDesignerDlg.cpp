// OsdStringDesignerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "OsdStringDesignerDlg.h"
#include "afxdialogex.h"


// COsdStringDesignerDlg 对话框

IMPLEMENT_DYNAMIC(COsdStringDesignerDlg, CDialog)

COsdStringDesignerDlg::COsdStringDesignerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COsdStringDesignerDlg::IDD, pParent)
{

}

COsdStringDesignerDlg::~COsdStringDesignerDlg()
{
}

void COsdStringDesignerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLORBTN_FORGND_COLOR, m_clrForegnd);
	DDX_Control(pDX, IDC_COLORBTN_BACKGND_COLOR, m_clrBackgnd);
	DDX_Control(pDX, IDC_RICHEDIT_OSD_STRING, m_REditString);
}


BEGIN_MESSAGE_MAP(COsdStringDesignerDlg, CDialog)
	ON_BN_CLICKED(IDC_MFCBTN_FONTDLG, &COsdStringDesignerDlg::OnBnClickedMfcbtnFontdlg)
	ON_BN_CLICKED(IDC_COLORBTN_FORGND_COLOR, &COsdStringDesignerDlg::OnBnClickedColorbtnForgndColor)
	ON_BN_CLICKED(IDC_COLORBTN_BACKGND_COLOR, &COsdStringDesignerDlg::OnBnClickedColorbtnBackgndColor)
END_MESSAGE_MAP()


// COsdStringDesignerDlg 消息处理程序


void COsdStringDesignerDlg::OnBnClickedMfcbtnFontdlg()
{
	CHARFORMAT2 cfmt;
	cfmt.cbSize = sizeof(CHARFORMAT2);
	m_REditString.GetDefaultCharFormat(cfmt);

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	_tcsncpy_s(lf.lfFaceName, cfmt.szFaceName, LF_FACESIZE);
	lf.lfCharSet = cfmt.bCharSet;
	lf.lfPitchAndFamily = cfmt.bPitchAndFamily;
	lf.lfHeight = cfmt.yHeight;
	lf.lfWeight = cfmt.wWeight;

	CFontDialog font;
	font.FillInLogFont(cfmt);
	if (font.DoModal() == IDOK)
	{
		font.GetCharFormat(cfmt);
		m_REditString.SetDefaultCharFormat(cfmt);
	}
}


void COsdStringDesignerDlg::OnBnClickedColorbtnForgndColor()
{
	COLORREF clorForegnd = m_clrForegnd.GetColor();

	CHARFORMAT2 cfmt;
	cfmt.cbSize = sizeof(CHARFORMAT2);
	m_REditString.GetDefaultCharFormat(cfmt);

	cfmt.crBackColor = clorForegnd;
	cfmt.crTextColor = clorForegnd;
	m_REditString.SetDefaultCharFormat(cfmt);
}


void COsdStringDesignerDlg::OnBnClickedColorbtnBackgndColor()
{
	COLORREF clrBackgnd = m_clrBackgnd.GetColor();
	m_REditString.SetBackgroundColor(FALSE, clrBackgnd);
}


BOOL COsdStringDesignerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	m_clrBackgnd.EnableOtherButton(_T("其它"));
	m_clrForegnd.EnableOtherButton(_T("其它"));

	CHARFORMAT2 cfmt2;
	memset(&cfmt2, 0, sizeof(cfmt2));
	cfmt2.cbSize = sizeof(CHARFORMAT2);

	m_REditString.GetDefaultCharFormat(cfmt2);

	_tcsncpy_s(cfmt2.szFaceName, "微软雅黑", LF_FACESIZE);
	cfmt2.wWeight = 400;
	cfmt2.yHeight = 320;

	m_clrBackgnd.SetColor(RGB(255,255,255));
	m_REditString.SetBackgroundColor(FALSE, m_clrBackgnd.GetColor());
	m_REditString.SetDefaultCharFormat(cfmt2);
	m_REditString.SetWindowText("This is a example strings!");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
