#pragma once

//******************************************
// CCustomizeManager
// 用户自定义管理器
//

class CCustomizeManager : public CObject
{
	friend class CGlobalManager;
	friend class CInitialManager;
public:
	CCustomizeManager();
	virtual ~CCustomizeManager();
};


