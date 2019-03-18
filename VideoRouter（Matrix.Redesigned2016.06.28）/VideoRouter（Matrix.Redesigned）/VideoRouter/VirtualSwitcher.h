#pragma once

//****************************************************************//
//                          ����������                            //
//----------------------------------------------------------------//
class CVirtualSwitcher
{
public:
	CVirtualSwitcher();
	CVirtualSwitcher(CPoint ptStart, int nFirstColWid, int nGridWid, int nGridHei, int nCol, int nRow);
	~CVirtualSwitcher();

	//���ݵ�ǰ����������Ⱥ͸߶ȣ���ʼ����������
	void Init();

	BOOL m_bLockUpdate;
	//���������������UnlockUpdate�ɶ�ʹ��
	void LockUpdate();
	//���������������������������
	void UnlockUpdate();

public:
	//--------------------------------------- ����������� -----------------------------------------//
	//��ȡָ�����������
	CRect GetGridRect(const int x, const int y) const;

	//��ȡ�����л�ʱ�������Ӧ����������ڣ� �����Ƿ����������ʵ������
	void GetClickedMatrix(const CPoint point, int &nRowIndex, int &nColIndex) const;

	//��ȡ�������һ���ϵ��к�
	int GetClickedRow(const CPoint point) const;


	//**********���·���ֻ����ͼ�����йأ��͹����޹�************//
	//--------------------------------------- ����������� ---------------------------------------//
	//��ȡ��һ���ź�Դ�еĿ��
	int GetFirstColWidth() const { return m_nFirstColWid; }
	//���õ�һ�п��
	void SetFirstColWidth(const int nWid);
	//��ȡ������
	int GetGridWidth() const { return m_nGridWid; }
	//����������
	void SetGridWidth(const int nWid);
	//��ȡ����߶�
	int GetGridHeight() const { return m_nGridHei; }	
	//��������߶�
	void SetGridHeight(const int nHei);

	//������߶Ȼ��ȱ仯ʱ����Ҫ���¸�����������꣬����ɾ����̬��������
	void UpdateGridRect();

	//��ȡ�������ĳߴ�
	CSize GetWholeGridSize() const { return m_nSize; }
	inline void SetWholeGridSize();

	//��ȡ�������
	CRect GetMatrixRect() const;

	//--------------------------------------- �������������� --------------------------------------//
	//��ȡ��������
	int GetColunmCount() const { return m_nColunms; }
	//��ȡ��������
	int GetRowCount() const { return m_nRows; }
	//������������
	void SetColunmCount(const int nCol);
	//������������
	void SetRowCount(const int nRow);
	//��ȡ��չ����
	int GetExtraColCount() const { return m_nExtraCol; }
	//��ȡ��չ����
	int GetExtraRowCount() const { return m_nExtraRow; }

	//--------------------------------------- �����ʼ�������� -----------------------------------//
	//���ñ������ͼ�е���ʼλ��
	void SetStartPoint(const CPoint ptStart) { m_ptStart = ptStart; }
	//��ȡ�������ͼ�е���ʼλ��
	CPoint GetStartPoint() const { return m_ptStart; }

	//--------------------------------------- ����ѡ������ ---------------------------------------------//
	//����ѡ�и�
	void SetFocusGrid(const CPoint ptFocus) ;
	//��ȡѡ�и�
	CPoint GetFocusGrid() const { return m_ptFocusGrid; }

private:
	CArray <CRect, CRect &> m_arrGridRect; //ά���������������
	int m_nFirstColWid;  //��һ�п��
	int m_nGridWid;      //��ͨ������
	int m_nGridHei;      //��ͨ����߶�
	int m_nColunms;      //�������� = ��չ���� + ʵ���������
	int m_nRows;         //�������� = ��չ���� + ʵ���������
	int m_nExtraCol;     //��չ��
	int m_nExtraRow;     //��չ��
	CSize m_nSize;       //�������ĳߴ� ����Ϊ��λ
	CPoint m_ptStart;    //������ʼ������

	CPoint m_ptFocusGrid;//ѡ�е����� x:�У�y:��

	//��ȡ��������кţ���ʵ���к�
	int LocateColunm(const CPoint point) const;
	//��ȡ��������кţ���ʵ���к�
	int LocateRow(const CPoint point) const;
	//����Ƿ�������Ч��Χ��
	bool IsMatrixClicked(const CPoint point) const;
};
