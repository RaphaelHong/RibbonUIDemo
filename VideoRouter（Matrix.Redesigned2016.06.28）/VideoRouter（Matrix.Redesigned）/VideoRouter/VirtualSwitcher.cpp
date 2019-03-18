#include "StdAfx.h"
#include "VirtualSwitcher.h"

//*************************************************************************************************************************************//
//                                                          ������� �����                                                            //

CVirtualSwitcher::CVirtualSwitcher()
{
	m_bLockUpdate = FALSE;

	//Ĭ��������ʼ��Ϊ����ԭ��
	m_ptStart = CPoint(0, 0);
	//Ĭ�ϵ�һ�п�ȣ��ź�Դ�п��
	m_nFirstColWid = 100;
	//Ĭ����ͨ������
	m_nGridWid = 16;
	//Ĭ����ͨ����߶�
	m_nGridHei = 16;
	//Ĭ���������
	m_nColunms = _MAX_OUTPUT_PORT+2;
	//Ĭ���������
	m_nRows = _MAX_INPUT_PORT+1;
	//Ĭ����չ����
	m_nExtraCol = 2;
	//Ĭ����չ����
	m_nExtraRow = 1;

	//�������������ռ�Ŀ�Ⱥ͸߶� ������Ϊ��λ
	SetWholeGridSize();

	m_ptFocusGrid = CPoint(INVALID_VALUE, INVALID_VALUE);

	//��ʼ�����������
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
	m_nColunms = nCol+m_nExtraCol; //���������ʵ������˿������2����һ��Ϊ�ź�Դ�У��ڶ���Ϊȫѡ��
	m_nRows = nRow+m_nExtraRow;   //���������ʵ�������ź�Դ�����1����һ��Ϊ����˿ں���

	//�������������ռ�Ŀ�Ⱥ͸߶� ������Ϊ��λ
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

	//������������������Ϣ
	m_arrGridRect.RemoveAll();

	//�ؼ�����������
	CRect rcTmp;
	for (int y=0; y<m_nRows; y++)
	{
		for (int x=0; x<m_nColunms; x++)
		{
			//���������λ��
			if (x == 0) {
				rcTmp.left = m_ptStart.x;
				rcTmp.right = m_ptStart.x+m_nFirstColWid;
			}
			else {
				rcTmp.left = m_ptStart.x+m_nFirstColWid+(x-1)*m_nGridWid;
				rcTmp.right = rcTmp.left + m_nGridWid;
			}
			//������ϡ���λ��
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
			//���������λ��
			if (x == 0) {
				rcTmp->left = m_ptStart.x;
				rcTmp->right = m_ptStart.x+m_nFirstColWid;
			}
			else {
				rcTmp->left = m_ptStart.x+m_nFirstColWid+(x-1)*m_nGridWid;
				rcTmp->right = rcTmp->left + m_nGridWid;
			}
			//������ϡ���λ��
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
	if (point.x < m_arrGridRect[0].left                            //�ڶ������
		|| point.x > m_arrGridRect[m_nColunms-1].right             //���һ���ұ�
		|| point.y < m_arrGridRect[0].top                          //�ڶ��е��ϱ�
		|| point.y > m_arrGridRect[(m_nRows-1)*m_nColunms].bottom) //���һ�е��±�
	{
		return false;
	}

	return true;
}

int CVirtualSwitcher::LocateColunm(const CPoint point) const
{
	//�������쳣
	if (m_arrGridRect.GetCount() != m_nColunms*m_nRows) return INVALID_VALUE;

	//������Чѡ������֮��
	if (!IsMatrixClicked(point)) return INVALID_VALUE;

	//��ѡ
	CRect rect;
	int nFindindex = INVALID_VALUE;
	for (int i=0; i<m_nColunms; i++)
	{
		//ֻ���жϵ�һ��
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
	//�������쳣
	if (m_arrGridRect.GetCount() != m_nColunms*m_nRows) return INVALID_VALUE;

	//������Чѡ������֮��
	if (!IsMatrixClicked(point)) return INVALID_VALUE;

	//��ѡ
	CRect rect;
	int nFindindex = INVALID_VALUE;
	for (int i=0; i<m_nRows; i++)
	{
		//ֻ���жϵ�һ��
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

	if (point.x < m_arrGridRect[0].left                            //��һ�����
		|| point.x > m_arrGridRect[m_nColunms-1].right             //���һ���ұ�
		|| point.y < m_arrGridRect[0].top                          //��һ�е��ϱ�
		|| point.y > m_arrGridRect[(m_nRows-1)*m_nColunms].bottom) //���һ�е��±�
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