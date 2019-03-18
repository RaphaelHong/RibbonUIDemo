#pragma once


// CMyEdit

class CNumberEdit : public CXTPEdit
{
	DECLARE_DYNAMIC(CNumberEdit)

public:
	CNumberEdit();
	virtual ~CNumberEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


