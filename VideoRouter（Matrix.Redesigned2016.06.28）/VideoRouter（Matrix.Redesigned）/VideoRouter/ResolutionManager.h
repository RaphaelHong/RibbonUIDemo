#pragma once
#include "BaseFileManager.h"

class CResolutionManager : public CBaseFileManager
{
public:
	CResolutionManager(void);
	~CResolutionManager(void);

private:
	char *m_pszResolution;
	CArray <TIMING, TIMING&> m_arrRes;

public:
	//从配置文件中读
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt=TRUE);
	//写入到配置文件
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt=TRUE);

	////读取加密过的配置文件
	//virtual BOOL DecryptLoadFile(LPCTSTR lpszFilePath);
	////加密写入配置
	//virtual BOOL EncryptWriteFile(LPCTSTR lpszFilePath);

	//获取分辨率总数
	int GetResolutionCount() const;
	//分辨率是否已存在
	int IsExist(const TIMING &timing) const;
	//添加分辨率
	int AddResolution(BOOL bInnerCall, TIMING &NewTiming);
	//恢复到默认分辨率
	void AddDefault();
	//编辑分辨率
	void ModifyResolution(const int index, TIMING &Timing);
	//删除分辨率
	void RemoveResolution(const TIMING &DelTiming);
	//删除所有自定义分辨率
	void RemoveAllCustomDefined();
	//删除所有分辨率
	void RemoveAllResolution();
	//获取某个分辨率的数组索引号
	int GetResolutionIndex(const TIMING &timing) const;
	//获取分辨率名称
	char *GetResolutionName(const TIMING &timing);
	char *GetResolutionName(const int index);
	//获取分辨详情
	TIMING GetResolutionDetail(const int index) const;
	//获取默认分辨率
	static TIMING GetDefaultResolution(const int nHact, const int nVact, const int nHz);
};

