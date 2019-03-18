#pragma once

//******************************************
// CGlobalManager
// 全局调度管理器
//

class CGlobalManager : public CObject
{
	friend class CInitialManager;

public:
	CGlobalManager();
	virtual ~CGlobalManager();

private:
	DWORD m_dwLangID;       //语言ID
	DEVICEINFO m_DevInfo;   //设备信息

public:
	// 获取执行程序的路径
	CString GetAppPath();

	// 初始化全局控制变量
	void Init();
	
	// 设备构造信息
	void InitDeviceStructure(pDEVICEINFO pInfo);
	pDEVICEINFO GetDeviceStructure() const;
};

// 定义一个全局配置管理对象
extern CGlobalManager *pGlobalManager;