#pragma once
#include "InputManager.h"

#define _OFF 0

// ��ȡ��������˿ڵ��ź�Դ״̬
typedef enum SourceState
{
	eSourceStateUndef,           // δ֪״̬
	eSourceStateSameCurrent,     // �����������ָ�����ź�ԴID����
	eSourceStateSameNoCurrent,   // �������������ͬ���ź�ԴID���룬��ID��ָ�����ź�ԴID��һ��
	eSourceStateDiffCurrent,     // ����������ź�ԴID���벻һ�£���������Щ�����ָ�����ź�ԴID����
	eSourceStateDiffNoCurrent,   // ����������ź�ԴID���벻һ�£���û�������ָ�����ź�ԴID����
	eSourceStateAllOFF,          // ���������û���ź�����

} SState;

//******************************************
// COutputManager
// ����ƴ��������˿ڹ�����
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
	//------------------------------ ��Դ�ź�ID���� -------------------//

	// ������Դ�ź�ID
	void SetFeederID(const int iKey, const int iFeederID, BOOL bInvs, BOOL byID=TRUE);

	// �������������ͬһ���ź�Դ����
	void SetSameFeederID(const int &iFeederID, BOOL bInvs);

	// ��ȡ��Դ�ź�ID
	int GetFeederID(const int iKey, BOOL byID=TRUE) const;

	////////////////////////////////////////////////
	// P2PSwitch ��Ե��л� �����ϵĵ����л�Ҳ���ô˷���
	// ������
	// iKey: ����byIDΪTRUE��iKey��ʾĳ����˿ڵ�ID�ţ�����Ϊ�ö˿ڵ������š�
	// iFeederID: ��Դ�źŵ�ID
	// bInvs: TRUE:  ��ѡ������ڵ���Դ�ź�ID��iFeederID��ȣ���رոö˿ڣ���֮�����øö˿���ԴIDΪiFeederID��
	//        FALSE: ֱͨ��ֱ�ӽ�������ڵ���ԴID��ΪiFeederID��
	// byID: ������ʾ���� iKey ������

	void P2PSwitch(const int iKey, const int &iFeederID, BOOL bInvs, BOOL byID=TRUE);

	////////////////////////////////////////////////
	// AllSwitch һ�������л� ��������ȫѡʱҲ���ô˷���
	// ������
	// iFeederID: ��Դ�źŵ�ID
	// bInvs: TRUE:  ��ѡ������ڵ���Դ�ź�ID��iFeederID��ȣ���رոö˿ڣ���֮�����øö˿���ԴIDΪiFeederID��
    //        FALSE: ֱͨ��ֱ�ӽ�������ڵ���ԴID��ΪiFeederID��

	void AllSwitch(const int &iFeederID, BOOL bInvs);
	
	////////////////////////////////////////////////
	// MultiSwitch һ�Զ��л�
	// ������
	// iFeederID: ��Դ�źŵ�ID
	// pOutputIDList: ��ѡ�������ID�б�
	// iCount: ��ѡ���������

	void MultiSwitch(const int &iFeederID, int *pOutputIDList, int iCount);

	////////////////////////////////////////////////
	// MappingSwitch ӳ���л�: 
	// ������
	// iSize: ӳ�����
	// pPair: ӳ���

	void MappingSwitch(const int &iSize, const INT_PAIR *pPair);
};


extern COutputManager *pOutputManager;