#pragma once

//****************************************************************//
//                          虚拟网格类                            //
//----------------------------------------------------------------//
class CVirtualSwitcher
{
public:
	CVirtualSwitcher();
	CVirtualSwitcher(CPoint ptStart, int nFirstColWid, int nGridWid, int nGridHei, int nCol, int nRow);
	~CVirtualSwitcher();

	//根据当前网格数、宽度和高度，初始化网格坐标
	void Init();

	BOOL m_bLockUpdate;
	//锁定数组操作，与UnlockUpdate成对使用
	void LockUpdate();
	//解锁数组操作，并更新数组内容
	void UnlockUpdate();

public:
	//--------------------------------------- 网格坐标操作 -----------------------------------------//
	//获取指定网格的坐标
	CRect GetGridRect(const int x, const int y) const;

	//获取矩阵切换时，网格对应的输入输出口， 而不是返回网格的真实行与列
	void GetClickedMatrix(const CPoint point, int &nRowIndex, int &nColIndex) const;

	//获取鼠标点击第一列上的行号
	int GetClickedRow(const CPoint point) const;


	//**********以下方法只和视图操作有关，和功能无关************//
	//--------------------------------------- 网格度量设置 ---------------------------------------//
	//获取第一列信号源列的宽度
	int GetFirstColWidth() const { return m_nFirstColWid; }
	//设置第一列宽度
	void SetFirstColWidth(const int nWid);
	//获取网格宽度
	int GetGridWidth() const { return m_nGridWid; }
	//设置网格宽度
	void SetGridWidth(const int nWid);
	//获取网格高度
	int GetGridHeight() const { return m_nGridHei; }	
	//设置网格高度
	void SetGridHeight(const int nHei);

	//当网格高度或宽度变化时，需要更新各个网格的坐标，无需删除动态网格数组
	void UpdateGridRect();

	//获取整个表格的尺寸
	CSize GetWholeGridSize() const { return m_nSize; }
	inline void SetWholeGridSize();

	//获取表格坐标
	CRect GetMatrixRect() const;

	//--------------------------------------- 网格行列数设置 --------------------------------------//
	//获取网格列数
	int GetColunmCount() const { return m_nColunms; }
	//获取网格行数
	int GetRowCount() const { return m_nRows; }
	//设置网格列数
	void SetColunmCount(const int nCol);
	//设置网格行数
	void SetRowCount(const int nRow);
	//获取扩展列数
	int GetExtraColCount() const { return m_nExtraCol; }
	//获取扩展行数
	int GetExtraRowCount() const { return m_nExtraRow; }

	//--------------------------------------- 表的起始坐标设置 -----------------------------------//
	//设置表格在视图中的起始位置
	void SetStartPoint(const CPoint ptStart) { m_ptStart = ptStart; }
	//获取表格在视图中的起始位置
	CPoint GetStartPoint() const { return m_ptStart; }

	//--------------------------------------- 网格选中设置 ---------------------------------------------//
	//设置选中格
	void SetFocusGrid(const CPoint ptFocus) ;
	//获取选中格
	CPoint GetFocusGrid() const { return m_ptFocusGrid; }

private:
	CArray <CRect, CRect &> m_arrGridRect; //维护所有网格的坐标
	int m_nFirstColWid;  //第一列宽度
	int m_nGridWid;      //普通网格宽度
	int m_nGridHei;      //普通网格高度
	int m_nColunms;      //网格列数 = 扩展列数 + 实际输出口数
	int m_nRows;         //网格行数 = 扩展行数 + 实际输入口数
	int m_nExtraCol;     //扩展列
	int m_nExtraRow;     //扩展行
	CSize m_nSize;       //整个表格的尺寸 像素为单位
	CPoint m_ptStart;    //网格起始点坐标

	CPoint m_ptFocusGrid;//选中的网格 x:列，y:行

	//获取鼠标点击的列号，真实的列号
	int LocateColunm(const CPoint point) const;
	//获取鼠标点击的行号，真实的行号
	int LocateRow(const CPoint point) const;
	//鼠标是否点击在有效范围内
	bool IsMatrixClicked(const CPoint point) const;
};
