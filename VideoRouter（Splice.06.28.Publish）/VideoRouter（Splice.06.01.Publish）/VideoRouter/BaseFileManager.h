#pragma once

//*************************************************************************
// 配置文件基类型

class CBaseFileManager : public CObject
{
public:
	CBaseFileManager(void);
	~CBaseFileManager(void);
	//默认配置文件路径
	char m_szFilePath[_MAX_PATH];
	BOOL m_bLockWriteFile;
public:
	// 数据块加密
	static void Encrypt(char *input, char* output, int nLength);
	// 数据块解密
	static void Decrypt(char *input, char* output, int nLength);
	// BCC(异或校验)
	static void SetBCCvalue(char *pblock, char &code, int nLength);
	static BOOL BlockCheckCharaters(char *pblock, const char &code, int nLength);
	//设置默认文件路径
	void SetConfigFilePath(char *pszPath);

	//加载配置文件
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt=TRUE) = 0;
	//写入配置文件
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt=TRUE) = 0;

	virtual BOOL WriteToFile(__in LPCTSTR lpszFilePath, __in char* pWriteBuffer, __in long length, __in BOOL bEncrypt=TRUE);
	virtual BOOL LoadFromFile(__in LPCTSTR lpszFilePath, __out char* pReadBuffer, _Inout_ long &length, __in BOOL bDecrypt=TRUE);

	virtual void LockWriteFile();
	virtual void UnlockWriteFile();
};
