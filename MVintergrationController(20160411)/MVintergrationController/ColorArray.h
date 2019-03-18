#pragma once

class CColorArray
{
public:
	CColorArray(void);
	~CColorArray(void);

	void ResetColor();
	void ResetSecColor(int nSectionIndex);
	int& DotColor(int nDotIndex);
	int  DotColor(int nDotIndex) const;
	int (&SetSectionColor(int nSectionIndex, int nStartValue, int nEndValue))[32];
	void GetSectionColor(int (&nSectionColor)[32], int nSectionIndex);
	void GetStdSectionColor(int (&nStdSectionColor)[32], int nSectionIndex);
	void operator=(CColorArray colorobj);
private:
	int  m_secColor[32];
	int  m_allColor[256];
};
