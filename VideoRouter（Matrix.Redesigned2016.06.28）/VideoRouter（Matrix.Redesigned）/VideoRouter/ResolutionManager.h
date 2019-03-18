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
	//�������ļ��ж�
	virtual BOOL LoadFromFile(LPCTSTR lpszFilePath, BOOL bDecrypt=TRUE);
	//д�뵽�����ļ�
	virtual BOOL WriteToFile(LPCTSTR lpszFilePath, BOOL bEncrypt=TRUE);

	////��ȡ���ܹ��������ļ�
	//virtual BOOL DecryptLoadFile(LPCTSTR lpszFilePath);
	////����д������
	//virtual BOOL EncryptWriteFile(LPCTSTR lpszFilePath);

	//��ȡ�ֱ�������
	int GetResolutionCount() const;
	//�ֱ����Ƿ��Ѵ���
	int IsExist(const TIMING &timing) const;
	//��ӷֱ���
	int AddResolution(BOOL bInnerCall, TIMING &NewTiming);
	//�ָ���Ĭ�Ϸֱ���
	void AddDefault();
	//�༭�ֱ���
	void ModifyResolution(const int index, TIMING &Timing);
	//ɾ���ֱ���
	void RemoveResolution(const TIMING &DelTiming);
	//ɾ�������Զ���ֱ���
	void RemoveAllCustomDefined();
	//ɾ�����зֱ���
	void RemoveAllResolution();
	//��ȡĳ���ֱ��ʵ�����������
	int GetResolutionIndex(const TIMING &timing) const;
	//��ȡ�ֱ�������
	char *GetResolutionName(const TIMING &timing);
	char *GetResolutionName(const int index);
	//��ȡ�ֱ�����
	TIMING GetResolutionDetail(const int index) const;
	//��ȡĬ�Ϸֱ���
	static TIMING GetDefaultResolution(const int nHact, const int nVact, const int nHz);
};

