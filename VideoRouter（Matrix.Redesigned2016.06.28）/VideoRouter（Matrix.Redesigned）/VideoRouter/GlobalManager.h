#pragma once

//******************************************
// CGlobalManager
// ȫ�ֵ��ȹ�����
//

class CGlobalManager : public CObject
{
	friend class CInitialManager;

public:
	CGlobalManager();
	virtual ~CGlobalManager();

private:
	DWORD m_dwLangID;       //����ID
	DEVICEINFO m_DevInfo;   //�豸��Ϣ

public:
	// ��ȡִ�г����·��
	CString GetAppPath();

	// ��ʼ��ȫ�ֿ��Ʊ���
	void Init();
	
	// �豸������Ϣ
	void InitDeviceStructure(pDEVICEINFO pInfo);
	pDEVICEINFO GetDeviceStructure() const;
};

// ����һ��ȫ�����ù������
extern CGlobalManager *pGlobalManager;