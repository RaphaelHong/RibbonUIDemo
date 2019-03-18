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

	emWorkMode m_eWorkMode; //工作模式

	DEVICEINFO m_DevInfo;   //设备信息

public:
	// 获取执行程序的路径
	CString GetAppPath();

	// 初始化全局控制变量
	void Init();
	// 设备构造信息
	void InitDeviceStructure(pDEVICEINFO pInfo);
	pDEVICEINFO GetDeviceStructure() const;

	// 获取当前设备的工作模式 ：eWordModeSplit:拼接模式 eWorkModeQuick:矩阵模式
	emWorkMode GetCurrencyWorkMode() const;
	emWorkMode &GetCurrencyWorkMode();
	// 获取当前APP语言
	DWORD GetCurrencyLangID() const;
};

// 定义一个全局配置管理对象
extern CGlobalManager *pGlobalManager;