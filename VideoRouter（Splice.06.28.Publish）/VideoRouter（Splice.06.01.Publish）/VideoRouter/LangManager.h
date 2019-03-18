//************************************************************************
// ���Թ�����

#pragma once
#include "BaseFileManager.h"

class CLangManager :
	public CBaseFileManager
{
public:
	CLangManager(UINT uLangID);
	~CLangManager(void);

	//���������ļ�
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath);
	//д�������ļ�
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath);
	//���ؼ��������ļ�
	virtual BOOL DecryptLoadFile(LPCTSTR lpszFilePath);
	//����д�������ļ�
	virtual BOOL EncryptWriteFile(LPCTSTR lpszFilePath);

	//��ȡһ������
	CString GetWord(const CString& strSec, const CString& strKey);
	void SetWord(const CString& strSec, const CString& strKey, const CString& strWord);

	//��ȡһϵ�д���
	void GetWordArray(const CString& strSec, CMap <UINT, UINT, CString, CString> &wordmap);
	void SetWordArray(const CString& strSec, const CMap <UINT , UINT, CString, CString> wordmap);

private:
	UINT g_uLangID; //ָ��Ҫ���ص�����ID
	//ָ�����Կ�,���������������ļ�·��
	void SetLangFilePath(UINT uLangID);
};

