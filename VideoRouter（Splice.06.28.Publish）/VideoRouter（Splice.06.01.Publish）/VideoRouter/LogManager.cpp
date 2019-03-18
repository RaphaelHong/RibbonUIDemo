#include "StdAfx.h"
#include "VideoRouter.h"
#include "LogManager.h"

CLogManager* pLogManager = NULL;

CLogManager::CLogManager(void)
{
	CString strPath;
	strPath = GetGlobalManager()->GetAppPath() + _T("Log\\mmtrix.log");

	SetConfigFilePath(strPath.GetBuffer(0));
}

CLogManager::~CLogManager(void)
{
}


BOOL CLogManager::LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt)
{
	//选择文件路径
	//LPCTSTR pszPath = NULL;
	//if (lpszFilePath == NULL)
	//	pszPath = m_szFilePath;
	//else 
	//	pszPath = lpszFilePath;

	return TRUE;
}

BOOL CLogManager::WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt)
{
	//选择文件路径
	//LPCTSTR pszPath = NULL;
	//if (lpszFilePath == NULL)
	//	pszPath = m_szFilePath;
	//else 
	//	pszPath = lpszFilePath;

	return TRUE;
}


void CLogManager::SetLogFilePath(LPCTSTR lpszFilePath)
{
	CWrapInitFile iniFile;
	iniFile.SetInitFilePath(m_szFilePath);

	CString strSec , strKey, strVal;
	strSec = _T("DefaultFilePath");
	strKey = _T("LogFilePath");
	strVal.Format(_T("%s"), lpszFilePath);

	iniFile.SetKeyValueString(strSec, strKey, strVal);
}

LPCTSTR CLogManager::GetLogFilePath()
{
	CWrapInitFile iniFile;
	iniFile.SetInitFilePath(m_szFilePath);

	CString strSec , strKey, strVal;
	strSec = _T("DefaultFilePath");
	strKey = _T("LogFilePath");
	strVal = GetGlobalManager()->GetAppPath() + _T("Log\\");

	strVal = iniFile.GetKeyValueStringDef(strSec, strKey, strVal);

	return strVal;
}

void CLogManager::PrintLog(void* pLogCnt, int nLen)
{
	if (nLen <= 0)
		return;

	CStdioFile 	logFile;
	if(logFile.Open(m_szFilePath, CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeWrite))
	{
		CTime t = CTime::GetCurrentTime();
		char *info = new char[nLen+64];
		sprintf_s(info, nLen+64, "[%d:%d:%d] %s\n", t.GetHour(), t.GetMinute(), t.GetSecond(), pLogCnt);
		logFile.SeekToEnd();
		logFile.Write(info, strlen(info));
		logFile.Close();
		delete [] info;
	}
}