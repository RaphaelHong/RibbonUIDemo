// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// VideoRouterDoc.cpp : CVideoRouterDoc ���ʵ��
//

#include "stdafx.h"
#include "VideoRouter.h"

#include "VideoRouterDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoRouterDoc

IMPLEMENT_DYNCREATE(CVideoRouterDoc, CDocument)

BEGIN_MESSAGE_MAP(CVideoRouterDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CVideoRouterDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CVideoRouterDoc::OnFileSave)
END_MESSAGE_MAP()


// CVideoRouterDoc ����/����

CVideoRouterDoc::CVideoRouterDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CVideoRouterDoc::~CVideoRouterDoc()
{
}

BOOL CVideoRouterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CVideoRouterDoc ���л�

void CVideoRouterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CVideoRouterDoc ���

#ifdef _DEBUG
void CVideoRouterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVideoRouterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVideoRouterDoc ����

void CVideoRouterDoc::OnFileSave()
{
	CSysConfManager sysconf;
	CFileDialog SaveFile(FALSE, _T(".scr"), _T("Matrix"), OFN_HIDEREADONLY|OFN_READONLY, "scr File(*.scr)|*.scr|All Files (*.*)|*.*|");
	SaveFile.m_ofn.lpstrInitialDir = sysconf.GetDefaultSceneFilePath();
	if (SaveFile.DoModal() == IDOK)
	{
		SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_PACKAGE_SCENE, WPARAM(SaveFile.GetPathName().GetBuffer(0)), 0);
	}
}


void CVideoRouterDoc::OnFileOpen()
{
	CSysConfManager sysconf;
	CFileDialog OpenFile(TRUE, _T(".scr"), _T("Matrix"), OFN_HIDEREADONLY|OFN_READONLY, "scr File(*.scr)|*.scr|All Files (*.*)|*.*|");
	OpenFile.m_ofn.lpstrInitialDir = sysconf.GetDefaultSceneFilePath();
	if (OpenFile.DoModal() == IDOK)
	{
		SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_UNPACK_SCENE, WPARAM(OpenFile.GetPathName().GetBuffer(0)), 0);
	}
}
