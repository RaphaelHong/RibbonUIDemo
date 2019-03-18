// BaseSceneManager.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoRouter.h"
#include "BaseSceneManager.h"


//////////////////////////////////////////////////////////////////////////
// CSceneHeadInfo

CSceneHeadInfo::CSceneHeadInfo()
{

}

CSceneHeadInfo::~CSceneHeadInfo()
{

}

void CSceneHeadInfo::operator=(const CSceneHeadInfo &refItem)
{
	m_nID  = refItem.m_nID;

	m_nLargeImage = refItem.m_nLargeImage;
	m_nSmallImage = refItem.m_nSmallImage;

	m_bQuickMenu = refItem.m_bQuickMenu;

	m_dwExcuteTime = refItem.m_dwExcuteTime;
	m_dwRemainTime = refItem.m_dwRemainTime;

	strncpy_s(m_szName, _MAX_SCENE_NAME, refItem.m_szName, _MAX_SCENE_NAME);
}

int CSceneHeadInfo::GetObjectSize()
{
	return \
	int(sizeof(int) + sizeof(int) +  sizeof(int) + sizeof(BOOL) + sizeof(long) + sizeof(long) + _MAX_SCENE_NAME);
	//    m_nID      m_nSmallImage  m_nLargeImage  m_bQuickMenu  m_dwExcuteTime  m_dwRemainTime     m_szName
}

int CSceneHeadInfo::Write(char* pBlock, const long lSize, long &lPos)
{
	if (lPos + GetObjectSize() > lSize) return -1;

	int nSize = 0;

	memcpy(pBlock+nSize, &m_nID, sizeof(int));  nSize += sizeof(int);
	memcpy(pBlock+nSize, &m_nSmallImage, sizeof(int)); nSize += sizeof(int);
	memcpy(pBlock+nSize, &m_nLargeImage, sizeof(int)); nSize += sizeof(int);
	memcpy(pBlock+nSize, &m_bQuickMenu, sizeof(BOOL)); nSize += sizeof(BOOL);
	memcpy(pBlock+nSize, &m_dwExcuteTime, sizeof(long)); nSize += sizeof(long);
	memcpy(pBlock+nSize, &m_dwRemainTime, sizeof(long)); nSize += sizeof(long);
	memcpy(pBlock+nSize, m_szName, _MAX_SCENE_NAME); nSize += _MAX_SCENE_NAME;

	lPos += nSize;

	return nSize;
}

int CSceneHeadInfo::Read(char* pBlock, const long lSize, long &lPos)
{
	if (lPos + GetObjectSize() > lSize) return -1;

	int nSize = 0;

	memcpy(&m_nID,          pBlock+nSize, sizeof(int));  nSize += sizeof(int);
	memcpy(&m_nSmallImage,  pBlock+nSize, sizeof(int));  nSize += sizeof(int);
	memcpy(&m_nLargeImage,  pBlock+nSize, sizeof(int));  nSize += sizeof(int);
	memcpy(&m_bQuickMenu,   pBlock+nSize, sizeof(BOOL)); nSize += sizeof(BOOL);
	memcpy(&m_dwExcuteTime, pBlock+nSize, sizeof(long)); nSize += sizeof(long);
	memcpy(&m_dwRemainTime, pBlock+nSize, sizeof(long)); nSize += sizeof(long);
	memcpy(m_szName,        pBlock+nSize, _MAX_SCENE_NAME); nSize += _MAX_SCENE_NAME;

	lPos += nSize;

	return nSize;
}

void CSceneHeadInfo::SetName(LPCTSTR lpszName)
{
	strncpy_s(m_szName, _MAX_SCENE_NAME, lpszName, strlen(lpszName));
}

LPCTSTR CSceneHeadInfo::GetName() const
{
	return (LPCTSTR)m_szName;
}


//////////////////////////////////////////////////////////////////////////
// CLoopItem 轮巡项

CLoopItem::CLoopItem()
{

}

CLoopItem::~CLoopItem()
{

}


int CSceneLoopManager::m_sLoopstate = THD_LOOP_IDLE;
CCriticalSection CSceneLoopManager::m_section;
//////////////////////////////////////////////////////////////////////////
// CSceneLoopManager

// 场景循环执行线程
UINT LoopThread(LPVOID lpvoid);

UINT LoopThread(LPVOID lpvoid)
{
	CSceneLoopManager *pLoopManager = (CSceneLoopManager*)lpvoid;

	CLoopItem* Loopdata;

	int nSecond = 0;
	int nIndex = 0;
	int nLoopState = THD_LOOP_IDLE;

	while(TRUE)
	{
		//获取当前场景循环状态
		pLoopManager->Lock();
		nLoopState = CSceneLoopManager::GetLoopState();
		pLoopManager->UnLock();

		if (nLoopState == THD_LOOP_IDLE)
		{
			//当前场景循环已停止
			Sleep(10);
			continue;
		}
		else if (nLoopState == THD_LOOP_EXIT)
		{
			//退出线程
			break;
		}
		else if (nLoopState == THD_LOOP_WORK)
		{
			//正在循环场景

			//锁定全局量
			pLoopManager->Lock();

			//如果场景数组为空，将循环模式切到空闲
			int nSize = pLoopManager->GetCount();
			if (nSize == 0) {
				CSceneLoopManager::SetLoopState(THD_LOOP_IDLE);
				pLoopManager->UnLock();
				continue;
			}

			//获取正在循环中的场景
			Loopdata = pLoopManager->GetOnturnItem(nIndex);
			if (Loopdata == NULL)
			{
				Loopdata = pLoopManager->GetLoopItem(0);
				Loopdata->dwStart  = ::GetTickCount();
				Loopdata->dwRemain = Loopdata->dwExcute;
				Loopdata->bOnTurn  = TRUE;

				GetSceneManager()->LoadSceneFromDevice(Loopdata->nID);
			}
			else
			{
				Loopdata->dwRemain = Loopdata->dwExcute - (::GetTickCount() - Loopdata->dwStart);
				//TRACE(_T("%d\n"), Loopdata->dwRemain);
				if (Loopdata->dwRemain < 0)
				{
					Loopdata->dwRemain = 0;
					Loopdata->bOnTurn  = FALSE;

					if (nSize > 1)
					{
						nIndex = (nIndex == (nSize-1)) ? 0 : nIndex+1;
						Loopdata = pLoopManager->GetLoopItem(nIndex);
						Loopdata->dwRemain = Loopdata->dwExcute;
						Loopdata->dwStart  = ::GetTickCount();
						Loopdata->bOnTurn = TRUE;

						GetSceneManager()->LoadSceneFromDevice(Loopdata->nID);
					}
				}
			}

			//解除锁定
			pLoopManager->UnLock();

			Sleep(50);
		}
	}

	return 0x1009;
}

CSceneLoopManager::CSceneLoopManager()
{
	m_hThread = NULL;
	//m_bExit = FALSE;
}

CSceneLoopManager::~CSceneLoopManager()
{
	if (m_hThread != NULL)
	{
		ExitLoopThread();
	}

	RemoveAllItem();
}

void CSceneLoopManager::SetLoopState(const int &nState)
{
	Lock();

	m_sLoopstate = nState;

	UnLock();
}

int CSceneLoopManager::GetLoopState()
{
	int nState;

	Lock();

	nState = m_sLoopstate;

	UnLock();

	return nState;
}

BOOL CSceneLoopManager::IsLoopping()
{
	BOOL bLoopping = FALSE;

	Lock();

	bLoopping = (m_sLoopstate == THD_LOOP_WORK);

	UnLock();

	return bLoopping;
}

void CSceneLoopManager::StartLoopThread()
{
	Lock();

	m_sLoopstate = THD_LOOP_WORK;

	UnLock();

	if (m_hThread == NULL)
	{
		m_hThread = AfxBeginThread(LoopThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
	}

	m_hThread->ResumeThread();	
}

void CSceneLoopManager::SuspendLoopThread()
{
	if (m_hThread != NULL)
	{
		Lock();

		m_sLoopstate = THD_LOOP_IDLE;

		UnLock();

		m_hThread->SuspendThread();
	}
}

void CSceneLoopManager::ResumeLoopThread()
{
	if (m_hThread != NULL)
	{
		Lock();

		m_sLoopstate = THD_LOOP_WORK;

		UnLock();

		m_hThread->ResumeThread();
	}
}

void CSceneLoopManager::ExitLoopThread()
{
	Lock();

	m_sLoopstate = THD_LOOP_EXIT;

	UnLock();

	if (m_hThread != NULL)
	{
		m_hThread->ResumeThread();

		Sleep(200);

		m_hThread = NULL;
	}
}


CLoopItem* CSceneLoopManager::AddLoopItem(int nID, DWORD dwExcuteTime)
{
	//return NULL;
	CLoopItem* pItem = new CLoopItem;
	pItem->nID = nID;
	pItem->dwExcute = dwExcuteTime;
	pItem->bOnTurn = FALSE;
	pItem->dwStart = 0;
	pItem->dwRemain = dwExcuteTime;

	m_arLoop.Add(pItem);

	return pItem;
}

CLoopItem* CSceneLoopManager::GetLoopItem(const int nIndex)
{
	CLoopItem* pItem = NULL;

	Lock();

	if (nIndex >= 0 && nIndex < m_arLoop.GetSize())
		pItem = m_arLoop[nIndex];

	UnLock();

	return pItem;
}

CLoopItem* CSceneLoopManager::GetOnturnItem(int &nIndex)
{
	Lock();

	for (int i=0; i<m_arLoop.GetSize(); i++)
	{
		if (m_arLoop[i]->bOnTurn == TRUE)
		{
			UnLock();
			nIndex = i;
			return m_arLoop[i];
		}
	}

	UnLock();

	nIndex = -1;
	return NULL;
}

int CSceneLoopManager::GetCount() 
{
	int nSize;

	Lock();

	nSize = m_arLoop.GetSize();

	UnLock();

	return nSize;
}


void CSceneLoopManager::RemoveAllItem()
{
	Lock();

	for (int i=0; i<m_arLoop.GetSize(); i++)
	{
		delete m_arLoop[i];//->InternalRelease();
	}

	m_arLoop.RemoveAll();

	UnLock();
}

void CSceneLoopManager::RemoveItem(int nID)
{
	Lock();

	for (int i=0; i<m_arLoop.GetSize(); i++)
	{
		if (m_arLoop[i]->nID == nID)
		{
			delete m_arLoop[i];//->InternalRelease();
			m_arLoop.RemoveAt(i);
			break;
		}
	}

	UnLock();
}


//////////////////////////////////////////////////////////////////////////
// CBaseSceneManager

CBaseSceneManager* pSceneManager = NULL;

CBaseSceneManager::CBaseSceneManager()
{
	CString strPath;
	strPath = GetGlobalManager()->GetAppPath() + _T("Config\\Scene.tmp");

	SetConfigFilePath(strPath.GetBuffer(0));
}

CBaseSceneManager::~CBaseSceneManager()
{
	m_bLockWriteFile = TRUE;
	RemoveAll();
	m_bLockWriteFile = FALSE;
}

void CBaseSceneManager::InitFile()
{
	m_bLockWriteFile = TRUE;
	LoadFromFile(NULL);
	m_bLockWriteFile = FALSE;
}

BOOL CBaseSceneManager::LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt)
{
	//选择文件路径
	LPCTSTR pszPath = NULL;
	if (lpszFilePath == NULL)
		pszPath = m_szFilePath;
	else 
		pszPath = lpszFilePath;

	CStdioFile OpenFile;
	if (OpenFile.Open(pszPath, CFile::modeRead|CFile::typeBinary, NULL) != TRUE)
	{
		return FALSE;
	}

	// 读取文件
	long nFileSize = (long)OpenFile.GetLength();
	CMemoryAllocate<char> alloc_char(nFileSize+1);
	OpenFile.Read(alloc_char.GetAddress(), nFileSize);

	if (bDecrypt)
		Decrypt(alloc_char.GetAddress(), alloc_char.GetAddress(), nFileSize);

	// 清空
	RemoveAll();

	// 提取
	int nRead = 0;
	long iPos = 0;
	CSceneHeadInfo* psci = NULL;
	while (iPos < nFileSize)
	{
		psci = new CSceneHeadInfo;
		nRead = psci->Read(alloc_char.GetAddress()+iPos, nFileSize, iPos);
		if (nRead < 0)
		{
			SAFE_DELETE(psci);
			break;
		}

		m_arSceneItem.Add(psci);
	}

	OpenFile.Close();

	psci = NULL;

	return TRUE;
}

BOOL CBaseSceneManager::WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt)
{
	if (m_bLockWriteFile)
		return TRUE;

	//选择文件路径
	LPCTSTR pszPath = NULL;
	if (lpszFilePath == NULL)
		pszPath = m_szFilePath;
	else 
		pszPath = lpszFilePath;

	CStdioFile SaveFile;
	if (SaveFile.Open(pszPath, CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary, NULL) != TRUE)
	{
		AfxMessageBox(_T("Err, 创建场景配置文件失败！"));
		return FALSE;
	}

	int nCount = m_arSceneItem.GetCount();
	if (nCount <=0 )
		return FALSE;

	int nSceneSize = sizeof(CSceneHeadInfo);
	int nFileSize = nCount*nSceneSize;
	CMemoryAllocate<char> alloc_char(nFileSize+1);

	int nWritten = 0;
	long iPos = 0;
	for (int i=0; i<nCount; i++)
	{
		nWritten = m_arSceneItem[i]->Write(alloc_char.GetAddress()+iPos, nFileSize, iPos);
		if (nWritten < 0)
		{
			AfxMessageBox(_T("Write file error! out memory!"));
			break;
		}
	}

	if (bEncrypt)
		Encrypt(alloc_char.GetAddress(), alloc_char.GetAddress(), nFileSize);

	SaveFile.Write(alloc_char.GetAddress(), iPos);
	SaveFile.Close();

	return TRUE;
}

// CBaseSceneManager 成员函数
int CBaseSceneManager::GetCount() const
{
	return m_arSceneItem.GetCount();
}

int CBaseSceneManager::AddItem(CSceneHeadInfo* pItem)
{
	if (pItem == NULL)
		return -1;

	int nPos = ArrangeAdd(pItem);
	WriteToFile(NULL);

	return nPos;
}

CSceneHeadInfo* CBaseSceneManager::Add(const CSceneHeadInfo &cstItem)
{
	CSceneHeadInfo* pItem = new CSceneHeadInfo;
	*pItem = cstItem;

	ArrangeAdd(pItem);
	WriteToFile(NULL);

	return pItem;
}

int CBaseSceneManager::ArrangeAdd(CSceneHeadInfo* pItem)
{
	ASSERT(pItem);

	if (m_arSceneItem.GetSize() == 0)
		return m_arSceneItem.Add(pItem);

	if (pItem->m_nID < m_arSceneItem[0]->m_nID)
	{
		m_arSceneItem.InsertAt(0, pItem);
		return 0;
	}

	if (pItem->m_nID > m_arSceneItem[m_arSceneItem.GetSize()-1]->m_nID)
		return m_arSceneItem.Add(pItem);

	for (int i=0; i<m_arSceneItem.GetSize()-1; i++)
	{
		if (pItem->m_nID > m_arSceneItem[i]->m_nID
		 && pItem->m_nID < m_arSceneItem[i+1]->m_nID)
		{
			m_arSceneItem.InsertAt(i+1, pItem);
			return i+1;
		}
	}

	return -1;
}

void CBaseSceneManager::RemoveAt(const int nVal, BOOL byID)
{
	int i = -1;
	if (byID)
		i = FindID(nVal);
	else
		i = nVal;

	if (i < 0 || i >= m_arSceneItem.GetSize())
		return;

	delete m_arSceneItem[i];
	m_arSceneItem.RemoveAt(i);

	WriteToFile(NULL);
}

void CBaseSceneManager::RemoveItem(CSceneHeadInfo* pItem)
{
	for (int i=0; i<m_arSceneItem.GetSize(); i++)
	{
		if (m_arSceneItem[i] == pItem)
		{
			delete pItem;
			m_arSceneItem.RemoveAt(i);
			break;
		}
	}

	WriteToFile(NULL);
}

void CBaseSceneManager::RemoveAll()
{
	for (int i=0; i<m_arSceneItem.GetSize(); i++)
	{
		delete m_arSceneItem[i];
	}

	m_arSceneItem.RemoveAll();

	WriteToFile(NULL);
}

CSceneHeadInfo* CBaseSceneManager::GetAt(const int nIndex) const
{
	if(nIndex < 0 || nIndex >= m_arSceneItem.GetSize())
		return NULL;

	return m_arSceneItem.GetAt(nIndex);
}

CSceneHeadInfo* CBaseSceneManager::FindItemByID(const int nID) const
{
	int nIndex = FindID(nID);

	if (nIndex < 0)
		return NULL;
	else
		return m_arSceneItem[nIndex];
}

int CBaseSceneManager::FindID(const int nID) const
{
	int nIndex = -1;
	for (int i=0; i<m_arSceneItem.GetSize(); i++)
	{
		if (m_arSceneItem[i]->m_nID == nID)
		{
			nIndex = i;
			break;
		}
	}

	return nIndex;
}

int CBaseSceneManager::PrepareID() const
{
	int nNewID = INVALID_VALUE;
	BOOL bUsed = FALSE;

	int nCount = m_arSceneItem.GetCount();
	for (int iScnID=1; iScnID<_MAX_SCENE_COUNT+1; iScnID++)
	{
		bUsed = FALSE;
		//从场景数组中找出ID等于iScnID的场景
		for (int iIndex=0; iIndex < nCount; iIndex++)
		{
			if(m_arSceneItem[iIndex]->m_nID == iScnID)
			{
				bUsed = TRUE;
				break;
			}
		}

		if (bUsed == FALSE)
		{
			nNewID = iScnID;
			break;
		}
	}

	// 没有空闲ID则将ID置1
	if (nNewID == INVALID_VALUE)
		nNewID = 1;

	return nNewID;
}

void CBaseSceneManager::NotifyToPackageFile(LPCTSTR lpszFilePath)
{
	::SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_PACKAGE_SCENE, (WPARAM)lpszFilePath, 0);
}

void CBaseSceneManager::NotifyToUnpackFile(LPCTSTR lpszFilePath)
{
	::SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_UNPACK_SCENE, (WPARAM)lpszFilePath, 0);
}

// 场景调用
BOOL CBaseSceneManager::LoadSceneFromDevice(const int nSceneID)
{	
	int nLen = 0;
	UINT* pData = NULL;

	if (GetCmdManager()->CmdCALL(SYSCARD_PORT, eCardTypeCtrl, nSceneID, pData, nLen, TRUE) == CMD_RTOK)
	{
		::SendMessage(GetMainFrame()->GetActiveView()->m_hWnd, WM_USER_RECV_SCENE_DATA, (WPARAM)pData, (LPARAM)&nLen);

		SAFE_DELETE_AR(pData);
		return TRUE;
	}

	SAFE_DELETE_AR(pData);
	return FALSE;
}

// 初始化所有场景
void CBaseSceneManager::InitSceneFromDevice()
{

}

void CBaseSceneManager::PrepareLoop(const int* pLoopID, const int &nLen)
{
	if (m_LoopManager.GetCount() > 0)
		m_LoopManager.RemoveAllItem();

	int i=0;

	while (i<nLen) {

		m_LoopManager.AddLoopItem(pLoopID[i], FindItemByID(pLoopID[i])->m_dwExcuteTime);
		i++;
	}
}

BOOL CBaseSceneManager::BeginLoop()
{
	if (m_LoopManager.GetCount() > 0)
		m_LoopManager.StartLoopThread();

	return (m_LoopManager.GetCount() > 0);
}

void CBaseSceneManager::PalseLoop()
{
	m_LoopManager.SuspendLoopThread();
}

void CBaseSceneManager::EndLoop()
{
	m_LoopManager.ExitLoopThread();
}

CSceneLoopManager* CBaseSceneManager::GetLoopManager()
{
	return &m_LoopManager;
}