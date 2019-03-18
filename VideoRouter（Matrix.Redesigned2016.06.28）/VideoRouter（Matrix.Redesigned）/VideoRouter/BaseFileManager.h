#pragma once

//*************************************************************************
// �����ļ�������

class CBaseFileManager : public CObject
{
public:
	CBaseFileManager(void);
	~CBaseFileManager(void);
	//Ĭ�������ļ�·��
	char m_szFilePath[_MAX_PATH];
	BOOL m_bLockWriteFile;
public:
	// ���ݿ����
	static void Encrypt(char *input, char* output, int nLength);
	// ���ݿ����
	static void Decrypt(char *input, char* output, int nLength);
	// BCC(���У��)
	static void SetBCCvalue(char *pblock, char &code, int nLength);
	static BOOL BlockCheckCharaters(char *pblock, const char &code, int nLength);
	//����Ĭ���ļ�·��
	void SetConfigFilePath(char *pszPath);

	//���������ļ�
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt=TRUE) = 0;
	//д�������ļ�
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt=TRUE) = 0;

	virtual BOOL WriteToFile(__in LPCTSTR lpszFilePath, __in char* pWriteBuffer, __in long length, __in BOOL bEncrypt=TRUE);
	virtual BOOL LoadFromFile(__in LPCTSTR lpszFilePath, __out char* pReadBuffer, _Inout_ long &length, __in BOOL bDecrypt=TRUE);

	virtual void LockWriteFile();
	virtual void UnlockWriteFile();
};
