#include "stdafx.h"
#include "ResolutionManager.h"

//CString strDefaultTimings[TOTAL_OUT_TIMING+1] = {
//	_T("1024x768  @ 60Hz"),
//	_T("1024x768  @ 120Hz"),
//	_T("1280x720  @ 60Hz"),
//	_T("1280x720  @ 120Hz"),
//	_T("1280x800  @ 60Hz"),
//	_T("1280x800  @ 120Hz"),
//	_T("1280x1024 @ 60Hz"),
//	_T("1366x768  @ 60Hz"),
//	_T("1400x1050 @ 60Hz"),
//	_T("1920x1080 @ 60Hz"),
//	_T("1920x1200 @ 60Hz"),
//	_T("2048x1080 @ 60Hz"),
//	_T("2048x1200 @ 60Hz"),
//	_T("END")
//};

CResolutionManager::CResolutionManager(void)
	: CBaseFileManager()
{
	//分辨率名称
	m_pszResolution = new char[_MAX_FNAME];
	//添加默认分辨率到动态数组
	AddDefault();
}


CResolutionManager::~CResolutionManager(void)
{
	//清理内存
	delete [] m_pszResolution;
	m_pszResolution = NULL;
}

BOOL CResolutionManager::LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt)
{
	LPCTSTR pszPath = NULL;
	if (lpszFilePath == NULL)
		pszPath = m_szFilePath;
	else 
		pszPath = lpszFilePath;

	CStdioFile File;
	if (!File.Open(pszPath, CFile::modeReadWrite|CFile::typeBinary, NULL))
	{
		return FALSE;
	}

	int nFileSize = (int)File.GetLength();
	int nBlockSize = sizeof(TIMING);
	int nCount = int(nFileSize/nBlockSize);
	if (nCount < 1)
		return FALSE;

	CMemoryAllocate<char> alloc_char(nFileSize+1);
	File.Read(alloc_char.GetAddress(), nFileSize);

	if (bDecrypt)
		Decrypt(alloc_char.GetAddress(), alloc_char.GetAddress(), nFileSize);

	TIMING st;
	for (int i = 0; i < nCount; i++)
	{
		memcpy(&st, alloc_char.GetAddress()+i*nBlockSize, nBlockSize);
		if (IsExist(st) == INVALID_VALUE)
			AddResolution(TRUE, st);
	}

	File.Close();

	return TRUE;
}

BOOL CResolutionManager::WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt)
{
	LPCTSTR pszPath = NULL;
	if (lpszFilePath == NULL)
		pszPath = m_szFilePath;
	else 
		pszPath = lpszFilePath;

	CStdioFile File;
	if (!File.Open(pszPath, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary, NULL))
		return FALSE;

	int nCount = m_arrRes.GetCount();
	if (nCount == 0)
		return TRUE;

	int nBlockSize = sizeof(TIMING);
	int nFileSize = nCount*nBlockSize;

	CMemoryAllocate<char> alloc_char(nFileSize+1);

	for (int i=0; i<nCount; i++)
	{
		memcpy(alloc_char.GetAddress()+i*nBlockSize, &m_arrRes[i], nBlockSize);
	}

	if (bEncrypt)
		Encrypt(alloc_char.GetAddress(), alloc_char.GetAddress(), nFileSize);

	File.Write(alloc_char.GetAddress(), nFileSize);
	File.Close();

	return TRUE;
}


int CResolutionManager::GetResolutionCount() const
{
	return m_arrRes.GetCount();
}

int CResolutionManager::IsExist(const TIMING &timing) const
{
	for (int i=0; i<m_arrRes.GetCount(); i++)
	{
		if (timing == m_arrRes[i])
		{
			return i;
		}
	}

	return INVALID_VALUE;
}

int CResolutionManager::AddResolution(BOOL bInnerCall, TIMING &NewTiming)
{
	if (IsExist(NewTiming) != INVALID_VALUE)
	{
		return INVALID_VALUE;
	}

	int nInsertAt = INVALID_VALUE;
	int nCount = m_arrRes.GetCount();
	if (nCount == 0)
	{
		nInsertAt = m_arrRes.Add(NewTiming);
	}
	else
	{
		for (int i=0; i<nCount; i++)
		{
			//先比较HActive
			if (NewTiming.m_HActive < m_arrRes[i].m_HActive)
			{
				m_arrRes.InsertAt(i, NewTiming);
				nInsertAt = i;
				break;
			}
			else if (NewTiming.m_HActive == m_arrRes[i].m_HActive)
			{
				//再比较VActive
				if (NewTiming.m_VActive < m_arrRes[i].m_VActive)
				{
					m_arrRes.InsertAt(i, NewTiming);
					nInsertAt = i;
					break;
				}
				else if (NewTiming.m_VActive == m_arrRes[i].m_VActive)
				{
					//最后比较HZ
					if (NewTiming.m_Hz < m_arrRes[i].m_Hz)
					{
						m_arrRes.InsertAt(i, NewTiming);
						nInsertAt = i;
						break;
					}
				}
			}
		}
	}

	if (!bInnerCall)
		WriteToFile(NULL);

	return nInsertAt;
}

void CResolutionManager::AddDefault()
{
	m_arrRes.RemoveAll();

	for (int i=0; i<_MAX_ORG_RESOLUTION; i++)
	{
		m_arrRes.Add(OriginalResolutions[i]);
	}

	//EncryptWriteFile(NULL);
}

void CResolutionManager::ModifyResolution(const int index, TIMING &Timing)
{
	if (index < 0 || index > m_arrRes.GetCount())
		return;

	m_arrRes[index] = Timing;

	WriteToFile(NULL);
}

void CResolutionManager::RemoveResolution(const TIMING &DelTiming)
{
	for (int i=0; i<m_arrRes.GetCount(); i++)
	{
		if (m_arrRes[i].m_HActive == DelTiming.m_HActive
			&& m_arrRes[i].m_VActive == DelTiming.m_VActive
			&& m_arrRes[i].m_Hz == DelTiming.m_Hz)
		{
			m_arrRes.RemoveAt(i);
			break;
		}
	}

	WriteToFile(NULL);
}

void CResolutionManager::RemoveAllCustomDefined()
{
	for (int i=0; i<m_arrRes.GetCount(); i++ )
	{
		if (m_arrRes[i].m_Type == CUSTOM_RESOLUTION)
		{
			m_arrRes.RemoveAt(i);
		}
	}

	WriteToFile(NULL);
}

void CResolutionManager::RemoveAllResolution()
{
	m_arrRes.RemoveAll();

	WriteToFile(NULL);
}

int CResolutionManager::GetResolutionIndex(const TIMING &timing) const
{
	for (int i=0; i<m_arrRes.GetCount(); i++)
	{
		if (m_arrRes[i].m_HActive == timing.m_HActive
			&& m_arrRes[i].m_VActive == timing.m_VActive
			&& m_arrRes[i].m_Hz == timing.m_Hz)
		{
			return i;
		}
	}

	return INVALID_VALUE;
}

char *CResolutionManager::GetResolutionName(const TIMING &timing)
{
	sprintf_s(m_pszResolution, _MAX_FNAME, "%d x %d @%dHz", timing.m_HActive, timing.m_VActive, timing.m_Hz);

	return m_pszResolution;
}

char *CResolutionManager::GetResolutionName(const int index)
{
	if (index < 0 || index >= m_arrRes.GetCount())
		return NULL;

	sprintf_s(m_pszResolution, _MAX_FNAME, "%d x %d @%dHz", m_arrRes[index].m_HActive, m_arrRes[index].m_VActive, m_arrRes[index].m_Hz);

	return m_pszResolution;
}

TIMING CResolutionManager::GetResolutionDetail(const int index) const
{
	TIMING st = {0};

	if (index < 0 || index >= m_arrRes.GetCount())
		return st;

	st = m_arrRes[index];

	return st;
}

TIMING CResolutionManager::GetDefaultResolution(const int nHact, const int nVact, const int nHz)
{
	TIMING st = {0};

	for (int i=0; i<_MAX_ORG_RESOLUTION; i++)
	{
		if (   OriginalResolutions[i].m_HActive == nHact
			&& OriginalResolutions[i].m_VActive == nVact
			&& OriginalResolutions[i].m_Hz      == nHz)
		{
			st = OriginalResolutions[i];
			break;
		}
	}

	return st;
}