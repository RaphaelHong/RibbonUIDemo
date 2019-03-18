#pragma once

#define FORWARD		0
#define INVERT		1

struct CGridSetting
{
	int fInvert;//1 revert, from output to input; 0 normal from input to output
	int cy;//lines ����
	int cx;//cols  ����

	int cxResIn;//input resolution   -- input Hactive
	int cyResIn;//input resolution   -- input Vactive
	int cxResOut;//output resolution -- output Hactive
	int cyResOut;//output resolution -- output Vactive
	int nHz;     //output resolution -- output Hz

	int cx4PointGraphSize;	//�ĵ�У������ͼ���Ĵ�С���������ϵĳߴ� 
	int cy4PointGraphSize;	 
	int cxCurveGraphSize;   //����У������ͼ���Ĵ�С���������ϵĳߴ�
	int cyCurveGraphSize;
	int cxDensityGraphSize; //���ܶ�У������ͼ���Ĵ�С���������ϵĳߴ�
	int cyDensityGraphSize;
	int cxComplexCurveGraphSize;  //���У������ͼ���Ĵ�С���������ϵĳߴ�
	int cyComplexCurveGraphSize;

	double cxScaleIn;//input scale    -- input_hactive/output_hactive
	double cyScaleIn;//input scale    -- input_vactive/output_vactive
	double cxScaleOut;//output scale  -- output_hactive/input_hactive
	double cyScaleOut;//output scale  -- output_vactive/input_vactive
public:
	CGridSetting();
};

/////////////////////////////////////////////////////////////////////////////
// CDoublePoint 
/////////////////////////////////////////////////////////////////////////////
struct CDoublePoint
{
	double x;
	double y;
public:
	CDoublePoint(double x=0, double y=0);
	bool operator==(CDoublePoint point) const throw();
};

class CDoublePointArray
{
protected:
	int m_xDim;
	int m_yDim;
public:
	CDoublePoint* m_pPoints;
public:
	CDoublePointArray(int xDim, int yDim);
	CDoublePointArray* Copy();
	virtual ~CDoublePointArray();
	//************************************
	// Method:    Size
	// FullName:  CDoublePointArray::Size
	// Access:    public 
	// Returns:   CSize
	// Qualifier: const
	//************************************
	CSize Size() const;
public:
	CDoublePoint Val(int x, int y) const;
	//vector 
	CDoublePoint& SetVal_X(int x, int y,double value) ;
	//vector 
	CDoublePoint& SetVal_Y(int x, int y,double value) ;

	CDoublePoint& Val(int x, int y);
	double X(int x, int y) const;
	double& X(int x, int y);
	double Y(int x, int y) const;
	double& Y(int x, int y);
	double XDistance(int x0, int y0, int x1, int y1);
	double YDistance(int x0, int y0, int x1, int y1);
};


struct CProperty
{
	COLORREF clrBackground;
	COLORREF clrForeground;
	COLORREF clrCursor;
	int		nLineWidth;
	int		nBlock;
	int		nSelectedBlock;

	enum	{shapeRect, shapeCross, shapeHollow };
	int		nShape;
	double	fArrowKeyStep;
	int     GD_MoveMode;
	int		GD_FitMode;

	BOOL GD_Move_Left_Sel;
	BOOL GD_Move_Right_Sel;
	BOOL GD_Move_Top_Sel;
	BOOL GD_Move_Bottom_Sel;

	BOOL GD_Fit_X_Sel;
	BOOL GD_Fit_Y_Sel;

	BOOL clrForeground_R;
	BOOL clrForeground_G;
	BOOL clrForeground_B;

	BOOL GD_Move_XYLink_Sel;

	CString strGD_Background;

public:
	CProperty();
};