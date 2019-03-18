#pragma once

//******************************************
// CInputManager
// 矩阵拼接器输入端口管理器
//

class CInputManager : public CObject
{
	friend class CGlobalManager;
	friend class CInitialManager;
	friend class COutputManager;
public:
	CInputManager();
	virtual ~CInputManager();

	virtual void DemoInit();
	virtual BOOL RealInit();

	// 端口初始化
	BOOL InitPorts();
	// 初始化各端口的信号输入
	BOOL InitSignals();

	// 信号刷新
	BOOL RefreshAllSignals();
	BOOL RefreshThisSignal(const int nIndex);

	// 复位
	virtual BOOL Reset(const int iKey, BOOL byID=TRUE);
	virtual BOOL ResetAll();

	//当前选中的信号源端口号
	static int m_sCurPort;

private:
	CArray <BASEPORT, BASEPORT&> m_arrPort;

public:
	//查找ID ，返回索引号
	int FindID(const int nID) const;
	//查找逻辑通信端口号，返回索引值
	int FindCmdPort(const int nCmdPort) const;
	int FindCmdPort(const int nCmdPort, const int nSubPort) const;

	//----------------------------- 数组动态管理 -------------------//
	//获取端口数
	virtual int GetCount() const;

	//清空所有端口
	virtual void RemoveAll();

	//删除指定端口
	virtual void RemoveAt(const int iKey, BOOL byID=TRUE); //byID=TRUE iKey表示一个端口ID，byID=FALSE iKey表示元素索引

	//添加端口
	virtual void Add(BASEPORT &basePort);

	//----------------------------- 端口ID管理 ----------------------//
	//获取端口ID，根据元素序号
	virtual int GetID(const int nIndex) const;
	virtual void SetID(const int nIndex, const int nID);

	//----------------------------- 端口名称管理 --------------------//
	//设置端口名称
	virtual void SetName(const int iKey, char* szName, BOOL byID=TRUE);
	//获取端口名称
	virtual char* GetName(const int iKey, BOOL byID=TRUE) const;
	virtual void Rename(const int nIndex, const CPoint ptDlgPos);

	//----------------------------- 端口类型管理 --------------------//
	//设置端口类型
	virtual void SetSignalType(const int iKey, emVideoType eVT, BOOL byID=TRUE);
	//获取端口类型
	virtual emVideoType GetSignalType(const int iKey, BOOL byID=TRUE) const;

	//----------------------------- 分辨率管理 -----------------------//
	//设置输入分辨率
	virtual void SetTiming(const int iKey, TIMING &st, BOOL byID=TRUE);
	//获取输入分辨率
	virtual TIMING GetTiming(const int iKey, BOOL byID=TRUE) const;

	//----------------------------- 通信端口号管理 -------------------//
	//设置通信端口号
	virtual void SetCmdPort(const int iKey, const int nPort, BOOL byID=TRUE);
	//获取通信端口号
	virtual int  GetCmdPort(const int iKey, BOOL byID=TRUE) const;

	//----------------------------- 逻辑子端口管理 -------------------//
	virtual void SetSubID(const int index, const int iSubID);
	virtual int  GetSubID(const int iKey, BOOL byID=TRUE) const;

	//----------------------------- EDID更新 -------------------//
	virtual BOOL UpdateEDID(const int iKey, BOOL byID=TRUE) const;

	//------------------------- 设置当前输入信号 ----------------//
	virtual void SetCurrencyResolution(const int iKey, const int iHactive, const int iVactive, const int iHz, BOOL byID=TRUE);
	virtual BOOL GetCurrencyResolution(const int iKey, int &iHactive, int &iVactive, int &iHz, BOOL byID=TRUE) const; 


	//------------ 逻辑信号与真实信号的转换关系处理 ------------//
	// 说明：
	//          一般地，一个物理端口实际上只能进一路物理信号，在端口管理器中也只占用一个逻辑信号ID，
	//     而特殊情况下，有些物理端口(如4k信号处理端口)的信号被当做两路或者更多路信号来处理，每一路都将占用一个逻辑信号ID，
	//     因此，在端口管理器中一个物理端口就可能占用两个或者更多的逻辑信号ID，在程序处理过程中需要做一些转换操作。
	//
	// 真实序号：物理信号的序号，排除所有子信号后的净序号
	// 逻辑序号：逻辑信号的序号，包括所有子信号在内的序号

	// 获取真实序号 （排除所有子信号之后的序号，由于子信号也占用了一个ID，故而真实序号不能与ID一一对应）
	virtual int GetRealIndex(const int nID) const;
	// 获取真实信号个数
	virtual int GetRealCount() const;
	// 将一个逻辑序号转换成真实序号
	virtual int LogicIndex2RealIndex(const int nLogicIndex) const;
	// 将一个真实序号转换成逻辑序号
	virtual int RealIndex2LogicIndex(const int nRealIndex) const;
};


extern CInputManager *pInputManager;