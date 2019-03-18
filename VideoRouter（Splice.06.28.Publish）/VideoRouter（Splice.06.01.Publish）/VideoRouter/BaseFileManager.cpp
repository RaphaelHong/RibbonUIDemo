#include "stdafx.h"
#include "VideoRouter.h"
#include "BaseFileManager.h"
#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")

UCHAR szMiwen[128] = {
    0xB9,0xAB,0xCB,0xBE,0xC3,0xFB,0xB3,0xC6,0xA3,0xBA,0xC9,0xCF,0xBA,0xA3,0xB4,0xF3,
    0xCA,0xD3,0xB5,0xE7,0xD7,0xD3,0xBF,0xC6,0xBC,0xBC,0xD3,0xD0,0xCF,0xDE,0xB9,0xAB,
    0x0D,0x0A,0xB9,0xAB,0xCB,0xBE,0xB5,0xD8,0xD6,0xB7,0xA3,0xBA,0xC9,0xCF,0xBA,0xA3,
    0xCA,0xD0,0xB1,0xA6,0xC9,0xBD,0xC7,0xF8,0xB3,0xA4,0xD2,0xDD,0xC2,0xB7,0x31,0x35,
    0xBA,0xC5,0xB8,0xB4,0xB5,0xA9,0xC8,0xED,0xBC,0xFE,0xD4,0xB0,0x42,0xB6,0xB0,0x31,
    0x34,0x30,0x38,0xCA,0xD2,0x0D,0x0A,0xB9,0xAB,0xCB,0xBE,0xB7,0xA8,0xC8,0xCB,0xA3,
    0xBA,0xD5,0xC5,0xCA,0xBF,0xD3,0xC2,0xA3,0xAC,0xC2,0xBD,0x95,0x44,0xA3,0xAC,0xCD,
    0xF5,0xCB,0xBC,0xB1,0xF8,0x0D,0x0A,0xB2,0xFA,0xC6,0xB7,0xC3,0xFB,0xB3,0xC6,0xA3
};

CBaseFileManager::CBaseFileManager(void)
{
	m_bLockWriteFile = FALSE;
}


CBaseFileManager::~CBaseFileManager(void)
{
}

void CBaseFileManager::SetConfigFilePath(char *pszPath)
{
	strncpy_s(m_szFilePath, pszPath, _MAX_PATH);

	MakeSureDirectoryPathExists(pszPath);
}


//*****************************************************************
//            简单文本加密
//方法三
//算法： 与密文一起混合运算
//-----------------------------------------------------------------
void CBaseFileManager::Encrypt(char *input, char* output, int nLength)
{
	if (nLength <= 1) 
		return;
// 方法三，因为没有带校验，此法不是很安全
	int nMiwen = 0;
	for (int i=0; i<nLength; i++)
	{
		output[i] = szMiwen[nMiwen]-input[i];
		output[i] = ((output[i]&0x0F) <<4) | (((~output[i]) & 0xF0)>>4);

		nMiwen++;

		if (nMiwen > 127)
			nMiwen = 0;
	}
}

//----------------------------------------------------------------
//         解密与加密正好相反
void CBaseFileManager::Decrypt(char *input, char* output, int nLength)
{
	if (nLength <= 1)
		return;
// 方法三，因为没有带校验，此法不是很安全
	int nMiwen = 0;
	for (int i=0; i<nLength; i++)
	{
		output[i] = ((input[i] & 0xF0) >> 4) | (((~input[i]) & 0x0F) << 4);
		output[i] = szMiwen[nMiwen]-output[i];

		nMiwen++;

		if (nMiwen>127)
			nMiwen = 0;
	}
}

void CBaseFileManager::SetBCCvalue(char *pblock, char &code, int nLength)
{
	code = pblock[0];

	for (int i=1; i<nLength; i++)
		code = code^pblock[i];
}

BOOL CBaseFileManager::BlockCheckCharaters(char *pblock, const char &code, int nLength)
{
	char ck = 0;
	ck = ck^code;

	for (int i=0; i<nLength; i++)
		ck = ck^pblock[i];

	return (ck == 0x00);
}

BOOL CBaseFileManager::WriteToFile(__in LPCTSTR lpszFilePath, __in char* pWriteBuffer, __in long length, __in BOOL bEncrypt)
{
	//选择文件路径
	LPCTSTR pszPath = NULL;
	if (lpszFilePath == NULL)
		pszPath = m_szFilePath;
	else 
		pszPath = lpszFilePath;

	CFile save;
	if (!save.Open(pszPath, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
	{
		CString str;
		str.Format(_T("创建文件：%s 失败！"), lpszFilePath);
		GetLogManager()->PrintLog(str.GetBuffer(), str.GetLength());
		return FALSE;
	}

	save.Write(pWriteBuffer, length);
	//if (bEncrypt)
	//	Encrypt(pWriteBuffer, pWriteBuffer, length);

	save.Close();

	return TRUE;
}

BOOL CBaseFileManager::LoadFromFile(__in LPCTSTR lpszFilePath, __out char* pReadBuffer, __in __out long &length, __in BOOL bDecrypt)
{
	//选择文件路径
	LPCTSTR pszPath = NULL;
	if (lpszFilePath == NULL)
		pszPath = m_szFilePath;
	else 
		pszPath = lpszFilePath;

	CFile open;
	if (!open.Open(pszPath, CFile::modeRead|CFile::typeBinary))
	{
		CString str;
		str.Format(_T("打开文件：%s 失败！"), pszPath);

		GetLogManager()->PrintLog(str.GetBuffer(), str.GetLength());
		return FALSE;
	}

	long lfilelength = (long)open.GetLength();

	length = min(length, lfilelength);

	open.Read(pReadBuffer, length);

	//if (bDecrypt)
	//	Decrypt(pReadBuffer, pReadBuffer, length);

	open.Close();

	return TRUE;
}

void CBaseFileManager::LockWriteFile()
{
	m_bLockWriteFile = TRUE;
}

void CBaseFileManager::UnlockWriteFile()
{
	m_bLockWriteFile = FALSE;

	WriteToFile(NULL);
}