#pragma once
#include "BaseFileManager.h"

//******************************************
// CSysConfManager
// ������ù�����
//

class CSysConfManager : public CBaseFileManager
{
	friend class CGlobalManager;
	friend class CInitialManager;
public:
	CSysConfManager();
	virtual ~CSysConfManager();

	//���������ļ�
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt=TRUE);
	//д�������ļ�
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
	//Ĭ��IP��ַ
	void SetDefaultIPAddress(CString lpIPaddr);
	CString GetDefaultIPAddress();

	//Ĭ������
	void SetDefaultLangID(UINT uLangID);
	UINT GetDefaultLangID();

	//Ĭ�ϳ����洢·��
	void SetDefaultSceneFilePath(LPCTSTR lpszFilePath);
	LPCTSTR GetDefaultSceneFilePath();

	//������õķֱ���
	void SetLastTryResolution(CString strRes, BOOL isInput);
	CString GetLastTryResolution(BOOL isInput);
};


