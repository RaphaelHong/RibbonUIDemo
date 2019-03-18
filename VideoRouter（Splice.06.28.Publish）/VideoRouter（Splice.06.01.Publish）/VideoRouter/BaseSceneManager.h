#pragma once

//////////////////////////////////////////////////////////////////////////
// CSceneHeadInfo ����ͷ��Ϣ
// 
class CSceneHeadInfo : public CObject
{
	friend class CBaseSceneManager;
	friend class CMatrixSceneManager;
	friend class CSpliceSceneManager;

public:
	CSceneHeadInfo();
	~CSceneHeadInfo();

	void operator=(const CSceneHeadInfo &refItem);

	int Write(char* pBlock, const long lSize, long &lPos);
	int Read(char* pBlock, const long lSize, long &lPos);
	static int GetObjectSize();

public:
	int GetID() const { return m_nID; }
	int& GetID() { return m_nID; }

	int GetSmallImage() const { return m_nSmallImage; }
	int& GetSmallImage() { return m_nSmallImage; }

	int GetLargeImage() const { return m_nLargeImage; }
	int& GetLargeImage() { return m_nLargeImage; }

	BOOL GetQuickMenuFlag() const { return m_bQuickMenu; }
	BOOL& GetQuickMenuFlag() { return m_bQuickMenu; }

	long GetExcuteTime() const { return m_dwExcuteTime; }
	long& GetExcuteTime() { return m_dwExcuteTime; }

	long GetRemainTime() const { return m_dwRemainTime; }
	long& GetRemainTime() { return m_dwRemainTime; }

	void SetName(LPCTSTR lpszName);
	LPCTSTR GetName() const;

private:
	int  m_nID;
	int  m_nSmallImage;
	int  m_nLargeImage;
	
	BOOL m_bQuickMenu;

	long m_dwExcuteTime; // millsecond
	long m_dwRemainTime; // millsecond

	char m_szName[_MAX_SCENE_NAME];

	//������������
	//���󳡾��洢�������뵽�����ӳ���ϵ
	//ƴ�ӳ����洢�������д�����Ϣ
};


//////////////////////////////////////////////////////////////////////////
// CSceneLoopManager  ��������������
// 

class CLoopItem
{
public:
	CLoopItem();
	virtual ~CLoopItem();
	int   nID;       //ѭ������ID
	BOOL  bOnTurn;   //����ѭ��
	long dwExcute;   //ִ����ʱ�� ���뵥λ
	long dwStart;    //ִ�п�ʼʱ�� ���뵥λ
	long dwRemain;   //ִ��ʣ��ʱ�� ���뵥λ
};

//////////////////////////////////////////////////////////////////////////
// ����ѭ��������
class CSceneLoopManager
{
public:
	CSceneLoopManager();
	~CSceneLoopManager();

	static void Lock() {
		m_section.Lock();
	}
	static void UnLock() {
		m_section.Unlock();
	}

	static void SetLoopState(const int &nState);
	static int GetLoopState();
	static BOOL IsLoopping();

	void StartLoopThread();
	void SuspendLoopThread();
	void ResumeLoopThread();
	void ExitLoopThread();

	CLoopItem* AddLoopItem(int nID, DWORD dwExcuteTime);
	CLoopItem* GetLoopItem(const int nIndex);
	CLoopItem* GetOnturnItem(int &nIndex);
	void RemoveAllItem();
	void RemoveItem(int nID);
	int GetCount();

private:
	CArray <CLoopItem*, CLoopItem*> m_arLoop;

	static CCriticalSection m_section;
	static int m_sLoopstate;

	// ѭ���߳̾��
	CWinThread *m_hThread;
};

//////////////////////////////////////////////////////////////////////////
// CBaseSceneManager  ��������������
// 

class CBaseSceneManager : public CBaseFileManager
{
public:
	CBaseSceneManager();
	virtual ~CBaseSceneManager();

	// ��ʼ�������ļ�
	void InitFile();

	//���������ļ�
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt=TRUE);
	//д�������ļ�
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt=TRUE);

public:
	int GetCount() const;
	int AddItem(CSceneHeadInfo* pItem);
	CSceneHeadInfo* Add(const CSceneHeadInfo &cstItem);

	void RemoveAt(const int nVal, BOOL byID=FALSE);
	void RemoveItem(CSceneHeadInfo* pItem);
	void RemoveAll();

	CSceneHeadInfo* GetAt(const int nIndex) const;
	CSceneHeadInfo* FindItemByID(const int nID) const;
	int FindID(const int nID) const;

	int PrepareID() const;

	// ֪ͨ��ص�λ�����������
	void NotifyToPackageFile(LPCTSTR lpszFilePath);
	// ֪ͨ��ص�λ���������ļ�
	void NotifyToUnpackFile(LPCTSTR lpszFilePath);

public:
	// �����豸����
	BOOL LoadSceneFromDevice(const int nSceneID);
	// ��ʼ����������
	void InitSceneFromDevice();

	// ����ѭ��
public:
	void PrepareLoop(const int* pLoopID, const int &nLen);
	BOOL BeginLoop();
	void PalseLoop();
	void EndLoop();
	CSceneLoopManager* GetLoopManager();

private:
	CArray <CSceneHeadInfo*, CSceneHeadInfo*> m_arSceneItem;
	int ArrangeAdd(CSceneHeadInfo* pItem);

	CSceneLoopManager m_LoopManager;
};

extern CBaseSceneManager* pSceneManager;
