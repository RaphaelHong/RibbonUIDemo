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

// VideoRouter.h : VideoRouter 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include "MainFrm.h"

#define MSG_NO_CONNECTION {\
	CString strMsg;\
	strMsg.LoadString(IDS_NO_CONNECTION);\
	AfxMessageBox(strMsg, MB_OK|MB_ICONERROR); }

// CVideoRouterApp:
// 有关此类的实现，请参阅 VideoRouter.cpp
//

class CVideoRouterApp : public CWinAppEx
{
public:
	CVideoRouterApp();

public:
	// GDI++初始化
	ULONG_PTR gdiplusToken;

	// 当前登录用户信息
	UserData g_CurrencyUser;

	//APP实例初始化
	void ApplicationInitialize();
	void ApplicationCleanUp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

// 一个全局的APP实例化对象
extern CVideoRouterApp theApp;

// 为了使用安全，请用下面的全局函数调用相关的管理器对象，而不要直接使用全局指针变量。
extern inline CGlobalManager *GetGlobalManager();   // {返回的是pGlobalManager指针}
extern inline CCommunicateManager *GetCmdManager(); // {返回的是pCmdManager指针}
extern inline CInputManager *GetInputManager();     // {返回的是pInputManager指针}
extern inline COutputManager *GetOutputManager();   // {返回的是pOutputManager指针}
extern inline CInitialManager *GetInitManager();    // {返回的是pInitManager指针}
extern inline CBaseSceneManager *GetSceneManager(); // {返回的是pSceneManager指针}
extern inline CMainFrame *GetMainFrame();           // {返回的是CMainFrame主框架的指针}
extern inline CLogManager *GetLogManager();         // {返回的是pLogManager指针}
