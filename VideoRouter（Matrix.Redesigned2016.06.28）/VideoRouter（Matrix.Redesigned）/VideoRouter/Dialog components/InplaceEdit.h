#pragma once


// CInplaceEdit

class CInplaceEdit : public CEdit
{
	DECLARE_DYNAMIC(CInplaceEdit)

public:
	CInplaceEdit();
	virtual ~CInplaceEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


