// Hdmi3dOptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "Hdmi3dOptionDlg.h"
#include "Hdmi3dDlg.h"

// CHdmi3dOptionDlg �Ի���

IMPLEMENT_DYNAMIC(CHdmi3dOptionDlg, CXTPResizeDialog)

CHdmi3dOptionDlg::CHdmi3dOptionDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CHdmi3dOptionDlg::IDD, pParent)
{

}

CHdmi3dOptionDlg::~CHdmi3dOptionDlg()
{
}

void CHdmi3dOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GBOX_HDMI_INPUT_3DMODE, m_GboxIn3Dmode);
	DDX_Control(pDX, IDC_GBOX_HDMI_OUTPUT_3DMODE, m_GboxOut3Dmode);
	DDX_Control(pDX, IDC_GBOX_HDMI_OCTAVE, m_GboxOctave);
	DDX_Control(pDX, IDC_GBOX_HDMI_EYE_SWITCH, m_GboxEyeSwitch);
	DDX_Control(pDX, IDC_CBOX_HDMI_INPUT_3DMODE, m_CboxIn3Dmode);
	DDX_Control(pDX, IDC_CBOX_HDMI_OUTPUT_3DMODE, m_CboxOut3Dmode);
	DDX_Control(pDX, IDC_CBOX_HDMI_OCTAVE, m_CboxOctave);
	DDX_Control(pDX, IDC_CBOX_HDMI_EYE_SWITCH, m_CboxEye);
	DDX_Control(pDX, IDC_BTN_HDMI_SET_INPUT3DMOD, m_BtnSetIn3D);
	DDX_Control(pDX, IDC_BTN_HDMI_SET_OUTPUT_3DMODE, m_BtnSetOut3D);
	DDX_Control(pDX, IDC_BTN_HDMI_SET_OCTAVE, m_BtnSetOctave);
	DDX_Control(pDX, IDC_BTN_HDMI_SWITCH_EYE, m_BtnSwitchEye);
}


BEGIN_MESSAGE_MAP(CHdmi3dOptionDlg, CXTPResizeDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_HDMI_SET_INPUT3DMOD, &CHdmi3dOptionDlg::OnBnClickedBtnHdmiSetInput3dmod)
	ON_BN_CLICKED(IDC_BTN_HDMI_SET_OUTPUT_3DMODE, &CHdmi3dOptionDlg::OnBnClickedBtnHdmiSetOutput3dmode)
	ON_BN_CLICKED(IDC_BTN_HDMI_SET_OCTAVE, &CHdmi3dOptionDlg::OnBnClickedBtnHdmiSetOctave)
	ON_BN_CLICKED(IDC_BTN_HDMI_SWITCH_EYE, &CHdmi3dOptionDlg::OnBnClickedBtnHdmiSwitchEye)
END_MESSAGE_MAP()


// CHdmi3dOptionDlg ��Ϣ�������

HBRUSH CHdmi3dOptionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(theApp.m_DlgSetting.clrText);
		
		return m_bkBrush;
	}

	return hbr;
}

BOOL CHdmi3dOptionDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	m_bkBrush.CreateSolidBrush(constClrDlgBk[4]);

	ResizeAll();

	SetTheme(xtpControlThemeResource);

	m_CboxIn3Dmode.ResetContent();
	for (int i=0; i<7; i++) {
		m_CboxIn3Dmode.AddString(st3Dmode[i].str3Dname);
	}

	return TRUE;
}

void CHdmi3dOptionDlg::ResizeAll()
{
	SetResize(IDC_GBOX_HDMI_INPUT_3DMODE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_GBOX_HDMI_OUTPUT_3DMODE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_GBOX_HDMI_OCTAVE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_GBOX_HDMI_EYE_SWITCH, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_INPUT3D, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_OUTPUT3D, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_OCTAVE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_TEXT_HDMI_EYE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_CBOX_HDMI_INPUT_3DMODE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_CBOX_HDMI_OUTPUT_3DMODE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_CBOX_HDMI_OCTAVE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_CBOX_HDMI_EYE_SWITCH, XTP_ANCHOR_MIDDLECENTER,XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_BTN_HDMI_SET_INPUT3DMOD, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_BTN_HDMI_SET_OUTPUT_3DMODE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_BTN_HDMI_SET_OCTAVE, XTP_ANCHOR_MIDDLECENTER, XTP_ANCHOR_MIDDLECENTER);
	SetResize(IDC_BTN_HDMI_SWITCH_EYE, XTP_ANCHOR_MIDDLECENTER,XTP_ANCHOR_MIDDLECENTER);
}

void CHdmi3dOptionDlg::SetTheme(XTPControlTheme theme)
{
	m_GboxIn3Dmode.SetTheme(theme);
	m_GboxOut3Dmode.SetTheme(theme);
	m_GboxOctave.SetTheme(theme);
	m_GboxEyeSwitch.SetTheme(theme);
	m_CboxIn3Dmode.SetTheme(theme);
	m_CboxOut3Dmode.SetTheme(theme);
	m_CboxOctave.SetTheme(theme);
	m_CboxEye.SetTheme(theme);
	m_BtnSetIn3D.SetTheme(theme);
	m_BtnSetOut3D.SetTheme(theme);
	m_BtnSwitchEye.SetTheme(theme);
	m_BtnSetOctave.SetTheme(theme);
}

void CHdmi3dOptionDlg::Initailize()
{
	CHdmi3dDlg *pOwner = (CHdmi3dDlg*)GetOwner();
	int nPort = pOwner->m_boardinfo.GetBoardPort();

	//��ȡ����3Dģʽ
	int nData = 0;
	if (theApp.HDMI_GetInput3Dmode(nData, nPort) == CMD_FAILURE) {
		MessageBox(_T("��ȡ����3Dģʽʧ�ܣ�"), _T("HDMI_3D��ʼ��"), MB_OK|MB_ICONERROR);
	}
	else {
		for (int i=0; i<7; i++) {
			if (nData == st3Dmode[i].n3Dindex) {
				m_CboxIn3Dmode.SetCurSel(i);
				break;
			}
		}
	}

	//��ȡ���3Dģʽ
	if (theApp.HDMI_GetOutput3Dmode(nData, nPort) == CMD_FAILURE) {
		MessageBox(_T("��ȡ���3Dģʽʧ�ܣ�"), _T("HDMI_3D��ʼ��"), MB_OK|MB_ICONERROR);
	}
	else {
		m_CboxOut3Dmode.SetCurSel(nData);
	}

	//��ȡ��Ƶ����
	if (theApp.HDMI_GetOctave(nData, nPort) == CMD_FAILURE) {
		MessageBox(_T("��ȡ��Ƶ����ʧ�ܣ�"), _T("HDMI_3D��ʼ��"), MB_OK|MB_ICONERROR);
	}
	else {
		m_CboxOctave.SetCurSel(nData-1);
	}

}
void CHdmi3dOptionDlg::OnBnClickedBtnHdmiSetInput3dmod()
{
	UpdateData();

	int nSel = m_CboxIn3Dmode.GetCurSel();
	if (nSel == -1) {
		MessageBox(_T("��ѡ��һ������3Dģʽ��"), _T("��������3Dģʽ"), MB_OK|MB_ICONWARNING);
		return;
	}
	
	CHdmi3dDlg *pOwner = (CHdmi3dDlg*)GetOwner();
	int nPort = pOwner->m_boardinfo.GetBoardPort();	

	if (theApp.HDMI_SetInput3Dmode(st3Dmode[nSel].n3Dindex, nPort) == CMD_FAILURE) {
		MessageBox(_T("��������3Dģʽʧ�ܣ�"), _T("��������3Dģʽ"), MB_OK|MB_ICONERROR);
	}
	else
		MessageBox(_T("��������3Dģʽ�ɹ���"), _T("��������3Dģʽ"), MB_OK|MB_ICONINFORMATION);
}

void CHdmi3dOptionDlg::OnBnClickedBtnHdmiSetOutput3dmode()
{
	UpdateData();

	int nSel = m_CboxOut3Dmode.GetCurSel();
	if (nSel == -1) {
		MessageBox(_T("��ѡ��һ�����3D����ģʽ��"), _T("�������3Dģʽ"), MB_OK|MB_ICONWARNING);
		return;
	}

	CHdmi3dDlg *pOwner = (CHdmi3dDlg*)GetOwner();
	int nPort = pOwner->m_boardinfo.GetBoardPort();	

	if (theApp.HDMI_SetOutput3Dmode(nSel, nPort) == CMD_FAILURE) {
		MessageBox(_T("�������3D����ģʽʧ�ܣ�"), _T("�������3Dģʽ"), MB_OK|MB_ICONERROR);
	}
	else
		MessageBox(_T("�������3D����ģʽ�ɹ���"), _T("�������3Dģʽ"), MB_OK|MB_ICONINFORMATION);
}

void CHdmi3dOptionDlg::OnBnClickedBtnHdmiSetOctave()
{
	UpdateData();

	int nSel = m_CboxOctave.GetCurSel();
	if (nSel == -1) {
		MessageBox(_T("��ѡ��Ƶ������"), _T("���ó�Ƶ����"), MB_OK|MB_ICONWARNING);
		return;
	}

	CHdmi3dDlg *pOwner = (CHdmi3dDlg*)GetOwner();
	int nPort = pOwner->m_boardinfo.GetBoardPort();	

	if (theApp.HDMI_SetOutput3Dmode(nSel+1, nPort) == CMD_FAILURE) {
		MessageBox(_T("���ó�Ƶ����ʧ�ܣ�"), _T("���ó�Ƶ����"), MB_OK|MB_ICONERROR);
	}
	else
		MessageBox(_T("���ó�Ƶ�����ɹ���"), _T("���ó�Ƶ����"), MB_OK|MB_ICONINFORMATION);
}

void CHdmi3dOptionDlg::OnBnClickedBtnHdmiSwitchEye()
{
	CHdmi3dDlg *pOwner = (CHdmi3dDlg*)GetOwner();
	int nPort = pOwner->m_boardinfo.GetBoardPort();	

	if (theApp.HDMI_SetLeftRightEye(nPort) == CMD_FAILURE) {
		MessageBox(_T("�������л�ʧ�ܣ�"), _T("���ó�Ƶ����"), MB_OK|MB_ICONERROR);
	}
	else
		MessageBox(_T("�������л��ɹ���"), _T("���ó�Ƶ����"), MB_OK|MB_ICONINFORMATION);
}
