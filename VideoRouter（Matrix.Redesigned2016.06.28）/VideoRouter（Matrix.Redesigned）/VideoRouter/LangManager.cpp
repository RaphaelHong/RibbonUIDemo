#include "stdafx.h"
#include "VideoRouter.h"
#include "LangManager.h"


CLangManager::CLangManager(UINT uLangID)
{
	SetLangFilePath(uLangID);
}


CLangManager::~CLangManager(void)
{
}

BOOL CLangManager::LoadFromFile(LPCTSTR lpszFilePath)
{
	//选择文件路径
	//LPCTSTR pszPath = NULL;
	//if (lpszFilePath == NULL)
	//	pszPath = m_szFilePath;
	//else 
	//	pszPath = lpszFilePath;

	return TRUE;
}

BOOL CLangManager::WriteToFile(LPCTSTR lpszFilePath)
{
	//选择文件路径
	//LPCTSTR pszPath = NULL;
	//if (lpszFilePath == NULL)
	//	pszPath = m_szFilePath;
	//else 
	//	pszPath = lpszFilePath;

	return TRUE;
}

BOOL CLangManager::DecryptLoadFile(LPCTSTR lpszFilePath)
{
	//选择文件路径
	//LPCTSTR pszPath = NULL;
	//if (lpszFilePath == NULL)
	//	pszPath = m_szFilePath;
	//else 
	//	pszPath = lpszFilePath;

	return TRUE;
}

BOOL CLangManager::EncryptWriteFile(LPCTSTR lpszFilePath)
{
	//选择文件路径
	//LPCTSTR pszPath = NULL;
	//if (lpszFilePath == NULL)
	//	pszPath = m_szFilePath;
	//else 
	//	pszPath = lpszFilePath;

	return TRUE;
}

void CLangManager::SetLangFilePath(UINT uLangID)
{
	g_uLangID = uLangID;

	CString strPath;
	strPath.Format(_T("Lang\\%d\\"), uLangID);
	if (uLangID == MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED))
		strPath += _T("简体中文(中国).ini");
	else if (uLangID == MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED))
		strPath += _T("English(US).ini");
	else if (uLangID == MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED))
		strPath += _T("繁體中文(中國台灣).ini");

	strPath = GetGlobalManager()->GetAppPath() + strPath;

	SetConfigFilePath(strPath.GetBuffer());
}

CString CLangManager::GetWord(const CString& strSec, const CString& strKey)
{
	CString strWord;

	CWrapInitFile iniFile;
	iniFile.SetInitFilePath(m_szFilePath);

	strWord = iniFile.GetKeyValueString(strSec, strKey);

	return strWord;
}

void CLangManager::SetWord(const CString& strSec, const CString& strKey, const CString& strWord)
{
	CWrapInitFile iniFile;
	iniFile.SetInitFilePath(m_szFilePath);

	iniFile.SetKeyValueString(strSec, strKey, strWord);
}

void CLangManager::GetWordArray(const CString& strSec, CMap<UINT, UINT, CString, CString> &wordmap)
{
	CWrapInitFile iniFile;
	iniFile.SetInitFilePath(m_szFilePath);

	CString strKey, strWord;
	UINT uKey;

	//遍历map中的所有关键词，并根据关键词从配置文件中读取相应的字符串
	POSITION pos = wordmap.GetStartPosition();
	while (pos)
	{
		wordmap.GetNextAssoc(pos, uKey, strWord);
		strKey.Format(_T("%u"), uKey);

		strWord = iniFile.GetKeyValueString(strSec, strKey);
		wordmap.SetAt(uKey, strWord);
	}
}

void CLangManager::SetWordArray(const CString& strSec, const CMap<UINT , UINT, CString, CString> wordmap)
{
	CWrapInitFile iniFile;
	iniFile.SetInitFilePath(m_szFilePath);

	CString strKey, strWord;
	UINT uKey;

	//将map中的所有元素写入到配置文件中
	POSITION pos = wordmap.GetStartPosition();
	while (pos)
	{
		wordmap.GetNextAssoc(pos, uKey, strWord);

		strKey.Format(_T("%u"), uKey);
		iniFile.SetKeyValueString(strSec, strKey, strWord);
	}
}