#pragma once
#include "BaseFileManager.h"

//******************************************
// CSysConfManager
// 软件配置管理器
//

class CSysConfManager : public CBaseFileManager
{
	friend class CGlobalManager;
	friend class CInitialManager;
public:
	CSysConfManager();
	virtual ~CSysConfManager();

	//加载配置文件
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt=TRUE);
	//写入配置文件
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt=TRUE);

	virtual BOOL LoadFromFile(__in LPCTSTR lpszFilePath, __out char* pReadBuffer, __in __out long &length, __in BOOL bDecrypt=TRUE);
	virtual BOOL WriteToFile(__in LPCTSTR lpszFilePath, __in char* pWriteBuffer, __in long length, __in BOOL bEncrypt=TRUE);

	void SetString(CString strSec, CString strKey, CString strVal);
	CString GetString(CString strSec, CString strKey, CString strDefault);

	void SetLPCTSTR(CString strSec, CString strKey, LPCTSTR lpString);
	LPCTSTR GetLPCTSTR(CString strSec, CString strKey, LPCTSTR lpDefault);

	void SetInt(CString strSec, CString strKey, int nValue);
	int GetInt(CString strSec, CString strKey, int nDefValue);

public:
	//默认IP地址
	void SetDefaultIPAddress(CString lpIPaddr);
	CString GetDefaultIPAddress();

	//默认语言
	void SetDefaultLangID(UINT uLangID);
	UINT GetDefaultLangID();

	//默认场景存储路径
	void SetDefaultSceneFilePath(LPCTSTR lpszFilePath);
	LPCTSTR GetDefaultSceneFilePath();

	//最近设置的分辨率
	void SetLastTryResolution(CString strRes, BOOL isInput);
	CString GetLastTryResolution(BOOL isInput);
};


