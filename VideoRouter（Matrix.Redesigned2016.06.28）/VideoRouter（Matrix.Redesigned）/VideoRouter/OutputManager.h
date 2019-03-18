#pragma once
#include "InputManager.h"

#define _OFF 0

// 获取所有输出端口的信号源状态
typedef enum SourceState
{
	eSourceStateUndef,           // 未知状态
	eSourceStateSameCurrent,     // 所有输出都由指定的信号源ID输入
	eSourceStateSameNoCurrent,   // 所有输出都由相同的信号源ID输入，此ID与指定的信号源ID不一样
	eSourceStateDiffCurrent,     // 所有输出的信号源ID输入不一致，且其中有些输出由指定的信号源ID输入
	eSourceStateDiffNoCurrent,   // 所有输出的信号源ID输入不一致，且没有输出由指定的信号源ID输入
	eSourceStateAllOFF,          // 所有输出都没有信号输入

} SState;

//******************************************
// COutputManager
// 矩阵拼接器输出端口管理器
//

class COutputManager : public CInputManager
{
	friend class CDrawScreenObj;
	friend class CGlobalManager;
	friend class CInitialManager;
	friend class CInputManager;
public:
	COutputManager();
	virtual ~COutputManager();

	virtual void DemoInit();

	virtual BOOL Reset(const int iKey, BOOL byID=TRUE);
	virtual BOOL ResetAll();

	static int m_sCurPort;

public:
	//------------------------------ 来源信号ID管理 -------------------//

	// 设置来源信号ID
	void SetFeederID(const int iKey, const int iFeederID, BOOL bInvs, BOOL byID=TRUE);

	// 设置所有输出由同一个信号源输入
	void SetSameFeederID(const int &iFeederID, BOOL bInvs);

	// 获取来源信号ID
	int GetFeederID(const int iKey, BOOL byID=TRUE) const;

	////////////////////////////////////////////////
	// P2PSwitch 点对点切换 界面上的单个切换也调用此方法
	// 参数：
	// iKey: 参数byID为TRUE，iKey表示某输出端口的ID号，否则为该端口的索引号。
	// iFeederID: 来源信号的ID
	// bInvs: TRUE:  反选，输出口的来源信号ID与iFeederID相等，则关闭该端口；反之，设置该端口来源ID为iFeederID。
	//        FALSE: 直通，直接将该输出口的来源ID置为iFeederID。
	// byID: 用来标示参数 iKey 的意义

	void P2PSwitch(const int iKey, const int &iFeederID, BOOL bInvs, BOOL byID=TRUE);

	////////////////////////////////////////////////
	// AllSwitch 一对所有切换 界面上作全选时也调用此方法
	// 参数：
	// iFeederID: 来源信号的ID
	// bInvs: TRUE:  反选，输出口的来源信号ID与iFeederID相等，则关闭该端口；反之，设置该端口来源ID为iFeederID。
    //        FALSE: 直通，直接将该输出口的来源ID置为iFeederID。

	void AllSwitch(const int &iFeederID, BOOL bInvs);
	
	////////////////////////////////////////////////
	// MultiSwitch 一对多切换
	// 参数：
	// iFeederID: 来源信号的ID
	// pOutputIDList: 所选的输出口ID列表
	// iCount: 所选的输出口数

	void MultiSwitch(const int &iFeederID, int *pOutputIDList, int iCount);

	////////////////////////////////////////////////
	// MappingSwitch 映射切换: 
	// 参数：
	// iSize: 映射个数
	// pPair: 映射表

	void MappingSwitch(const int &iSize, const INT_PAIR *pPair);
};


extern COutputManager *pOutputManager;