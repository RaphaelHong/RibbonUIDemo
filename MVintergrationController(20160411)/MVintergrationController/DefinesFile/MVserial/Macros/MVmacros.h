#pragma once

#include "DefinesFile/MVserial/Timings/MVtimings.h"
/****************** �豸���Ŷ���,����V2015.5.26���飬DDC֮���ȡʱ��ʹ��Detailed Timing Block#3 �����ֲ�ͬ�� �豸�Լ��豸����ʵ�ֵĹ��� ****************/
// ** �豸����Ψһ�� 
// ** ���豸��EDID����
typedef enum emMviewProductID
{
	INVALID_PRODUCT_ID         =    0x0000,  // ��Ч���豸����
	DEV_PRD_MV102S_HDVA        =    0x1912,
	DEV_PRD_MV102S_HDVB        =    0x1913,

	DEV_PRD_MV103_HDMA         =    0x1900,
	DEV_PRD_MV103_HDMB         =    0x1901,

	DEV_PRD_MV102_HDMA         =    0x1910,
	DEV_PRD_MV102_HDMB         =    0x1911,

	DEV_PRD_MV103S_HDVE        =    0x1916,  //normal
	DEV_PRD_MV103S_HDVEP       =    0x1917,   //pro
	DEV_PRD_MV103S_HDVEH       =    0x1918,   //120Hz

	DEV_PRD_MV103S_HDP         =    0x1919,  //normal
	DEV_PRD_MV103S_HDP_P       =    0x191a,   //pro
	DEV_PRD_MV103S_HDP_H       =    0x191b,   //120Hz
};

/****************** �豸ID�Ŷ��� ****************/
//** �豸ID�Ų���Ψһ
typedef enum emMviewDevID
{
	INVALID_MV_ID              =    0x00, //��Ч��ID��
	//-------- MV30X ϵ�� ---------------
	DEV_ID_MV102S_HDVI         =    0x1101,

	DEV_ID_MV102_HDMI          =    0x1102,

	DEV_ID_MV103_HDMI          =    0x1902,

	DEV_ID_MV103S_HDVE         =    0x1a01,
	DEV_ID_MV103S_HDVE_PRO     =    0x1a02,
	DEV_ID_MV103S_HDVE_120Hz   =    0x1a03,

	DEV_ID_MV103S_HDP          =    0x1b01,
	DEV_ID_MV103S_HDP_PRO      =    0x1b02,
	DEV_ID_MV103S_HDP_120Hz    =    0x1b03
};

/****************** �豸��̬�����붨�� ****************/
typedef enum emMviewFunCode
{
	INVALIDE_MV_FUNC           =   0x00000000,  //��Ч�Ĺ�����

	DEV_FUNC_MV102S_HDVI       =   0x10fe0211,
	DEV_FUNC_MV102_HDMI        =   0x12fe0212,

	DEV_FUNC_MV103S_HDVE       =   0x11fe0311,
	DEV_FUNC_MV103S_HDVE_PRO   =   0x11fe0312,
	DEV_FUNC_MV103S_HDVE_120Hz =   0x11fe0313,

	DEV_FUNC_MV103S_HDP        =   0x13fe0311,
	DEV_FUNC_MV103S_HDP_PRO    =   0x13fe0312,
	DEV_FUNC_MV103S_HDP_120Hz  =   0x13fe0313,

	DEV_FUNC_MV103_HDMI        =   0x12fe0322
};

#define TOTAL_MVIEW_TYPES  12

DEVICE_STRUCT stMviewDevTypes[TOTAL_MVIEW_TYPES] = {
        //EDID_Name               Dev_Name              EDID_PRD_ID            DEV_ID                   Dev_Function_ID                Timing_Name                        Timing_Details
	{ _T("MV102S_HDVA"),  _T("MV102S_HDVI"),       DEV_PRD_MV102S_HDVA ,  DEV_ID_MV102S_HDVI      ,  DEV_FUNC_MV102S_HDVI      , m_MV102S_HDVI_strTimings,       MV102S_HDVI_detailed_timings       },                                     
	{ _T("MV102S_HDVB"),  _T("MV102S_HDVI"),       DEV_PRD_MV102S_HDVB ,  DEV_ID_MV102S_HDVI      ,  DEV_FUNC_MV102S_HDVI      , m_MV102S_HDVI_strTimings,       MV102S_HDVI_detailed_timings       },                                           
                                                                                                                                                                                                    
	{ _T("MV103_HDMA"),   _T("MV103_HDMI"),        DEV_PRD_MV103_HDMA  ,  DEV_ID_MV103_HDMI       ,  DEV_FUNC_MV103_HDMI       , m_MV103_HDMI_strTimings,        MV103_HDMI_detailed_timings        },                                           
	{ _T("MV103_HDMB"),   _T("MV103_HDMI"),        DEV_PRD_MV103_HDMB  ,  DEV_ID_MV103_HDMI       ,  DEV_FUNC_MV103_HDMI       , m_MV103_HDMI_strTimings,        MV103_HDMI_detailed_timings        },                                     
                                                                                                                                                                                                    
	{ _T("MV102_HDMA"),   _T("MV102_HDMI"),        DEV_PRD_MV102_HDMA  ,  DEV_ID_MV102_HDMI       ,  DEV_FUNC_MV102_HDMI       , m_MV102_HDMI_strTimings,        MV102_HDMI_detailed_timings        },                                               
	{ _T("MV102_HDMB"),   _T("MV102_HDMI"),        DEV_PRD_MV102_HDMB  ,  DEV_ID_MV102_HDMI       ,  DEV_FUNC_MV102_HDMI       , m_MV102_HDMI_strTimings,        MV102_HDMI_detailed_timings        },                                               
                                                                                                                                                                                                    
	{ _T("MV103S_HDVE"),  _T("MV103S_HDVE"),       DEV_PRD_MV103S_HDVE ,  DEV_ID_MV103S_HDVE      ,  DEV_FUNC_MV103S_HDVE      , m_MV103S_HDVE_strTimings,       MV103S_HDVE_detailed_timings       },                                              
	{ _T("MV103S_HDVEP"), _T("MV103S_HDVE_Pro"),   DEV_PRD_MV103S_HDVEP,  DEV_ID_MV103S_HDVE_PRO  ,  DEV_FUNC_MV103S_HDVE_PRO  , m_MV103S_HDVE_PRO_strTimings,   MV103S_HDVE_PRO_detailed_timings   },                                              
	{ _T("MV103S_HDVEH"), _T("MV103S_HDVE_120Hz"), DEV_PRD_MV103S_HDVEH,  DEV_ID_MV103S_HDVE_120Hz,  DEV_FUNC_MV103S_HDVE_120Hz, m_MV103S_HDVE_120Hz_strTimings, MV103S_HDVE_120Hz_detailed_timings },                                              
                                                                                                                                                                                                    
	{ _T("MV103S_HDP"),   _T("MV103S_HDP"),        DEV_PRD_MV103S_HDP  ,  DEV_ID_MV103S_HDP       ,  DEV_FUNC_MV103S_HDP       , m_MV103S_HDVE_strTimings,       MV103S_HDVE_detailed_timings       },       
	{ _T("MV103S_HDP_P"), _T("MV103S_HDP_Pro"),    DEV_PRD_MV103S_HDP_P,  DEV_ID_MV103S_HDP_PRO   ,  DEV_FUNC_MV103S_HDP_PRO   , m_MV103S_HDVE_PRO_strTimings,   MV103S_HDVE_PRO_detailed_timings   },   
	{ _T("MV103S_HDP_H"), _T("MV103S_HDP_120Hz"),  DEV_PRD_MV103S_HDP_H,  DEV_ID_MV103S_HDP_120Hz ,  DEV_FUNC_MV103S_HDP_120Hz , m_MV103S_HDVE_120Hz_strTimings, MV103S_HDVE_120Hz_detailed_timings },   	
};