// SysConfManager.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "SysConfManager.h"


// CSysConfManager

CSysConfManager::CSysConfManager()
{
	CString strPath;
	strPath = GetGlobalManager()->GetAppPath() + _T("Config\\sys.ini");

	SetConfigFilePath(strPath.GetBuffer(0));
}

CSysConfManager::~CSysConfManager()
{
}

// CSysConfManager 成员函数
BOOL CSysConfManager::LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt)
{

	return TRUE;
}

BOOL CSysConfManager::WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt)
{

	return TRUE;
}

BOOL CSysConfManager::LoadFromFile(__in LPCTSTR lpszFilePath, __out char* pReadBuffer, __in __out long &length, __in BOOL bDecrypt)
{
	return CBaseFileManager::LoadFromFile(lpszFilePath, pReadBuffer, length, bDecrypt);
}

BOOL CSysConfManager::WriteToFile(__in LPCTSTR lpszFilePath, __in char* pWriteBuffer, __in long length, __in BOOL bEncrypt)
{
	return CBaseFileManager::WriteToFile(lpszFilePath, pWriteBuffer, length, bEncrypt);
}

void CSysConfManager::SetString(CString strSec, CString strKey, CString strVal)
{
	CWrapInitFile iniFile;
	iniFile.SetInitFilePath(m_szFilePath);

	iniFile.SetKeyValueString(strSec, strKey, strVal);
}

CString CSysConfManager::GetString(CString strSec, CString strKey, CString strDefault)
{
	CWrapInitFile iniFile;
	iniFile.SetInitFilePath(m_szFilePath);

	CString strVal = iniFile.GetKeyValueStringDef(strSec, strKey, strDefault);

	return strVal;
}

void CSysConfManager::SetLPCTSTR(CString strSec, CString strKey, LPCTSTR lpString)
{
	CString strVal;
	strVal.Format(_T("%s"), lpString);

	SetString(strSec, strKey, strVal);
}

LPCTSTR CSysConfManager::GetLPCTSTR(CString strSec, CString strKey, LPCTSTR lpDefault)
{
	CString strVal;
	strVal.Format(_T("%s"), lpDefault);

	return GetString(strSec, strKey, strVal);
}

void CSysConfManager::SetInt(CString strSec, CString strKey, int nValue)
{
	CWrapInitFile iniFile;
	iniFile.SetInitFilePath(m_szFilePath);

	iniFile.SetKeyValueInt(strSec, strKey, nValue);
}

int CSysConfManager::GetInt(CString strSec, CString strKey, int nDefValue)
{
	CWrapInitFile iniFile;
	iniFile.SetInitFilePath(m_szFilePath);

	int nValue = iniFile.GetKeyValueIntDef(strSec, strKey, nDefValue);

	return nValue;
}

void CSysConfManager::SetDefaultIPAddress(CString lpIPaddr)
{
	SetString(_T("DefaultAdddresss"), _T("DefaultIP"), lpIPaddr);
}

CString CSysConfManager::GetDefaultIPAddress()
{
	return GetString(_T("DefaultAdddresss"), _T("DefaultIP"), _T("192.168.1.200"));
}

void CSysConfManager::SetDefaultLangID(UINT uLangID)
{
	SetInt(_T("Language"), _T("LangID"), uLangID);
}

UINT CSysConfManager::GetDefaultLangID()
{
	return (UINT)GetInt(_T("Language"), _T("LangID"), MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
}

void CSysConfManager::SetDefaultSceneFilePath(LPCTSTR lpszFilePath)
{
	SetString(_T("DefaultFilePath"), _T("DefaultSceneFilePath"), lpszFilePath);
}

LPCTSTR CSysConfManager::GetDefaultSceneFilePath()
{
	return GetLPCTSTR(_T("DefaultFilePath"), _T("DefaultSceneFilePath"), GetGlobalManager()->GetAppPath() + _T("Config\\"));
}

void CSysConfManager::SetLastTryResolution(CString strRes, BOOL isInput)
{
	CString strSec , strKey;
	if (isInput)
		strSec = _T("InputResolution");
	else
		strSec = _T("OutputResolution");

	strKey = _T("LastTryRes");

	SetString(strSec, strKey, strRes);
}

CString CSysConfManager::GetLastTryResolution(BOOL isInput)
{
	CString strSec , strKey, strVal;
	if (isInput)
		strSec = _T("InputResolution");
	else
		strSec = _T("OutputResolution");

	strKey = _T("LastTryRes");

	strVal = _T("");

	return GetString(strSec, strKey, strVal);
}