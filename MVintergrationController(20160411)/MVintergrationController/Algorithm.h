#pragma once

double Interpol_Lagr_Global(double x[],double y[],double xx,int n);//拉格朗日整体

double Interpol_Lagr_3Point(double x[],double y[],double xx,int n);//拉格朗日局部

double Interpol_2Pow_3Point(double x[],double y[],double xx,int n);//二点三次插值

double Interpol_Complicated(double x[],double y[],double xx,int n);//复杂多项式插值

double Interpol_PointAdjust(double x[],double y[],double xx, int nPoint,int n);//可调点数插值

void Check_Warp_Table_Error(CDoublePoint *CheckPoint,unsigned int In_Hactive,unsigned int In_Vactive);

double Calc_Warp_Table(unsigned int max_times,    unsigned int ReferPoint_Num,CDoublePoint *ReferPoint_In,CDoublePoint *ReferPoint_Out,
					   unsigned int *min_err_time,unsigned int CalcPoint_Num, CDoublePoint *CalcPoint_In, CDoublePoint *CalcPoint_Out   );

double Calc_Keystone_InToOut_Table(unsigned int row_y_num,unsigned int column_x_num,  CDoublePoint *ReferPoint_In,CDoublePoint *ReferPoint_Out,
								   unsigned int CalcPoint_Num, CDoublePoint *CalcPoint_In, CDoublePoint *CalcPoint_Out   );

//void CalcGridPoint(CString strGridsFolder);
void Calc_Point_Table_dir(unsigned int cx,unsigned int cy,unsigned int In_Hactive,unsigned int In_Vactive,
						  CDoublePoint *CalcPoint_In, CDoublePoint *CalcPoint_Out   );

bool CDoublePoint_ComparisonEqual(CDoublePoint p1,CDoublePoint p2);

//*******************************************
//  Func name: ConvertGridPointData
//      usage: 将多点校正x*y的数据转换成65*65的数据
//      input: iOrig_cx:源数据网格列数；
//      input: iOrig_cy:源数据网格行数；
//      input: pOrigData:源数据
//     output: pDestData:目标数据
bool ConvertGridPointData(int iOrig_cx, int iOrig_cy, CDoublePoint *pOrigData, CDoublePoint *pDestData);

void FindMaxAndMinMethod4(int *pArr, int nStart, int nEnd, int &nMax, int &nMin);