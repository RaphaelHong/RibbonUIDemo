#pragma once

class CMySliderCtrl : public CSliderCtrl
{
public:
	CMySliderCtrl(void);
	~CMySliderCtrl(void);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
