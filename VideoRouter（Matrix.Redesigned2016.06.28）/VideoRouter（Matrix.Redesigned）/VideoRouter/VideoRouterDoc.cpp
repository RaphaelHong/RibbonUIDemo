// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// ("Fluent UI")，该示例仅作为参考资料提供， 
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有 (C) Microsoft Corporation
// 保留所有权利。

// VideoRouterDoc.cpp : CVideoRouterDoc 类的实现
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


// CVideoRouterDoc 构造/析构

CVideoRouterDoc::CVideoRouterDoc()
{
	// TODO: 在此添加一次性构造代码

}

CVideoRouterDoc::~CVideoRouterDoc()
{
}

BOOL CVideoRouterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CVideoRouterDoc 序列化

void CVideoRouterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CVideoRouterDoc 诊断

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


// CVideoRouterDoc 命令

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
