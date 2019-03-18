//************************************************************************
// 语言管理类

#pragma once
#include "BaseFileManager.h"

class CLangManager :
	public CBaseFileManager
{
public:
	CLangManager(UINT uLangID);
	~CLangManager(void);

	//加载配置文件
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath);
	//写入配置文件
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath);
	//加载加密配置文件
	virtual BOOL DecryptLoadFile(LPCTSTR lpszFilePath);
	//加密写入配置文件
	virtual BOOL EncryptWriteFile(LPCTSTR lpszFilePath);

	//获取一条词语
	CString GetWord(const CString& strSec, const CString& strKey);
	void SetWord(const CString& strSec, const CString& strKey, const CString& strWord);

	//获取一系列词语
	void GetWordArray(const CString& strSec, CMap <UINT, UINT, CString, CString> &wordmap);
	void SetWordArray(const CString& strSec, const CMap <UINT , UINT, CString, CString> wordmap);

private:
	UINT g_uLangID; //指定要加载的语言ID
	//指定语言库,并设置语言配置文件路径
	void SetLangFilePath(UINT uLangID);
};

