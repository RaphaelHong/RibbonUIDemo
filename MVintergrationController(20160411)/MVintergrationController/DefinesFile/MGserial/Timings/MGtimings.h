#pragma once

#define TOTAL_MG_TIMING 13

CString strMGtimings[TOTAL_MG_TIMING+1] = {
	_T("1024x768  @ 60Hz"),
	_T("1024x768  @ 120Hz"),
	_T("1280x720  @ 60Hz"),
	_T("1280x720  @ 120Hz"),
	_T("1280x800  @ 60Hz"),
	_T("1280x800  @ 120Hz"),
	_T("1280x1024 @ 60Hz"),
	_T("1366x768  @ 60Hz"),
	_T("1400x1050 @ 60Hz"),
	_T("1920x1080 @ 60Hz"),
	_T("1920x1200 @ 60Hz"),
	_T("2048x1080 @ 60Hz"),
	_T("2048x1200 @ 60Hz"),
	_T("END")
};


TIMING_STRUCT MG_Detailed_timing[TOTAL_MG_TIMING] = {
	/*all 30*//*  0*/                  { 6500,   1024,     320,     768,     38,       24,     136,      3,        6,     1024,    768,     0,      0,    0,        0,        60,     1024+320,     768+38},	/* ��(1024x768  @ 60Hz) */ 
	/*all 31*//*  1*/ /*RDC*/          {11551,   1024,     160,     768,     45,       48,     32,       3,        4,     1024,    768,     1,      0,    0,        0,        120,    1024+160,     768+45},	/* ��(1024x768  @ 120Hz) */ 
	/*all 32*//*  2*/                  { 7425,   1280,     370,     720,     30,       110,    40,       5,        5,     1280,    720,     1,      1,    0,        0,        60,     1280+370,     720+30},	/* ��(1280x720  @ 60Hz) */ 
	/*all 33*//*  3*/                  {14850,   1280,     370,     720,     30,       16,     64,       2,        6,     1280,    720,     1,      1,    0,        0,        120,    1280+370,     720+30},	/* ��(1280x720  @ 120Hz) */ 
	/*all 35*//*  5*/                  { 8376,   1280,     400,     800,     31,       72,     128,      3,        6,     1280,    800,     1,      0,    0,        0,        60,     1280+400,     800+31},	/* ��(1280x800  @ 60Hz) */ 
	/*all 36*//*  6*/                  {14636,   1280,     160,     800,     47,       48,     32,       3,        6,     1280,    800,     1,      0,    0,        0,        120,    1280+160,     800+47},	/* ��(1280x800  @ 120Hz) */ 
	/*all 38*//*  8*/                  {10796,   1280,     408,     1024,    42,       48,     112,      1,        3,     1280,    1024,    1,      1,    0,        0,        60,     1280+408,     1024+42},	/* ��(1280x1024 @ 60Hz) */ 
	/*all 40*//*  10*/                 { 8548,   1366,     426,     768,     27,       64,     112,      3,        6,     1366,    768,     1,      1,    0,        0,        60,     1366+426,     768+27},	/* ��(1366x768  @ 60Hz) */ 
	/*all a40*//* a10*/                {12179,   1400,     464,     1050,    39,       88,     144,      3,        4,     1400,    1050,    1,      0,    0,        0,        60,     1400+464,     1050+39},	/* ��(1400x1050 @ 60Hz) */ 
	/*all 44*//* 14*/                  {14850,   1920,     280,     1080,    45,       88,     44,       4,        5,     1920,    1080,    1,      1,    0,        0,        60,     1920+280,     1080+45},	/* -��(1920x1080 @ 60Hz) */ 
	/*all 45*//* 15*/                  {15413,   1920,     160,     1200,    35,       48,     32,       3,        6,     1920,    1200,    1,      0,    0,        0,        60,     1920+160,     1200+35},	/* ��(1920x1200 @ 60Hz) */ 
	/*all 46*//* 16*//*wide*/          {14652,   2048,     160,     1080,    26,       48,     32,       3,        10,    2048,    1080,    1,      1,    0,        0,        60,     2048+160,     1080+26},	/* ��(2048x1080 @ 60Hz) */ 
	/*all 47*/                         {16242,   2048,     160,     1200,    26,       48,     32,       3,        5,     2048,    1200,    1,      0,    0,        0,        60,     2048+160,     1200+26},   /* ��(2048x1200 @ 60Hz) */ 
};//<0,WINT,3,60,1680,1280,72,128,1,831,800,3,6,0,8367>