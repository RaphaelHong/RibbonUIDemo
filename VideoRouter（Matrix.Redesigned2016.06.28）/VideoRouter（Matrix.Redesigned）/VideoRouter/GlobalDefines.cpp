#include "stdafx.h"
#include "GlobalDefines.h"

/****************** 系统色彩 ********************/
COLORREF ListColorBackground = RGB(255, 255, 255);   //列表框背景颜色
COLORREF ListColorItemSelected = RGB(124, 124, 114);    //列表项被选中背景颜色
COLORREF ListColorActionFailed = RGB(255, 00, 00);     //列表项执行失败背景颜色
COLORREF ListColorActionSucceed = RGB(100, 100, 255);      //列表项执行成功背景颜色
COLORREF ListColorText = RGB(0, 0, 0);         //列表框字体颜色
COLORREF DialogColorBackground  = RGB(83, 83, 83);  //对话框背景颜色
//----------  [3/10/2016 Administrator] ---------/

BOOL tagTiming::IsValid()
{
	if ((m_HActive <= 0) || (m_VActive <=0) || (m_Hz <=0) )
		return FALSE;

	if (m_HActive >= m_HTotal || m_VActive >= m_VTotal)
		return FALSE;

	return TRUE;
}

BOOL tagTiming::operator==(const tagTiming &Tobj) const
{
	if (   m_HActive  != Tobj.m_HActive
		//|| m_HTotal   != Tobj.m_HTotal
		//|| m_HFPorch  != Tobj.m_HFPorch
		//|| m_HSyncWid != Tobj.m_HSyncWid
		//|| m_HPol     != Tobj.m_HPol
		|| m_VActive  != Tobj.m_VActive
		//|| m_VTotal   != Tobj.m_VTotal
		//|| m_VFPorch  != Tobj.m_VFPorch
		//|| m_VSyncWid != Tobj.m_VSyncWid
		//|| m_VPol     != Tobj.m_VPol
		|| m_Hz != Tobj.m_Hz)
	{
		return FALSE;
	}

	return TRUE;
}

//预定义分辨率
TIMING OriginalResolutions[_MAX_ORG_RESOLUTION] = {
	{ 4000,   800,      256,     600,     28,       40,	    128,      1,        4,     800,     600,     1,      1,    0,        0,	       60,     800+256,      600+28,   DEFAULT_RESOLUTION},		//800*600 @ 60HZ
	{ 6500,   1024,     320,     768,     38,       24,     136,      3,        6,     1024,    768,     0,      0,    0,        0,        60,     1024+320,     768+38,   DEFAULT_RESOLUTION},	/* 单(1024x768  @ 60Hz) */ 
	{ 7425,   1280,     370,     720,     30,       110,    40,       5,        5,     1280,    720,     1,      1,    0,        0,        60,     1280+370,     720+30,   DEFAULT_RESOLUTION},	/* 单(1280x720  @ 60Hz) */ 
	{ 8376,   1280,     400,     800,     31,       72,     128,      3,        6,     1280,    800,     1,      0,    0,        0,        60,     1280+400,     800+31,   DEFAULT_RESOLUTION},	/* 单(1280x800  @ 60Hz) */ 
	{10796,   1280,     408,     1024,    42,       48,     112,      1,        3,     1280,    1024,    1,      1,    0,        0,        60,     1280+408,     1024+42,  DEFAULT_RESOLUTION},	/* 单(1280x1024 @ 60Hz) */ 
	{ 8548,   1366,     426,     768,     27,       64,     112,      3,        6,     1366,    768,     1,      1,    0,        0,        60,     1366+426,     768+27,   DEFAULT_RESOLUTION},	/* 单(1366x768  @ 60Hz) */ 
	{10670,   1440,     464,     900,     34,       80,     154,      3,        6,     1440,    900,     1,      0,    0,        0,        60,     1440+464,     900+34,   DEFAULT_RESOLUTION}, /* 单(1440x900 @ 60Hz) */
	{12179,   1400,     464,     1050,    39,       88,     144,      3,        4,     1400,    1050,    1,      0,    0,        0,        60,     1400+464,     1050+39,  DEFAULT_RESOLUTION},	/* 单(1400x1050 @ 60Hz) */ 
	{11900,   1680,     160,     1050,    30,       48,     32,	      3,        6,     1680,    1050,    1,	     0,	   0,        0,        60,     1680+160,     1050+30,  DEFAULT_RESOLUTION}, /* 单(1680*1050 @ 60HZ) */
	{14850,   1920,     280,     1080,    45,       88,     44,       4,        5,     1920,    1080,    1,      1,    0,        0,        60,     1920+280,     1080+45,  DEFAULT_RESOLUTION},	/* 单(1920x1080 @ 60Hz) */ 
	{15413,   1920,     160,     1200,    35,       48,     32,       3,        6,     1920,    1200,    1,      0,    0,        0,        60,     1920+160,     1200+35,  DEFAULT_RESOLUTION},	/* 单(1920x1200 @ 60Hz) */ 
	//{11551,   1024,     160,     768,     45,       48,     32,       3,        4,     1024,    768,     1,      0,    0,        0,        120,    1024+160,     768+45,   DEFAULT_RESOLUTION},	/* 单(1024x768  @ 120Hz) */ 
	//{14850,   1280,     370,     720,     30,       16,     64,       2,        6,     1280,    720,     1,      1,    0,        0,        120,    1280+370,     720+30,   DEFAULT_RESOLUTION},	/* 单(1280x720  @ 120Hz) */ 
	//{14636,   1280,     160,     800,     47,       48,     32,       3,        6,     1280,    800,     1,      0,    0,        0,        120,    1280+160,     800+47,   DEFAULT_RESOLUTION},	/* 单(1280x800  @ 120Hz) */ 
	//{14652,   2048,     160,     1080,    26,       48,     32,       3,        10,    2048,    1080,    1,      1,    0,        0,        60,     2048+160,     1080+26,  DEFAULT_RESOLUTION}, /* 单(2048x1080 @ 60Hz) */ 
	//{16242,   2048,     160,     1200,    26,       48,     32,       3,        5,     2048,    1200,    1,      0,    0,        0,        60,     2048+160,     1200+26,  DEFAULT_RESOLUTION}, /* 单(2048x1200 @ 60Hz) */ 
};

CStringW GetVideoName(emVideoType eVT)
{
	CStringW str;
	if (eVT == vtCVBS)
		str.Format(L"CVBS");
	else if (eVT == vtYpbpr)
		str.Format(L"YPbPr");
	else if (eVT == vtVGA)
		str.Format(L"VGA");
	else if (eVT == vtSDI)
		str.Format(L"SDI");
	else if (eVT == vtHDMI)
		str.Format(L"HDMI");
	else if (eVT == vtDVI)
		str.Format(L"DVI");
	else if (eVT == vtDP)
		str.Format(L"DP");
	else if (eVT == vt4K_HDMI)
		str.Format(L"4K_HDMI");
	else if (eVT == vtHDBASET)
		str.Format(L"HDBASET");
	else if (eVT == vtDLDVI)
		str.Format(L"DLDVI");
	else if (eVT == vtPAL_D)
		str.Format(L"PAL_D");
	else if (eVT == vtNTSC_D)
		str.Format(L"NTSC_D");
	else if (eVT == vtPAL_W)
		str.Format(L"PAL_W");
	else if (eVT == vtNTSC_W)
		str.Format(L"NTSC_W");
	else
		str.Format(L"!");

	return str;
}

COLORREF GetVideoColor(emVideoType eVT)
{
	COLORREF clr;

	if (eVT == vtCVBS)
		clr = 0x395BA9;
	else if (eVT == vtYpbpr)
		clr = 0x9E922E;
	else if (eVT == vtVGA)
		clr = 0x7F4125;
	else if (eVT == vtSDI)
		clr = 0x8F296E;
	else if (eVT == vtHDMI)
		clr = 0xFE3A8C;
	else if (eVT == vtDVI)
		clr = 0xB25252;
	else if (eVT == vtDP)
		clr = 0xE52A9C;
	else if (eVT == vt4K_HDMI)
		clr = 0xe0739B;
	else if (eVT == vtHDBASET)
		clr = 0x155c32;
	else if (eVT == vtDLDVI)
		clr = 0x8B0000;
	else if (eVT == vtPAL_D)
		clr = 0x266C5D;
	else if (eVT == vtNTSC_D)
		clr = 0xAb22e5;
	else if (eVT == vtPAL_W)
		clr = 0xe5567c;
	else if (eVT == vtNTSC_W)
		clr = 0xaa5839;
	else
		clr = 0x242424;

	return clr;
}


CARDINFO CardInfo[_MAX_CARD_DEF] = {
	{ eMM10X_SYS_CTRL,  eCardTypeCtrl,   0 },
	{ eMM10X_4IN_DVIM,  eCardTypeInput,  4 },
	{ eMM10X_2IN_DPDVI, eCardTypeInput,  2 },
	{ eMM10X_4OUT_DVII, eCardTypeOutput, 4 },
};

emCardType GetCardType(const UINT uCardID)
{
	for (int i=0; i<_MAX_CARD_DEF; i++)
	{
		if (CardInfo[i].m_eCardID == uCardID)
		{
			return CardInfo[i].m_eCardType;
		}
	}

	return eCardTypeUndef;
}


BOOL BitmapToImage(CBitmap *pBitmap, Image *&pImg)
{
	HBITMAP hBmp = (HBITMAP)pBitmap->GetSafeHandle();

	if (hBmp == NULL)
		return FALSE;

	pImg = new Bitmap(hBmp, NULL);

	return TRUE;
}

BOOL ImageFromIDResource(UINT nID, LPCTSTR lpType, Image *&pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID), lpType); // type
	if (!hRsrc)
		return FALSE;

	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;

	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem,lpRsrc,len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);

	// load from stream
	pImg = Gdiplus::Image::FromStream(pstm);

	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
	GlobalFree(m_hMem);

	return TRUE;
}