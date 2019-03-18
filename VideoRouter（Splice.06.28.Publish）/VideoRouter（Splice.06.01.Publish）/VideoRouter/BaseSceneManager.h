#pragma once

//////////////////////////////////////////////////////////////////////////
// CSceneHeadInfo 场景头信息
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

	//其他场景内容
	//矩阵场景存储的是输入到输出的映射关系
	//拼接场景存储的是所有窗体信息
};


//////////////////////////////////////////////////////////////////////////
// CSceneLoopManager  基本场景管理器
// 

class CLoopItem
{
public:
	CLoopItem();
	virtual ~CLoopItem();
	int   nID;       //循环场景ID
	BOOL  bOnTurn;   //正在循环
	long dwExcute;   //执行总时间 毫秒单位
	long dwStart;    //执行开始时间 毫秒单位
	long dwRemain;   //执行剩余时间 毫秒单位
};

//////////////////////////////////////////////////////////////////////////
// 场景循环管理器
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

	// 循环线程句柄
	CWinThread *m_hThread;
};

//////////////////////////////////////////////////////////////////////////
// CBaseSceneManager  基本场景管理器
// 

class CBaseSceneManager : public CBaseFileManager
{
public:
	CBaseSceneManager();
	virtual ~CBaseSceneManager();

	// 初始化场景文件
	void InitFile();

	//加载配置文件
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt=TRUE);
	//写入配置文件
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

	// 通知相关单位打包场景数据
	void NotifyToPackageFile(LPCTSTR lpszFilePath);
	// 通知相关单位解析场景文件
	void NotifyToUnpackFile(LPCTSTR lpszFilePath);

public:
	// 加载设备场景
	BOOL LoadSceneFromDevice(const int nSceneID);
	// 初始化场景配置
	void InitSceneFromDevice();

	// 场景循环
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
