#pragma once
#include "GridSetting.h"

int Round(double value);

void ExpandAllGridItems(CXTPPropertyGridItems *pItems, BOOL bCollapseExpand);

double Interpol_Lagr_Global(double x[],double y[],double xx,int n);//������������

double Interpol_Lagr_3Point(double x[],double y[],double xx,int n);//�������վֲ�

double Interpol_2Pow_3Point(double x[],double y[],double xx,int n);//�������β�ֵ

double Interpol_Complicated(double x[],double y[],double xx,int n);//���Ӷ���ʽ��ֵ

double Interpol_PointAdjust(double x[],double y[],double xx, int nPoint,int n);//�ɵ�������ֵ

void Check_Warp_Table_Error(CDoublePoint *CheckPoint,unsigned int In_Hactive,unsigned int In_Vactive);

double Calc_Warp_Table(unsigned int max_times, 
					   unsigned int ReferPoint_Num,
					   CDoublePoint *ReferPoint_In,
					   CDoublePoint *ReferPoint_Out,
					   unsigned int *min_err_time,
					   unsigned int CalcPoint_Num,
					   CDoublePoint *CalcPoint_In, 
					   CDoublePoint *CalcPoint_Out  );

double Calc_Keystone_InToOut_Table(unsigned int row_y_num,         /*����*/
								   unsigned int column_x_num,      /*����*/
								   CDoublePoint *ReferPoint_In,    /*�ο�����*/
								   CDoublePoint *ReferPoint_Out,   /*�ο����keystone*/
								   unsigned int CalcPoint_Num,     /*�ܵ���*/
								   CDoublePoint *CalcPoint_In,     /*�ο����Curve*/
								   CDoublePoint *CalcPoint_Out  ); /*�������*/

void Calc_Point_Table_dir(unsigned int cx,                     /*����*/
						  unsigned int cy,                     /*����*/
						  unsigned int In_Hactive,             /*ˮƽ���ص�*/
						  unsigned int In_Vactive,             /*��ֱ���ص�*/
						  CDoublePoint *CalcPoint_In,          /*�������*/
						  CDoublePoint *CalcPoint_Out   );     /*�������*/

bool CDoublePoint_ComparisonEqual(CDoublePoint p1,CDoublePoint p2);

//*******************************************
//  Func name: ConvertGridPointData
//      usage: �����У��x*y������ת����65*65������
//      input: iOrig_cx:Դ��������������
//      input: iOrig_cy:Դ��������������
//      input: pOrigData:Դ����
//     output: pDestData:Ŀ������
bool ConvertGridPointData(int iOrig_cx, int iOrig_cy, CDoublePoint *pOrigData, CDoublePoint *pDestData);