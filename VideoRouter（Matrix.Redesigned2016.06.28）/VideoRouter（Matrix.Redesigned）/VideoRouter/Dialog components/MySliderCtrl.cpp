#include "StdAfx.h"
#include "MySliderCtrl.h"

CMySliderCtrl::CMySliderCtrl(void)
{
}

CMySliderCtrl::~CMySliderCtrl(void)
{
}
BEGIN_MESSAGE_MAP(CMySliderCtrl, CSliderCtrl)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CMySliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//((CSourcePropertyDlg*)GetParent())->OnSliderLBtnUp(this);
	::SendMessage(GetParent()->m_hWnd, WM_USER_SLIDER_LBTNUP, WPARAM(this), 0);

	CSliderCtrl::OnLButtonUp(nFlags, point);
}

