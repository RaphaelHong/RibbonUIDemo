// InplaceEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "..\VideoRouter.h"
#include "InplaceEdit.h"


// CInplaceEdit

IMPLEMENT_DYNAMIC(CInplaceEdit, CEdit)

CInplaceEdit::CInplaceEdit()
{

}

CInplaceEdit::~CInplaceEdit()
{
}


BEGIN_MESSAGE_MAP(CInplaceEdit, CEdit)
END_MESSAGE_MAP()



// CInplaceEdit 消息处理程序



BOOL CInplaceEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			::SendMessage(GetParent()->m_hWnd, WM_USER_CANCEL_EDIT, 0, 0);
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			::SendMessage(GetParent()->m_hWnd, WM_USER_END_EDIT, 0, 0);
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
