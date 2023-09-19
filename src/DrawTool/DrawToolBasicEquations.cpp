/**
MIT License
Copyright (c) 2015-2023 bbqz007 <https://github.com/bbqz007, http://www.cnblogs.com/bbqzsl>
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////
//  Title: Tool for Drawing Assistant Line and Assistant Graph
//  Auth: dennis. Z.
//  Date: June, July 2012
//
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DrawToolBasic.h"
#include "DrawToolBasicUtil.h"

DTEquation::~DTEquation()
{
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Line Equation
int DTLineEquation::HitTest(const tagDTPoint& _Point)
{
	if(m_bVertical)
		return _Point.x == m_eOffsetX;

	float _y = _Point.x * m_eCoefficentA + m_eCoefficentB;
	return (CMP_WITH_PERMISSIBLE_ERROR(_y, (float)_Point.y, 2.0)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
}

int DTLineEquation::Retrieve(const tagDTRect& _ViewRect, tagDTLine& _ValidLine)
{
	if(m_bVertical)
	{
		_ValidLine.ptBegin.x = m_eOffsetX;
		_ValidLine.ptBegin.y = _ViewRect.top;
		_ValidLine.ptEnd.x = m_eOffsetX;
		_ValidLine.ptEnd.y = _ViewRect.bottom;
	}
	else
	{
		_ValidLine.ptBegin.x = _ViewRect.left;
		_ValidLine.ptBegin.y = _ValidLine.ptBegin.x * m_eCoefficentA + m_eCoefficentB;
		_ValidLine.ptEnd.x = _ViewRect.right;
		_ValidLine.ptEnd.y = _ValidLine.ptEnd.x * m_eCoefficentA + m_eCoefficentB;
	}
	return 0;
}

int DTLineEquation::MoveTo(const tagDTPoint& _Point)
{
	m_eCoefficentB = _Point.y - _Point.x * m_eCoefficentA;
	m_eOffsetX = _Point.x;
	m_eOffsetY = _Point.y;
	return 0;
}

int DTLineEquation::Offset(long _delta_X, long _delta_Y)
{
	m_eOffsetX += _delta_X;
	m_eOffsetY += _delta_Y;
	return 0;
}

int DTLineEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	tagDTLine _ValidLine;
	Retrieve(_Rect, _ValidLine);
	::MoveToEx(_hdc, _ValidLine.ptBegin.x, _ValidLine.ptBegin.y, (LPPOINT) NULL);
	::LineTo(_hdc, _ValidLine.ptEnd.x, _ValidLine.ptEnd.y);
	return 0;
}

int DTLineEquation::Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB)
{
	float _DistanceOfYAxis = _PointB.y - _PointA.y;
	float _DistanceOfXAxis = _PointB.x - _PointA.x;

	if(_DistanceOfXAxis > -1e-6 && _DistanceOfXAxis < 1e-6)
	{
		m_eCoefficentA = ::tanf(g_ConstPi / 2);
		m_eCoefficentB = 0;
		m_eOffsetX = _PointA.x;
		m_eOffsetY = _PointA.y;
		m_bVertical = true;
	}
	else
	{
		m_eCoefficentA = _DistanceOfYAxis / _DistanceOfXAxis;
		MoveTo(_PointA);
		m_bVertical = false;
	}
	
	return 0;
}	

int DTLineEquation::Build(float _eCoefficentA, float _eCoefficentB)
{
	m_eCoefficentA = _eCoefficentA;
	m_eCoefficentB = _eCoefficentB;
	m_eOffsetX = 0;
	m_eOffsetY = m_eCoefficentB;
	m_bVertical = false;
	return 0;
}

int DTLineEquation::BuildPerpendicular(const tagDTPoint& _PointA, const tagDTPoint& _PointB)
{
	if(_PointA.x == _PointB.x)
	{
		m_eCoefficentA = 0;
		m_eCoefficentB = (float)(_PointA.y + _PointB.y) / 2;
		m_eOffsetX = _PointA.x;
		m_eOffsetY = m_eCoefficentB;
		m_bVertical = false;
	}
	else if(_PointA.y == _PointB.y)
	{
		m_bVertical = true;
		m_eCoefficentA = FLOAT_POSITIVE_LIMIT;
		m_eCoefficentB = FLOAT_POSITIVE_LIMIT;
		m_eOffsetX = (_PointA.x + _PointB.x) / 2;
		m_eOffsetY = _PointA.y;
	}
	else
	{
		m_eCoefficentA = (double)(_PointB.x - _PointA.x) / (_PointA.y - _PointB.y);
		m_eCoefficentB = (::pow((double)_PointA.x, 2) - ::pow((double)_PointB.x, 2) + ::pow((double)_PointA.y, 2) - ::pow((double)_PointB.y, 2)) / (2 * (_PointA.y - _PointB.y));
		m_eOffsetX = (_PointA.x + _PointB.x) / 2;
		m_eOffsetY = (_PointA.y + _PointB.y) / 2;
		m_bVertical = false;
	}
	return 0;
}

int DTLineEquation::IntersectAt(DTLineEquation& _OtherLine, tagDTPoint& _PointAt)
{
	if(m_bVertical && _OtherLine.IsVertical())
		return -1;

	if((m_eCoefficentA - _OtherLine.m_eCoefficentA) > -1e-4 && (m_eCoefficentA - _OtherLine.m_eCoefficentA) < -1e+4)
		return -1;

	if(m_bVertical)
	{
		_PointAt.x = m_eOffsetX;
		_PointAt.y = _OtherLine.m_eCoefficentA * _PointAt.x + _OtherLine.m_eCoefficentB;
	}
	else if(_OtherLine.IsVertical())
	{
		_PointAt.x = _OtherLine.m_eOffsetX;
		_PointAt.y = m_eCoefficentA * _PointAt.x + m_eCoefficentB;
	}
	else
	{
		_PointAt.x = (m_eCoefficentB - _OtherLine.m_eCoefficentB) / (_OtherLine.m_eCoefficentA - m_eCoefficentA);
		_PointAt.y = (m_eCoefficentB * _OtherLine.m_eCoefficentA - _OtherLine.m_eCoefficentB * m_eCoefficentA) / (_OtherLine.m_eCoefficentA - m_eCoefficentA);
	}
	return 0;
}

int DTLineEquation::Perpendicular()
{
	if(m_bVertical)
	{
		m_eCoefficentA = 0;
		m_bVertical = false;
	}
	else if(m_eCoefficentA > -1e-2 && m_eCoefficentA < 1e-2)
	{
		m_eCoefficentA = ::tanf(g_ConstPi / 2);
		m_bVertical = true;
	}
	else
	{
		m_eCoefficentA = -1 / m_eCoefficentA;
		m_eCoefficentB = m_eOffsetY - m_eOffsetX * m_eCoefficentA;
		m_bVertical = false;
	}
	return 0;
}

bool DTLineEquation::IsVertical()
{
	return m_bVertical;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vertical Equation
int DTVerticalEquation::HitTest(const tagDTPoint& _Point)
{
	return (_Point.x == m_eOffsetX) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
}

int DTVerticalEquation::Retrieve(const tagDTRect& _ViewRect, tagDTLine& _ValidLine)
{
	_ValidLine.ptBegin.x = m_eOffsetX;
	_ValidLine.ptBegin.y = _ViewRect.top;
	_ValidLine.ptEnd.x = m_eOffsetX;
	_ValidLine.ptEnd.y = _ViewRect.bottom;
	return 0;
}

int DTVerticalEquation::MoveTo(const tagDTPoint& _Point)
{
	m_eOffsetX = _Point.x;
	return 0;
}

int DTVerticalEquation::Offset(long _delta_X, long _delta_Y)
{
	m_eOffsetX += _delta_X;
	return 0;
}

int DTVerticalEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	tagDTLine _ValidLine;
	Retrieve(_Rect, _ValidLine);
	::MoveToEx(_hdc, _ValidLine.ptBegin.x, _ValidLine.ptBegin.y, (LPPOINT) NULL);
	::LineTo(_hdc, _ValidLine.ptEnd.x, _ValidLine.ptEnd.y);
	return 0;
}

int DTVerticalEquation::Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB)
{
	MoveTo(_PointA);
	return 0;
}	


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Horizon Equation
int DTHorizonEquation::HitTest(const tagDTPoint& _Point)
{
	return (_Point.y == m_eOffsetY) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
}

int DTHorizonEquation::Retrieve(const tagDTRect& _ViewRect, tagDTLine& _ValidLine)
{
	_ValidLine.ptBegin.x = _ViewRect.left;
	_ValidLine.ptBegin.y = m_eOffsetY;
	_ValidLine.ptEnd.x = _ViewRect.right;
	_ValidLine.ptEnd.y = m_eOffsetY;
	return 0;
}

int DTHorizonEquation::MoveTo(const tagDTPoint& _Point)
{
	m_eOffsetY = _Point.y;
	return 0;
}

int DTHorizonEquation::Offset(long _delta_X, long _delta_Y)
{
	m_eOffsetY += _delta_Y;
	return 0;
}

int DTHorizonEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	tagDTLine _ValidLine;
	Retrieve(_Rect, _ValidLine);
	::MoveToEx(_hdc, _ValidLine.ptBegin.x, _ValidLine.ptBegin.y, (LPPOINT) NULL);
	::LineTo(_hdc, _ValidLine.ptEnd.x, _ValidLine.ptEnd.y);
	return 0;
}

int DTHorizonEquation::Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB)
{
	MoveTo(_PointA);
	return 0;
}	


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ellipse Arc Equation
int DTEllipseArcEquation::HitTest(const tagDTPoint& _Point)
{
	switch(m_Direct)
	{
	case ENUM_ASSIST_DIRECT_UNKNOWN:
	case ENUM_ASSIST_DIRECT_LEFT:
	case ENUM_ASSIST_DIRECT_UP:
	case ENUM_ASSIST_DIRECT_RIGHT:
	case ENUM_ASSIST_DIRECT_DOWN:
		return ENUM_ASSIST_HITTEST_NULL;
		break;
	default:
		break;
	}

	float _tmp = m_eCoefficentB * ::sqrtf(1 - ::powf((float)(_Point.x - m_eOffsetX), 2) / ::powf((float)m_eCoefficentA, 2));
	tagDTRect _Rect;
	_Rect.left = m_eOffsetX - m_eCoefficentA;
	_Rect.top = m_eOffsetY - m_eCoefficentB;
	_Rect.right = m_eOffsetX + m_eCoefficentA;
	_Rect.bottom = m_eOffsetY + m_eCoefficentB;

	switch(m_Direct)
	{
	case ENUM_ASSIST_DIRECT_RIGHTUP:
		_Rect.left += m_eCoefficentA;
		if(::PtInRect(&_Rect, _Point))
		{
			float _yh = (float) m_eOffsetY - _tmp;
			float _yl = (float) m_eOffsetY + _tmp;
			return (CMP_WITH_PERMISSIBLE_ERROR(_yh, (float)_Point.y, 2.0) || CMP_WITH_PERMISSIBLE_ERROR(_yl, (float)_Point.y, 2.0)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
		}
		break;
	case ENUM_ASSIST_DIRECT_LEFTDOWN:
		_Rect.right -= m_eCoefficentA;
		if(::PtInRect(&_Rect, _Point))
		{
			float _yh = (float) m_eOffsetY - _tmp;
			float _yl = (float) m_eOffsetY + _tmp;
			return (CMP_WITH_PERMISSIBLE_ERROR(_yh, (float)_Point.y, 2.0) || CMP_WITH_PERMISSIBLE_ERROR(_yl, (float)_Point.y, 2.0)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
		}
		break;
	case ENUM_ASSIST_DIRECT_LEFTUP:
		_Rect.bottom -= m_eCoefficentB;
		if(::PtInRect(&_Rect, _Point))
		{
			float _yh = (float) m_eOffsetY - _tmp;
			return (CMP_WITH_PERMISSIBLE_ERROR(_yh, (float)_Point.y, 2.0)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
		}
		break;
	case ENUM_ASSIST_DIRECT_RIGHTDOWN:
		_Rect.top += m_eCoefficentB;
		if(::PtInRect(&_Rect, _Point))
		{
			float _yl = (float) m_eOffsetY + _tmp;
			return (CMP_WITH_PERMISSIBLE_ERROR(_yl, (float)_Point.y, 2.0)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
		}
		break;
	default:
		break;
	}
	
	return ENUM_ASSIST_HITTEST_NULL;
}

int DTEllipseArcEquation::MoveTo(const tagDTPoint& _Point)
{
	m_eOffsetY = _Point.y;
	m_eOffsetX = _Point.x;
	switch(m_Direct)
	{
	case ENUM_ASSIST_DIRECT_RIGHTUP:
		m_eOffsetY -= m_eCoefficentB;
		break;
	case ENUM_ASSIST_DIRECT_LEFTDOWN:
		m_eOffsetY += m_eCoefficentB;
		break;
	case ENUM_ASSIST_DIRECT_LEFTUP:
		m_eOffsetX -= m_eCoefficentA;
		break;
	case ENUM_ASSIST_DIRECT_RIGHTDOWN:
		m_eOffsetX += m_eCoefficentA;
		break;
	case ENUM_ASSIST_DIRECT_UNKNOWN:
		break;
	}
	return 0;
}

int DTEllipseArcEquation::Offset(long _delta_X, long _delta_Y)
{
	m_eOffsetX += _delta_X;
	m_eOffsetY += _delta_Y;
	return 0;
}

int DTEllipseArcEquation::Retrieve(const tagDTRect& _ViewRect, tagDTRect& _Rect, tagDTLine& _ValidLine)
{
	switch(m_Direct)
	{
	case ENUM_ASSIST_DIRECT_UNKNOWN:
	case ENUM_ASSIST_DIRECT_LEFT:
	case ENUM_ASSIST_DIRECT_UP:
	case ENUM_ASSIST_DIRECT_RIGHT:
	case ENUM_ASSIST_DIRECT_DOWN:
		return -1;
		break;
	default:
		break;
	}

	_Rect.left = m_eOffsetX - m_eCoefficentA;
	_Rect.top = m_eOffsetY - m_eCoefficentB;
	_Rect.right = m_eOffsetX + m_eCoefficentA;
	_Rect.bottom = m_eOffsetY + m_eCoefficentB;

	switch(m_Direct)
	{
	case ENUM_ASSIST_DIRECT_RIGHTUP:
		_ValidLine.ptBegin.x = m_eOffsetX;
		_ValidLine.ptBegin.y = _Rect.bottom;
		_ValidLine.ptEnd.x = m_eOffsetX;
		_ValidLine.ptEnd.y = _Rect.top;
		break;
	case ENUM_ASSIST_DIRECT_LEFTDOWN:
		_ValidLine.ptBegin.x = m_eOffsetX;
		_ValidLine.ptBegin.y = _Rect.top;
		_ValidLine.ptEnd.x = m_eOffsetX;
		_ValidLine.ptEnd.y = _Rect.bottom;
		break;
	case ENUM_ASSIST_DIRECT_LEFTUP:
		_ValidLine.ptBegin.x = _Rect.right;
		_ValidLine.ptBegin.y = m_eOffsetY;
		_ValidLine.ptEnd.x = _Rect.left;
		_ValidLine.ptEnd.y = m_eOffsetY;
		break;
	case ENUM_ASSIST_DIRECT_RIGHTDOWN:
		_ValidLine.ptBegin.x = _Rect.left;
		_ValidLine.ptBegin.y = m_eOffsetY;
		_ValidLine.ptEnd.x = _Rect.right;
		_ValidLine.ptEnd.y = m_eOffsetY;
		break;
	default:
		return -1;
		break;
	}

	return 0;
}

int DTEllipseArcEquation::Render(HDC _hdc, const tagDTRect& _ViewRect)
{
	tagDTLine _ValidLine;
	tagDTRect _Rect;
	if(ISDTOK(Retrieve(_ViewRect, _Rect, _ValidLine)))
	{
		::SetArcDirection(_hdc, AD_COUNTERCLOCKWISE);
		::Arc(_hdc, _Rect.left, _Rect.top, _Rect.right, _Rect.bottom, _ValidLine.ptBegin.x, _ValidLine.ptBegin.y, _ValidLine.ptEnd.x, _ValidLine.ptEnd.y);
	}
	return 0;
}

int DTEllipseArcEquation::Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB)
{
	long _delta_X = _PointB.x - _PointA.x;
	long _delta_Y = _PointB.y - _PointA.y;

	m_Direct = ENUM_ASSIST_DIRECT_UNKNOWN;
	if(_delta_X < 0)
	{
		if(_delta_Y < 0)
		{
			m_Direct = ENUM_ASSIST_DIRECT_LEFTUP;
		}
		else if(_delta_Y > 0)
		{
			m_Direct = ENUM_ASSIST_DIRECT_LEFTDOWN;
		}
	}
	else if(_delta_X > 0)
	{
		if(_delta_Y < 0)
		{
			m_Direct = ENUM_ASSIST_DIRECT_RIGHTUP;
		}
		else if(_delta_Y > 0)
		{
			m_Direct = ENUM_ASSIST_DIRECT_RIGHTDOWN;
		}
	}
	else
	{
		if(_delta_Y < 0)
		{
			m_Direct = ENUM_ASSIST_DIRECT_UP;
		}
		else if(_delta_Y > 0)
		{
			m_Direct = ENUM_ASSIST_DIRECT_DOWN;
		}
	}

	if(_delta_Y == 0)
	{
		if(_delta_X < 0)
		{
			m_Direct = ENUM_ASSIST_DIRECT_LEFT;
		}
		else if(_delta_X > 0)
		{
			m_Direct = ENUM_ASSIST_DIRECT_RIGHT;
		}
	}

	m_eCoefficentA = ::abs(_delta_X);
	m_eCoefficentB = ::abs(_delta_Y);
	MoveTo(_PointA);

	return 0;
}	


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Circle Equation
int DTCircleEquation::HitTest(const tagDTPoint& _Point)
{
	if(!m_bAvaiable)
		return ENUM_ASSIST_HITTEST_NULL;

	float _tmp = ::sqrtf(::powf((float)m_eCoefficentR, 2) - ::powf((float)(_Point.x - m_eOffsetX), 2));
	float _yh = (float) m_eOffsetY + _tmp;
	float _yl = (float) m_eOffsetY - _tmp;
	return (CMP_WITH_PERMISSIBLE_ERROR(_yh, (float)_Point.y, 2.0) || CMP_WITH_PERMISSIBLE_ERROR(_yl, (float)_Point.y, 2.0)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
}

int DTCircleEquation::MoveTo(const tagDTPoint& _Point)
{
	m_eOffsetY = _Point.y;
	m_eOffsetX = _Point.x;
	return 0;
}

int DTCircleEquation::Offset(long _delta_X, long _delta_Y)
{
	m_eOffsetX += _delta_X;
	m_eOffsetY += _delta_Y;
	return 0;
}

int DTCircleEquation::Render(HDC _hdc, const tagDTRect& _ViewRect)
{
	tagDTRect _Rect;
	tagDTLine _ValidLine;
	if(ISDTOK(Retrieve(_ViewRect, _Rect, _ValidLine)))
	{
		::SetArcDirection(_hdc, AD_COUNTERCLOCKWISE);
		::Arc(_hdc, _Rect.left, _Rect.top, _Rect.right, _Rect.bottom, _ValidLine.ptBegin.x, _ValidLine.ptBegin.y, _ValidLine.ptEnd.x, _ValidLine.ptEnd.y);
	}
	return 0;
}

int DTCircleEquation::Retrieve(const tagDTRect& _ViewRect, tagDTRect& _Rect, tagDTLine& _ValidLine)
{
	if(!m_bAvaiable)
		return -1;

	_Rect.left = m_eOffsetX - m_eCoefficentR;
	_Rect.top = m_eOffsetY - m_eCoefficentR;
	_Rect.right = m_eOffsetX + m_eCoefficentR;
	_Rect.bottom = m_eOffsetY + m_eCoefficentR;

	_ValidLine.ptBegin.x = _Rect.left;
	_ValidLine.ptBegin.y = m_eOffsetY;
	_ValidLine.ptEnd.x = _ValidLine.ptBegin.x;
	_ValidLine.ptEnd.y = _ValidLine.ptBegin.y;
	return 0;
}

int DTCircleEquation::Build(const tagDTPoint& _PointO, const tagDTPoint& _PointA)
{
	if(_PointO.x == _PointA.x && _PointO.y == _PointA.y)
	{
		m_bAvaiable = false;
	}
	else
	{
		m_eOffsetX = _PointO.x;
		m_eOffsetY = _PointO.y;
		m_eCoefficentR = DTEquationHelper::DistanceBetweenTowPoints(_PointA, _PointO);
		m_bAvaiable = true;
	}
	return 0;
}

int DTCircleEquation::Build(const tagDTPoint& _PointA, const tagDTPoint& _PointB, const tagDTPoint& _PointC)
{
	DTLineEquation _LineAtoB;
	DTLineEquation _LineAtoC;
	tagDTPoint _PointAt;
	//tagDTPoint _PointMAB = { ::abs(_PointA.x + _PointB.x) / 2, ::abs(_PointA.y + _PointB.y) / 2 };
	//tagDTPoint _PointMAC = { ::abs(_PointA.x + _PointC.x) / 2, ::abs(_PointA.y + _PointC.y) / 2 };

	//_LineAtoB.Build(_PointA, _PointB);
	//_LineAtoB.Perpendicular();
	//_LineAtoB.MoveTo(_PointMAB);

	//_LineAtoC.Build(_PointA, _PointC);
	//_LineAtoC.Perpendicular();
	//_LineAtoC.MoveTo(_PointMAC);

	_LineAtoB.BuildPerpendicular(_PointA, _PointB);
	_LineAtoC.BuildPerpendicular(_PointA, _PointC);

	if(!ISDTOK(_LineAtoB.IntersectAt(_LineAtoC, _PointAt)))
	{
		m_bAvaiable = false;
	}
	else
	{
		m_eOffsetX = _PointAt.x;
		m_eOffsetY = _PointAt.y;
		m_eCoefficentR = (DTEquationHelper::DistanceBetweenTowPoints(_PointA, _PointAt)
							+ DTEquationHelper::DistanceBetweenTowPoints(_PointB, _PointAt)
							+ DTEquationHelper::DistanceBetweenTowPoints(_PointC, _PointAt)) / 3;
		m_bAvaiable = true;
		
	}
	return 0;
}
