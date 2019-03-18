#include "StdAfx.h"
#include "VideoRouter.h"
#include "UserManager.h"

CUserManager::CUserManager(void)
{
	CString strPath;
	strPath = GetGlobalManager()->GetAppPath() + _T("Config\\user.dat");
	SetConfigFilePath(strPath.GetBuffer(0));

	LoadFromFile(NULL, TRUE);
	AddDefaultAdmin();
}

CUserManager::~CUserManager(void)
{
}

BOOL CUserManager::LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt)
{
	//选择文件路径
	LPCTSTR pszPath = NULL;
	if (lpszFilePath == NULL)
		pszPath = m_szFilePath;
	else 
		pszPath = lpszFilePath;

	CStdioFile File;
	if (!File.Open(pszPath, CFile::modeRead|CFile::typeBinary))
		return FALSE;

	long len = (long)File.GetLength();
	const int nObjSize = sizeof(UserData);
	if (len < nObjSize)
		return FALSE;

	RemoveAllGeneralUser();

	char* pBuf = new char[len+1];
	File.Read(pBuf, len);

	if (bDecrypt)
		Decrypt(pBuf, pBuf, len);

	int nRead = 0;

	UserData user;

	while( nRead < len)
	{
		ZeroMemory(&user, nObjSize);

		memcpy_s(&user, nObjSize, pBuf+nRead, nObjSize);

		nRead += nObjSize;

		m_arrUser.Add(user);
	}

	File.Close();

	delete [] pBuf;

	return TRUE;
}

BOOL CUserManager::WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt)
{
	//选择文件路径
	LPCTSTR pszPath = NULL;
	if (lpszFilePath == NULL)
		pszPath = m_szFilePath;
	else 
		pszPath = lpszFilePath;

	CStdioFile File;
	if (!File.Open(pszPath, CFile::modeCreate|CFile::modeWrite))
		return FALSE;

	if (m_arrUser.IsEmpty())
		return TRUE;

	const int nObjSize = sizeof(UserData);
	long len = nObjSize*m_arrUser.GetSize();
	char* pBuf = new char[len+1];

	for (int i=0; i<m_arrUser.GetSize(); i++)
	{
		memcpy_s(pBuf+i*nObjSize, len, &m_arrUser[i], nObjSize);
	}

	if (bEncrypt)
		Encrypt(pBuf, pBuf, len);

	File.Write(pBuf, len);
	File.Close();

	delete [] pBuf;

	return TRUE;
}

int CUserManager::GetUserCount() const
{
	return m_arrUser.GetSize();
}

void CUserManager::RemoveAllGeneralUser()
{
	if (m_arrUser.IsEmpty())
		return;

	for (int i=m_arrUser.GetSize()-1; i>=0; i--)
	{
		if (m_arrUser[i].eAuthority != eAdministrator)
		{
			m_arrUser.RemoveAt(i);
		}
	}

	// 写配置文件
	WriteToFile(NULL, TRUE);
}

void CUserManager::RemoveUser(const UINT &nUserID)
{
	for (int i=0; i<m_arrUser.GetSize(); i++)
	{
		if (m_arrUser[i].nUserID == nUserID)
		{
			m_arrUser.RemoveAt(i);
		}
	}

	// 写配置文件
	WriteToFile(NULL, TRUE);
}

void CUserManager::RemoveAt(const int nIndex)
{
	if (nIndex < 0 || nIndex >= m_arrUser.GetSize())
		return;

	m_arrUser.RemoveAt(nIndex);

	WriteToFile(NULL, TRUE);
}

void CUserManager::AddDefaultAdmin()
{
	for (int i=0; i<m_arrUser.GetSize(); i++)
	{
		if (m_arrUser[i].eAuthority == eAdministrator)
		{
			return;
		}
	}

	UINT nID = 0;
	char* szName = "管理员";
	char* szPwd  = "";
	Authority au = eAdministrator;

	AddUser(nID, szName, szPwd, au);
	WriteToFile(NULL, TRUE);
}

void CUserManager::AddUser(UserData user)
{
	if (m_arrUser.IsEmpty())
	{
		m_arrUser.Add(user);
	}
	else if (m_arrUser[0].nUserID > user.nUserID)
	{
		m_arrUser.InsertAt(0, user);
	}
	else if (m_arrUser[m_arrUser.GetSize()-1].nUserID < user.nUserID)
	{
		m_arrUser.Add(user);
	}
	else
	{
		for (int i=0; i<m_arrUser.GetSize()-1; i++)
		{
			if (m_arrUser[i].nUserID < user.nUserID && m_arrUser[i+1].nUserID > user.nUserID)
			{
				m_arrUser.InsertAt(i+1, user);
				break;
			}
		}
	}

	WriteToFile(NULL, TRUE);

}

void CUserManager::AddUser(const UINT &nUserID, const char* pszName, const char* pszPwd, const Authority &au)
{
	UserData user;
	ZeroMemory(&user, sizeof(UserData));

	user.nUserID = nUserID;
	strncpy_s(user.szUserName, _MAX_UNAME, pszName, strlen(pszName));
	strncpy_s(user.szUserPwd, _MAX_PASSWORD, pszPwd, strlen(pszPwd));
	user.eAuthority = au;

	m_arrUser.Add(user);

	WriteToFile(NULL, TRUE);
}

BOOL CUserManager::ModifyUser(const UINT &nUserID, const char* pszName, const char* pszPwd, const Authority &au)
{
	UserData* pUser = GetUser(nUserID);
	if (pUser == NULL)
		return FALSE;

	ZeroMemory(pUser, sizeof(UserData));
	pUser->nUserID = nUserID;
	strncpy_s(pUser->szUserName, _MAX_UNAME, pszName, strlen(pszName));
	strncpy_s(pUser->szUserPwd, _MAX_PASSWORD, pszPwd, strlen(pszPwd));
	pUser->eAuthority = au;

	WriteToFile(NULL, TRUE);

	return TRUE;
}

pUserData CUserManager::GetUser(const int nIndex) const
{
	if (nIndex < 0 || nIndex >= m_arrUser.GetSize())
		return NULL;

	return (pUserData)&m_arrUser[nIndex];
}

pUserData CUserManager::GetUserByID(const UINT nUserID) const
{
	for (int i=0; i<m_arrUser.GetSize(); i++)
	{
		if (m_arrUser[i].nUserID == nUserID)
		{
			return (pUserData)&m_arrUser[i];
		}
	}

	return NULL;
}

char* CUserManager::GetUserName(const UINT nUserID)
{
	pUserData pUser = GetUserByID(nUserID);
	if (pUser==NULL)
		return NULL;

	return pUser->szUserName;
}

char* CUserManager::GetUserPassword(const UINT nUserID)
{
	UserData* pUser = GetUserByID(nUserID);
	if (pUser==NULL)
		return NULL;

	return pUser->szUserPwd;
}

Authority CUserManager::GetUserAuthority(const UINT nUserID)
{
	UserData* pUser = GetUserByID(nUserID);
	if (pUser==NULL)
		return eAuthorityErr;

	return pUser->eAuthority;
}

int CUserManager::FindUserByName(const char* szName) const
{
	for (int i=0; i<m_arrUser.GetSize(); i++)
	{
		if (strcmp(m_arrUser[i].szUserName, szName) == 0)
		{
			return i;
		}
	}

	return -1;
}

int CUserManager::FindDefaultAdmin() const
{
	for (int i=0; i<m_arrUser.GetSize(); i++)
	{
		if (m_arrUser[i].eAuthority == eAdministrator)
		{
			return i;
		}
	}

	return -1;
}

int CUserManager::PrepareUserID() const
{
	int nNewID = INVALID_VALUE;

	for (int i=0; i<m_arrUser.GetSize(); i++)
	{
		nNewID = i;
		for (int j=0; j<m_arrUser.GetSize(); j++)
		{
			if (m_arrUser[j].nUserID == nNewID)
			{
				nNewID = INVALID_VALUE;
				break;
			}
		}

		if (nNewID != INVALID_VALUE)
			break;
	}

	if (nNewID == INVALID_VALUE)
		nNewID = m_arrUser.GetSize();

	return nNewID;
}