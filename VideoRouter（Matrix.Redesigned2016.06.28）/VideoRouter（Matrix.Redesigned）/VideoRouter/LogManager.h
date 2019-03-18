#pragma once
#include "basefilemanager.h"

//////////////////////////////////////////////////////////////////////////
// 日志管理器

class CLogManager :
	public CBaseFileManager
{
public:
	CLogManager(void);
	~CLogManager(void);

	//加载配置文件
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt=TRUE);
	//写入配置文件
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt=TRUE);

	//Log file 路径
	void SetLogFilePath(LPCTSTR lpszFilePath);
	LPCTSTR GetLogFilePath();

	void PrintLog(void* pLogCnt, int nLen);
};

extern CLogManager* pLogManager;