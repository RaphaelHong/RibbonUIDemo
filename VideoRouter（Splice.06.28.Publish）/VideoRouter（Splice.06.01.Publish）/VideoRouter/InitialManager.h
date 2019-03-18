#pragma once

//******************************************
// CInitialManager
// 初始化事务管理器
//

class CInitialManager : public CObject
{
	friend class CGlobalManager;
public:
	CInitialManager();
	virtual ~CInitialManager();

	// 初始化各管理器{全局事务管理器、通信事务管理器、输入管理器、输出管理器等}
	void InitManagers();

	// 释放内存资源
	void ReleaseManagers();

	// 初始化设备数据 {建立通信后从设备读取数据并初始化到界面}
	BOOL InitDeviceContents();
	// 初始化拼接属性
	BOOL InitSpliceLayout();
	// 初始化拼接场景
	BOOL InitSpliceScene();

	/* 设置事务类型 ， 在调用事务处理线程之前(BeginTransaction函数)，先设置事务类型 */
	void SetTransactionType(const emTransaction eTransaction);

	/* 特殊事务处理 */
	BOOL BeginTransaction(BOOL bShowWaitWnd);
	void EndTransacation();

	/* 设置初始化标志量，在每次连接初始化完毕后设为TRUE，每次断开连接后设为FALSE */
	void SetInitializedFlag(BOOL bInit);
	BOOL GetInitializedFlag() const;

private:
	emTransaction m_eTransaction;
	BOOL m_bInitialized; //程序已通过连接初始化过。
};


extern CInitialManager *pInitManager;