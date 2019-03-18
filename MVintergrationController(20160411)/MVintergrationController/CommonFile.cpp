#include "stdafx.h"
#include "CommonFile.h"

int Round(double value)
{
	return static_cast<int>(floor(value+0.5));
}

void ExpandAllGridItems(CXTPPropertyGridItems *pItems, BOOL bCollapseExpand)
{
	for (int i = 0; i < pItems->GetCount(); i++)
	{
		CXTPPropertyGridItem* pItem = pItems->GetAt(i);
		if (pItem->HasChilds())
		{
			if (bCollapseExpand) pItem->Expand();
			else pItem->Collapse();
			ExpandAllGridItems(pItem->GetChilds(), bCollapseExpand);
		}
	}
}

double Interpol_Lagr_Global(double x[],double y[],double xx,int n)//拉格朗日整体
{
	double yy = 0., item;
	int i, j;
	for (i = 0; i < n; i++) {
		item = 1.;
		for (j = 0; j < n; j++) {
			if (i != j) {
				item *= (xx - x[j]) / (x[i] - x[j]);
			}
		}
		yy += item * y[i];
	}
	return yy;
}

double Interpol_Lagr_3Point(double x[],double y[],double xx,int n)//拉格朗日局部
{
	int i = 0, j, k, m;
	double yy, item;
	while (1) {
		if (xx < (x[i + 1] + x[i + 2]) / 2.) {
			m = i + 2;
			yy = 0.;
			for (j = i; j <= m; j++) {
				item = 1.;
				for (k = i; k <= m; k++) {
					if (j != k) {
						item *= (xx - x[k]) / (x[j] - x[k]);
					}
				}
				yy += item * y[j];
			}
			return yy;
		}
		else if (xx > (x[n - 3] + x[n - 2]) / 2.) {
			i = n - 3;
			m = i + 2;
			yy = 0.;
			for (j = i; j <= m; j++) {
				item = 1.;
				for (k = i; k <= m; k++) {
					if (j != k) {
						item *= (xx - x[k]) / (x[j] - x[k]);
					}
				}
				yy += item * y[j];
			}
			return yy;
		}
		i++;
	}
}

double Interpol_2Pow_3Point(double x[],double y[],double xx,int n)//二点三次插值
{
	int n1, i;
	double x1, x2, x3, a1, a2, a3;

	n1 = n-1;

	for (i = 1; i < n1; i ++)
	{
		if (xx <= x[i]) break;
	}
	if (i >= n1)
	{
		i = n1 - 1;
	}
	else if (i > 2)
	{
		if (xx - x[i - 1] < x[i] - xx) 
			i--;
	}

	x1 = x[i - 1];
	x2 = x[i];
	x3 = x[i + 1];
	a1 = ((xx - x2) * (xx - x3)) / ((x1 - x2) * (x1 - x3));
	a2 = ((xx - x3) * (xx - x1)) / ((x2 - x3) * (x2 - x1));
	a3 = ((xx - x1) * (xx - x2)) / ((x3 - x1) * (x3 - x2));

	return a1 * y[i - 1] + a2 * y[i] + a3 * y[i + 1];
}

double Interpol_Complicated(double x[],double y[],double xx,int n)//复杂多项式插值
{
	int k = -1, l, m;
	double g1, g2, u1, u2, u3, u4, u5, s, yy, a, b, c, d;
	yy = 0.;
	if (n <= 0) return yy; 
	if (n == 1)
	{
		yy = y[0];
		k = 0;
		a = y[0];
		b = 0.;
		c = 0.;
		d = 0.;
		return yy;
	}
	if (n == 2)
	{
		yy = (y[0] * (xx - x[1]) - y[1] * (xx - x[0])) / (x[0] - x[1]);
		k = 1;
		a = y[0];
		b = (y[1] - y[0]) / (x[1] - x[0]);
		c = 0.;
		d = 0.;
		return yy;
	}
	s = 1.;
	if (k <= 0)
	{
		s = -1.;
		if (xx <= x[1])
		{
			k = 1;
		}
		else if (xx >= x[n - 1])
		{
			k = n - 1;
		}
		else
		{
			k = 1;
			m = n;
			while (abs(k - m) != 1)
			{
				l = (k + m) / 2;
				if (xx < x[l - 1])
				{
					m = l;
				}
				else
				{
					k = l;
				}
			}
		}
	}
	if (k >= n) k = n - 1;
	u3 = (y[k] - y[k - 1]) / (x[k] - x[k - 1]);
	if (n == 3)
	{
		if (k == 1)
		{
			u4 = (y[2] - y[1]) / (x[2] - x[1]);
			u5 = 2. * u4 - u3;
			u2 = 2. * u3 - u4;
			u1 = 2. * u2 - u3;
		}
		else if (k == 2)
		{
			u2 = (y[1] - y[0]) / (x[1] - x[0]);
			u1 = 2. * u2 - u3;
			u4 = 2. * u3 - u2;
			u5 = 2. * u4 - u3;
		}
	}
	else
	{
		if (k <= 2)
		{
			u4 = (y[k + 1] - y[k]) / (x[k + 1] - x[k]);
			if (k == 2)
			{
				u2 = (y[1] - y[0]) / (x[1] - x[0]);
				u1 = 2. * u2 - u3;
				if (n == 4)
				{
					u5 = 2. * u4 - u3;
				}
				else
				{
					u5 = (y[4] - y[3]) / (x[4] - x[3]);
				}
			}
			else
			{
				u2 = 2. * u3 - u4;
				u1 = 2. * u2 - u3;
				u5 = (y[3] - y[2]) / (x[3] - x[2]);
			}
		}
		else if (k >= n - 2)
		{
			u2 = (y[k - 1] - y[k - 2]) / (x[k - 1] - x[k - 2]);
			if (k == n - 2)
			{
				u4 = (y[n - 1] - y[n - 2]) / (x[n - 1] - x[n - 2]);
				u5 = 2. * u4 - u3;
				if (n == 4)
				{
					u1 = 2. * u2 - u3;
				}
				else
				{
					u1 = (y[k - 2] - y[k - 3]) / (x[k - 2] - x[k - 3]);
				}
			}
			else
			{
				u4 = 2. * u3 - u2;
				u5 = 2. * u4 - u3;
				u1 = (y[k - 2] - y[k - 3]) / (x[k - 2] - x[k - 3]);
			}
		}
		else
		{
			u2 = (y[k - 1] - y[k - 2]) / (x[k - 1] - x[k - 2]);
			u1 = (y[k - 2] - y[k - 3]) / (x[k - 2] - x[k - 3]);
			u4 = (y[k + 1] - y[k]) / (x[k + 1] - x[k]);
			u5 = (y[k + 2] - y[k + 1]) / (x[k + 2] - x[k + 1]);
		}
	}
	a = fabs(u4 - u3);
	b = fabs(u1 - u2);
	if ((a + 1. == 1.) && (b + 1. == 1.))
	{
		g1 = (u2 + u3) / 2.;
	}
	else
	{
		g1 = (a * u2 + b * u3) / (a + b);
	}
	a = fabs(u4 - u5);
	b = fabs(u3 - u2);
	if ((a + 1. == 1.) && (b + 1. == 1.))
	{
		g2 = (u3 + u4) / 2.;
	}
	else
	{
		g2 = (a * u3 + b * u4) / (a + b);
	}
	a = y[k - 1];
	b = g1;
	d = x[k] - x[k - 1];
	c = (3. * u3 - 2. * g1 - g2) / d;
	d = (g2 + g1 - 2. * u3) / (d * d);
	if (s < 0.)
	{
		s = xx - x[k - 1];
		yy = a + b * s + c * s * s + d * s * s * s;
	}
	return yy;
}

double Interpol_PointAdjust(double x[],double y[],double xx, int nPoint,int n)//可调点数插值
{
	double* z = new double [nPoint];
	double* f = new double [nPoint];
	double fi, zi;
	int i, j, m = nPoint, i1, m1;

	if (m > n) m = n;

	for (i = 0; i < n; i ++) {
		if (xx <= x[i]) break;
	}
	if (i == n) i --;

	if (xx == x[i])
	{
		delete []z;
		delete []f;
		return y[i];
	}

	if (m%2 != 0 && i != 0 && xx - x[i - 1] < x[i] - xx) 
		i --;
	i -= m / 2;
	if (i >= 0) {
		if (i + m >= n) i = n - m;
	}
	else {
		i = 0;
	}

	for (j = 0; j < m; j ++) {
		z[j] = xx - x[i];
		f[j] = y[i];
		i ++;
	}

	m1 = m - 1;

	for (i = 0; i < m1; i ++) {
		fi = f[i];
		zi = z[i];
		i1 = i + 1;
		for (j = i1; j < m; j ++) {
			f[j] = fi + zi * (f[j] - fi) / (zi - z[j]);
		}
	}

	double ret = f[m - 1];
	delete []z;
	delete []f;
	return ret;
}

bool CDoublePoint_ComparisonEqual(CDoublePoint p1,CDoublePoint p2)
{
	if( (p1.x != p2.x)||(p1.y != p2.y) )
	{
		return false;
	}

	return true;
}

void Check_Warp_Table_Error(CDoublePoint *CheckPoint,unsigned int In_Hactive,unsigned int In_Vactive)
{
	int Y_Num = (In_Vactive+15)/16 + 1; //1200/16;
	int X_Num = (In_Hactive+15)/16 + 1; //2048/16;

	// **************************** 检测整体数据的合理性([i].x<=[i+1].x)*********************************************
	for(int j=0;j<Y_Num;j++)//针对 V = 1200的分辨率
	{	
		for(int i=X_Num-1;i>0;i--)
		{
			if(CheckPoint[j*2064/16+i-1].x > CheckPoint[j*2064/16+i].x)
			{
				CheckPoint[j*2064/16+i-1].x = CheckPoint[j*2064/16+i].x;
			}
		}
	}

	for(int i=0;i<X_Num;i++)
	{
		for(int j=Y_Num;j>0;j--)//针对 V = 1200的分辨率
		{
			if(CheckPoint[(j-1)*2064/16+i-1].y > CheckPoint[j*2064/16+i].y)
			{
				CheckPoint[(j-1)*2064/16+i-1].y = CheckPoint[j*2064/16+i].y;
			}
		}
	}


	// **************************** 检测最小负数与最大正数的值 *********************************************
	/*for(int j=0;j<Y_Num;j++)//针对 V = 1200的分辨率
	{	
		for(int i=0;i<X_Num;i++)
		{
			if(CheckPoint[j*2064/16+i].x < - 1023)
			{
				CheckPoint[j*2064/16+i].x = -1023;
			}
			if(CheckPoint[j*2064/16+i].y < - 1023)
			{
				CheckPoint[j*2064/16+i].y = -1023;
			}

			if(CheckPoint[j*2064/16+i].x > 3000)
			{
				CheckPoint[j*2064/16+i].x = 3000;
			}
			if(CheckPoint[j*2064/16+i].y > 3000)
			{
				CheckPoint[j*2064/16+i].y = 3000;
			}
		}
	}*/

}

/*******************************************计算Warp Table 的值 ******************************************************************/
/*
根据grid文件(输入坐标(0,0),(0,16)等在输出端的位置)，求出Warp文件（输出坐标(0,0),(0,16)所占输入端的位置，即输入端的(u,v)在输出端(x,y)
输出端x=0,16,32,i*16..... y=0,16,32,j*16.....;而输入端的位置(u,v)是所求的值。
(正交曲面拟合算法)
参数:
max_times      : 正交函数H的最大阶数，2,3......
ReferPoint_Num : 参考点的个数;   
ReferPoint_In  : 参考点的输入值，即输出点的坐标(12.5,26.32等); 
ReferPoint_Out : 参考点的输出值，即输入点的坐标(0,16,32等)

min_err_time   : 误差最小的阶数值
CalcPoint_Num  : 所需要计算点的个数;   
CalcPoint_In   : 所需要计算点的输入值，如输出坐标点的标准坐标点(0,0),(0,16)，或者是曲面所对应输出点的值，用于重叠计算 
CalcPoint_Out  : 所需要计算点的输出值，如输入点在输出点上的坐标值(12.23,52.1225)等，或者曲面点相对于直角坐标系的的输入点的坐标值
返回值 :  double       Min_Error_Return

最小误差值
*/

double Calc_Warp_Table(unsigned int max_times,    unsigned int ReferPoint_Num,CDoublePoint *ReferPoint_In,CDoublePoint *ReferPoint_Out,
					   unsigned int *min_err_time,unsigned int CalcPoint_Num, CDoublePoint *CalcPoint_In, CDoublePoint *CalcPoint_Out   )
{
	double       Min_Error_Return;
	unsigned int Min_Error_Index;

	unsigned int MAX_TIMES = max_times;    //最高次方，如x^4,y^4
	unsigned int H_NUM     = ((MAX_TIMES+1)*(MAX_TIMES+2)/2);    //H函数的数量 6, 10 , 15 等

	unsigned int POINT_NUM = ReferPoint_Num;   //Number of points

	double **H_Fun;
	H_Fun = new double*[POINT_NUM]; //设置行 或直接double **data=new double*[m]; 一个指针指向一个指针数组。
	for(unsigned int i=0;i<POINT_NUM;i++)
	{
		H_Fun[i] = new double[H_NUM];        //这个指针数组的每个指针元素又指向一个数组。
	}

	double **P_Fun;
	P_Fun = new double*[POINT_NUM]; //设置行 或直接double **data=new double*[m]; 一个指针指向一个指针数组。
	for(unsigned int i=0;i<POINT_NUM;i++)
	{
		P_Fun[i] = new double[H_NUM];        //这个指针数组的每个指针元素又指向一个数组。
	}

	double **Alpha;
	Alpha = new double*[H_NUM]; //设置行 或直接double **data=new double*[m]; 一个指针指向一个指针数组。
	for(unsigned int i=0;i<H_NUM;i++)
	{
		Alpha[i] = new double[H_NUM];        //这个指针数组的每个指针元素又指向一个数组。
	}

	double *A = new double[H_NUM];
	double *B = new double[H_NUM];

	double *Xk = new double[POINT_NUM];
	double *Yk = new double[POINT_NUM];

	double *Uk = new double[POINT_NUM];
	double *Vk = new double[POINT_NUM];
	double *Error_All = new double[MAX_TIMES];//从二阶开始计算

	unsigned int k,h,t,yn,xn;
	unsigned int i,j;


	for(i=0;i<POINT_NUM;i++)
	{
		Xk[i] = ReferPoint_In[i].x;
		Yk[i] = ReferPoint_In[i].y;

		Uk[i] = ReferPoint_Out[i].x;
		Vk[i] = ReferPoint_Out[i].y;
	}

	//得到h(x,y)的所有值[K][num_h]	
	for(k=0;k<POINT_NUM;k++)//K组数据
	{			
		h =0;
		for(t=0;t<=MAX_TIMES;t++) //for(t=0;t<=MAX_TIMES;t++) //从0阶到MAX_TIMES阶依次排列
		{
			for(yn=0;yn<=t;yn++)
			{  
				xn = t-yn;  //xn+yn= t；按照从低阶到高阶依次排序。（注：pow((double)0,(double)0) == 1）;
				H_Fun[k][h++] = pow(Xk[k],(double)xn)*pow(Yk[k],(double)yn); //1,x,y,x^2,x*y,y^2,x^3,x^2*y,x*y^2,y^4 ........ 
				//TRACE(" %08x  ",(unsigned int)H_Fun1[k][h-1]);
			} 
		}
		//TRACE("\r\n" );
	}

	//P初始化
	for(k=0;k<POINT_NUM;k++)
	{
		for(h=0;h<H_NUM;h++)
		{
			P_Fun[k][h] = 1;
		}
	}

	//Alpha[][]初始化
	for(i=0;i<H_NUM;i++)
	{
		for(j=0;j<=i;j++)
		{
			Alpha[i][j] = 1;
		}
		for(;j<H_NUM;j++)
		{
			Alpha[i][j] = 0;
		}
	}

	double tmp_top,tmp_bottom;

	for(h=1;h<H_NUM;h++)//alpha[i][j] i==j
	{
		tmp_top =0;
		tmp_bottom = 0;
		for(k=0;k<POINT_NUM;k++)
		{
			tmp_top    += pow(P_Fun[k][0],2);
			tmp_bottom += P_Fun[k][0]*H_Fun[k][h];          
		} 	      
		Alpha[h][h] = -1*tmp_top/tmp_bottom; //alpha[h][h]
	}

	unsigned int a_num;	
	for(i=2;i<H_NUM;i++) //alpha[i][j]  i != j
	{
		for(j=1;j<i;j++)//j  P的
		{
			for(k=0;k<POINT_NUM;k++)//pj 下的所有K 的 point的值
			{
				P_Fun[k][j] =0; 
				for(a_num=0;a_num<j;a_num++)//P_Fun[K][j] ;  K -- piont num, j -- P1, P2对应
				{
					P_Fun[k][j] += Alpha[j][a_num]*P_Fun[k][a_num];
				}
				P_Fun[k][j] += Alpha[j][j]*H_Fun[k][j];
			} 	               
			tmp_top =0;
			tmp_bottom = 0;
			for(k=0;k<POINT_NUM;k++)
			{   
				tmp_top    += P_Fun[k][j]*H_Fun[k][i];
				tmp_bottom += pow(P_Fun[k][j],2);      
			} 
			Alpha[i][j] = -1*Alpha[i][i]*tmp_top/tmp_bottom; //alpha[i][j]
		}
	}

	for(k=0;k<POINT_NUM;k++)//计算P[k][H_NUM-1]的值 
	{
		P_Fun[k][H_NUM-1] =0; 
		for(a_num=0;a_num<(H_NUM-1);a_num++)//P_Fun[K][j] ;  K -- piont num, j -- P1, P2对应
		{
			P_Fun[k][H_NUM-1] += Alpha[H_NUM-1][a_num]*P_Fun[k][a_num];
		}     
		P_Fun[k][H_NUM-1] += Alpha[H_NUM-1][H_NUM-1]*H_Fun[k][H_NUM-1];
	} 

	//A[h]
	for(h=0;h<H_NUM;h++)
	{
		tmp_top =0;
		tmp_bottom = 0;
		for(k=0;k<POINT_NUM;k++)
		{
			tmp_top    += Uk[k]*P_Fun[k][h];  
			tmp_bottom += pow(P_Fun[k][h],2);      
		} 	      
		A[h] = tmp_top/tmp_bottom; //A
	}

	//B[h]
	for(h=0;h<H_NUM;h++)
	{
		tmp_top =0;
		tmp_bottom = 0;
		for(k=0;k<POINT_NUM;k++)
		{
			tmp_top += Vk[k]*P_Fun[k][h];  
			tmp_bottom += pow(P_Fun[k][h],2);      
		} 

		B[h] = tmp_top/tmp_bottom; //B
	}

	/*********************************************** 检测 计算误差 **********************************************************/
	double  *Uk1 = new double[POINT_NUM];
	double *Vk1 =  new double[POINT_NUM];        

	double Uk_Err,Vk_Err;     
	unsigned int tmp_h_num;	
	Min_Error_Index =2; 
	for(t=2;t<=MAX_TIMES;t++)
	{	
		tmp_h_num     = (t+1)*(t+2)/2;    	
		for(k=0;k<POINT_NUM;k++)
		{
			Uk1[k] = 0;    Vk1[k] = 0;   
			for(h=0;h<tmp_h_num;h++)
			{
				Uk1[k] += A[h]*P_Fun[k][h];
				Vk1[k] += B[h]*P_Fun[k][h];
			}
		}

		Uk_Err=0;    Vk_Err=0;    
		for(i=0;i<POINT_NUM;i++)
		{
			Uk_Err += pow( (Uk1[i] - Uk[i]), 2);
			Vk_Err += pow( (Vk1[i] - Vk[i]), 2);
		}

		Error_All[t-2] = Uk_Err +Vk_Err;    
		if(Error_All[t-2]<Error_All[Min_Error_Index-2])
		{
			Min_Error_Index = t;

		}
	}	 
	delete[] Uk1;
	delete[] Vk1;

	*min_err_time     = Min_Error_Index;  //return 
	Min_Error_Return = Error_All[Min_Error_Index-2]; 

	/************************************************** 计算所需输入点的输出值************************************************************/	    	
	double x,y; //输入点在输出坐标中位置
	double ux,vx; //输出点所占输入点的位置
	unsigned int tmp_times;
	double *tmp_H = new double[H_NUM];
	double *tmp_P = new double[H_NUM];

	unsigned int in_y=0;;
	unsigned int in_x=0;

	tmp_times = Min_Error_Index;
	tmp_h_num = (tmp_times+1)*(tmp_times+2)/2;

	for(unsigned int index=0; index<CalcPoint_Num; index++)
	{
		x = CalcPoint_In[index].x;
		y = CalcPoint_In[index].y;

		h =0;
		for(t=0;t<=tmp_times;t++) //for(t=0;t<=MAX_TIMES;t++) //从0阶到MAX_TIMES阶依次排列
		{
			for(yn=0;yn<=t;yn++)
			{  
				xn = t-yn;  //xn+yn= t；按照从低阶到高阶依次排序。（注：pow((double)0,(double)0) == 1）;
				tmp_H[h++] = pow(x,(double)xn)*pow(y,(double)yn); //1,x,y,x^2,x*y,y^2,x^3,x^2*y,x*y^2,y^4 ........ 				
			} 
		}

		for(h=0;h<tmp_h_num;h++)
		{
			tmp_P[h] = 0;
			for(i=0;i<h;i++)
			{
				tmp_P[h] += (double)Alpha[h][i]*tmp_P[i];
			}
			tmp_P[h] += (double)Alpha[h][h]*tmp_H[h];
		}

		ux = 0;   vx = 0;   			
		for(h=0;h<tmp_h_num;h++)
		{
			ux += (double)A[h]*tmp_P[h];
			vx += (double)B[h]*tmp_P[h];
		}
		CalcPoint_Out[index].x = ux;
		CalcPoint_Out[index].y = vx;
	}
	delete[] tmp_H;
	delete[] tmp_P;

	/********************** delete  动态分配的变量******************************************************************/
	for (unsigned int i=0;i<POINT_NUM;i++) 
	{
		delete[] H_Fun[i]; 
		delete[] P_Fun[i]; //先撤销指针元素所指向的数组	
	}   
	delete[] H_Fun; 
	delete[] P_Fun; 

	for (unsigned int i=0;i<H_NUM;i++) 
	{
		delete[] Alpha[i]; 		
	}   
	delete[] Alpha; 

	delete[] A; 
	delete[] B; 

	delete[] Xk; 
	delete[] Yk; 

	delete[] Uk; 
	delete[] Vk; 
	delete[] Error_All;

	return Min_Error_Return;//误差 返回
}

/*******************************************计算Keystone Table 的值 ******************************************************************/
/*
根据grid文件(输入坐标(0,0),(0,16)等在输出端的位置)，求出Warp文件（输出坐标(0,0),(0,16)所占输入端的位置，即输入端的(u,v)在输出端(x,y)
输出端x=0,16,32,i*16..... y=0,16,32,j*16.....;而输入端的位置(u,v)是所求的值。
(正交曲面拟合算法)
参数: row_y_num      : 梯形4点行数
column_x_num   : 梯形4点列数   
ReferPoint_In  : 参考点的输入值，即输入点的坐标(0,16,32等)
ReferPoint_Out : 参考点的输出值，即输出点的坐标(12.5,26.32等); 

CalcPoint_Num  : 所需要计算点的个数;   
CalcPoint_In   : 所需要计算点的输入值，如输出坐标点的标准坐标点(0,0),(0,16)，或者是曲面所对应输出点的值，用于重叠计算 
CalcPoint_Out  : 所需要计算点的输出值，如输入点在输出点上的坐标值(12.23,52.1225)等，或者曲面点相对于直角坐标系的的输入点的坐标值
返回值 :  误差和

*/

double Calc_Keystone_InToOut_Table(unsigned int row_y_num,unsigned int column_x_num,  CDoublePoint *ReferPoint_In,CDoublePoint *ReferPoint_Out,
								   unsigned int CalcPoint_Num, CDoublePoint *CalcPoint_In, CDoublePoint *CalcPoint_Out   )
{
	unsigned int i,j,index;
	double in_x,in_y;
	double Error;

	CDoublePoint TLPiont_In,TLPiont_Out;  //左上角坐标
	CDoublePoint TRPiont_In,TRPiont_Out;  //右上角坐标
	CDoublePoint BLPiont_In,BLPiont_Out;  //左下角坐标
	CDoublePoint BRPiont_In,BRPiont_Out;  //右下角坐标

	//左上角坐标
	TLPiont_In  = ReferPoint_In[0];   
	TLPiont_Out = ReferPoint_Out[0];
	//右上角坐标
	TRPiont_In  = ReferPoint_In[column_x_num-1];   
	TRPiont_Out = ReferPoint_Out[column_x_num-1];
	//左下角坐标
	BLPiont_In  = ReferPoint_In[(row_y_num-1)*column_x_num];
	BLPiont_Out = ReferPoint_Out[(row_y_num-1)*column_x_num];  
	//右下角坐标
	BRPiont_In  = ReferPoint_In[row_y_num*column_x_num-1];
	BRPiont_Out = ReferPoint_Out[row_y_num*column_x_num-1];  

	//输入分辨率
	unsigned int Hactive,Vactive;
	Hactive = (unsigned int)BRPiont_In.x;
	Vactive = (unsigned int)BRPiont_In.y;

	double aver0_x,aver0_y;
	double aver1_x,aver1_y;

	//第0行 均分
	aver0_x = (TRPiont_Out.x - TLPiont_Out.x)/Hactive;
	aver0_y = (TRPiont_Out.y - TLPiont_Out.y)/Hactive;

	//第最后一行 均分
	aver1_x = (BRPiont_Out.x - BLPiont_Out.x)/Hactive;
	aver1_y = (BRPiont_Out.y - BLPiont_Out.y)/Hactive;

	CDoublePoint tmp0_Point,tmp1_Point;

	/************************************* 计算误差 ************************************************/
	index = 0;
	double err0,err1;
	err0 =0;
	err1 = 0;
	for(i=0;i<row_y_num;i++)
	{
		for(j=0;j<column_x_num;j++)
		{

			in_x = ReferPoint_In[index].x;
			in_y = ReferPoint_In[index].y;

			//(CalcPoint_In[i].x,0)的坐标
			tmp0_Point.x = TLPiont_Out.x + (TRPiont_Out.x - TLPiont_Out.x)*in_x/Hactive;
			tmp0_Point.y = TLPiont_Out.y + (TRPiont_Out.y - TLPiont_Out.y)*in_x/Hactive;

			//(CalcPoint_In[i].x,Vactive)的坐标
			tmp1_Point.x = BLPiont_Out.x + (BRPiont_Out.x - BLPiont_Out.x)*in_x/Hactive;
			tmp1_Point.y = BLPiont_Out.y + (BRPiont_Out.y - BLPiont_Out.y)*in_x/Hactive;

			//得到(CalcPoint_In[i].x,CalcPoint_In[i].y)的坐标
			err0 += pow( tmp0_Point.x + (tmp1_Point.x-tmp0_Point.x)*in_y/Vactive - ReferPoint_Out[index].x, 2);
			err1 += pow( tmp0_Point.y + (tmp1_Point.y-tmp0_Point.y)*in_y/Vactive - ReferPoint_Out[index].y, 2);
			index++;
		}
	}

	Error = err0 + err1;

	/******************************** 计算所需的输出值 *************************************************/
	for(i=0;i<CalcPoint_Num;i++)
	{
		/*
		//(CalcPoint_In[i].x,0)的坐标
		tmp0_Point.x = TLPiont_Out.x + aver0_x*CalcPoint_In[i].x;
		tmp0_Point.y = TLPiont_Out.y + aver0_y*CalcPoint_In[i].x;

		//(CalcPoint_In[i].x,Vactive)的坐标
		tmp1_Point.x = BLPiont_Out.x + aver1_x*CalcPoint_In[i].x;
		tmp1_Point.y = BLPiont_Out.y + aver1_y*CalcPoint_In[i].x;
		*/
		in_x = CalcPoint_In[i].x;
		in_y = CalcPoint_In[i].y;

		//(CalcPoint_In[i].x,0)的坐标
		tmp0_Point.x = TLPiont_Out.x + (TRPiont_Out.x - TLPiont_Out.x)*in_x/Hactive;
		tmp0_Point.y = TLPiont_Out.y + (TRPiont_Out.y - TLPiont_Out.y)*in_x/Hactive;

		//(CalcPoint_In[i].x,Vactive)的坐标
		tmp1_Point.x = BLPiont_Out.x + (BRPiont_Out.x - BLPiont_Out.x)*in_x/Hactive;
		tmp1_Point.y = BLPiont_Out.y + (BRPiont_Out.y - BLPiont_Out.y)*in_x/Hactive;

		//得到(CalcPoint_In[i].x,CalcPoint_In[i].y)的坐标
		CalcPoint_Out[i].x = tmp0_Point.x + (tmp1_Point.x-tmp0_Point.x)*in_y/Vactive;
		CalcPoint_Out[i].y = tmp0_Point.y + (tmp1_Point.y-tmp0_Point.y)*in_y/Vactive;
	}
	return Error;
}

/********************************************************
判断点是否在线段的右侧或者线段上
z = (x-x1)*(y1-y0) + (y1-y0)*(x1-x0)
Z> 0在线的右侧;  z = 0; 在线上;  z<0;   在线的左侧       
**********************************************************/

unsigned char PtOnRightSide(CDoublePoint pt, CDoublePoint Start, CDoublePoint end)
{
	if( (pt.x - end.x)*(end.y - Start.y) +  (end.y - pt.y)*(end.x - Start.x) >= 0 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/****************************************************
判断点p是否在三角形中，或者其边缘上
Pt 为需判断的点， pkst存储三角形的3个顶点的值
返回：1 - 在此三角形中或者边沿上， 0 - 表示在三角形之外
******************************************************/
unsigned char PtInTri(CDoublePoint pt, CDoublePoint *ptri)
{
	if( (PtOnRightSide(pt,ptri[0],ptri[1]) >=0)
		&&(PtOnRightSide(pt,ptri[1],ptri[2]) >=0)
		&&(PtOnRightSide(pt,ptri[2],ptri[0]) >=0) )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/***************************************************
根据梯形四点计算 梯形内某点的坐标(双线性插值运算)
Pt 为需判断的点， pkst存储梯形的四个顶点的值
*************************************************/  
/*

P0  P1  P2

P20 = P2-P0
P21 = P2 -P1
i        j       k 
P20.x  P20.y    f(P20)
P21.x  P21.y    f(P21)
A*i                                    B*j                         C*k 
(P20.y*f(P21)-P21.y*f(P20))i + (f(P20)*P21.x-P20.x*f(P21))j + (P20.x*P21.y-P20.y*P21.x)k  

A(P.x-P0.x)+B(P.y-p0.y)+C(f(P)-f(P0)) = 0

f(p) = (-A/C)*P.x + (-B/C)*P.y + (f(P0) + A/C*P0.x + B/C*P0.y);
输入参数：
ptri_f: 0,16,32,...... 
ptri_p: P[0][0], P[0][2], ...  P[3][0] ... 的值 
输出：u = k0*x + k1*y + k2
v = k0*y + k1*y + k2

*************************************************************************************/

void GetEquationCoef(CDoublePoint *ptri_p,CDoublePoint *ptri_f, CDoublePoint* coef)
{
	CDoublePoint P20,P21;
	CDoublePoint f20,f21;
	CDoublePoint P0,A,B,C,K0,K1,K2;

	P20.x = ptri_p[2].x - ptri_p[0].x;
	P20.y = ptri_p[2].y - ptri_p[0].y;
	P21.x = ptri_p[2].x - ptri_p[1].x;
	P21.y = ptri_p[2].y - ptri_p[1].y;
	P0 = ptri_p[0];

	f20.x = ptri_f[2].x - ptri_f[0].x;
	f20.y = ptri_f[2].y - ptri_f[0].y;
	f21.x = ptri_f[2].x - ptri_f[1].x;
	f21.y = ptri_f[2].y - ptri_f[1].y;

	A.x = P20.y * f21.x - P21.y * f20.x;
	B.x = f20.x * P21.x - P20.x * f21.x;
	C.x = P20.x * P21.y - P20.y * P21.x;

	K0.x = -A.x/C.x;
	K1.x = -B.x/C.x;	
	K2.x = ptri_f[0].x + A.x/C.x*P0.x + B.x/C.x*P0.y;

	A.y = P20.y * f21.y - P21.y * f20.y;
	B.y = P21.x * f20.y - P20.x * f21.y;
	C.y = P20.x * P21.y - P20.y * P21.x;

	K0.y = -A.y/C.y;
	K1.y = -B.y/C.y;	
	K2.y = ptri_f[0].y + A.y/C.y*P0.x + B.y/C.y*P0.y;

	coef[0] = K0; 
	coef[1] = K1;  
	coef[2] = K2;
}

/**************************************************************
u = k1*x + k2*X + k3
v = k1*x + k2*X + k3
*********************************************************************/
CDoublePoint CalcPointByCoef(CDoublePoint pt, CDoublePoint *coef)
{
	double x,y;

	x = coef[0].x * pt.x + coef[1].x * pt.y + coef[2].x;
	y = coef[0].y * pt.x + coef[1].y * pt.y + coef[2].y;

	CDoublePoint res(x,y);
	return res;
}

/*******************************************计算Warp Table 的值 ******************************************************************/
/*
根据grid文件(输入坐标(0,0),(0,16)等在输出端的位置)，求出Warp文件（输出坐标(0,0),(0,16)所占输入端的位置，即输入端的(u,v)在输出端(x,y)
输出端x=0,16,32,i*16..... y=0,16,32,j*16.....;而输入端的位置(u,v)是所求的值。
(正交曲面拟合算法)
参数: max_times      : 正交函数H的最大阶数，2,3......
ReferPoint_Num : 参考点的个数;   
ReferPoint_In  : 参考点的输入值，即输出点的坐标(12.5,26.32等); 
ReferPoint_Out : 参考点的输出值，即输入点的坐标(0,16,32等)

min_err_time   : 误差最小的阶数值
CalcPoint_Num  : 所需要计算点的个数;   
CalcPoint_In   : 所需要计算点的输入值，如输出坐标点的标准坐标点(0,0),(0,16)，或者是曲面所对应输出点的值，用于重叠计算 
CalcPoint_Out  : 所需要计算点的输出值，如输入点在输出点上的坐标值(12.23,52.1225)等，或者曲面点相对于直角坐标系的的输入点的坐标值
返回值 :  double       Min_Error_Return

最小误差值


CalcPoint_In[i+cx*j]

*/
void Calc_Point_Table_dir(unsigned int cx,unsigned int cy,unsigned int In_Hactive,unsigned int In_Vactive,
						  CDoublePoint *CalcPoint_In, CDoublePoint *CalcPoint_Out   )
{
	double INTERVAL_X, INTERVAL_Y;
	unsigned int i,j,Calc_X_Start,Calc_X_End,Calc_Y_Start,Calc_Y_End;
	double tmp_double;
	CDoublePoint pt,ptri_left[3],ptri_right[3],pf_left[3],pf_right[3],coef_left[3],coef_right[3];

	INTERVAL_X = (double)In_Hactive/(cx-1);
	INTERVAL_Y = (double)In_Vactive/(cy-1);

	//输出值初始化 为 负数 
	int index = 0x00;
	for(int yn =0;yn<=1200;yn+=16)
	{
		for(int xn=0;xn<=2048;xn+=16)
		{
			CalcPoint_Out[index].x = 3000.0;
			CalcPoint_Out[index].y = 3000.0;

			index++;
		}
	}

	for(unsigned int x_i = 0;x_i<cx-1;x_i++)
	{
		for(unsigned int y_j = 0;y_j<cy-1;y_j++)
		{
			//确认最小值 X方向
			tmp_double = CalcPoint_In[x_i+cx*y_j].x > CalcPoint_In[x_i+cx*(y_j+1)].x ? CalcPoint_In[x_i+cx*(y_j+1)].x : CalcPoint_In[x_i+cx*y_j].x;
			Calc_X_Start = (int)(tmp_double/16)*16;
			//确认最大值 X方向
			tmp_double = CalcPoint_In[(x_i+1)+cx*y_j].x > CalcPoint_In[(x_i+1)+cx*(y_j+1)].x ? CalcPoint_In[(x_i+1)+cx*y_j].x : CalcPoint_In[(x_i+1)+cx*(y_j+1)].x;
			Calc_X_End = (int)(tmp_double/16)*16 + 16; //结束位置多检测一个

			//确认最小值 Y方向
			tmp_double = CalcPoint_In[x_i+cx*y_j].y > CalcPoint_In[(x_i+1)+cx*y_j].y ? CalcPoint_In[(x_i+1)+cx*y_j].y : CalcPoint_In[x_i+cx*y_j].y;
			Calc_Y_Start = (int)(tmp_double/16)*16;
			//确认最大值 Y方向
			tmp_double = CalcPoint_In[x_i+cx*(y_j+1)].y > CalcPoint_In[(x_i+1)+cx*(y_j+1)].y ? CalcPoint_In[x_i+cx*(y_j+1)].y : CalcPoint_In[(x_i+1)+cx*(y_j+1)].y;
			Calc_Y_End = (int)(tmp_double/16)*16 + 16; //结束完成多检测一个

			//********************************** 判断数据的合理性 *********************
			if( (Calc_X_Start > (In_Hactive+16))||(Calc_Y_Start > (In_Vactive+16)) )  
			{
				continue;
			}

			if(Calc_X_End > (In_Hactive+16))
			{
				Calc_X_End = In_Hactive+16;
			}

			if(Calc_Y_End > (In_Vactive+16))
			{
				Calc_Y_End = In_Vactive+16;
			}

			if(x_i == 0) //首列 
			{
				Calc_X_Start = 0;
			}
			if(y_j == 0) //首行 
			{
				Calc_Y_Start = 0;
			}
			if(x_i == cx-2) //最后一列 
			{
				Calc_X_End = In_Hactive+16;
			}
			if(y_j == cy-2) //最后一行 
			{
				Calc_Y_End = In_Vactive+16;;
			}

			//左边三角形   
			ptri_left[0] = CalcPoint_In[x_i+cx*y_j];
			ptri_left[1] = CalcPoint_In[x_i+cx*(y_j+1)]; 
			ptri_left[2] = CalcPoint_In[(x_i+1)+cx*(y_j+1)];

			pf_left[0] = CDoublePoint(x_i*INTERVAL_X,      y_j*INTERVAL_Y);
			pf_left[1] = CDoublePoint(x_i*INTERVAL_X,     (y_j+1)*INTERVAL_Y);
			pf_left[2] = CDoublePoint((x_i+1)*INTERVAL_X, (y_j+1)*INTERVAL_Y);
			//右边三角形
			ptri_right[0] = CalcPoint_In[x_i+cx*y_j];
			ptri_right[1] = CalcPoint_In[(x_i+1)+cx*y_j]; 
			ptri_right[2] = CalcPoint_In[(x_i+1)+cx*(y_j+1)];
			pf_right[0] = CDoublePoint(x_i*INTERVAL_X,      y_j*INTERVAL_Y);
			pf_right[1] = CDoublePoint((x_i+1)*INTERVAL_X,  y_j*INTERVAL_Y);
			pf_right[2] = CDoublePoint((x_i+1)*INTERVAL_X, (y_j+1)*INTERVAL_Y);

			//计算左边三角形的平面方程系数
			GetEquationCoef(ptri_left,  pf_left,  coef_left);
			//计算右边三角形的平面方程系数
			GetEquationCoef(ptri_right, pf_right, coef_right);

			for(unsigned int line_x=Calc_X_Start;line_x<Calc_X_End; line_x+=16)
			{
				for(unsigned int line_y=Calc_Y_Start; line_y<Calc_Y_End; line_y+=16)
				{
					pt.x = line_x; pt.y = line_y;
					i = line_x/16;
					j = line_y/16;

					if( (PtOnRightSide(pt,ptri_left[0],ptri_left[1]) >=0)&&(PtOnRightSide(pt,ptri_left[2],ptri_left[0]) >=0) )//if(PtInTri(pt,ptri_left)) //此点在左边三角形中,获取边缘上          
					{
						CalcPoint_Out[i+j*2064/16] = CalcPointByCoef(pt, coef_left);  //根据平面方程系数，计算
					}
					else  if( (PtOnRightSide(pt,ptri_right[0],ptri_right[1]) >=0)&&(PtOnRightSide(pt,ptri_right[2],ptri_right[0]) >=0) )// if(PtInTri(pt,ptri_right)) //此点在右边三角形中，或者在边缘上
					{
						CalcPoint_Out[i+j*2064/16] = CalcPointByCoef(pt, coef_right);  //根据平面方程系数，计算
					}
					else //不在这两个三角形，即不再梯形内或者其边缘 
					{
						if(x_i == 0x00)//左边缘
						{
							if(y_j == 0x00) //第一行
							{
								//在左边区域中
								if( (PtOnRightSide(pt,ptri_left[0],ptri_left[1]) >=0)&&(PtOnRightSide(pt,ptri_left[1],ptri_left[2]) >=0) )
								{
									CalcPoint_Out[i+j*2064/16] = CalcPointByCoef(pt, coef_left);  //根据平面方程系数，计算
								}
								else if((PtOnRightSide(pt,ptri_right[1],ptri_right[2]) >=0)&&(PtOnRightSide(pt,ptri_right[2],ptri_right[0]) >=0))
								{
									CalcPoint_Out[i+j*2064/16] = CalcPointByCoef(pt, coef_right);  //根据平面方程系数，计算
								}                                                                                                             
							}
							else if(y_j = cy-2) //最后一行
							{
								if( (PtOnRightSide(pt,ptri_right[0],ptri_right[1]) >=0)&&(PtOnRightSide(pt,ptri_right[1],ptri_right[2]) >=0) )
								{
									CalcPoint_Out[i+j*2064/16] = CalcPointByCoef(pt, coef_left);  //根据平面方程系数，计算
								} 
							}
							else //左边缘的
							{
								if( (PtOnRightSide(pt,ptri_right[0],ptri_right[1]) >=0)&&(PtOnRightSide(pt,ptri_left[1],ptri_left[2]) >=0) )
								{
									CalcPoint_Out[i+j*2064/16] = CalcPointByCoef(pt, coef_left);  //根据平面方程系数，计算
								}
							}
						}
						else if(y_j == 0x00)// 顶部一行 
						{
							if(x_i == cx-2) // 最右边
							{
								if((PtOnRightSide(pt,ptri_left[1],ptri_left[2]) >=0)&&(PtOnRightSide(pt,ptri_left[2],ptri_left[0]) >=0))
								{
									CalcPoint_Out[i+j*2064/16] = CalcPointByCoef(pt, coef_right);  //根据平面方程系数，计算
								}    
							}
							else
							{
								if((PtOnRightSide(pt,ptri_right[1],ptri_right[2]) >=0)&&(PtOnRightSide(pt,ptri_left[2],ptri_left[0]) >=0))
								{
									CalcPoint_Out[i+j*2064/16] = CalcPointByCoef(pt, coef_right);  //根据平面方程系数，计算
								}   
							}
						}
					}
				}
			}  
		}
	}

}


//********************************************************
// ConvertGridPointData
// 转换多点校正数据由原网格数转到65*65的网格数据
bool ConvertGridPointData(int iCol, int iRow, CDoublePoint *pOrigData, CDoublePoint *pDestData)
{
	//网格数只能是17、33 或 65
	if (iCol != 17 && iCol != 33 && iCol != 65) return false;
	if (iRow != 17 && iRow != 33 && iRow != 65) return false;

	//已经是65x65的网格了
	if (iCol==65 && iRow==65) return true;

	//数据指针有效
	if (pOrigData == NULL || pDestData == NULL) return false;

	//网格倍数
	int nTimes_cx = 65/(iCol-1);
	int nTimes_cy = 65/(iRow-1);

	//往每行相邻两点之间插值，插值的平均距离
	double x_avr_space = .0;
	//往每列相邻两点之间插值，插值的平均距离
	double y_avr_space = .0;

	CDoublePoint *pTempData = new CDoublePoint[65*iRow];

	// 横向扩展数据 得到了一个[65*iRow]的网格数据
	for (int iy = 0; iy < iRow; iy++)
	{
		for (int ix=0; ix < iCol; ix++) 
		{
			//将源数据复制到目标数据中
			pTempData[65*iy+ix*nTimes_cx].x = pOrigData[ix+iy*iCol].x;
			pTempData[65*iy+ix*nTimes_cx].y = pOrigData[ix+iy*iCol].y;

			//最后一列，插值完毕
			if(ix == iCol-1) break;

			//源数据和目标数据的列数相等无需插值
			if(iCol == 65) continue;

			//列间插值
			x_avr_space = (pOrigData[iCol*iy+ix+1].x - pOrigData[iCol*iy+ix].x)/nTimes_cx;
			y_avr_space = (pOrigData[iCol*iy+ix+1].y - pOrigData[iCol*iy+ix].y)/nTimes_cx;
			for (int jx=1; jx<nTimes_cx; jx++)
			{
				pTempData[65*iy+ix*nTimes_cx+jx].x = pOrigData[iCol*iy+ix].x+x_avr_space*jx;
				pTempData[65*iy+ix*nTimes_cx+jx].y = pOrigData[iCol*iy+ix].y+y_avr_space*jx;
			}
		}
	}

	// 纵向扩展数据 得到了一个[65*65]的网格数据
	if (iRow != 65)
	{
		for (int iy=0; iy<iRow; iy++)
		{			
			for (int ix=0; ix<65; ix++)
			{	
				//复制数据
				pDestData[iy*nTimes_cy*65+ix].x = pTempData[iy*65+ix].x;
				pDestData[iy*nTimes_cy*65+ix].y = pTempData[iy*65+ix].y;

				if(iy == iRow-1) continue;
				//行间插值		
				x_avr_space = (pTempData[ix+(iy+1)*65].x - pTempData[ix+iy*65].x)/nTimes_cy;
				y_avr_space = (pTempData[ix+(iy+1)*65].y - pTempData[ix+iy*65].y)/nTimes_cy;
				for (int jy=1; jy<nTimes_cy; jy++)
				{
					pDestData[(iy*nTimes_cy+jy)*65+ix].x = pTempData[ix+iy*65].x+x_avr_space*jy;
					pDestData[(iy*nTimes_cy+jy)*65+ix].y = pTempData[ix+iy*65].y+y_avr_space*jy;
				}
			}
		}
	}
	else
	{
		for (int i=0; i<65*65; i++)
		{
			pDestData[i].x = pTempData[i].x;
			pDestData[i].y = pTempData[i].y;
		}
	}

	delete pTempData;
	pTempData = NULL;

	return true;
}