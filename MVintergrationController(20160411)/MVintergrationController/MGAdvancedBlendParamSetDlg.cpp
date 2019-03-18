// AdvancedBlendParamSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MVintergrationController.h"
#include "MGAdvancedBlendParamSetDlg.h"

//******************************************************************************************************************
//                                            CAdvancedBlendParamSetDlg.cpp
//******************************************************************************************************************

// CAdvancedBlendParamSetDlg 对话框

IMPLEMENT_DYNAMIC(CAdvancedBlendParamSetDlg, CDialog)

CAdvancedBlendParamSetDlg::CAdvancedBlendParamSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvancedBlendParamSetDlg::IDD, pParent)
	,m_nLowerPos(1)
	,m_nUpperPos(64)
	,m_nIndivPos(32)
{

}

CAdvancedBlendParamSetDlg::~CAdvancedBlendParamSetDlg()
{

}

void CAdvancedBlendParamSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEFTNONIUS_POS, m_nLowerPos);
	DDX_Text(pDX, IDC_EDIT_LEFTNONIUS_VAL, m_nLowerVal);
	DDX_Text(pDX, IDC_EDIT_RIGHTNONIUS_POS, m_nUpperPos);
	DDX_Text(pDX, IDC_EDIT_RIGHTNONIUS_VAL, m_nUpperVal);
	DDX_Text(pDX, IDC_EDIT_INDIVNONIUS_POS, m_nIndivPos);
	DDX_Text(pDX, IDC_EDIT_INDIVNONIUS_VAL, m_nIndivVal);
}


BEGIN_MESSAGE_MAP(CAdvancedBlendParamSetDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LEFTNONIUS_POS, &CAdvancedBlendParamSetDlg::OnDeltaposSpinNonius)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LEFTNONIUS_VAL, &CAdvancedBlendParamSetDlg::OnDeltaposSpinNonius)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RIGHTNONIUS_POS, &CAdvancedBlendParamSetDlg::OnDeltaposSpinNonius)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RIGHTNONIUS_VAL, &CAdvancedBlendParamSetDlg::OnDeltaposSpinNonius)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INDIVNONIUS_POS, &CAdvancedBlendParamSetDlg::OnDeltaposSpinNonius)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INDIVNONIUS_VAL, &CAdvancedBlendParamSetDlg::OnDeltaposSpinNonius)
END_MESSAGE_MAP()


// CAdvancedBlendParamSetDlg 消息处理程序

BOOL CAdvancedBlendParamSetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetSpinButtonBuddy();
	
	CreateView();

	InitViewData();

	pView->SetBlenddParaData();

	UpdateDlgData(m_nLowerPos, pView->m_BlendParamData[m_nLowerPos], m_nUpperPos, pView->m_BlendParamData[m_nUpperPos], m_nIndivPos, pView->m_BlendParamData[m_nIndivPos]);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CAdvancedBlendParamSetDlg::SetSpinButtonBuddy()
{
	CSpinButtonCtrl *pSpinBtn;
	pSpinBtn = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_LEFTNONIUS_POS);
	pSpinBtn->SetBuddy(GetDlgItem(IDC_EDIT_LEFTNONIUS_POS));
	pSpinBtn = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_LEFTNONIUS_VAL);
	pSpinBtn->SetBuddy(GetDlgItem(IDC_EDIT_LEFTNONIUS_VAL));

	pSpinBtn = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_RIGHTNONIUS_POS);
	pSpinBtn->SetBuddy(GetDlgItem(IDC_EDIT_RIGHTNONIUS_POS));
	pSpinBtn = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_RIGHTNONIUS_VAL);
	pSpinBtn->SetBuddy(GetDlgItem(IDC_EDIT_RIGHTNONIUS_VAL));

	pSpinBtn = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_INDIVNONIUS_POS);
	pSpinBtn->SetBuddy(GetDlgItem(IDC_EDIT_INDIVNONIUS_POS));
	pSpinBtn = (CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_INDIVNONIUS_VAL);
	pSpinBtn->SetBuddy(GetDlgItem(IDC_EDIT_INDIVNONIUS_VAL));
}

BOOL CAdvancedBlendParamSetDlg::CreateView()
{
	CRect rcView;
	GetDlgItem(IDC_PICTURE_VIEW)->GetWindowRect(&rcView);
	ScreenToClient(&rcView);
	rcView.InflateRect(-3,-3,-3, -3);

	CFrameWnd *pFrameWnd = (CFrameWnd*)this;
	CCreateContext context;
	context.m_pNewViewClass = RUNTIME_CLASS(CAdvancedBlendParamView);

	pView = (CAdvancedBlendParamView *)(pFrameWnd->CreateView(&context));
	ASSERT(pView);

	pView->MoveWindow(&rcView);

	return TRUE;
}

void CAdvancedBlendParamSetDlg::InitViewData()
{
	ASSERT(pView);

	pView->m_nChannel = m_nPort;
	pView->m_BlendWidth = m_nBlendWidth;
	pView->m_nPureWhite = m_PureWhite;
	pView->alpha = blend_alpha;
	pView->p = blend_p;
	pView->gamma = blend_gamma;
	pView->m_nSide = m_nBldSide;

	//memcpy(pView->m_BlendParamData, m_nBlendVal, 1024*sizeof(USHORT));
}

void CAdvancedBlendParamSetDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

BOOL CAdvancedBlendParamSetDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			if (GetFocus() == GetDlgItem(IDC_EDIT_LEFTNONIUS_POS)
			 ||(GetFocus() == GetDlgItem(IDC_EDIT_LEFTNONIUS_VAL))
			 ||(GetFocus() == GetDlgItem(IDC_EDIT_RIGHTNONIUS_POS))
			 ||(GetFocus() == GetDlgItem(IDC_EDIT_RIGHTNONIUS_VAL))
			 ||(GetFocus() == GetDlgItem(IDC_EDIT_INDIVNONIUS_POS))
			 ||(GetFocus() == GetDlgItem(IDC_EDIT_INDIVNONIUS_VAL)))
			{
				OnVirtrulKeyEnter(GetFocus());
			}

			return TRUE;
		}
		else if (pMsg->wParam == VK_UP
			|| pMsg->wParam == VK_DOWN
			|| pMsg->wParam == VK_LEFT
			|| pMsg->wParam == VK_RIGHT)
		{
			pView->OnVKdown(pMsg->wParam);

			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CAdvancedBlendParamSetDlg::OnVirtrulKeyEnter(CWnd *pDataWnd)
{
	UpdateData();

	CEdit *pEdit = (CEdit*)pDataWnd;

	if (GetDlgItem(IDC_EDIT_LEFTNONIUS_POS) == pDataWnd)
	{
		if (m_nLowerPos <= m_nUpperPos-5 && m_nLowerPos >= 1)
		{
			pView->SetNoniusState(Nonius_LeftPos, m_nLowerPos-1);
		}
		else
		{
			pEdit->GetFocus();
			pEdit->SetSel(0, pEdit->GetWindowTextLength());
			AfxMessageBox(_T("左游标位置应大于0，小于右游标位置！"));
		}
	}
	else if (GetDlgItem(IDC_EDIT_LEFTNONIUS_VAL) == pDataWnd)
	{
		if (m_nLowerVal <= 1024 && m_nLowerPos >= 0)
		{
			pView->SetNoniusState(Nonius_LeftVal, m_nLowerVal);
		}
		else
		{
			pEdit->GetFocus();
			pEdit->SetSel(0, pEdit->GetWindowTextLength());
			AfxMessageBox(_T("融合值范围：0 - 1024 ！"));
		}
	}
	else if (GetDlgItem(IDC_EDIT_RIGHTNONIUS_POS) == pDataWnd)
	{
		if (m_nUpperPos > m_nLowerPos+5 && m_nUpperPos <= m_nBlendWidth)
		{
			pView->SetNoniusState(Nonius_RigtPos, m_nUpperPos-1);
		}
		else
		{
			pEdit->GetFocus();
			pEdit->SetSel(0, pEdit->GetWindowTextLength());
			AfxMessageBox(_T("右游标位置应大于左游标位置，小于最大融合带宽度 ！"));
		}
	}
	else if (GetDlgItem(IDC_EDIT_RIGHTNONIUS_VAL) == pDataWnd)
	{
		if (m_nUpperVal <= 1024 && m_nUpperVal >= 0)
		{
			pView->SetNoniusState(Nonius_RigtVal, m_nUpperVal);
		}
		else
		{
			pEdit->GetFocus();
			pEdit->SetSel(0, pEdit->GetWindowTextLength());
			AfxMessageBox(_T("融合值范围：0 - 1024 ！"));
		}
	}
	else if (GetDlgItem(IDC_EDIT_INDIVNONIUS_POS) == pDataWnd)
	{
		if (m_nIndivPos <= m_nBlendWidth && m_nIndivPos >= 1)
		{
			pView->SetNoniusState(Nonius_IndvPos, m_nIndivPos-1);
		}
		else
		{
			pEdit->GetFocus();
			pEdit->SetSel(0, pEdit->GetWindowTextLength());
			AfxMessageBox(_T("独立游标位置应大于0，小于最大融合带宽度！"));
		}
	}
	else if (GetDlgItem(IDC_EDIT_INDIVNONIUS_VAL) == pDataWnd)
	{
		if (m_nIndivVal <= 1024 && m_nIndivVal >= 0)
		{ 
			pView->SetNoniusState(Nonius_IndvVal, m_nIndivVal); 
		}
		else
		{
			pEdit->GetFocus();
			pEdit->SetSel(0, pEdit->GetWindowTextLength());
			AfxMessageBox(_T("融合值范围：0 - 1024 ！"));
		}
	}

}

void CAdvancedBlendParamSetDlg::OnDeltaposSpinNonius(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码\

	UINT_PTR idFrom = pNMHDR->idFrom;
	int iDelta = pNMUpDown->iDelta;
	int nNewVal;

	switch(idFrom)
	{
	case IDC_SPIN_INDIVNONIUS_POS:
		nNewVal = m_nIndivPos-iDelta;
		if((nNewVal < 1) || (nNewVal > m_nBlendWidth)) return;
		m_nIndivPos = nNewVal;
		pView->SetNoniusState(Nonius_IndvPos, m_nIndivPos-1);
		break;
	case IDC_SPIN_INDIVNONIUS_VAL:
		nNewVal = m_nIndivVal-iDelta;
		if((nNewVal < 0) || (nNewVal > 1024)) return;
		m_nIndivVal = nNewVal;
		pView->SetNoniusState(Nonius_IndvVal, m_nIndivVal);
		break;
	case IDC_SPIN_RIGHTNONIUS_POS:
		nNewVal = m_nUpperPos-iDelta;
		if((nNewVal < m_nLowerPos+5) || (nNewVal > m_nBlendWidth)) return;
		m_nUpperPos = nNewVal;
		pView->SetNoniusState(Nonius_RigtPos, m_nUpperPos-1);
		break;
	case IDC_SPIN_RIGHTNONIUS_VAL:
		nNewVal = m_nUpperVal-iDelta;
		if((nNewVal < 0) || (nNewVal > 1024)) return;
		m_nUpperVal = nNewVal;
		pView->SetNoniusState(Nonius_RigtVal, m_nUpperVal);
		break;
		break;
	case IDC_SPIN_LEFTNONIUS_POS:
		nNewVal = m_nLowerPos-iDelta;
		if((nNewVal > m_nUpperPos-5) || (nNewVal < 1)) return;
		m_nLowerPos = nNewVal;
		pView->SetNoniusState(Nonius_LeftPos, m_nLowerPos-1);
		break;
	case IDC_SPIN_LEFTNONIUS_VAL:
		nNewVal = m_nLowerVal-iDelta;
		if((nNewVal < 0) || (nNewVal > 1024)) return;
		m_nLowerVal = nNewVal;
		pView->SetNoniusState(Nonius_LeftVal, m_nLowerVal);
		break;
	default: break;
	}

	UpdateData(FALSE);

	*pResult = 0;
}

void CAdvancedBlendParamSetDlg::UpdateDlgData(int nPosLeft, int nValLeft, int nPosRight, int nValRight, int nPosIndiv, int nValIndiv)
{
	m_nLowerPos = nPosLeft;
	m_nLowerVal = nValLeft;
	m_nUpperPos = nPosRight;
	m_nUpperVal = nValRight;
	m_nIndivPos = nPosIndiv;
	m_nIndivVal = nValIndiv;

	UpdateData(FALSE);
}

//融化曲线 table
// pblend_para: side_id, white_blend, alpa, P, gama

bool UpdateBlendTable(unsigned int *pblend_para, int nwidth, double alpha, double p, double gamma)
{
	unsigned int i;
	unsigned int  blend_width;
	double  blend_alpha;
	double  blend_P;
	double  blend_gamma;
	double  fx,fx_gama;
	unsigned int	tempR;

	//Get Blend Width	  
	blend_width = nwidth;//0-L; 1-T; 2-R; 3-B
	if(blend_width == 0)
	{
		return false;
	}

	blend_alpha = alpha;//float(pblend_para[2]/65535.0);  //blend_alpha 	
	blend_P = p;//float(pblend_para[3]/65535.0*16);  //blend_P   
	blend_gamma = gamma;//float(pblend_para[4]/65535.0*16);  //blend_gamma 

	if(blend_gamma<0.01)
	{
		blend_gamma = float(0.01);
	}

	for(i=0;i<blend_width;i++)
	{
		if(i<(blend_width/2))
		{
			fx = blend_alpha*pow(float(2.0*(float)i/(blend_width-1)), (float)blend_P);
		}
		else
		{
			fx = float(1.0 - (1.0-blend_alpha)*pow(float(2.0-2.0*(float)i/(blend_width-1)), (float)blend_P));
		}

		fx_gama = pow((float)fx,float(1.0/blend_gamma));

		tempR = (unsigned int)(fx_gama*1024.0);	

		if(tempR >1024)
		{
			tempR = 1024 ;
		}

		pblend_para[i] = tempR;
	}

	return true;
}


//******************************************************************************************************************
//                                            AdvancedBlendParamView.cpp
//******************************************************************************************************************
// AdvancedBlendParamView.cpp : 实现文件
//

// CAdvancedBlendParamView

IMPLEMENT_DYNCREATE(CAdvancedBlendParamView, CView)

CAdvancedBlendParamView::CAdvancedBlendParamView()
{
	InitMemberVariables();
}

CAdvancedBlendParamView::~CAdvancedBlendParamView()
{
}

BEGIN_MESSAGE_MAP(CAdvancedBlendParamView, CView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CAdvancedBlendParamView 绘图

void CAdvancedBlendParamView::InitMemberVariables()
{
	InitButtons();

	m_nRightNonius = 64; //右括号位置
	m_nLeftNonius = 0;   //左括号位置
	m_nIndivNonius = 32; //单独控制点位置

	m_bLBtnDownInNonius = FALSE;

	m_eNoniusType = Nonius_Null;
	m_eCurNonius = Nonius_Null;
}

void CAdvancedBlendParamView::InitButtons()
{
	for (int i=0; i<4; i++)
	{
		m_ZonSelBtn[i].bClicked = FALSE;
		m_ZonSelBtn[i].bMouseOver = FALSE;
	}
}

bool CAdvancedBlendParamView::SetBlenddParaData()
{
	return UpdateBlendTable(m_BlendParamData, m_BlendWidth, alpha, p, gamma);
}

void CAdvancedBlendParamView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码

	//获取视图区大小
	CRect rcClient;
	GetClientRect(rcClient);

	//创建兼容DC
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());
	//创建空位图
	CBitmap *pOldbmp = dcMem.SelectObject(&MemBitmap); 
	//用指定颜色清除背景
	dcMem.FillSolidRect(0, 0, rcClient.Width(), rcClient.Height(), RGB(0x40, 0x00, 0x00));

	//画背景
	CPen pen,*pOldPen;
	pen.CreatePen(PS_SOLID, 1, RGB(0xe8, 0xe0, 0xb0));
	pOldPen = dcMem.SelectObject(&pen);
	dcMem.Rectangle(CRect(rcClient.left+1, rcClient.top+1, rcClient.right-1, rcClient.bottom-1));
	dcMem.SelectObject(pOldPen);
	dcMem.FillSolidRect(CRect(rcClient.left+2, rcClient.top+2, rcClient.right-2, rcClient.bottom-2), RGB(238,238,227));

	//计算合适的按钮和坐标轴位置
	m_ZonSelBtn[0].rcBtn = CRect(rcClient.left+2, rcClient.top+22, rcClient.left+22, rcClient.bottom-22);    //左边按钮
	m_ZonSelBtn[1].rcBtn = CRect(rcClient.left+22, rcClient.top+2, rcClient.right-22,   rcClient.top+22);    //顶部按钮
	m_ZonSelBtn[2].rcBtn = CRect(rcClient.right-22, rcClient.top+22, rcClient.right-2, rcClient.bottom-22);  //右边按钮
	m_ZonSelBtn[3].rcBtn = CRect(rcClient.left+22, rcClient.bottom-22, rcClient.right-22, rcClient.bottom-2);//底部按钮
	
	m_rcChart = CRect(rcClient.left+50, rcClient.top+50, rcClient.right-50, rcClient.bottom-50);

	//画坐标轴
	DrawAxis(&dcMem);

	//画数据线
	DrawDataLine(&dcMem);

	//画控制符
	DrawRangeNonius(&dcMem);
	DrawIndivNonius(&dcMem);

	//将内存DC拷贝到真实DC中
	pDC->BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &dcMem, 0 , 0, SRCCOPY);
}

//********************************************************************
// 画上下左右四个融合带选择按钮 鼠标在其上则显示，鼠标离开则隐藏
// ZoneSel : 0 - left; 1 - top; 2 - right; 3 - bottom
void CAdvancedBlendParamView::DrawBlendZoneSelectionButton(CDC *pValidDC, BOOL bShow, int ZoneSel/*融合带选择*/)
{
	CRect rcZone = m_ZonSelBtn[ZoneSel].rcBtn;
	CPoint ptTriangle[3]; //三角形的三个顶点
	CPoint ptTextAlign;

	if (ZoneSel==0) 
	{
		ptTriangle[0] = CPoint(rcZone.left+2,  int(rcZone.Height()*0.5));  //左	
		ptTriangle[1] = CPoint(rcZone.right-2, int(rcZone.Height()*0.5+8));//下
		ptTriangle[2] = CPoint(rcZone.right-2, int(rcZone.Height()*0.5-8));//上
		ptTextAlign   = CPoint(rcZone.left+2,  int(rcZone.Height()*0.5+8));
	}
	else if(ZoneSel==1)
	{
		ptTriangle[0] = CPoint(int(rcZone.Width()*0.5-8), rcZone.bottom-2); //左
		ptTriangle[2] = CPoint(int(rcZone.Width()*0.5),   rcZone.top+2);    //上	
		ptTriangle[1] = CPoint(int(rcZone.Width()*0.5+8), rcZone.bottom-2); //右
		ptTextAlign   = CPoint(int(rcZone.Width()*0.5+8), rcZone.top+2);
	}
	else if(ZoneSel==2)
	{
		ptTriangle[0] = CPoint(rcZone.right-2, int(rcZone.Height()*0.5));   //右	
		ptTriangle[1] = CPoint(rcZone.left+2,  int(rcZone.Height()*0.5+8)); //下
		ptTriangle[2] = CPoint(rcZone.left+2,  int(rcZone.Height()*0.5-8)); //上
		ptTextAlign   = CPoint(rcZone.left+2,  int(rcZone.Height()*0.5+8));
	}
	else if(ZoneSel==3)
	{
		ptTriangle[0] = CPoint(int(rcZone.Width()*0.5-8), rcZone.top+2);    //左
		ptTriangle[1] = CPoint(int(rcZone.Width()*0.5),   rcZone.bottom-2); //下
		ptTriangle[2] = CPoint(int(rcZone.Width()*0.5+8), rcZone.top+2);    //右
		ptTextAlign   = CPoint(int(rcZone.Width()*0.5+8), rcZone.top+2);
	}

	//创建按钮背景画刷
	CBrush grayBr, arrowBr, *pOldBr;
	HBRUSH hBr;
	grayBr.CreateSolidBrush(RGB(200,200,200));
	arrowBr.CreateSolidBrush(RGB(255,0,0));
	pOldBr = pValidDC->SelectObject(&grayBr);

	//画按钮背景
	pValidDC->FillRect(&rcZone, &grayBr);

	//画箭头标记
	hBr = (HBRUSH)arrowBr.GetSafeHandle();
	DrawTriangle(pValidDC, ptTriangle, hBr);

	//按钮文本
	CFont textFont, *pOldFont;
	textFont.CreatePointFont(120, _T("Arial"));
	pValidDC->SetTextColor(RGB(0,0,0));
	pValidDC->SetBkMode(TRANSPARENT);
    pOldFont = pValidDC->SelectObject(&textFont);

	CSize textExt;
	CString strText[9] = {_T("双"), _T("击"), _T("此"),_T("处"),_T("切"),_T("换"),_T("融"),_T("合"),_T("带"),};
	textExt = pValidDC->GetTextExtent(_T("双"));
	if (ZoneSel == 0 || ZoneSel == 2)	
	{
		for (int i=0; i<9; i++)
		{
			pValidDC->TextOut(ptTextAlign.x + int((20-textExt.cx)*0.5), ptTextAlign.y+i*(textExt.cy+1), strText[i]);
		}
	}
	else
	{
		pValidDC->TextOut(ptTextAlign.x, ptTextAlign.y + int((20-textExt.cy)*0.5), _T("双击此处切换融合带"));
	}

	pValidDC->SelectObject(pOldBr);
	pValidDC->SelectObject(pOldFont);
}

void CAdvancedBlendParamView::DrawTriangle(CDC *pDC, const LPPOINT points, HBRUSH hbr)
{
	CRgn triangleRgn;
	CBrush *br;
	br = CBrush::FromHandle(hbr);
	triangleRgn.CreatePolygonRgn(points, 3, WINDING);
	pDC->FillRgn(&triangleRgn, br);
	triangleRgn.DeleteObject();
}

void CAdvancedBlendParamView::DrawAxis(CDC *pDC)
{
	CPen pen,dotpen,*pOldpen;
	pen.CreatePen(PS_SOLID, 1, RGB(40,30,20));
	dotpen.CreatePen(PS_DOT, 1, RGB(117,117,128));
	pOldpen = pDC->SelectObject(&pen);

	//纵向坐标轴
	pDC->MoveTo(CPoint(m_rcChart.left, m_rcChart.top-10));
	pDC->LineTo(CPoint(m_rcChart.left, m_rcChart.bottom+5));
	pDC->MoveTo(CPoint(m_rcChart.left-5, m_rcChart.top));
	pDC->LineTo(CPoint(m_rcChart.left, m_rcChart.top));
	//横向坐标轴
	pDC->MoveTo(CPoint(m_rcChart.left-5, m_rcChart.bottom));
	pDC->LineTo(CPoint(m_rcChart.right+10, m_rcChart.bottom));
	pDC->MoveTo(CPoint(m_rcChart.right, m_rcChart.bottom));
	pDC->LineTo(CPoint(m_rcChart.right, m_rcChart.bottom+5));

	//画封闭线
	pDC->SelectObject(&dotpen);
	pDC->MoveTo(CPoint(m_rcChart.left+1, m_rcChart.top));
	pDC->LineTo(CPoint(m_rcChart.right+10, m_rcChart.top));
	pDC->MoveTo(CPoint(m_rcChart.right, m_rcChart.top-10));
	pDC->LineTo(CPoint(m_rcChart.right, m_rcChart.bottom-1));

	CFont font, *pOldfont;
	font.CreatePointFont(80, _T("Arial"));
	pOldfont = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(10,10,10));
	pDC->SetBkMode(TRANSPARENT);

	//纵轴上限值 -- 最大融合值
	CString strInfo;
	strInfo.Format(_T("%d"), 1024);
	CSize textExt;
	textExt = pDC->GetTextExtent(strInfo);
	pDC->TextOut(m_rcChart.left-textExt.cx-5, m_rcChart.top-int(textExt.cy*0.5), strInfo);

	//横轴上限制 -- 融合带宽度
	strInfo.Format(_T("%d"), m_BlendWidth);
	textExt = pDC->GetTextExtent(strInfo);
	pDC->TextOut(m_rcChart.right-int(textExt.cx*0.5), m_rcChart.bottom+5, strInfo);

	//纵轴下限值
	strInfo.Format(_T("%d"), 0);
	textExt = pDC->GetTextExtent(strInfo);
	pDC->TextOut(m_rcChart.left-textExt.cx-5, m_rcChart.bottom-int(textExt.cy*0.5), strInfo);

	//横轴下限值
	strInfo.Format(_T("%d"), 1);
	textExt = pDC->GetTextExtent(strInfo);
	pDC->TextOut(m_rcChart.left-int(textExt.cx*0.5), m_rcChart.bottom+5, strInfo);

	//标注融合带---左 右 上 下
	strInfo = m_nSide == 0 ? _T("左边融合带") : m_nSide==1 ? _T("顶部融合带") : m_nSide==2 ? _T("右边融合带") : _T("底部融合带");
	textExt = pDC->GetTextExtent(strInfo);
	pDC->TextOut(int((m_rcChart.Width()-textExt.cx)*0.5)+m_rcChart.left, m_rcChart.bottom+10+textExt.cy, strInfo);

	//标注融合带的方向
	CString strLeftFlag, strRightFlag;
	strLeftFlag  = m_nSide==0 ? _T("左边缘") : m_nSide==1 ? _T("上边缘") : m_nSide==2 ? _T("右边缘") : _T("下边缘");
	strRightFlag = m_nSide==0 ? _T("右部") : m_nSide==1 ? _T("下部") : m_nSide==2 ? _T("左部") : _T("上部");
	textExt = pDC->GetTextExtent(strLeftFlag);

	pDC->TextOut(m_rcChart.left+textExt.cx+5, m_rcChart.bottom+10+textExt.cy, strLeftFlag);
	pDC->TextOut(m_rcChart.right-textExt.cx*2-5, m_rcChart.bottom+10+textExt.cy, strRightFlag);

	//标注当前调整方式
	if (m_eCurNonius==Nonius_Null) strInfo = _T("移动游标调整融合值");
	else if(m_eCurNonius==Nonius_LeftPos || m_eCurNonius== Nonius_LeftVal) strInfo = _T("下限点调整模式");
	else if(m_eCurNonius==Nonius_RigtPos || m_eCurNonius== Nonius_RigtVal) strInfo = _T("上限点调整模式");
	else if(m_eCurNonius==Nonius_IndvPos || m_eCurNonius== Nonius_IndvVal) strInfo = _T("独立点调整模式");
	textExt = pDC->GetTextExtent(strInfo);
	pDC->TextOut(int((m_rcChart.Width()-textExt.cx)*0.5)+m_rcChart.left, m_rcChart.top-textExt.cy-5, strInfo);
	
	pDC->SelectObject(pOldpen);
	pDC->SelectObject(pOldfont);
}


void CAdvancedBlendParamView::DrawDataLine(CDC *pDC)
{
	CPen pen,*pOldpen;
	pen.CreatePen(PS_SOLID, 1, RGB(150,180,90));
	pOldpen = pDC->SelectObject(&pen);

	double Xscale = double(m_rcChart.Width())/(double(m_BlendWidth-1));
	double Yscale = double(m_rcChart.Height())/(1024.0);

	CBrush GradientBr;
	CRgn rgn;
	CPoint ppt[4];
	BYTE nRgb;
	//将数据点连线
	for (int i=0; i<m_BlendWidth-1; i++)
	{
		//pDC->MoveTo(m_rcChart.left+int(i*Xscale), m_rcChart.bottom-int(m_BlendParamData[i]*Yscale));
		//pDC->LineTo(m_rcChart.left+int((i+1)*Xscale), m_rcChart.bottom-int(m_BlendParamData[i+1]*Yscale));
		ppt[0] = CPoint(m_rcChart.left+int(i*Xscale), m_rcChart.bottom-int(m_BlendParamData[i]*Yscale));
		ppt[1] = CPoint(m_rcChart.left+int((i+1)*Xscale), m_rcChart.bottom-int(m_BlendParamData[i+1]*Yscale));
		ppt[2] = CPoint(m_rcChart.left+int((i+1)*Xscale), m_rcChart.bottom);
		ppt[3] = CPoint(m_rcChart.left+int(i*Xscale), m_rcChart.bottom);

		rgn.CreatePolygonRgn(ppt, 4, ALTERNATE);
		nRgb = int(m_BlendParamData[i]/4) > 255 ? 255 : int(m_BlendParamData[i]/4);
		GradientBr.CreateSolidBrush(RGB(nRgb, nRgb, nRgb));		
		pDC->FillRgn(&rgn, &GradientBr);
		GradientBr.DeleteObject();
		rgn.DeleteObject();
	}

	pDC->SelectObject(pOldpen);
}

//****************************************************
// 画范围游标控制符
void CAdvancedBlendParamView::DrawRangeNonius(CDC *pDC)
{
	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, 1, RGB(19,80, 180));
	pOldPen = pDC->SelectObject(&pen);

	double Xscale = double(m_rcChart.Width())/(double(m_BlendWidth-1));
	double Yscale = double(m_rcChart.Height())/(1024.0);

	CPoint leftNoniusPt, rightNoniusPt;
	leftNoniusPt  = CPoint(m_rcChart.left+int(m_nLeftNonius*Xscale), m_rcChart.bottom-int(m_BlendParamData[m_nLeftNonius]*Yscale));
	rightNoniusPt = CPoint(m_rcChart.left+int(m_nRightNonius*Xscale), m_rcChart.bottom-int(m_BlendParamData[m_nRightNonius]*Yscale));

	//画左游标
	CRgn rgnUpper, rgnBelow;
	CPoint ptRgn[5], ptDiam[4];
	ptRgn[0] = CPoint(leftNoniusPt.x-4, leftNoniusPt.y-25);
	ptRgn[1] = CPoint(leftNoniusPt.x+4, leftNoniusPt.y-25);
	ptRgn[2] = CPoint(leftNoniusPt.x+4, leftNoniusPt.y-15);
	ptRgn[3] = CPoint(leftNoniusPt.x,   leftNoniusPt.y-10);
	ptRgn[4] = CPoint(leftNoniusPt.x-4, leftNoniusPt.y-15);

	ptDiam[0] = CPoint(leftNoniusPt.x,   leftNoniusPt.y-4);
	ptDiam[1] = CPoint(leftNoniusPt.x+4, leftNoniusPt.y);
	ptDiam[2] = CPoint(leftNoniusPt.x,   leftNoniusPt.y+4);
	ptDiam[3] = CPoint(leftNoniusPt.x-4, leftNoniusPt.y);

	rgnUpper.CreatePolygonRgn(ptRgn, 5, ALTERNATE);
	rgnBelow.CreatePolygonRgn(ptDiam, 4, ALTERNATE);

	CBrush leftbr, blackBr, whiteBr, *pOldBr;
	blackBr.CreateSolidBrush(RGB(50, 40, 30));
	leftbr.CreateSolidBrush(RGB(19,80, 180));
	whiteBr.CreateSolidBrush(RGB(250,251,252));
	pOldBr = pDC->SelectObject(&whiteBr);

	pDC->FillRgn(&rgnUpper, m_eCurNonius == Nonius_LeftPos ? &blackBr : &leftbr);
	pDC->FillRgn(&rgnBelow, m_eCurNonius == Nonius_LeftVal ? &blackBr : &leftbr);

	if (m_eCurNonius == Nonius_LeftPos) pDC->Ellipse(CRect(leftNoniusPt.x-2, leftNoniusPt.y-22, leftNoniusPt.x+2, leftNoniusPt.y-18));
	else if (m_eCurNonius == Nonius_LeftVal) pDC->Ellipse(CRect(leftNoniusPt.x-2, leftNoniusPt.y-2, leftNoniusPt.x+2, leftNoniusPt.y+2));

	pDC->MoveTo(ptRgn[3]);
	pDC->LineTo(ptDiam[0]);

	rgnBelow.DeleteObject();
	rgnUpper.DeleteObject();

	//画右游标
	ptRgn[0] = CPoint(rightNoniusPt.x-4, rightNoniusPt.y-25);
	ptRgn[1] = CPoint(rightNoniusPt.x+4, rightNoniusPt.y-25);
	ptRgn[2] = CPoint(rightNoniusPt.x+4, rightNoniusPt.y-15);
	ptRgn[3] = CPoint(rightNoniusPt.x,   rightNoniusPt.y-10);
	ptRgn[4] = CPoint(rightNoniusPt.x-4, rightNoniusPt.y-15);

	ptDiam[0] = CPoint(rightNoniusPt.x,   rightNoniusPt.y-4);
	ptDiam[1] = CPoint(rightNoniusPt.x+4, rightNoniusPt.y);
	ptDiam[2] = CPoint(rightNoniusPt.x,   rightNoniusPt.y+4);
	ptDiam[3] = CPoint(rightNoniusPt.x-4, rightNoniusPt.y);

	rgnUpper.CreatePolygonRgn(ptRgn, 5, ALTERNATE);
	rgnBelow.CreatePolygonRgn(ptDiam, 4, ALTERNATE);

	CBrush rightbr;
	rightbr.CreateSolidBrush(RGB(180, 80, 19));
	pDC->FillRgn(&rgnUpper, m_eCurNonius == Nonius_RigtPos ? &blackBr : &rightbr);
	pDC->FillRgn(&rgnBelow, m_eCurNonius == Nonius_RigtVal ? &blackBr : &rightbr);

	if (m_eCurNonius == Nonius_RigtPos) pDC->Ellipse(CRect(rightNoniusPt.x-2, rightNoniusPt.y-22, rightNoniusPt.x+2, rightNoniusPt.y-18));
	else if (m_eCurNonius == Nonius_RigtVal) pDC->Ellipse(CRect(rightNoniusPt.x-2, rightNoniusPt.y-2, rightNoniusPt.x+2, rightNoniusPt.y+2));

	pDC->MoveTo(ptRgn[3]);
	pDC->LineTo(ptDiam[0]);

	rgnBelow.DeleteObject();
	rgnUpper.DeleteObject();

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBr);
}


//*******************************************************
//画单个控制点
void CAdvancedBlendParamView::DrawIndivNonius(CDC *pDC)
{
	CPen pen, *pOldPen;
	pen.CreatePen(PS_SOLID, 1, RGB(150, 240, 150));
	pOldPen = pDC->SelectObject(&pen);

	double Xscale = double(m_rcChart.Width())/(double(m_BlendWidth-1));
	double Yscale = double(m_rcChart.Height())/(1024.0);

	CPoint ptbuoy, ptCtrl;
	ptbuoy = CPoint(m_rcChart.left+int(Xscale*m_nIndivNonius), m_rcChart.bottom);
	ptCtrl = CPoint(m_rcChart.left+int(Xscale*m_nIndivNonius), m_rcChart.bottom-int(Yscale*m_BlendParamData[m_nIndivNonius]));

	//底部的浮动块
	CRect rc;
	rc = CRect(ptbuoy.x-2, ptbuoy.y-4, ptbuoy.x+2, ptbuoy.y+4);

	if (m_eCurNonius == Nonius_IndvPos)
	{
		pDC->FillSolidRect(&rc, RGB(50, 40, 30));
		rc.InflateRect(-1,-2);
		pDC->FillSolidRect(&rc, RGB(250,251,252));
	}
	else
		pDC->FillSolidRect(&rc, RGB(18,180,20));


	//浮动标记线
	pDC->MoveTo(ptbuoy.x, ptbuoy.y-5);
	pDC->LineTo(ptbuoy.x, m_rcChart.top);

	//参数值控制点
	CRgn rgn;
	CPoint ptDiam[4];
	ptDiam[0] = CPoint(ptCtrl.x,   ptCtrl.y-4);
	ptDiam[1] = CPoint(ptCtrl.x+4, ptCtrl.y);
	ptDiam[2] = CPoint(ptCtrl.x,   ptCtrl.y+4);
	ptDiam[3] = CPoint(ptCtrl.x-4, ptCtrl.y);
	rgn.CreatePolygonRgn(ptDiam, 4, ALTERNATE);

	CBrush br, whiteBr, *pOldBr;
	whiteBr.CreateSolidBrush(RGB(250,251,252));
	br.CreateSolidBrush(m_eCurNonius == Nonius_IndvVal ? RGB(50, 40, 30) : RGB(80, 180, 19));

	pOldBr = pDC->SelectObject(&whiteBr);
	pDC->FillRgn(&rgn, &br);
	if (m_eCurNonius == Nonius_IndvVal) pDC->Ellipse(CRect(ptCtrl.x-2, ptCtrl.y-2, ptCtrl.x+2, ptCtrl.y+2));

	rgn.DeleteObject();
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBr);
}

// CAdvancedBlendParamView 诊断

#ifdef _DEBUG
void CAdvancedBlendParamView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAdvancedBlendParamView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAdvancedBlendParamView 消息处理程序

BOOL CAdvancedBlendParamView::OnEraseBkgnd(CDC* pDC)
{
	//防止闪烁
	return TRUE;

	return CView::OnEraseBkgnd(pDC);
}

void CAdvancedBlendParamView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLBtnDownInNonius = IsMouseOverNonius(point, m_eNoniusType);
    if (m_bLBtnDownInNonius) {m_eCurNonius = m_eNoniusType; Invalidate();}

	CView::OnLButtonDown(nFlags, point);
}

void CAdvancedBlendParamView::OnLButtonUp(UINT nFlags, CPoint point)
{
	//隐藏提示信息
	if (m_bLBtnDownInNonius)
	{
		//CMsgTipArray msgarr;
		//ShowMsgTip(FALSE, msgarr);
	}

	//重置控制变量
	ResetCtrlVariables();

	CView::OnLButtonUp(nFlags, point);
}

void CAdvancedBlendParamView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CView::OnLButtonDblClk(nFlags, point);
}

void CAdvancedBlendParamView::OnMouseMove(UINT nFlags, CPoint point)
{
	//显示和隐藏四个融合带切换按钮
	//ShowBlendZoneBtn(point);
	if (m_bLBtnDownInNonius)
	{
		//游标校正
		NoniusMoveProc(point);
	}
	else
	{
		//提示信息
	}

	CView::OnMouseMove(nFlags, point);
}

void CAdvancedBlendParamView::ShowBlendZoneBtn(CPoint point)
{
	BOOL bOldStat, bNewStat;
	for (int i=0; i<4; i++)
	{
		bOldStat = m_ZonSelBtn[i].bMouseOver;
		bNewStat = PtInRect(m_ZonSelBtn[i].rcBtn, point);
		m_ZonSelBtn[i].bMouseOver = bNewStat;
		if (bOldStat == bNewStat)
		{
			continue;
		}
		else
		{
			if (bNewStat)
			{
				CDC *pDC = GetDC();
				DrawBlendZoneSelectionButton(pDC, TRUE, i);
				ReleaseDC(pDC);
			}
			else
				InvalidateRect(m_ZonSelBtn[i].rcBtn);
		}
	}
}

void CAdvancedBlendParamView::NoniusMoveProc(CPoint point)
{
	double Xscale = (double(m_BlendWidth-1))/(double(m_rcChart.Width()));
	double Yscale = 1024.0/(double(m_rcChart.Height()));

	int nBlendPos = int((point.x - m_rcChart.left)*Xscale);
	int nBlendVal = int((m_rcChart.bottom - point.y)*Yscale);

	switch(m_eNoniusType)
	{
	case Nonius_LeftPos:
		if ((nBlendPos > m_BlendWidth-1) || (nBlendPos < 0)) return;
		if (nBlendPos > m_nRightNonius-5) return; //限制左游标位置在右游标位置左边
		m_nLeftNonius = nBlendPos;
		nBlendVal = m_BlendParamData[m_nLeftNonius];
		break;
	case Nonius_LeftVal:
		if ((nBlendVal > 1024) || (nBlendVal < 0)) return;
		m_BlendParamData[m_nLeftNonius] = nBlendVal;
		AverageBlendData();
		WriteBlendParaData(m_nLeftNonius, m_nRightNonius);
		break;
	case Nonius_RigtPos:
		if ((nBlendPos > m_BlendWidth-1) || (nBlendPos < 0)) return;
		if (nBlendPos < m_nLeftNonius+5) return; //限制右游标位置在左游标位置右边
		m_nRightNonius = nBlendPos;
		nBlendVal = m_BlendParamData[m_nRightNonius];
		break;
	case Nonius_RigtVal:
		if ((nBlendVal > 1024) || (nBlendVal < 0)) return;
		m_BlendParamData[m_nRightNonius] = nBlendVal;
		AverageBlendData();
		WriteBlendParaData(m_nLeftNonius, m_nRightNonius);
		break;
	case Nonius_IndvPos:
		if ((nBlendPos > m_BlendWidth-1) || (nBlendPos < 0)) return;
		m_nIndivNonius = nBlendPos;
		nBlendVal = m_BlendParamData[m_nIndivNonius];
		break;
	case Nonius_IndvVal:
		if ((nBlendVal > 1024) || (nBlendVal < 0)) return;
		m_BlendParamData[m_nIndivNonius] = nBlendVal;
		WriteBlendParaData(m_nIndivNonius, m_nIndivNonius);
		break;
	default: break;
	}

	//更新对话框数据
	CAdvancedBlendParamSetDlg *pDlg = (CAdvancedBlendParamSetDlg *)GetParent();
	pDlg->UpdateDlgData(m_nLeftNonius+1, m_BlendParamData[m_nLeftNonius],
		                m_nRightNonius+1, m_BlendParamData[m_nRightNonius],
						m_nIndivNonius+1, m_BlendParamData[m_nIndivNonius]);

	//显示提示信息
	
	//刷新视图
	InvalidateRect(CRect(m_rcChart.left-10, m_rcChart.top-30, m_rcChart.right+10, m_rcChart.bottom+10));
}

void CAdvancedBlendParamView::AverageBlendData()
{
	int nUpper = m_BlendParamData[m_nRightNonius];
	int nLower = m_BlendParamData[m_nLeftNonius];

	double avrVal = double(nUpper-nLower)/(double(m_nRightNonius-m_nLeftNonius-1));

	for (int i=m_nLeftNonius; i<m_nRightNonius; i++)
	{
		m_BlendParamData[i] = nLower+Round(avrVal*(i-m_nLeftNonius));
	}
}

BOOL CAdvancedBlendParamView::IsMouseOverNonius(CPoint point, emNoniusType &eNoniustype)
{
	double Xscale = double(m_rcChart.Width())/(double(m_BlendWidth-1));
	double Yscale = double(m_rcChart.Height())/(1024.0);

	CPoint leftNoniusPt, rightNoniusPt, IndivPosNoniusPt, IndivValNoniusPt;
	leftNoniusPt     = CPoint(m_rcChart.left+int(m_nLeftNonius*Xscale),  m_rcChart.bottom-int(m_BlendParamData[m_nLeftNonius]*Yscale));
	rightNoniusPt    = CPoint(m_rcChart.left+int(m_nRightNonius*Xscale), m_rcChart.bottom-int(m_BlendParamData[m_nRightNonius]*Yscale));
	IndivValNoniusPt = CPoint(m_rcChart.left+int(m_nIndivNonius*Xscale), m_rcChart.bottom-int(m_BlendParamData[m_nIndivNonius]*Yscale));
	IndivPosNoniusPt = CPoint(m_rcChart.left+int(Xscale*m_nIndivNonius), m_rcChart.bottom);

	CRgn rgnPosNoniusL,rgnPosNoniusR, rgnPosNoniusInd; //位置游标
	CRgn rgnValNoniusL, rgnValNoniusR, rgnValNoniusInd;//值游标
	CPoint ptRgn[5], ptDiam[4];
	//左游标
	ptRgn[0] = CPoint(leftNoniusPt.x-4, leftNoniusPt.y-25);
	ptRgn[1] = CPoint(leftNoniusPt.x+4, leftNoniusPt.y-25);
	ptRgn[2] = CPoint(leftNoniusPt.x+4, leftNoniusPt.y-15);
	ptRgn[3] = CPoint(leftNoniusPt.x,   leftNoniusPt.y-10);
	ptRgn[4] = CPoint(leftNoniusPt.x-4, leftNoniusPt.y-15);
	rgnPosNoniusL.CreatePolygonRgn(ptRgn, 5, ALTERNATE);

	if (rgnPosNoniusL.PtInRegion(point)) { eNoniustype = Nonius_LeftPos; rgnPosNoniusL.DeleteObject(); return TRUE; }
	else { rgnPosNoniusL.DeleteObject(); }

	ptDiam[0] = CPoint(leftNoniusPt.x,   leftNoniusPt.y-4);
	ptDiam[1] = CPoint(leftNoniusPt.x+4, leftNoniusPt.y);
	ptDiam[2] = CPoint(leftNoniusPt.x,   leftNoniusPt.y+4);
	ptDiam[3] = CPoint(leftNoniusPt.x-4, leftNoniusPt.y);	
	rgnValNoniusL.CreatePolygonRgn(ptDiam, 4, ALTERNATE);

	if (rgnValNoniusL.PtInRegion(point)) { eNoniustype = Nonius_LeftVal; rgnValNoniusL.DeleteObject(); return TRUE; }
	else { rgnValNoniusL.DeleteObject(); }

	//右游标
	ptRgn[0] = CPoint(rightNoniusPt.x-4, rightNoniusPt.y-25);
	ptRgn[1] = CPoint(rightNoniusPt.x+4, rightNoniusPt.y-25);
	ptRgn[2] = CPoint(rightNoniusPt.x+4, rightNoniusPt.y-15);
	ptRgn[3] = CPoint(rightNoniusPt.x,   rightNoniusPt.y-10);
	ptRgn[4] = CPoint(rightNoniusPt.x-4, rightNoniusPt.y-15);
	rgnPosNoniusR.CreatePolygonRgn(ptRgn, 5, ALTERNATE);

	if (rgnPosNoniusR.PtInRegion(point)) { eNoniustype = Nonius_RigtPos; rgnPosNoniusR.DeleteObject(); return TRUE; }
	else { rgnPosNoniusR.DeleteObject(); }

	ptDiam[0] = CPoint(rightNoniusPt.x,   rightNoniusPt.y-4);
	ptDiam[1] = CPoint(rightNoniusPt.x+4, rightNoniusPt.y);
	ptDiam[2] = CPoint(rightNoniusPt.x,   rightNoniusPt.y+4);
	ptDiam[3] = CPoint(rightNoniusPt.x-4, rightNoniusPt.y);	
	rgnValNoniusR.CreatePolygonRgn(ptDiam, 4, ALTERNATE);

	if (rgnValNoniusR.PtInRegion(point)) { eNoniustype = Nonius_RigtVal; rgnValNoniusR.DeleteObject(); return TRUE; }
	else { rgnValNoniusR.DeleteObject(); }

	//独立游标
	ptRgn[0] = CPoint(IndivPosNoniusPt.x-2, IndivPosNoniusPt.y-4);
	ptRgn[1] = CPoint(IndivPosNoniusPt.x+2, IndivPosNoniusPt.y-4);
	ptRgn[2] = CPoint(IndivPosNoniusPt.x+2, IndivPosNoniusPt.y+4);
	ptRgn[3] = CPoint(IndivPosNoniusPt.x-2, IndivPosNoniusPt.y+4);
	rgnPosNoniusInd.CreatePolygonRgn(ptRgn, 4, ALTERNATE);

	if (rgnPosNoniusInd.PtInRegion(point)) { eNoniustype = Nonius_IndvPos; rgnPosNoniusInd.DeleteObject(); return TRUE; }
	else { rgnPosNoniusInd.DeleteObject(); }

	ptDiam[0] = CPoint(IndivValNoniusPt.x,   IndivValNoniusPt.y-4);
	ptDiam[1] = CPoint(IndivValNoniusPt.x+4, IndivValNoniusPt.y);
	ptDiam[2] = CPoint(IndivValNoniusPt.x,   IndivValNoniusPt.y+4);
	ptDiam[3] = CPoint(IndivValNoniusPt.x-4, IndivValNoniusPt.y);	
	rgnValNoniusInd.CreatePolygonRgn(ptDiam,4, ALTERNATE);

	if (rgnValNoniusInd.PtInRegion(point)) { eNoniustype = Nonius_IndvVal; rgnValNoniusInd.DeleteObject(); return TRUE;}
	else { rgnValNoniusInd.DeleteObject(); }

	return FALSE;
}

void CAdvancedBlendParamView::ResetCtrlVariables()
{
	m_bLBtnDownInNonius = FALSE;
	m_eNoniusType = Nonius_Null;
}


void CAdvancedBlendParamView::SetNoniusState(int nNoniusType, int nNoniusVal)
{
	if (Nonius_LeftPos == nNoniusType)
	{
		m_nLeftNonius = nNoniusVal;
	}
	else if (Nonius_LeftVal == nNoniusType)
	{
		m_BlendParamData[m_nLeftNonius] = nNoniusVal;
		AverageBlendData();
		WriteBlendParaData(m_nLeftNonius, m_nRightNonius);
	}
	else if (Nonius_RigtPos == nNoniusType)
	{
		m_nRightNonius = nNoniusVal;
	}
	else if (Nonius_RigtVal == nNoniusType)
	{
		m_BlendParamData[m_nRightNonius] = nNoniusVal;
		AverageBlendData();
		WriteBlendParaData(m_nLeftNonius, m_nRightNonius);
	}
	else if (Nonius_IndvPos == nNoniusType)
	{
		m_nIndivNonius = nNoniusVal;
	}
	else if (Nonius_IndvVal == nNoniusType)
	{
		m_BlendParamData[m_nIndivNonius] = nNoniusVal;
		WriteBlendParaData(m_nIndivNonius, m_nIndivNonius);
	}
	else return;

	CAdvancedBlendParamSetDlg* pDlg = (CAdvancedBlendParamSetDlg*)GetParent();
	pDlg->UpdateDlgData(m_nLeftNonius+1, m_BlendParamData[m_nLeftNonius],
		m_nRightNonius+1, m_BlendParamData[m_nRightNonius],
		m_nIndivNonius+1, m_BlendParamData[m_nIndivNonius]);

	Invalidate();
}


void CAdvancedBlendParamView::OnVKdown(UINT vkType)
{
	if(vkType == VK_LEFT)
	{
		if ((m_eCurNonius == Nonius_LeftPos || m_eCurNonius == Nonius_LeftVal) && (m_nLeftNonius > 0)) m_nLeftNonius--;
		else if ((m_eCurNonius == Nonius_RigtPos || m_eCurNonius == Nonius_RigtVal) && (m_nRightNonius > m_nLeftNonius+5)) m_nRightNonius--;
		else if ((m_eCurNonius == Nonius_IndvPos || m_eCurNonius == Nonius_IndvVal) && (m_nIndivNonius > 0)) m_nIndivNonius--;
		else return;
	}
	else if (vkType == VK_RIGHT)
	{
		if ((m_eCurNonius == Nonius_LeftPos || m_eCurNonius == Nonius_LeftVal) && (m_nLeftNonius < m_nRightNonius-5))	 m_nLeftNonius++;
		else if ((m_eCurNonius == Nonius_RigtPos || m_eCurNonius == Nonius_RigtVal) && (m_nRightNonius < m_BlendWidth-1))  m_nRightNonius++;
		else if ((m_eCurNonius == Nonius_IndvPos || m_eCurNonius == Nonius_IndvVal) && (m_nIndivNonius < m_BlendWidth-1))  m_nIndivNonius++;
		else return;
	}
	else if (vkType == VK_UP)
	{
		if ((m_eCurNonius == Nonius_LeftPos || m_eCurNonius == Nonius_LeftVal) && (m_BlendParamData[m_nLeftNonius] < 1024))
		{
			m_BlendParamData[m_nLeftNonius]++;
			AverageBlendData();
			WriteBlendParaData(m_nLeftNonius, m_nRightNonius);
		}
		else if ((m_eCurNonius == Nonius_RigtPos || m_eCurNonius == Nonius_RigtVal) && (m_BlendParamData[m_nRightNonius] < 1024))
		{
			m_BlendParamData[m_nRightNonius]++;
			AverageBlendData();
			WriteBlendParaData(m_nLeftNonius, m_nRightNonius);
		}
		else if ((m_eCurNonius == Nonius_IndvPos || m_eCurNonius == Nonius_IndvVal) && (m_BlendParamData[m_nIndivNonius] < 1024))
		{
			m_BlendParamData[m_nIndivNonius]++;
			WriteBlendParaData(m_nIndivNonius, m_nIndivNonius);
		}
		else return;
	}
	else if (vkType == VK_DOWN)
	{
		if ((m_eCurNonius == Nonius_LeftPos || m_eCurNonius == Nonius_LeftVal) && (m_BlendParamData[m_nLeftNonius] > 0))
		{
			m_BlendParamData[m_nLeftNonius]--;
			AverageBlendData();
			WriteBlendParaData(m_nLeftNonius, m_nRightNonius);
		}
		else if ((m_eCurNonius == Nonius_RigtPos || m_eCurNonius == Nonius_RigtVal) && (m_BlendParamData[m_nRightNonius] > 0))
		{
			m_BlendParamData[m_nRightNonius]--;
			AverageBlendData();
			WriteBlendParaData(m_nLeftNonius, m_nRightNonius);
		}
		else if ((m_eCurNonius == Nonius_IndvPos || m_eCurNonius == Nonius_IndvVal) && (m_BlendParamData[m_nIndivNonius] > 0))
		{
			m_BlendParamData[m_nIndivNonius]--;
			WriteBlendParaData(m_nIndivNonius, m_nIndivNonius);
		}
		else return;
	}
	else return;

	CAdvancedBlendParamSetDlg* pDlg = (CAdvancedBlendParamSetDlg*)GetParent();
	pDlg->UpdateDlgData(m_nLeftNonius+1, m_BlendParamData[m_nLeftNonius],
		m_nRightNonius+1, m_BlendParamData[m_nRightNonius],
		m_nIndivNonius+1, m_BlendParamData[m_nIndivNonius]);

	Invalidate();
}

void CAdvancedBlendParamView::WriteBlendParaData(int nBegin, int nEnd)
{
	theApp.MG_WriteBlendParamDatas(m_BlendParamData, nBegin, nEnd, m_nSide, m_nPureWhite, m_nChannel);
}