#pragma once

class CBaseBoard
{
public:
	CBaseBoard(void);
	~CBaseBoard(void);
private:
	int m_nBoardFuncID;	
	int m_nBoardPort;
	int m_nBoardID;
	emProductType m_ProductType;
	CString m_strBoardName;

public:
	int GetBoardFuncID() const;
	void SetBoardFuncID(int uFuncID);
	int GetFunCodeByte(int nByteOder) const;

	int GetBoardPort() const;
	void SetBoardPort(int uPort);

	int GetBoardID() const;
	void SetBoardID(int uID);

	CString GetBoardName() const;	
	void SetExtendedName(int nExt/*第几块同类型板卡*/);  //扩展板卡名，相同类型板卡名后边加数字后缀以作为甄别

	void SetProductType();
	emProductType GetProductType() const;

	CString *GetTimingName(const emProductType etype) const;
	TIMING_STRUCT *GetDetailTiming(const emProductType etype) const;
	int GetTotalTimings(const emProductType etype) const;
};
