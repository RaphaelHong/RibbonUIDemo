#pragma once
#include "basefilemanager.h"

//////////////////////////////////////////////////////////////////////////
// ��־������

class CLogManager :
	public CBaseFileManager
{
public:
	CLogManager(void);
	~CLogManager(void);

	//���������ļ�
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt=TRUE);
	//д�������ļ�
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt=TRUE);

	//Log file ·��
	void SetLogFilePath(LPCTSTR lpszFilePath);
	LPCTSTR GetLogFilePath();

	void PrintLog(void* pLogCnt, int nLen);
};

extern CLogManager* pLogManager;