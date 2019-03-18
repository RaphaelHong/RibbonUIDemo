// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

using namespace Gdiplus;

// CLoginDlg �Ի���

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLoginDlg::IDD, pParent)
{
	m_BkPos = CPoint(0, 0);
	m_pMemDC = NULL;
}

CLoginDlg::~CLoginDlg()
{
	if (m_pMemDC != NULL)
	{
		delete m_pMemDC;
		m_pMemDC = NULL;
	}
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CBOX_USER_NAME, m_cboxUser);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnOK)
END_MESSAGE_MAP()


// CLoginDlg ��Ϣ�������
BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_KEY)), FALSE);

	CRect rc;
	GetWindowRect(rc);

	//CRgn rgn;
	//rgn.CreateRoundRectRgn(rc.left, rc.top, rc.right, rc.bottom, 15, 15);
	//SetWindowRgn(rgn, TRUE);

	//DWORD dwStyle = GetClassLong(m_hWnd, GCL_STYLE);
	//SetClassLong(m_hWnd, GCL_STYLE, dwStyle | CS_DROPSHADOW);

	AddUsers();

	SetTooltips();

	return TRUE;
}

void CLoginDlg::SetTooltips()
{
	m_tips.Create(this);
	m_tips.AddTool(GetDlgItem(IDC_CBOX_USER_NAME), _T("ѡ���û�"));
	m_tips.AddTool(GetDlgItem(IDC_EDIT_USER_PASSWORD), _T("��������"));
	m_tips.AddTool(GetDlgItem(IDOK), _T("�����¼"));
	m_tips.AddTool(GetDlgItem(IDCANCEL), _T("�˳���¼"));

	m_tips.SetDelayTime(200);
}


void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect  rect;     
	GetClientRect(&rect);  

	CBitmap bkBmp;
	bkBmp.LoadBitmap(IDB_BITMAP_LOG);

	Graphics gp(dc.m_hDC);
	Image* img;
	LinearGradientBrush bkBrush(Point(rect.left, rect.top), Point(rect.right, rect.bottom), Color(200, 147, 183, 240), Color(200, 75, 243, 182));
	if (BitmapToImage(&bkBmp, img))
	{
		gp.DrawImage(img, Rect(rect.left, rect.top, rect.Width(), rect.Height()));
	}
	else
		gp.FillRectangle(&bkBrush, rect.left, rect.top, rect.Width(), rect.Height());

	DrawTitle(&dc);
}

void CLoginDlg::DrawTitle(CDC *pValidDC)
{
	Graphics gp(pValidDC->m_hDC);

	CRect rcCli;
	GetClientRect(&rcCli);

	RectF textLayout;
	textLayout = RectF(0.0, 0.0, REAL(rcCli.Width()), 40.0);

	FontFamily fontFamily(L"������");
	Gdiplus::Font font(&fontFamily, 24, FontStyleUnderline|FontStyleBoldItalic, UnitPixel);
	SolidBrush blackBr(Color(255, 190, 212, 60));
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);	
	gp.DrawString(L"Welcome!", -1, &font, textLayout, &format, &blackBr);

	CRect rcUser, rcPassword;
	GetDlgItem(IDC_CBOX_USER_NAME)->GetWindowRect(rcUser);
	GetDlgItem(IDC_EDIT_USER_PASSWORD)->GetWindowRect(rcPassword);
	ScreenToClient(&rcUser);
	ScreenToClient(&rcPassword);

	format.SetAlignment(StringAlignmentNear);
	Gdiplus::Font ft(&fontFamily, 12, FontStyleRegular, UnitPixel);
	textLayout = RectF(REAL(rcUser.left - 90), REAL(rcUser.top), 90.0f, REAL(rcUser.Height()));
	gp.DrawString(L"�û�����", -1, &ft, textLayout, &format, &blackBr);

	textLayout = RectF(REAL(rcPassword.left - 90), REAL(rcPassword.top), 90.0f, REAL(rcPassword.Height()));
	gp.DrawString(L"���룺", -1, &ft, textLayout, &format, &blackBr);

	Gdiplus::Font ftInfo(&fontFamily, 12, FontStyleUnderline, UnitPixel);
	blackBr.SetColor(Color(255, 73, 34, 54));
	textLayout = RectF(REAL(rcCli.left+5), REAL(rcCli.bottom - 30), REAL(rcCli.Width()), 15.0f);
	gp.DrawString(L"��Ͼ�����Ƴ��򣬰汾1.0.0.1", -1, &ftInfo, textLayout, &format, &blackBr);

	textLayout = RectF(REAL(rcCli.left+5), REAL(rcCli.bottom - 15), REAL(rcCli.Width()), 15.0f);
	gp.DrawString(L"(P) 2016 RaphaeL.H. All rights reserved.", -1, &ftInfo, textLayout, &format, &blackBr);
}

void CLoginDlg::SetWndRgn(CDC *pDC, UINT uBackBmpID, COLORREF clrTrans)
{
	CDC dcMem;  
	if(!dcMem.CreateCompatibleDC(pDC))  
	{  
		MessageBox(_T("��������DCʧ�ܣ�"));  
	}  

	CBitmap bitmap;  
	if(!bitmap.LoadBitmap(uBackBmpID))  
	{  
		MessageBox(_T("����λͼʧ�ܣ�"));  
	}  

	if(!dcMem.SelectObject(&bitmap))  
	{  
		MessageBox(_T("ѡ���豸������ʧ�ܣ�"));  
	}  

	BITMAP bitmapinfo;  
	bitmap.GetBitmap(&bitmapinfo);  
	//�Ѵ�����ΪͼƬ�Ĵ�С,ȥ������Ļ�����ô����ָ�᲻��ȷ  
	MoveWindow(0,0,bitmapinfo.bmWidth,bitmapinfo.bmHeight,true);   
	//��������  
	CRgn rgn;  
	CRgn tmpRgn;  
	rgn.CreateRectRgn(0,0,bitmapinfo.bmWidth,bitmapinfo.bmHeight);  
	//�������������޳����к�ɫ��������  
	for(int i=0;i<bitmapinfo.bmWidth;i++)  
	{  
		for(int j=0;j<bitmapinfo.bmHeight;j++)  
		{  
			COLORREF cl=dcMem.GetPixel(i,j);  
			if(cl== clrTrans)  
			{  
				tmpRgn.CreateRectRgn(i,j,i+1,j+1);  
				rgn.CombineRgn(&rgn,&tmpRgn,RGN_XOR);  
				tmpRgn.DeleteObject();  
			}  
		}  
	}  
	//���ô�����ʾ����  
	SetWindowRgn(rgn,true);
}


LRESULT CLoginDlg::OnNcHitTest(CPoint point)
{
	//UINT nHitTest = CDialog::OnNcHitTest(point);

	//return nHitTest = (nHitTest== HTCAPTION ? nHitTest : HTCAPTION);

	return CDialog::OnNcHitTest(point);
}


void CLoginDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		//����λͼ�ĳߴ���Ϣ
		CBitmap bkbmp;
		bkbmp.LoadBitmap(IDB_BITMAP_LOG);
		BITMAP bmp;
		bkbmp.GetBitmap(&bmp);

		CRect rc;
		GetClientRect(rc);

		m_BkPos.x += 1;
		if (m_BkPos.x > bmp.bmWidth-rc.Width())
			m_BkPos.x = 0;

		m_BkPos.y += 1;
		if (m_BkPos.y > bmp.bmHeight-rc.Height())
			m_BkPos.y = 0;

		Invalidate();
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CLoginDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	return CDialog::OnEraseBkgnd(pDC);
}


inline void CLoginDlg::CopyBitmap(CBitmap *pBmpDest, CBitmap *pBmpSrc, int nPos, CRect Rect, CWnd *pWnd)  
{  
	CDC *pDC = pWnd->GetDC();  
	CDC MemDC;  
	CBrush brush; 

	MemDC.CreateCompatibleDC(pDC);        // �����봰��DC���ݵ��ڴ�DC  
	brush.CreatePatternBrush(pBmpSrc);    // ��Դλͼ������ˢ  
	pBmpDest->CreateCompatibleBitmap(pDC, Rect.Width(), Rect.Height());    // ������DC���ݵ�λͼ  

	MemDC.SelectObject(pBmpDest);         // ��λͼ����ѡ���ڴ�DC��  
	MemDC.SelectObject(brush);            // ����ˢѡ���ڴ�DC��  
	MemDC.Rectangle(0, 0, Rect.Width(), Rect.Height());    // ��ԭλͼ��ˢ���ڴ�DC�У��˲��Ժ�  

	pWnd->ReleaseDC(pDC);  
}

void CLoginDlg::AddUsers()
{
	m_cboxUser.ResetContent();

	CString strUser;
	pUserData pUser=NULL;
	for (int i=0; i<m_UserManager.GetUserCount(); i++)
	{
		pUser = m_UserManager.GetUser(i);
		if (pUser != NULL)
		{
			strUser.Format(_T("%s"), m_UserManager.GetUser(i)->szUserName);
			m_cboxUser.AddString(strUser);
		}		
	}

	if (theApp.g_CurrencyUser.nUserID >= 0)
	{
		strUser.Format(_T("%s"), theApp.g_CurrencyUser.szUserName);
		m_cboxUser.SelectString(0, strUser);
	}
	else
	{
		m_cboxUser.SetCurSel(0);
	}
}

void CLoginDlg::OnOK()
{
	CString strUser;
	m_cboxUser.GetWindowText(strUser);
	if (strUser.IsEmpty())
	{
		MessageBox(_T("�������û������ߴ��û��б���ѡ��һ���û���"), _T("��¼"), MB_OK|MB_ICONERROR);
		return;
	}

	int nIndex = m_UserManager.FindUserByName(strUser.GetBuffer(0));
	if (nIndex < 0)
	{
		MessageBox(_T("�Ҳ������û�����Ϣ��"), _T("��¼"), MB_OK|MB_ICONERROR);
		return;
	}

	pUserData pUser = m_UserManager.GetUser(nIndex);

	CString strPwd, strPwdInput;
	GetDlgItem(IDC_EDIT_USER_PASSWORD)->GetWindowText(strPwdInput);
	strPwd.Format(_T("%s"), pUser->szUserPwd);

	if (strPwd == strPwdInput)
	{
		theApp.g_CurrencyUser = *pUser;

		CDialog::OnOK();
	}
	else
	{
		MessageBox(_T("���������������ȷ���û����룡"), _T("��¼"), MB_OK|MB_ICONERROR);
		return;
	}

}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_tips.GetSafeHwnd())
	{
		m_tips.RelayEvent(pMsg);
	}

	return CDialog::PreTranslateMessage(pMsg);
}