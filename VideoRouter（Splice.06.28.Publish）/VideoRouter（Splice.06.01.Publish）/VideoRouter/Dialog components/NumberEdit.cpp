// NumberEdit.cpp : ʵ���ļ�
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



// CMyEdit ��Ϣ�������



void CNumberEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// ȫѡʱ��ձ༭�������
	int nStart, nEnd;
	GetSel(nStart, nEnd);
	CString str;
	GetWindowText(str);

	if ((nStart == 0) && (nEnd == GetWindowTextLength()) && (GetWindowTextLength() != 0))
	{
		//�ַ���ȫѡ��ʱ�����µ����뽫�����ַ��������
		if (nChar == '.'      //С����
			||nChar == '-'    //����
			||nChar == 0x08   //�˸��
			||nChar == 0x10   //���м�
			||nChar == 0x2e   //ɾ����
			||(nChar >= '0' && nChar <= '9'))  //����
		{
			return CEdit::OnChar(nChar, nRepCnt, nFlags);
		}		
	}

	if (nChar == '.') 
	{
		//����С����
		if (str.Find('.') == -1)
		{
			//��ǰ�ַ����в�û��С����
			return CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
	}
	else if (nChar == '-') 
	{
		//���븺��
		//��ǰ�ַ������Ѿ����ָ���
		if (str.Find('-') != -1) return;

		if (nEnd == 0) 
		{
			//���ͣ���ַ�����λ�ã�ֱ�����븺��
			return CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			//��겻���ַ�����λ�ã��������Ƶ���λ��
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
			//���ֵ��ײ����ܳ�����������0
			if (str.Find('-') != -1)
			{ // ����
				if (str.Find('0') == 1 && (nEnd == 1 || nEnd == 2))
					return;
			}
			else
			{ // ����
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