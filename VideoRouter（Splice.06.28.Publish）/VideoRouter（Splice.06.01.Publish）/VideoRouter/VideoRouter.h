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

// VideoRouter.h : VideoRouter Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "MainFrm.h"

#define MSG_NO_CONNECTION {\
	CString strMsg;\
	strMsg.LoadString(IDS_NO_CONNECTION);\
	AfxMessageBox(strMsg, MB_OK|MB_ICONERROR); }

// CVideoRouterApp:
// �йش����ʵ�֣������ VideoRouter.cpp
//

class CVideoRouterApp : public CWinAppEx
{
public:
	CVideoRouterApp();

public:
	// GDI++��ʼ��
	ULONG_PTR gdiplusToken;

	// ��ǰ��¼�û���Ϣ
	UserData g_CurrencyUser;

	//APPʵ����ʼ��
	void ApplicationInitialize();
	void ApplicationCleanUp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

// һ��ȫ�ֵ�APPʵ��������
extern CVideoRouterApp theApp;

// Ϊ��ʹ�ð�ȫ�����������ȫ�ֺ���������صĹ��������󣬶���Ҫֱ��ʹ��ȫ��ָ�������
extern inline CGlobalManager *GetGlobalManager();   // {���ص���pGlobalManagerָ��}
extern inline CCommunicateManager *GetCmdManager(); // {���ص���pCmdManagerָ��}
extern inline CInputManager *GetInputManager();     // {���ص���pInputManagerָ��}
extern inline COutputManager *GetOutputManager();   // {���ص���pOutputManagerָ��}
extern inline CInitialManager *GetInitManager();    // {���ص���pInitManagerָ��}
extern inline CBaseSceneManager *GetSceneManager(); // {���ص���pSceneManagerָ��}
extern inline CMainFrame *GetMainFrame();           // {���ص���CMainFrame����ܵ�ָ��}
extern inline CLogManager *GetLogManager();         // {���ص���pLogManagerָ��}
