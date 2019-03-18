// GlobalManager.cpp : ʵ���ļ�
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

// CGlobalManager ��Ա����

CString CGlobalManager::GetAppPath()
{
	CString strAppPath;

	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH��WINDEF.h�ж����ˣ�����260  
	memset(exeFullPath,0,MAX_PATH);  

	GetModuleFileName(NULL,exeFullPath,MAX_PATH);  
	(_tcsrchr(exeFullPath, '\\'))[0] = 0;

	strAppPath.Format(_T("%s\\"), exeFullPath);

	return strAppPath;
}

void CGlobalManager::Init()
{
	// Ĭ������
	m_dwLangID = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);
	// Ĭ���������ģʽ
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