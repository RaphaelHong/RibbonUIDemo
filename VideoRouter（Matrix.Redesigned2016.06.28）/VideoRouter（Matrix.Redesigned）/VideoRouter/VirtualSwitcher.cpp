#include "StdAfx.h"
#include "VirtualSwitcher.h"

//*************************************************************************************************************************************//
//                                                          虚拟矩阵 表管理                                                            //

CVirtualSwitcher::CVirtualSwitcher()
{
	m_bLockUpdate = FALSE;

	//默认网格起始点为坐标原点
	m_ptStart = CPoint(0, 0);
	//默认第一列宽度，信号源列宽度
	m_nFirstColWid = 100;
	//默认普通网格宽度
	m_nGridWid = 16;
	//默认普通网格高度
	m_nGridHei = 16;
	//默认最大列数
	m_nColunms = _MAX_OUTPUT_PORT+2;
	//默认最大行数
	m_nRows = _MAX_INPUT_PORT+1;
	//默认扩展列数
	m_nExtraCol = 2;
	//默认扩展行数
	m_nExtraRow = 1;

	//计算整个表格所占的宽度和高度 以像素为单位
	SetWholeGridSize();

	m_ptFocusGrid = CPoint(INVALID_VALUE, INVALID_VALUE);

	//初始化网格点坐标
	Init();
}

CVirtualSwitcher::CVirtualSwitcher(CPoint ptStart, int nFirstColWid, int nGridWid, int nGridHei, int nCol, int nRow)
{
	m_bLockUpdate = FALSE;
	m_ptStart = ptStart;
	m_nSize = CSize(0, 0);
	m_nFirstColWid = nFirstColWid;
	m_nGridHei = nGridHei;
	m_nGridWid = nGridWid;
	m_nExtraRow = 1;
	m_nExtraCol = 2;
	m_nColunms = nCol+m_nExtraCol; //表格列数比实际输出端口数多出2，第一列为信号源列，第二列为全选列
	m_nRows = nRow+m_nExtraRow;   //表格行数比实际输入信号源数多出1，第一行为输出端口号行

	//计算整个表格所占的宽度和高度 以像素为单位
	SetWholeGridSize();

	m_ptFocusGrid = CPoint(INVALID_VALUE, INVALID_VALUE);

	Init();
}

CVirtualSwitcher::~CVirtualSwitcher()
{
}

void CVirtualSwitcher::LockUpdate()
{
	m_bLockUpdate = TRUE;
}

void CVirtualSwitcher::UnlockUpdate()
{
	m_bLockUpdate = FALSE;

	Init();
}

void CVirtualSwitcher::Init()
{
	if (m_bLockUpdate)
		return;

	//清理所有网格坐标信息
	m_arrGridRect.RemoveAll();

	//重计算网格坐标
	CRect rcTmp;
	for (int y=0; y<m_nRows; y++)
	{
		for (int x=0; x<m_nColunms; x++)
		{
			//网格的左、右位置
			if (x == 0) {
				rcTmp.left = m_ptStart.x;
				rcTmp.right = m_ptStart.x+m_nFirstColWid;
			}
			else {
				rcTmp.left = m_ptStart.x+m_nFirstColWid+(x-1)*m_nGridWid;
				rcTmp.right = rcTmp.left + m_nGridWid;
			}
			//网格的上、下位置
			rcTmp.top = m_ptStart.y + m_nGridHei*y;
			rcTmp.bottom = m_ptStart.y+m_nGridHei*y+m_nGridHei;

			m_arrGridRect.Add(rcTmp);
		}
	}
}

void CVirtualSwitcher::SetFirstColWidth(const int nWid)
{
	m_nFirstColWid = nWid;
	SetWholeGridSize();
	UpdateGridRect();
}

void CVirtualSwitcher::SetGridWidth(const int nWid)
{ 
	m_nGridWid = nWid;
	SetWholeGridSize();
	UpdateGridRect();
}

void CVirtualSwitcher::SetGridHeight(const int nHei)
{
	m_nGridHei = nHei;
	SetWholeGridSize();
	UpdateGridRect();
}

void CVirtualSwitcher::SetRowCount(const int nRow)
{
	m_nRows = nRow+m_nExtraRow;
	SetWholeGridSize();
	Init();
}

void CVirtualSwitcher::SetColunmCount(const int nCol)
{
	m_nColunms = nCol+m_nExtraCol;
	SetWholeGridSize();
	Init();
}

inline void CVirtualSwitcher::SetWholeGridSize()
{
	m_nSize.cx = m_nFirstColWid+(m_nColunms-1)*m_nGridWid;
	m_nSize.cy = m_nRows*m_nGridHei;
}

void CVirtualSwitcher::UpdateGridRect()
{
	if (m_bLockUpdate)
		return;

	CRect *rcTmp;
	for (int y=0; y<m_nRows; y++)
	{
		for (int x=0; x<m_nColunms; x++)
		{
			rcTmp = &m_arrGridRect[y*m_nColunms+x];
			//网格的左、右位置
			if (x == 0) {
				rcTmp->left = m_ptStart.x;
				rcTmp->right = m_ptStart.x+m_nFirstColWid;
			}
			else {
				rcTmp->left = m_ptStart.x+m_nFirstColWid+(x-1)*m_nGridWid;
				rcTmp->right = rcTmp->left + m_nGridWid;
			}
			//网格的上、下位置
			rcTmp->top = m_ptStart.y + m_nGridHei*y;
			rcTmp->bottom = m_ptStart.y+m_nGridHei*y+m_nGridHei;
		}
	}
}

CRect CVirtualSwitcher::GetMatrixRect() const
{
	CRect rcMatrix;
	rcMatrix = CRect(m_ptStart.x+m_nFirstColWid,
		m_ptStart.y+m_nGridHei,
		m_ptStart.x+m_nSize.cx,
		m_ptStart.y+m_nSize.cy);

	return rcMatrix;
}

CRect CVirtualSwitcher::GetGridRect(const int x, const int y ) const
{
	CRect rcGrid;
	if (m_arrGridRect.IsEmpty()) return rcGrid;
	if (x >= m_nColunms || y >= m_nRows) return rcGrid;

	rcGrid = m_arrGridRect[y*m_nColunms+x];
	return rcGrid;
}

void CVirtualSwitcher::SetFocusGrid(const CPoint ptFocus)
{
	if (ptFocus.x < 1 || ptFocus.x >= m_nColunms) return;
	if (ptFocus.y < 1 || ptFocus.y >= m_nRows) return;

	m_ptFocusGrid = ptFocus;
}

bool CVirtualSwitcher::IsMatrixClicked(const CPoint point) const
{	
	if (point.x < m_arrGridRect[0].left                            //第二列左边
		|| point.x > m_arrGridRect[m_nColunms-1].right             //最后一列右边
		|| point.y < m_arrGridRect[0].top                          //第二行的上边
		|| point.y > m_arrGridRect[(m_nRows-1)*m_nColunms].bottom) //最后一行的下边
	{
		return false;
	}

	return true;
}

int CVirtualSwitcher::LocateColunm(const CPoint point) const
{
	//网格数异常
	if (m_arrGridRect.GetCount() != m_nColunms*m_nRows) return INVALID_VALUE;

	//点在有效选择网格之外
	if (!IsMatrixClicked(point)) return INVALID_VALUE;

	//单选
	CRect rect;
	int nFindindex = INVALID_VALUE;
	for (int i=0; i<m_nColunms; i++)
	{
		//只需判断第一行
		rect = m_arrGridRect[i];
		if (point.x < rect.right && point.x >= rect.left)
		{
			nFindindex = i;
			break;
		}
	}

	return nFindindex;
}

int CVirtualSwitcher::LocateRow(const CPoint point) const
{
	//网格数异常
	if (m_arrGridRect.GetCount() != m_nColunms*m_nRows) return INVALID_VALUE;

	//点在有效选择网格之外
	if (!IsMatrixClicked(point)) return INVALID_VALUE;

	//单选
	CRect rect;
	int nFindindex = INVALID_VALUE;
	for (int i=0; i<m_nRows; i++)
	{
		//只需判断第一列
		rect = m_arrGridRect[i*m_nColunms];
		if (point.y < rect.bottom && point.y >= rect.top)
		{
			nFindindex = i;
			break;
		}
	}

	return nFindindex;
}

void CVirtualSwitcher::GetClickedMatrix(const CPoint point, int &nRowIndex, int &nColIndex) const
{
	int nColunmSel = LocateColunm(point);
	int nRowSel = LocateRow(point);

	if (nColunmSel < 1
		|| nColunmSel >= m_nColunms
		|| nRowSel < 1
		|| nRowSel >= m_nRows)
	{
		nRowIndex = INVALID_VALUE;
		nColIndex = INVALID_VALUE;
	}
	else
	{
		nRowIndex = nRowSel-m_nExtraRow;
		if (nColunmSel == 1) { nColIndex = ALL_SELECTED; }
		else { nColIndex = nColunmSel-m_nExtraCol; }
	}
}

int CVirtualSwitcher::GetClickedRow(const CPoint point) const
{
	if (m_arrGridRect.IsEmpty()) return INVALID_VALUE;

	if (point.x < m_arrGridRect[0].left                            //第一列左边
		|| point.x > m_arrGridRect[m_nColunms-1].right             //最后一列右边
		|| point.y < m_arrGridRect[0].top                          //第一行的上边
		|| point.y > m_arrGridRect[(m_nRows-1)*m_nColunms].bottom) //最后一行的下边
	{
		return INVALID_VALUE;
	}

	int nFindSrcID = INVALID_VALUE;
	for (int i=1; i<m_nRows; i++) {
		if (PtInRect(m_arrGridRect[i*m_nColunms], point)) {
			nFindSrcID = i-1;
			break;
		}
	}

	return nFindSrcID;
}

//------------------------------------------------------------------------------------------------------------------------------------------------//