#pragma once
#include "NumberEdit.h"

typedef enum __emGridWarpStyle
{
	eStyle_Keystone,
	eStyle_InterKeystone,
	eStyle_InterRow,
	eStyle_InterCol,
	eStyle_FullRow,
	eStyle_FullCol,
	eStyle_SinglePoint
} emGridWarpStyle; //���У���ļ��ַ�ʽ

typedef struct tagRecord
{
	CDoublePoint dpaData[65*65];
} UndoRecord, RedoRecord;

#define MAX_RECORD 20

// CMGgridwarpDlg �Ի���

class CMGgridwarpDlg : public CXTPResizeDialog
{
	DECLARE_DYNAMIC(CMGgridwarpDlg)

public:
	CMGgridwarpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMGgridwarpDlg();

// �Ի�������
	enum { IDD = IDD_DLG_MG_GRIDWARP };

private:
	CXTPButton m_GboxFuncCtrl;
	CXTPButton m_GboxAjustCtrl;
	CXTPButton m_GboxBlanceCtrl;
	CXTPButton m_GboxCursorPosCtrl;
	CXTPButton m_GboxGridLinesCtrl;

	CXTPButton m_BtnLoadGeoData;
	CXTPButton m_BtnUndoGrid;
	CXTPButton m_BtnRedoGrid;
	CXTPButton m_BtnResetGrid;

	CXTPButton m_BtnSwitchAjustMode;
	CXTPButton m_CheckBlanceAll;
	CXTPButton m_CheckBlanceLeft;
	CXTPButton m_CheckBlanceUpper;
	CXTPButton m_CheckBlanceRight;
	CXTPButton m_CheckBlanceDown;

	CXTPButton m_CheckSmoothGrid;

	CXTPButton m_BtnSaveAndLoad;

	CNumberEdit m_EditCursorPosX;
	CNumberEdit m_EditCursorPosY;

	CXTPComboBox m_CBoxIncrement;

	CXTPButton m_BtnDoubleGridRow;
	CXTPButton m_BtnHalfGridRow;
	CXTPButton m_BtnDoubleGridCol;
	CXTPButton m_BtnHalfGridCol;

	CBrush m_BkgBrush;  //������ˢ

	//���ڿ�����˸�ı��ı���
	COLORREF m_clrTipText; //�ı���ǰ����ɫ
	int m_nColorCount;     //��ɫ������
	BOOL m_bDecAsc;        //�������ݼ���־

	void ResizeIt();  //��̬�������ںͿؼ��Ĵ�С��λ��
	void SetControlTheme(XTPControlTheme theme);
	void SetButtonIcons();
	void SetToolTips();

private:
	// ��ͼ
	CRect m_rcPaint; //��ͼ����С
	CRect m_rcChart; //ʵ�ʹ�����
	int m_nDeflate;  //����������һ���ߴ�

	double m_xScale;  //GraphicsSize_X / HActive
	double m_yScale;  //GraphicsSize_Y / VActive

	void GetPaintRect();   //��ȡ��ͼ����С
	void SetGraphicSize(); //��������������Ϣ

	void DrawBkgGrid(CDC *pValidDC);           //������
	void DrawDataGrid(CDC *pValidDC);          //������
	void DrawKeystone(CDC *pValidDC);          //������У��
	void DrawInterKeystone(CDC *pValidDC);     //�������ĵ�
	void DrawHighLightRow(CDC *pValidDC);      //������ʾ��
	void DrawHighLightCol(CDC *pValidDC);      //������ʾ��
	void DrawOneRow(CDC *pValidDC);            //��һ��
	void DrawOneCol(CDC *pValidDC);            //��һ��
	void DrawSinglePoint(CDC *pValidDC);       //�������Ƶ�
	void DrawCrossLine(CDC *pValidDC);         //�����ƶ�ʱ��ʾ�����к���
	void DrawResolution(CDC *pValidDC);        //�ֱ���
	void DrawTipText(CDC *pValidDC);           //��ʾ�û���ô����
	void DrawSelectRectangle(CDC *dc, CPoint ptBegin, CPoint ptEnd); //�����ĵ�ѡȡ��ʱ�����ѡȡ��Ĺ켣
	inline void DrawLine(CDC *dc, CPoint ptLineto, int xInc=0, int yInc=0);
	inline void DrawEllipse(CDC *dc, CPoint ptCenter, int nRadius);

private:
	CGridSetting m_gridsetting;  //����������Ϣ
	CDoublePointArray *m_pDpaGridWarpPoints; //������
	emGridWarpStyle m_TrimMode; //У����ʽ

	void InitDpaData();	   //��ʼ��Dpa����
	void ResetDpaData();   //��λDpa����
	CDoublePoint OrignalPos(int x, int y) const;
	CPoint GetGridIndex(CPoint ptLogic) const;  //�����߼������ȡ��ǰ���Ƶ������������

	CPoint m_ptCursor; //��ǰ���Ƶ������������
	CDoublePoint m_dpaCursorPos; //��ǰ���Ƶ����������
	double m_CursorOffsetX; //��ǰ���Ƶ��ƶ���ˮƽ����
	double m_CursorOffsetY; //��ǰ���Ƶ��ƶ��Ĵ�ֱ����

	BOOL m_bLBtnDownAtGridPoint; //�������Ƿ������������
	BOOL IsLBtnDownAtGridPoint(CPoint point); //�������Ƿ������������

	BOOL m_bLBtnDown;    //����������
	BOOL m_bMouseMoving; //��������ƶ�

	//�����ĵ�����йصı���
	CPoint m_ptBegin;             //����������ʱ��λ��
	CPoint m_ptEnd;               //��굱ǰ��λ��
	CRect m_KeyRect;              //�����ĵ�У�����ĸ����λ�ã����кţ�
	BOOL m_bBeginSelectKeyRect;   //�Ƿ�ʼѡȡ�ĵ�����
	void SelectInterKeystoneRect(CPoint ptEnd); //ѡȡ�ĵ�����

	BOOL m_bBlanceLeft;   //��߾���
	BOOL m_bBlanceUpper;  //�Ϸ�����
	BOOL m_bBlanceRight;  //�ұ߾���
	BOOL m_bBlanceDown;   //�·�����

	void OnAjust(CDoublePoint point);  //У������	

	void Warp_Auto_Kestone(int left, int top, int right, int bottom);  //����У��
	void Warp_Auto_RowX(int nRow);  //�о���
	void Warp_Auto_RowY(int nCol);  //�о���

	void BlanceLeft();  //�������
	void BlanceUpper(); //���Ͼ���
	void BlanceRight(); //���Ҿ���
	void BlanceDown();  //���¾���

	void FullRowMove(); //�ƶ�һ��
	void FullColMove(); //�ƶ�һ��

	void Warp_Auto_InterKeystone();  //�����ĵ�У��

public:
	void Initialize();         //���豸��ȡ���ݳ�ʼ��
	void ShowCursorCoords();   //��ʾ��ǰ���Ƶ������
	void OnCustomizedCoords(); //�ֶ���������ֵ
	void OnVkArrowSwitchCursorPoint(UINT nVKcode); //�л����Ƶ�
	void OnVkArrowMoveCursorPoint(UINT nVKcode);   //�÷�����ƶ����Ƶ�
	inline int GetBoardPort();    //��ȡ�����忨��ͨ�Ŷ˿ں�
	inline void Packaging(TaskPackage &pack, int nMode); //�������װ

	double m_dIncrement;
	int  m_nSmooth; //�Ƿ�ƽ������

private:
	int SaveGridFile();  //�����У�����ݱ��浽�����ļ�  return: 0��ʧ�� 1���ɹ� 2��ȡ��
	int ReadGridFile();  //�ӱ����ļ���ȡ�������        return: 0��ʧ�� 1���ɹ� 2��ȡ��
	bool WriteGridData(); //�����ݱ��浽�豸
	bool LoadGridData();  //���豸��ȡ����

private:
	CList <UndoRecord, UndoRecord&> m_UndoList;
	CList <RedoRecord, RedoRecord&> m_RedoList;

	void AddRecord();
	void Undo();
	void Redo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnAjustStyleDropDown();
	afx_msg void OnBtnLoadAndSaveDropDown();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCommandAjustStyle(UINT nCmdID);
	afx_msg void OnCommandLoadAndSave(UINT nCmdID);
	afx_msg void OnCheckBlanceAllDirections();
	afx_msg void OnCheckBlanceLeft();
	afx_msg void OnCheckBlanceUpper();
	afx_msg void OnCheckBlanceRight();
	afx_msg void OnCheckBlanceDown();
	afx_msg void OnCheckGridSmooth();
	afx_msg void OnKeystoneSelect();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCbnSelChangeIncrement();
	afx_msg void OnBtnClickedReset();
	afx_msg void OnBtnClickedUndo();
	afx_msg void OnBtnClickedRedo();
	afx_msg void OnBtnClickedLoadGeoData();
	afx_msg void OnBtnClickedDoubleCols();
	afx_msg void OnBtnClickedHalfCols();
	afx_msg void OnBtnClickedDoubleRows();
	afx_msg void OnBtnClickedHalfRows();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
