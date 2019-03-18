#include "StdAfx.h"

/////////////////////////////////////////////////////////////////////////////
// CGridSetting 
/////////////////////////////////////////////////////////////////////////////
CGridSetting::CGridSetting()
{
	fInvert = FORWARD;

	cy = 17;
	cx = 17;

	cxResIn = 1024;
	cyResIn = 768;
	cxResOut = 1024;
	cyResOut = 768;
	nHz  = 60;

	cx4PointGraphSize = 0;
	cy4PointGraphSize = 0;
	cxCurveGraphSize = 0;
	cyCurveGraphSize = 0;

	cxScaleIn = 1;
	cyScaleIn = 1;
	cxScaleOut = 1;
	cyScaleOut = 1;
}


/////////////////////////////////////////////////////////////////////////////
// CDoublePoint 
/////////////////////////////////////////////////////////////////////////////
CDoublePoint::CDoublePoint(double x, double y)
{
	this->x = x;
	this->y = y;
}

bool CDoublePoint::operator== (CDoublePoint point) const
{
	return (this->x == point.x && this->y == point.y);
}

/////////////////////////////////////////////////////////////////////////////
// CDoublePointArray
/////////////////////////////////////////////////////////////////////////////
CDoublePointArray::CDoublePointArray(int xDim, int yDim)//创建数值，大小为xDim*yDim
{
	ASSERT( xDim > 0 && yDim > 0 );
	m_xDim = xDim;
	m_yDim = yDim;
	m_pPoints = new CDoublePoint[xDim*yDim];
}

CDoublePointArray::~CDoublePointArray()
{
	delete []m_pPoints;
}

CSize CDoublePointArray::Size() const
{
	return CSize(m_xDim, m_yDim);
}

CDoublePointArray* CDoublePointArray::Copy()
{
	CDoublePointArray* pdpa2d = new CDoublePointArray(m_xDim,m_yDim);
	ASSERT( pdpa2d != NULL );

	memcpy( pdpa2d->m_pPoints, m_pPoints, m_xDim*m_yDim*sizeof(CDoublePoint));

	return pdpa2d;
}

double CDoublePointArray::X(int x, int y) const
{
	ASSERT( x<m_xDim && y<m_yDim);
	return m_pPoints[x+y*m_xDim].x;
}

double& CDoublePointArray::X(int x, int y)
{
	ASSERT( x<m_xDim && y<m_yDim);
	return m_pPoints[x+y*m_xDim].x;
}

double CDoublePointArray::Y(int x, int y) const
{
	ASSERT( x<m_xDim && y<m_yDim);
	return m_pPoints[x+y*m_xDim].y;
}

double& CDoublePointArray::Y(int x, int y)
{
	ASSERT( x<m_xDim && y<m_yDim);
	return m_pPoints[x+y*m_xDim].y;
}

CDoublePoint CDoublePointArray::Val(int x, int y) const
{
	ASSERT( x<m_xDim && y<m_yDim);
	ASSERT( x>=0 && y>=0);
	return m_pPoints[x+y*m_xDim];
}

CDoublePoint& CDoublePointArray::Val(int x, int y) 
{
	ASSERT( x<m_xDim && y<m_yDim);
	ASSERT( x>=0 && y>=0);
	return m_pPoints[x+y*m_xDim];
}

//vector 
CDoublePoint& CDoublePointArray::SetVal_X(int x, int y,double value) 
{
	ASSERT( x<m_xDim && y<m_yDim);
	ASSERT( x>=0 && y>=0);

	m_pPoints[x+y*m_xDim].x = value;
	return m_pPoints[x+y*m_xDim];
}
//vector 
CDoublePoint& CDoublePointArray::SetVal_Y(int x, int y,double value) 
{
	ASSERT( x<m_xDim && y<m_yDim);

	m_pPoints[x+y*m_xDim].y = value;
	return m_pPoints[x+y*m_xDim];
}

double CDoublePointArray::XDistance(int x0, int y0, int x1, int y1)
{
	ASSERT(x0<m_xDim && x1<m_xDim && y0<m_yDim && y1<m_yDim);

	return m_pPoints[x1+y1*m_xDim].x - m_pPoints[x0+y0*m_xDim].x;
}

double CDoublePointArray::YDistance(int x0, int y0, int x1, int y1)
{
	ASSERT(x0<m_xDim && x1<m_xDim && y0<m_yDim && y1<m_yDim);

	return m_pPoints[x1+y1*m_xDim].y - m_pPoints[x0+y0*m_xDim].y;
}

/////////////////////////////////////////////////////////////////////////////
// CProperty 
/////////////////////////////////////////////////////////////////////////////
CProperty::CProperty()
{
	clrBackground = RGB(0,0,0);	// background color (default to black)
	clrForeground = RGB(0,0,0);	// grid color (default to black)
	clrCursor = RGB(0,0,0);		

	nLineWidth = 1;
	
	nBlock = 6;;
	nSelectedBlock = 20;

	nShape = shapeRect;
	fArrowKeyStep = 0.5;

	GD_MoveMode = 0;
	GD_FitMode = 0;

	clrForeground_R      = TRUE;
	clrForeground_G      = TRUE;
	clrForeground_B      = TRUE;

	GD_Fit_X_Sel		 = FALSE;
	GD_Fit_Y_Sel		 = FALSE;

	GD_Move_Left_Sel     = FALSE;
	GD_Move_Right_Sel    = FALSE;
	GD_Move_Top_Sel      = FALSE;
	GD_Move_Bottom_Sel   = FALSE;

	GD_Move_XYLink_Sel   = FALSE;

	strGD_Background = _T("");
}
