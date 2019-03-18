#pragma once
#include "BaseFileManager.h"

class CUserManager : public CBaseFileManager
{
public:
	CUserManager(void);
	~CUserManager(void);

	//加载配置文件
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt);
	//写入配置文件
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt);

public:
	int GetUserCount() const;
	void RemoveAllGeneralUser();
	void RemoveUser(const UINT &nUserID);
	void RemoveAt(const int nIndex);
	void AddDefaultAdmin();
	void AddUser(UserData user);
	void AddUser(const UINT &nUserID, const char* pszName, const char* pszPwd, const Authority &au);
	BOOL ModifyUser(const UINT &nUserID, const char* pszName, const char* pszPwd, const Authority &au);

	pUserData GetUserByID(const UINT nUserID) const;
	pUserData GetUser(const int nIndex) const;

	char* GetUserName(const UINT nUserID);
	char* GetUserPassword(const UINT nUserID);
	Authority GetUserAuthority(const UINT nUserID);

	int FindUserByName(const char* szName) const;
	int FindDefaultAdmin() const;

	int PrepareUserID() const;

private:
	CArray <UserData, UserData&> m_arrUser;
};
