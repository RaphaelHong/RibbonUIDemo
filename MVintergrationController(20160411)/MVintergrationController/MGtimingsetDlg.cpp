// MGtimingsetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGtimingsetDlg.h"
#include "MGeoMainDlg.h"


// CMGtimingsetDlg �Ի���

IMPLEMENT_DYNAMIC(CMGtimingsetDlg, CXTPResizeDialog)

CMGtimingsetDlg::CMGtimingsetDlg(CWnd* pParent /*=NULL*/)
	: CXTPResizeDialog(CMGtimingsetDlg::IDD, pParent)
{
	memset(&m_curTiming, 0, sizeof(TIMING_STRUCT));

	m_pstrTming = NULL;
	m_pDetailedTiming = NULL;
	m_nTotalTiming = 0;
}

CMGtimingsetDlg::~CMGtimingsetDlg()
{
}

void CMGtimingsetDlg::DoDataExchange(CDataExchange* pDX)
{
	CXTPResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBOX_COMBMODE, m_cmbMode);
	DDX_Control(pDX, IDC_CBOX_TIMING_MG, m_cmbTiming);
	DDX_Control(pDX, IDC_BTN_TIMINGSET_MG, m_btnSet);
	DDX_Control(pDX, IDC_EDIT_TIMINGINFO, m_editTimingInfo);
}


BEGIN_MESSAGE_MAP(CMGtimingsetDlg, CXTPResizeDialog)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()


// CMGtimingsetDlg ��Ϣ�������
BOOL CMGtimingsetDlg::OnInitDialog()
{
	CXTPResizeDialog::OnInitDialog();

	CMGeoMainDlg *pOwer = (CMGeoMainDlg*)GetOwner();
	//m_curTiming = pOwer->m_boardInfo.GetDetailTiming(Product_MG)[0];

	if (m_wndPropertyGrid.GetSafeHwnd() == 0)
	{
		m_wndPropertyGrid.Create( CRect(0, 0, 0, 0), this, 0 );
		m_wndPropertyGrid.SetOwner(this);

		CFont font;
		font.CreatePointFont(60, _T("Marlett"));

		LOGFONT lf;
		font.GetLogFont(&lf);

		CXTPPropertyGridItem* pInfos           = m_wndPropertyGrid.AddCategory(_T("��ǰ�ֱ���"));
		pInfos->SetDescription(_T("��ǰ�����ź�Դ����Ϣ"));


		CXTPPropertyGridItem* pSignalInfo      = pInfos->AddChildItem(new CXTPPropertyGridItem(_T("�ź�Դ����"), _T("��")));
		CXTPPropertyGridItem* pTimingInfo      = pInfos->AddChildItem(new CXTPPropertyGridItem(_T("�źŷֱ���"), _T("��")));

		pSignalInfo->SetReadOnly(TRUE);
		pTimingInfo->SetReadOnly(TRUE);

		CXTPPropertyGridItem* pSettings        = m_wndPropertyGrid.AddCategory(_T("����"));
		CXTPPropertyGridItem* pCombModes       = pSettings->AddChildItem(new CXTPPropertyGridItem(_T("����ģʽ")));
		CXTPPropertyGridItem* pTimings         = pSettings->AddChildItem(new CXTPPropertyGridItem(_T("�ֱ���")));
		pSettings->SetDescription(_T("��������ֱ���"));
		pTimings->SetDescription(_T("���Ҳ�ѡ�����ѡ����ʵķֱ���"));

		
		CXTPPropertyGridItemConstraints* pList = pCombModes->GetConstraints();
		int i=0;
		while(stSplitMode[i].strmode != _T("END"))
		{
			pList->AddConstraint(stSplitMode[i].strmode);
			i++;
		}
		pCombModes->SetFlags(xtpGridItemHasComboButton);
		pCombModes->SetHidden(TRUE);

		//pList = pTimings->GetConstraints();
		//i=0;
		//CString *pStrTiming = pOwer->m_boardInfo.GetTimingName(Product_MG);
		//TIMING_STRUCT *pTiming = pOwer->m_boardInfo.GetDetailTiming(Product_MG);
		//int nFuncTag = (pOwer->m_boardInfo.GetFunCodeByte(0) & 0x0f);
		//while(pStrTiming[i] != _T("END"))
		//{
		//	if (nFuncTag == 1 )
		//	{//����
		//		if (pTiming[i].m_HActive > 1280 || pTiming[i].m_Hz == 120)  {
		//			i++; 
		//			continue; 
		//		}
		//	}
		//	else if (nFuncTag == 2)
		//	{//����
		//		if (pTiming[i].m_Hz == 120)
		//		{
		//			i++;
		//			continue;
		//		}
		//	}
		//		
		//	pList->AddConstraint(pStrTiming[i]);
		//	i++;
		//}
		pTimings->SetFlags(xtpGridItemHasComboButton);

		CXTPPropertyGridInplaceButton *pButton = pTimings->GetInplaceButtons()->AddButton(new CXTPPropertyGridInplaceButton(XTP_BUTTON_SETTIMING));
		pButton->SetCaption(_T("����"));	
		
		
		m_wndPropertyGrid.SetTheme(xtpGridThemeResource);
		m_wndPropertyGrid.HighlightChangedItems(TRUE);

		ExpandAllGridItems(m_wndPropertyGrid.GetCategories(), TRUE);
	}

	SetResize(IDC_EDIT_TIMINGINFO, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_CBOX_COMBMODE, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPCENTER);
	SetResize(IDC_CBOX_TIMING_MG, XTP_ANCHOR_TOPCENTER, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_BTN_TIMINGSET_MG, XTP_ANCHOR_TOPRIGHT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SEPARATOR1_TIMING_MG, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);
	SetResize(IDC_SEPARATOR2_TIMING_MG, XTP_ANCHOR_TOPLEFT, XTP_ANCHOR_TOPRIGHT);

	SetControlsTheme(xtpControlThemeResource);

	m_editTimingInfo.SetSel(-1,-1);
	m_editTimingInfo.ReplaceSel(_T("��ǰ�����źţ�DVI�ź�\n"));
	m_editTimingInfo.SetSel(-1,-1);
	m_editTimingInfo.ReplaceSel(_T("��ǰ�ֱ��ʣ�1920x1080 @ 60Hz"));

	m_cmbMode.ResetContent();
	int i=0;
	while(stSplitMode[i].strmode != _T("END"))
	{
		m_cmbMode.AddString(stSplitMode[i].strmode);
		i++;
	}
	m_cmbMode.SetCurSel(0);

	//����������ˢ
	m_bkBrush.CreateSolidBrush(constClrDlgBk[1]);


	return TRUE; 
}

void CMGtimingsetDlg::SetControlsTheme(XTPControlTheme theme)
{
	m_editTimingInfo.SetTheme(xtpControlThemeResource);
	m_editTimingInfo.SetFlatStyle(TRUE);
	m_cmbMode.SetTheme(theme);
	m_cmbTiming.SetTheme(theme);
	m_btnSet.SetTheme(theme);
}


HBRUSH CMGtimingsetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXTPResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		return m_bkBrush;
	}
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(15,15,1));

		return m_bkBrush;
	}

	return hbr;
}

void CMGtimingsetDlg::OnOK()
{
	//��ֹEsc Enter���˳�
	//CXTPResizeDialog::OnOK();
}
void CMGtimingsetDlg::OnCancel()
{
	//��ֹEsc Enter���˳�
	//CXTPResizeDialog::OnCancel();
}


void CMGtimingsetDlg::OnSize(UINT nType, int cx, int cy)
{
	CXTPResizeDialog::OnSize(nType, cx, cy);

	if (m_wndPropertyGrid.GetSafeHwnd())
	{
		m_wndPropertyGrid.MoveWindow(0,0,cx, cy);
	}
}

LRESULT CMGtimingsetDlg::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_INPLACEBUTTONDOWN)
	{
		CXTPPropertyGridInplaceButton *pBtn = (CXTPPropertyGridInplaceButton*)lParam;
		if (pBtn->GetID() == XTP_BUTTON_SETTIMING)
		{	
			CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(_T("�ֱ���"));
			int nSel = pItem->GetConstraints()->GetCurrent();
			if (nSel != -1)
			{
				int nRet = SetTiming(nSel);

				if (nRet == CMD_RTOK)
				{
					pItem = m_wndPropertyGrid.FindItem(_T("�źŷֱ���"));
					int nPos = 0;
					CMGeoMainDlg *pOwer = (CMGeoMainDlg*)GetOwner();
					pItem->SetValue(pOwer->m_boardInfo.GetTimingName(Product_MG)[nSel].Tokenize(_T("("), nPos));
				}
			}

			return TRUE;
		}		
	}

	if (wParam == XTP_PGN_SELECTION_CHANGED)
	{

	}

	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)
	{
		
	}
	

	return 0;
}

int CMGtimingsetDlg::SetTiming(int nIndex)
{
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();

	CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(_T("�ֱ���"));
	CString strTiming = pItem->GetValue();

	int nCursel = -1;
	for (int i=0; i<m_nTotalTiming; i++)
	{
		if (m_pstrTming[i] == strTiming)
		{
			nCursel =  i;
			break;
		}
	}

	if (nCursel == -1) 
	{ //û�ҵ���Ӧ�ķֱ��� һ�㲻����������
		MessageBox(_T("�ֱ��ʲ�������"), _T("���÷ֱ���"), MB_OK|MB_ICONSTOP);
		return -1;
	}

	pTIMING_STRUCT pTS;
	pTS = &m_pDetailedTiming[nCursel];

	int nRet = theApp.SetInputTiming(pTS, 3, 0xff, pOwner->m_boardInfo.GetBoardPort());

	if (nRet == CMD_RTOK)
	{
		m_curTiming = *pTS;
		MessageBox(_T("���÷ֱ��ʳɹ������²�������ź�����Ч��"), _T("���÷ֱ���"), MB_OK|MB_ICONINFORMATION);
	}
	else MessageBox(_T("���÷ֱ���ʧ�ܣ�"), _T("���÷ֱ���"), MB_ICONSTOP);

	return nRet;
}

void CMGtimingsetDlg::Initialize()
{
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();

	m_pDetailedTiming = pOwner->m_boardInfo.GetDetailTiming(Product_MG);
	m_pstrTming       = pOwner->m_boardInfo.GetTimingName(Product_MG);
	m_nTotalTiming    = pOwner->m_boardInfo.GetTotalTimings(Product_MG);

	AddTimingToList();

	//pTIMING_STRUCT pTS  = pOwner->m_boardInfo.GetDetailTiming(Product_MG);
	//CString *pStrTiming = pOwner->m_boardInfo.GetTimingName(Product_MG);
	//int nTotalTiming    = pOwner->m_boardInfo.GetTotalTimings(Product_MG);

	TIMING_STRUCT ts;
	memset(&ts, 0, sizeof(TIMING_STRUCT));

	int nRet = theApp.GetInputTiming(&ts, 3, 0xff, pOwner->m_boardInfo.GetBoardPort());
	//int nRet = theApp.GetCurrencyInputTiming((UINT&)ts.m_Hz, (UINT&)ts.m_HTotal, (UINT&)ts.m_HActive, (UINT&)ts.m_VTotal, (UINT&)ts.m_VActive, pOwner->m_boardInfo.GetBoardPort());

	int nIndex = 0;
	if (nRet == CMD_RTOK)
	{		
		while(nIndex < m_nTotalTiming)
		{
			if (m_pDetailedTiming[nIndex].m_HActive == ts.m_HActive
			  &&m_pDetailedTiming[nIndex].m_VActive == ts.m_VActive
			  &&m_pDetailedTiming[nIndex].m_Hz == ts.m_Hz)
			{
				m_curTiming = m_pDetailedTiming[nIndex];
				break;
			}

			nIndex++;
		}
	}
	else
	{
		nIndex = -1;
		MessageBox(_T("��ȡ����ֱ���ʧ�ܣ�"), _T("��ȡ����ֱ���"), MB_ICONERROR);
	}

	CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(_T("�ź�Դ����"));
	pItem->SetValue(_T("DVI�ź�"));

	if (nIndex < m_nTotalTiming && nIndex != -1)
	{
		int nPos =0;
		pItem = m_wndPropertyGrid.FindItem(_T("�źŷֱ���"));
		pItem->SetValue(m_pstrTming[nIndex].Tokenize(_T("("), nPos));

		pItem = m_wndPropertyGrid.FindItem(_T("�ֱ���"));
		pItem->SetValue(m_pstrTming[nIndex]);
	}
	else
	{
		if (ts.m_HActive <=0 || ts.m_VActive <= 0)
		{
			//MessageBox(_T("���棺��ǰ�忨δ�����źţ�����У���Ͷ��У��������Ĭ�Ϸֱ���(1024x768 @ 60Hz)��"),_T("����У����ʼ��"), MB_OK|MB_ICONERROR);
			MessageBox(_T("û�������źţ���"), _T("��ȡ����ֱ���"), MB_ICONERROR);
		}
		else
			MessageBox(_T("δ���������ֱ��ʣ�"), _T("��ȡ����ֱ���"), MB_ICONERROR);
	}
}

void CMGtimingsetDlg::AddTimingToList()
{
	CMGeoMainDlg *pOwner = (CMGeoMainDlg*)GetOwner();

	CXTPPropertyGridItem* pTimingItem = m_wndPropertyGrid.FindItem(_T("�ֱ���"));
	CXTPPropertyGridItemConstraints* pList = pTimingItem->GetConstraints();
	pList = pTimingItem->GetConstraints();

	int i=0;
	//CString *pStrTiming = pOwner->m_boardInfo.GetTimingName(Product_MG);
	//TIMING_STRUCT *pTiming = pOwner->m_boardInfo.GetDetailTiming(Product_MG);
	int nFuncTag = (pOwner->m_boardInfo.GetFunCodeByte(0) & 0x0f);
	while(m_pstrTming[i] != _T("END"))
	{
		if (nFuncTag == 1 )
		{//����
			if (m_pDetailedTiming[i].m_HActive > 1280 || m_pDetailedTiming[i].m_Hz == 120)  {
				i++; 
				continue; 
			}
		}
		else if (nFuncTag == 2)
		{//����
			if (m_pDetailedTiming[i].m_Hz == 120)
			{
				i++;
				continue;
			}
		}
			
		pList->AddConstraint(m_pstrTming[i]);
		i++;
	}
}