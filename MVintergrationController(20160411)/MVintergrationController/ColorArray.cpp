#include "StdAfx.h"
#include "ColorArray.h"

CColorArray::CColorArray(void)
{
}

CColorArray::~CColorArray(void)
{
}

void CColorArray::operator =(CColorArray colorobj)
{
	for (int i=0; i<256; i++)
	{
		m_allColor[i] = colorobj.m_allColor[i];
	}

	for (int i=0; i<32; i++)
	{
		m_secColor[i] = colorobj.m_secColor[i];
	}
}

void CColorArray::ResetColor()
{
	for (int i=0; i<256; i++)
	{
		m_allColor[i] = i;
	}
}

void CColorArray::ResetSecColor(int nSectionIndex)
{
	ASSERT(nSectionIndex <=7 && nSectionIndex >=0);
	for (int i=0; i<32; i++)
	{
		m_allColor[nSectionIndex*32+i] = nSectionIndex*32+i;
	}
}

int & CColorArray::DotColor(int nDotIndex)
{
	if(nDotIndex < 0) return m_allColor[0];
	if(nDotIndex > 255) return m_allColor[255];
	return m_allColor[nDotIndex];
}

int CColorArray::DotColor(int nDotIndex) const
{
	if(nDotIndex < 0) return m_allColor[0];
	if(nDotIndex > 255) return m_allColor[255];
	return m_allColor[nDotIndex];
}

int (&CColorArray::SetSectionColor(int nSectionIndex, int nStartValue, int nEndValue))[32]
{
	ASSERT(nSectionIndex < 8 && nSectionIndex >=0);

	for (int i=0; i<32; i++)
	{
		m_secColor[i] = Round((nEndValue - nStartValue)*0.03125*i) + nStartValue;
	}

	return m_secColor;
}

void CColorArray::GetSectionColor(int (&nSectionColor)[32], int nSectionIndex)
{
	ASSERT(nSectionIndex < 8 && nSectionIndex >=0);

	for (int i=0; i<32; i++)
	{
		nSectionColor[i] = m_allColor[nSectionIndex*32+i];
	}
}


void CColorArray::GetStdSectionColor(int (&nStdSectionColor)[32], int nSectionIndex)
{
	ASSERT(nSectionIndex < 8 && nSectionIndex >=0);
	for (int i=0; i<32; i++)
	{
		nStdSectionColor[i] = nSectionIndex*32+i;
	}
}