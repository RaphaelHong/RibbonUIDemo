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

	emWorkMode m_eWorkMode; //����ģʽ

	DEVICEINFO m_DevInfo;   //�豸��Ϣ

public:
	// ��ȡִ�г����·��
	CString GetAppPath();

	// ��ʼ��ȫ�ֿ��Ʊ���
	void Init();
	// �豸������Ϣ
	void InitDeviceStructure(pDEVICEINFO pInfo);
	pDEVICEINFO GetDeviceStructure() const;

	// ��ȡ��ǰ�豸�Ĺ���ģʽ ��eWordModeSplit:ƴ��ģʽ eWorkModeQuick:����ģʽ
	emWorkMode GetCurrencyWorkMode() const;
	emWorkMode &GetCurrencyWorkMode();
	// ��ȡ��ǰAPP����
	DWORD GetCurrencyLangID() const;
};

// ����һ��ȫ�����ù������
extern CGlobalManager *pGlobalManager;