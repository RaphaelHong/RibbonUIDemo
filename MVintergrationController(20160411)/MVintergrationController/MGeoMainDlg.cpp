// MGeomoduleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGeoMainDlg.h"
// CMGeomoduleDlg �Ի���


IMPLEMENT_DYNAMIC(CMGeoMainDlg, CXTPResizeDialog)

CMGeoMainDlg::CMGeoMainDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMGeoMainDlg::IDD, pParent)
{
}

CMGeoMainDlg::~CMGeoMainDlg()
{

}

void CMGeoMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMGeoMainDlg, CXTPResizeDialog)
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MG_TABCONTROL, OnSelectChanged)
END_MESSAGE_MAP()


// CMGeomoduleDlg ��Ϣ�������

BOOL CMGeoMainDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	//CreateSubWindows();	

	CRect rc;
	GetClientRect(&rc);

	m_wndTabCtrl.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, rc, this, IDC_MG_TABCONTROL);
	m_wndTabCtrl.GetPaintManager()->SetAppearance(xtpTabAppearancePropertyPage2003); 
	m_wndTabCtrl.GetPaintManager()->DisableLunaColors(FALSE); //ֻ��office2003 2007 2010���������Luna colors
	m_wndTabCtrl.GetPaintManager()->SetOneNoteColors(TRUE);
	m_wndTabCtrl.GetPaintManager()->m_bHotTracking = TRUE;
	m_wndTabCtrl.GetPaintManager()->m_bShowIcons = TRUE;
	m_wndTabCtrl.GetPaintManager()->SetPosition(xtpTabPositionBottom);
	m_wndTabCtrl.GetPaintManager()->m_bBoldSelected = TRUE;
	m_wndTabCtrl.GetImageManager()->SetIcons(IDB_BITMAP_MG_TAP, NULL,  0, CSize(16,16), xtpImageNormal);

	SetResize(IDC_MG_TABCONTROL, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_BOTTOMRIGHT);

	return TRUE;
}

void CMGeoMainDlg::CreateSubWindows()
{
	int nItem = 0;

	//�ֱ�������ѡ�
	m_TimingSetDlg.Create(IDD_DLG_MG_TIMING, &m_wndTabCtrl);
	m_TimingSetDlg.SetOwner(this);
	m_wndTabCtrl.InsertItem(nItem++, _T("�ֱ���"),   m_TimingSetDlg,   0);

	//ɫ��У��ѡ�
	m_ColorSetDlg.Create(IDD_DLG_MG_COLOR, &m_wndTabCtrl);
	m_ColorSetDlg.SetOwner(this);
	m_wndTabCtrl.InsertItem(nItem++, _T("ɫ��"),     m_ColorSetDlg,    1);

	// MG310�ͺŵİ忨��֧���ںϴ��ͺڵ�ƽ�� MG300��֧��
	if (m_boardInfo.GetFunCodeByte(3) == 0x31)
	{
		//�ںϴ�����ѡ�
		m_BlendSetDlg.Create(IDD_DLG_MG_BLEND, &m_wndTabCtrl);
		m_BlendSetDlg.SetOwner(this);
		m_wndTabCtrl.InsertItem(nItem++, _T("�ںϴ�"),   m_BlendSetDlg,    2);

		//�ڵ�ƽ����ѡ�
		m_BlacklvlSetDlg.Create(IDD_DLG_MG_BLACKLVL, &m_wndTabCtrl);
		m_BlacklvlSetDlg.SetOwner(this);
		m_wndTabCtrl.InsertItem(nItem++, _T("�ڵ�ƽ"),   m_BlacklvlSetDlg, 3);
	}

	//����У��ѡ�
	m_GeometricDlg.Create(IDD_DLG_MG_GEOMETRIC, &m_wndTabCtrl);
	m_GeometricDlg.SetOwner(this);
	m_wndTabCtrl.InsertItem(nItem++, _T("����У��"), m_GeometricDlg,   4);

	//���У��ѡ�
	m_GridWarpDlg.Create(IDD_DLG_MG_GRIDWARP, &m_wndTabCtrl);
	m_GridWarpDlg.SetOwner(this);
	m_wndTabCtrl.InsertItem(nItem++, _T("���У��"), m_GridWarpDlg,    5);

	//ͼ��ۻ�ѡ�
	m_FilterDlg.Create(IDD_DLG_MG_FILTER, &m_wndTabCtrl);
	m_FilterDlg.SetOwner(this);
	m_wndTabCtrl.InsertItem(nItem++, _T("ͼ��ۻ�"), m_FilterDlg,      6);
}

void CMGeoMainDlg::OnOK()
{
	//��ֹEnter Esc���˳�
	//CXTPResizeDialog::OnOK();
}
void CMGeoMainDlg::OnCancel()
{
	//��ֹEnter Esc���˳�
	//CXTPResizeDialog::OnCancel();
}

void CMGeoMainDlg::SetBoardInfo(CBaseBoard boardinfo)
{
	m_boardInfo = boardinfo;

	CreateSubWindows();
}

void CMGeoMainDlg::Initialize()
{
	//����У���߳�
	if ((theApp.m_ThreadAjustHandle == NULL) && (!theApp.StartThread(0)))
	{
		MessageBox(_T("Warning��failed to start ajust thread��"), _T("Start Work Thread"), MB_OK|MB_ICONSTOP);
	}
	else
		theApp.ResumeThread(AJUST_THREAD);

	CWaitCursor wait;

	if (m_TimingSetDlg.GetSafeHwnd())   m_TimingSetDlg.Initialize();
	Sleep(50);
	if (m_ColorSetDlg.GetSafeHwnd())    m_ColorSetDlg.Initialize();
	Sleep(50);
	if (m_BlendSetDlg.GetSafeHwnd())    m_BlendSetDlg.Initialize();
	Sleep(50);
	if (m_BlacklvlSetDlg.GetSafeHwnd()) m_BlacklvlSetDlg.Initialize();
	Sleep(50);
	if (m_GeometricDlg.GetSafeHwnd())   m_GeometricDlg.Initialize();
	Sleep(50);
	if (m_FilterDlg.GetSafeHwnd())      m_FilterDlg.Initialize();
	Sleep(50);
	if (m_GridWarpDlg.GetSafeHwnd())    m_GridWarpDlg.Initialize();
}

BOOL CMGeoMainDlg::SaveAll()
{
	//����SALL����
	if (theApp.SaveAll(m_boardInfo.GetBoardPort()) == CMD_FAILURE)	return FALSE;
	
	//����У�����ݱ���
	if (m_GeometricDlg.GetSafeHwnd())	{
		if (!m_GeometricDlg.SaveGeoDatas()) return FALSE;
	}

	return TRUE;
}

BOOL CMGeoMainDlg::OnEraseBkgnd(CDC* pDC)
{
	return CXTPResizeDialog::OnEraseBkgnd(pDC);
}

void CMGeoMainDlg::OnSelectChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);

	int nCurSel = m_wndTabCtrl.GetCurSel();

	CXTPTabManagerItem *pTabItem = m_wndTabCtrl.GetItem(nCurSel);
	CWnd *pWnd = FromHandle(pTabItem->GetHandle());
	if (pWnd != NULL) pWnd->SetFocus();

	*pResult = 0;
}

void CMGeoMainDlg::GetCurrencyTiming(TIMING_STRUCT &ts)
{
	if (m_TimingSetDlg.GetSafeHwnd())
	{
		ts = m_TimingSetDlg.m_curTiming;
	}
	else
	{
		memset(&ts, 0, sizeof(TIMING_STRUCT));
	}
}

int CMGeoMainDlg::GetCurrencyBlendWidth(int nBlendSide)
{
	if (m_BlendSetDlg.GetSafeHwnd())
	{
		return m_BlendSetDlg.m_stBlendSetting[nBlendSide].nBlendWidth;
	}

	return 0;
}

void CMGeoMainDlg::NotifyBlendWidthChanged()
{
	if (m_BlacklvlSetDlg.GetSafeHwnd())
	{
		m_BlacklvlSetDlg.SetTotalBlendZone();
	}
}

void CMGeoMainDlg::GetGeoDpaDatas(CDoublePointArray *&p4Point, CDoublePointArray *&pCurve, CDoublePointArray *&pDensity)
{
	if (m_GeometricDlg.GetSafeHwnd())
	{
		p4Point  = m_GeometricDlg.m_pDpa4points;
		pCurve   = m_GeometricDlg.m_pDpaCurvepoints;
		pDensity = m_GeometricDlg.m_pDpaDensitypoints;
	}
}