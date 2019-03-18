// GlobalManager.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "GlobalManager.h"

// CGlobalManager

CGlobalManager::CGlobalManager()
{
	Init();
}

CGlobalManager::~CGlobalManager()
{
}

CGlobalManager *pGlobalManager = NULL;

// CGlobalManager 成员函数

CString CGlobalManager::GetAppPath()
{
	CString strAppPath;

	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH在WINDEF.h中定义了，等于260  
	memset(exeFullPath,0,MAX_PATH);  

	GetModuleFileName(NULL,exeFullPath,MAX_PATH);  
	(_tcsrchr(exeFullPath, '\\'))[0] = 0;

	strAppPath.Format(_T("%s\\"), exeFullPath);

	return strAppPath;
}

void CGlobalManager::Init()
{
	// 默认语言
	m_dwLangID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	// 默认软件工作模式
	m_eWorkMode = eWorkModeSplit;
}

void CGlobalManager::InitDeviceStructure(pDEVICEINFO pInfo)
{
	m_DevInfo = *pInfo;
}

pDEVICEINFO CGlobalManager::GetDeviceStructure() const
{
	return const_cast<pDEVICEINFO>(&m_DevInfo);
}

emWorkMode CGlobalManager::GetCurrencyWorkMode() const 
{
	return m_eWorkMode;
}

emWorkMode &CGlobalManager::GetCurrencyWorkMode()
{
	return m_eWorkMode;
}