// NumberEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "NumberEdit.h"


// CMyEdit

IMPLEMENT_DYNAMIC(CNumberEdit, CXTPEdit)

CNumberEdit::CNumberEdit()
{

}

CNumberEdit::~CNumberEdit()
{
}


BEGIN_MESSAGE_MAP(CNumberEdit, CXTPEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CMyEdit 消息处理程序



void CNumberEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 全选时清空编辑框的内容
	int nStart, nEnd;
	GetSel(nStart, nEnd);
	CString str;
	GetWindowText(str);

	if ((nStart == 0) && (nEnd == GetWindowTextLength()) && (GetWindowTextLength() != 0))
	{
		//字符串全选中时，以下的输入将导致字符串被清空
		if (nChar == '.'      //小数点
			||nChar == '-'    //负号
			||nChar == 0x08   //退格键
			||nChar == 0x10   //换行键
			||nChar == 0x2e   //删除键
			||(nChar >= '0' && nChar <= '9'))  //数字
		{
			return CEdit::OnChar(nChar, nRepCnt, nFlags);
		}		
	}

	if (nChar == '.') 
	{
		//输入小数点
		if (str.Find('.') == -1)
		{
			//当前字符串中并没有小数点
			return CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
	else if (nChar == '-') 
	{
		//输入负号
		//当前字符串中已经出现负号
		if (str.Find('-') != -1) return;

		if (nEnd == 0) 
		{
			//光标停在字符串首位置，直接输入负号
			return CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			//光标不在字符串首位置，将负号移到首位置
			str.Insert(0, nChar);
			SetWindowText(str);
			SetSel(nEnd+1, nEnd+1);
			return;
		}
	}
	else if ((nChar <= '9' && nChar >= '0') || nChar == 0x08 || nChar == 0x10)
	{
		if (str.IsEmpty())
			return CEdit::OnChar(nChar, nRepCnt, nFlags);
		
		if (nChar == '0')
		{
			//数字的首部不能出现连续几个0
			if (str.Find('-') != -1)
			{ // 负数
				if (str.Find('0') == 1 && (nEnd == 1 || nEnd == 2))
					return;
			}
			else
			{ // 正数
				if (str.Find('0') == 0 && (nEnd == 0 || nEnd == 1))
					return;
			}
		}

		if (atoi(str) == 0)
		{
			if (str.Find('-') != -1 && nChar != '0')
			{

			}
		}

		return CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}