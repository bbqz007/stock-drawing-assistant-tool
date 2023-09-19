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

DTAbsEquation::~DTAbsEquation()
{
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Equal Difference Vertical Equation On Absolute Coordinate
DTEqualDifferenceVerticalAbsEquation::DTEqualDifferenceVerticalAbsEquation(DTCoordinateTransform* Coord)
: _Coord(Coord)
{
	m_OffsetDate = 0;
	m_EqualDifference = 0;
}

int DTEqualDifferenceVerticalAbsEquation::HitTest(const tagDTAbsPoint& _Point)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	_HitTest = (_Point.xDate == m_OffsetDate) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL) 
	{
		long _index_BaseX = _Coord->IndexOfDate(m_OffsetDate);
		long _index_HitAbsX = _Coord->IndexOfDate(_Point.xDate);
		if(_index_BaseX > 0 && _index_HitAbsX > 0 
			&& _index_BaseX <= _index_HitAbsX 
			&& (m_EqualDifference != 0 && (_index_HitAbsX - _index_BaseX) % m_EqualDifference) == 0)
			_HitTest = ENUM_ASSIST_HITTEST_TARGET;
	}
	return _HitTest;
}

int DTEqualDifferenceVerticalAbsEquation::MoveTo(const tagDTAbsPoint& _Point)
{
	m_OffsetDate = _Point.xDate;
	return 0;
}

int DTEqualDifferenceVerticalAbsEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	if(m_EqualDifference == 0)
		return -1;

	long _SizeOfAbsPoints = _Coord->SizeOfAbsPoints();
	long _IndexOfBoundOnAbsCoord = _Coord->IndexOfBoundOnAbsCoord();
	long _IndexOfOriginOnAbsCoord = _Coord->IndexOfOriginOnAbsCoord();
	long _IndexOfDate = _Coord->IndexOfDate(m_OffsetDate);
	long _IndexOfBeginningOnAbsCoord = _IndexOfDate;

	if(_IndexOfDate < 0 || _SizeOfAbsPoints == 0)
		return -1;

	if(_IndexOfDate < _IndexOfOriginOnAbsCoord)
		_IndexOfBeginningOnAbsCoord = m_EqualDifference - ((_IndexOfOriginOnAbsCoord - _IndexOfDate)  % m_EqualDifference) + _IndexOfOriginOnAbsCoord;

	tagDTPoint _LbPoint = { LONG_POSITIVE_LIMIT, LONG_POSITIVE_LIMIT };
	tagDTPoint _Point = { LONG_POSITIVE_LIMIT, LONG_POSITIVE_LIMIT };
	DTVerticalEquation _VerticalEquation;
	for(long i = _IndexOfBeginningOnAbsCoord; i < _IndexOfBoundOnAbsCoord; i += m_EqualDifference)
	{
		_Point.x = _Coord->ViewXByIndex(i);
		_LbPoint.x = min(_LbPoint.x, _Point.x);
		_VerticalEquation.MoveTo(_Point);
		_VerticalEquation.Render(_hdc, _Rect);
	}

	if(IsShowLabel())
	{
		//::OutputDebugStringA("DTEqualDifferenceVerticalAbsEquation::ShowLabel");
		tagDTRect _TextRect;
		char _OutText[128];
		_TextRect.left = _LbPoint.x + m_LabelSpan;
		_TextRect.top = _Rect.top;
		_TextRect.right = _Rect.right;
		_TextRect.bottom = _TextRect.top + m_LabelFontSize;
		sprintf(_OutText, "Span %d", m_EqualDifference);
		
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
		::SetBkMode(_hdc, oldBkMode);
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gann Sequence Vertical Equation On Absolute Coordinate
DTGannSequenceVerticalAbsEquation::DTGannSequenceVerticalAbsEquation(DTCoordinateTransform* Coord)
: _Coord(Coord)
{
	m_OffsetDate = 0;
}

long DTGannSequenceVerticalAbsEquation::m_GannSequenceModel[enum_gann_seq_max] = 
{
	 0,  4,  8, 10, 13, 16, 20, 22, 24, 31, 36, 
	40,
	44, 49, 56, 58, 60, 64, 67, 70, 72, 76, 80
};

int DTGannSequenceVerticalAbsEquation::HitTest(const tagDTAbsPoint& _Point)
{
	int _Ret = ENUM_ASSIST_HITTEST_NULL;
	long _IndexOfDate = _Coord->IndexOfDate(m_OffsetDate);
	long _IndexOfTarget = _Coord->IndexOfDate(_Point.xDate);
	long _SizeOfAbsPoints = _Coord->SizeOfAbsPoints();

	if(_IndexOfTarget < 0)
		return _Ret;

	if(_IndexOfDate >= 0)
	{
		for(int i = 0; i < enum_gann_seq_max && (_IndexOfDate + m_GannSequenceModel[i]) < _SizeOfAbsPoints; ++i)
		{
			if(_IndexOfTarget == (_IndexOfDate + m_GannSequenceModel[i]))
			{
				_Ret = ENUM_ASSIST_HITTEST_TARGET;
				break;
			}
		}
	}

	return _Ret;
}

int DTGannSequenceVerticalAbsEquation::MoveTo(const tagDTAbsPoint& _Point)
{
	m_OffsetDate = _Point.xDate;
	return 0;
}

int DTGannSequenceVerticalAbsEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	long _SizeOfAbsPoints = _Coord->SizeOfAbsPoints();
	long _IndexOfBoundOnAbsCoord = _Coord->IndexOfBoundOnAbsCoord();
	long _IndexOfOriginOnAbsCoord = _Coord->IndexOfOriginOnAbsCoord();
	long _IndexOfDate = _Coord->IndexOfDate(m_OffsetDate);

	if(_IndexOfDate < 0 || _SizeOfAbsPoints == 0)
		return -1;

	tagDTRect _TextRect;
	char _OutText[16];
	tagDTPoint _Point;
	DTVerticalEquation _VerticalEquation;
	for(long i = 0; 
		i < enum_gann_seq_max && (_IndexOfDate + m_GannSequenceModel[i]) < _IndexOfBoundOnAbsCoord; 
		++i)
	{
		long _index_Gann = _IndexOfDate + m_GannSequenceModel[i];
		if(_index_Gann >= _IndexOfOriginOnAbsCoord)
		{
			_Point.x = _Coord->ViewXByIndex(_index_Gann);
			_VerticalEquation.MoveTo(_Point);
			_VerticalEquation.Render(_hdc, _Rect);

			if(IsShowLabel())
			{
				_TextRect.left = _Point.x + m_LabelSpan;
				_TextRect.top = _Rect.top;
				_TextRect.right = _Rect.right;
				_TextRect.bottom = _TextRect.top + m_LabelFontSize;

				sprintf(_OutText, "%d", m_GannSequenceModel[i] + 1);
				::SetBkMode(_hdc, TRANSPARENT);
				::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
			}
		}
	}

	return 0;
}

#pragma warning(disable:4305)
#pragma warning(disable:4996)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fibonacci Sequence Vertical Equation On Absolute Coordinate
#define ADJUST_TO_INDEX(x) (x - 1), x
long DTFibonacciSequenceVerticalAbsEquation::m_SequenceModel[enum_seq_max][enum_cols] = 
{
	ADJUST_TO_INDEX(1), ADJUST_TO_INDEX(2), ADJUST_TO_INDEX(3), ADJUST_TO_INDEX(5), ADJUST_TO_INDEX(8), 
	ADJUST_TO_INDEX(13), ADJUST_TO_INDEX(21), ADJUST_TO_INDEX(34), ADJUST_TO_INDEX(55), ADJUST_TO_INDEX(89), 
	ADJUST_TO_INDEX(144), ADJUST_TO_INDEX(233), ADJUST_TO_INDEX(377), ADJUST_TO_INDEX(610), ADJUST_TO_INDEX(987), 
	ADJUST_TO_INDEX(1597), ADJUST_TO_INDEX(2584), ADJUST_TO_INDEX(4181), ADJUST_TO_INDEX(6765), ADJUST_TO_INDEX(10946)
};
#undef ADJUST_TO_INDEX

DTFibonacciSequenceVerticalAbsEquation::DTFibonacciSequenceVerticalAbsEquation(DTCoordinateTransform* Coord)
: _Coord(Coord)
{
	m_OffsetDate = 0;
}

int DTFibonacciSequenceVerticalAbsEquation::HitTest(const tagDTAbsPoint& _Point)
{
	int _Ret = ENUM_ASSIST_HITTEST_NULL;
	long _IndexOfDate = _Coord->IndexOfDate(m_OffsetDate);
	long _IndexOfTarget = _Coord->IndexOfDate(_Point.xDate);
	long _SizeOfAbsPoints = _Coord->SizeOfAbsPoints();

	if(_IndexOfTarget < 0)
		return _Ret;

	if(_IndexOfDate >= 0)
	{
		for(int i = 0; i < enum_seq_max && (_IndexOfDate + m_SequenceModel[i][enum_index]) < _SizeOfAbsPoints; ++i)
		{
			if(_IndexOfTarget == (_IndexOfDate + m_SequenceModel[i][enum_index]))
			{
				_Ret = ENUM_ASSIST_HITTEST_TARGET;
				break;
			}
		}
	}

	return _Ret;
}

int DTFibonacciSequenceVerticalAbsEquation::MoveTo(const tagDTAbsPoint& _Point)
{
	m_OffsetDate = _Point.xDate;
	//m_bAvaiable = CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_OffsetDate);
	return 0;
}

int DTFibonacciSequenceVerticalAbsEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	long _SizeOfAbsPoints = _Coord->SizeOfAbsPoints();
	long _IndexOfBoundOnAbsCoord = _Coord->IndexOfBoundOnAbsCoord();
	long _IndexOfOriginOnAbsCoord = _Coord->IndexOfOriginOnAbsCoord();
	long _IndexOfDate = _Coord->IndexOfDate(m_OffsetDate);

	if(_IndexOfDate < 0 || _SizeOfAbsPoints == 0)
		return -1;

	tagDTPoint _Point;
	DTVerticalEquation _VerticalEquation;
	tagDTRect _TextRect;
	char _OutText[16];

	for(long i = 0; 
		i < enum_seq_max && (_IndexOfDate + m_SequenceModel[i][enum_index]) < _IndexOfBoundOnAbsCoord; 
		++i)
	{
		long _index_Fib = _IndexOfDate + m_SequenceModel[i][enum_index];
		if(_index_Fib >= _IndexOfOriginOnAbsCoord)
		{
			_Point.x = _Coord->ViewXByIndex(_index_Fib);
			_VerticalEquation.MoveTo(_Point);
			_VerticalEquation.Render(_hdc, _Rect);

			if(IsShowLabel())
			{
				_TextRect.left = _Point.x + m_LabelSpan;
				_TextRect.top = _Rect.top;
				_TextRect.right = _Rect.right;
				_TextRect.bottom = _TextRect.top + m_LabelFontSize;

				sprintf(_OutText, "%d", m_SequenceModel[i][enum_value]);
				::SetBkMode(_hdc, TRANSPARENT);
				::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
			}
		}
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Golden Price Horizon Equation On Absolute Coordinate
float DTGoldenPriceHorizonAbsEquation::m_SequenceModel[enum_seq_max] = 
{
	 19.1e-2,	 23.6e-2,	 38.2e-2,	 50.0e-2,	 61.8e-2,	 80.9e-2, 
	119.1e-2,	123.6e-2,	138.2e-2,	150.0e-2,	161.8e-2,	180.9e-2,	200.0e-2
};

char* DTGoldenPriceHorizonAbsEquation::m_SequenceLabel[enum_seq_max] = 
{
	"19.1%",	"23.6%",	"38.2%",	"50%",	"61.8%",	"80.9%", 
	"119.1%",	"123.6%",	"138.2%",	"150%",	"161.8%",	"180.9%",	"200%"
};

DTGoldenPriceHorizonAbsEquation::DTGoldenPriceHorizonAbsEquation(DTCoordinateTransform* Coord)
: _Coord(Coord)
{
	m_OffsetValue = 0;
}

int DTGoldenPriceHorizonAbsEquation::HitTest(const tagDTAbsPoint& _Point)
{
	float _yValue;
	float _yUnit = max(1.f, _Coord->UnitHeight());
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_yValue = m_OffsetValue * m_SequenceModel[i];
		if(CMP_WITH_PERMISSIBLE_ERROR(_yValue, _Point.yValue, _yUnit))
		{
			return ENUM_ASSIST_HITTEST_TARGET;
		}
	}
	if(CMP_WITH_PERMISSIBLE_ERROR(m_OffsetValue, _Point.yValue, _yUnit))
		return ENUM_ASSIST_HITTEST_TARGET;
	return ENUM_ASSIST_HITTEST_NULL;
}

int DTGoldenPriceHorizonAbsEquation::MoveTo(const tagDTAbsPoint& _Point)
{
	m_OffsetValue = _Point.yValue;
	return 0;
}

int DTGoldenPriceHorizonAbsEquation::RenderBase(HDC _hdc, const tagDTRect& _Rect)
{
	DTHorizonEquation _Horizon;
	tagDTPoint _Point;
	tagDTRect _TextRect;
	char _OutText[128];

	_Point.x = _Rect.left;
	_Point.y = _Coord->AbsYtoViewY(m_OffsetValue);
	_Horizon.MoveTo(_Point);
	_Horizon.Render(_hdc, _Rect);

	_TextRect.left = _Point.x;
	_TextRect.top = _Point.y - m_LabelFontSize - m_LabelSpan;
	_TextRect.right = _Point.x + 120;
	_TextRect.bottom = _Point.y - m_LabelSpan;
	sprintf(_OutText, "Base %0.3f", m_OffsetValue);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
		::SetBkMode(_hdc, oldBkMode);
	}

	return 0;
}

int DTGoldenPriceHorizonAbsEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	DTHorizonEquation _Horizon;
	tagDTPoint _Point;
	char _OutText[128];

	_Point.x = _Rect.left;

	float _yValue[ enum_seq_max ];
	tagDTRect _TextRect[ enum_seq_max ];

	HPEN dotPen = ::CreatePen(PS_DOT, 1, ::GetDCPenColor(_hdc));
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_yValue[i] = m_OffsetValue * m_SequenceModel[i];
		_Point.y = _Coord->AbsYtoViewY(_yValue[i]);
		_Horizon.MoveTo(_Point);
		_Horizon.Render(_hdc, _Rect);

		_TextRect[i].left = _Point.x + m_LabelSpan + m_LabelSpan;
		_TextRect[i].top = _Point.y - m_LabelFontSize/* - m_LabelSpan*/;
		_TextRect[i].right = _Point.x + 120;
		_TextRect[i].bottom = _Point.y - m_LabelSpan;
	}
	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		for(int i = 0; i < enum_seq_max; ++i)
		{
			sprintf(_OutText, "%s %0.3f", m_SequenceLabel[i], _yValue[i]);
			::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect[i], DT_LEFT);
		}
		::SetBkMode(_hdc, oldBkMode);
	}

	return RenderBase(_hdc, _Rect);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Golden Aim Horizon Equation On Absolute Coordinate

float DTGoldenAimHorizonAbsEquation::m_SequenceModel[enum_seq_max] = 
{
	 19.1e-2,	 38.2e-2,	 50.0e-2,	 61.8e-2,	 80.9e-2
};

char* DTGoldenAimHorizonAbsEquation::m_SequenceLabel[enum_seq_max] = 
{
	"19.1%",	"38.2%",	"50.0%",	"61.8%",	"80.9%"
};

DTGoldenAimHorizonAbsEquation::DTGoldenAimHorizonAbsEquation(DTCoordinateTransform* Coord)
: _Coord(Coord)
{
	m_OffsetValue = 0;
	m_DistanceValue = 0;
	m_bPositive = true;
}

int DTGoldenAimHorizonAbsEquation::HitTest(const tagDTAbsPoint& _Point)
{
	float _yValue;
	float _yUnit = max(1.f, _Coord->UnitHeight());
	float _yFactor = (m_bPositive) ? 1.0 : -1.0;
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_yValue =  m_OffsetValue + m_DistanceValue * _yFactor * m_SequenceModel[i];
		if(CMP_WITH_PERMISSIBLE_ERROR(_yValue, _Point.yValue, _yUnit))
		{
			return ENUM_ASSIST_HITTEST_TARGET;
		}
	}
	if(CMP_WITH_PERMISSIBLE_ERROR(m_OffsetValue, _Point.yValue, _yUnit))
		return ENUM_ASSIST_HITTEST_TARGET;
	_yValue = m_OffsetValue + m_DistanceValue * _yFactor;
	if(CMP_WITH_PERMISSIBLE_ERROR(_yValue, _Point.yValue, _yUnit))
		return ENUM_ASSIST_HITTEST_TARGET;
	return ENUM_ASSIST_HITTEST_NULL;
}

int DTGoldenAimHorizonAbsEquation::MoveTo(const tagDTAbsPoint& _Point)
{
	m_OffsetValue = _Point.yValue;
	return 0;
}

int DTGoldenAimHorizonAbsEquation::RenderBase(HDC _hdc, const tagDTRect& _Rect)
{
	DTHorizonEquation _Horizon;
	tagDTPoint _Point;
	tagDTRect _TextRect;
	char _OutText[128];

	float _yFactor = (m_bPositive) ? 1.0 : -1.0;

	_Point.x = _Rect.left;
	_Point.y = _Coord->AbsYtoViewY(m_OffsetValue);
	_Horizon.MoveTo(_Point);
	_Horizon.Render(_hdc, _Rect);

	_TextRect.left = _Point.x;
	_TextRect.top = _Point.y - m_LabelFontSize - m_LabelSpan;
	_TextRect.right = _Point.x + 120;
	_TextRect.bottom = _Point.y - m_LabelSpan;
	sprintf(_OutText, "Base %0.3f", m_OffsetValue);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
		::SetBkMode(_hdc, oldBkMode);
	}

	_Point.x = _Rect.left;
	_Point.y = _Coord->AbsYtoViewY(m_OffsetValue + m_DistanceValue * _yFactor);
	_Horizon.MoveTo(_Point);
	_Horizon.Render(_hdc, _Rect);

	_TextRect.left = _Point.x;
	_TextRect.top = _Point.y - m_LabelFontSize - m_LabelSpan;
	_TextRect.right = _Point.x + 120;
	_TextRect.bottom = _Point.y - m_LabelSpan;
	sprintf(_OutText, "100%% %0.3f", m_OffsetValue + m_DistanceValue * _yFactor);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
		::SetBkMode(_hdc, oldBkMode);
	}

	return 0;
}

int DTGoldenAimHorizonAbsEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	DTHorizonEquation _Horizon;
	tagDTPoint _Point;
	char _OutText[128];

	_Point.x = _Rect.left;

	float _yValue[ enum_seq_max ];
	tagDTRect _TextRect[ enum_seq_max ];
	float _yFactor = (m_bPositive) ? 1.0 : -1.0;

	HPEN dotPen = ::CreatePen(PS_DOT, 1, ::GetDCPenColor(_hdc));
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_yValue[i] = m_OffsetValue + m_DistanceValue * _yFactor * m_SequenceModel[i];
		_Point.y = _Coord->AbsYtoViewY(_yValue[i]);
		_Horizon.MoveTo(_Point);
		_Horizon.Render(_hdc, _Rect);

		_TextRect[i].left = _Point.x + m_LabelSpan + m_LabelSpan;
		_TextRect[i].top = _Point.y - m_LabelFontSize/* - m_LabelSpan*/;
		_TextRect[i].right = _Point.x + 120;
		_TextRect[i].bottom = _Point.y - m_LabelSpan;
	}
	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		for(int i = 0; i < enum_seq_max; ++i)
		{
			sprintf(_OutText, "%s %0.3f", m_SequenceLabel[i], _yValue[i]);
			::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect[i], DT_LEFT);
		}
		::SetBkMode(_hdc, oldBkMode);
	}

	return RenderBase(_hdc, _Rect);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Golden Section Horizon Equation On Absolute Coordinate

float DTGoldenSectionHorizonAbsEquation::m_SequenceModel[enum_seq_max] = 
{
	 38.2e-2,	 50.0e-2,	 61.8e-2,	 80.9e-2,
	138.2e-2,	150.0e-2,	161.8e-2,	180.9e-2,
	200.0e-2,	238.2e-2,	261.8e-2
};

char* DTGoldenSectionHorizonAbsEquation::m_SequenceLabel[enum_seq_max] = 
{
	"38.2%",	"50.0%",	"61.8%",	"80.9%",
	"138.2%",	"150.0%",	"161.8%",	"180.9%",
	"200%",		"238.2%",	"261.8%"
};

DTGoldenSectionHorizonAbsEquation::DTGoldenSectionHorizonAbsEquation(DTCoordinateTransform* Coord)
: _Coord(Coord)
{
	m_OffsetValue = 0;
	m_DistanceValue = 0;
	m_bPositive = true;
}

int DTGoldenSectionHorizonAbsEquation::HitTest(const tagDTAbsPoint& _Point)
{
	float _yValue;
	float _yUnit = max(1.f, _Coord->UnitHeight());
	float _yFactor = (m_bPositive) ? 1.0 : -1.0;
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_yValue =  m_OffsetValue + m_DistanceValue * _yFactor * m_SequenceModel[i];
		if(CMP_WITH_PERMISSIBLE_ERROR(_yValue, _Point.yValue, _yUnit))
		{
			return ENUM_ASSIST_HITTEST_TARGET;
		}
	}
	if(CMP_WITH_PERMISSIBLE_ERROR(m_OffsetValue, _Point.yValue, _yUnit))
		return ENUM_ASSIST_HITTEST_TARGET;
	_yValue = m_OffsetValue + m_DistanceValue * _yFactor;
	if(CMP_WITH_PERMISSIBLE_ERROR(_yValue, _Point.yValue, _yUnit))
		return ENUM_ASSIST_HITTEST_TARGET;
	return ENUM_ASSIST_HITTEST_NULL;
}

int DTGoldenSectionHorizonAbsEquation::MoveTo(const tagDTAbsPoint& _Point)
{
	m_OffsetValue = _Point.yValue;
	return 0;
}

int DTGoldenSectionHorizonAbsEquation::RenderBase(HDC _hdc, const tagDTRect& _Rect)
{
	DTHorizonEquation _Horizon;
	tagDTPoint _Point;
	tagDTRect _TextRect;
	char _OutText[128];

	float _yFactor = (m_bPositive) ? 1.0 : -1.0;

	_Point.x = _Rect.left;
	_Point.y = _Coord->AbsYtoViewY(m_OffsetValue);
	_Horizon.MoveTo(_Point);
	_Horizon.Render(_hdc, _Rect);

	_TextRect.left = _Point.x;
	_TextRect.top = _Point.y - m_LabelFontSize - m_LabelSpan;
	_TextRect.right = _Point.x + 120;
	_TextRect.bottom = _Point.y - m_LabelSpan;
	sprintf(_OutText, "Base %0.3f", m_OffsetValue);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
		::SetBkMode(_hdc, oldBkMode);
	}

	_Point.x = _Rect.left;
	_Point.y = _Coord->AbsYtoViewY(m_OffsetValue + m_DistanceValue * _yFactor);
	_Horizon.MoveTo(_Point);
	_Horizon.Render(_hdc, _Rect);

	_TextRect.left = _Point.x;
	_TextRect.top = _Point.y - m_LabelFontSize - m_LabelSpan;
	_TextRect.right = _Point.x + 120;
	_TextRect.bottom = _Point.y - m_LabelSpan;
	sprintf(_OutText, "100%% %0.3f", m_OffsetValue + m_DistanceValue * _yFactor);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
		::SetBkMode(_hdc, oldBkMode);
	}

	return 0;
}

int DTGoldenSectionHorizonAbsEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	DTHorizonEquation _Horizon;
	tagDTPoint _Point;
	char _OutText[128];

	_Point.x = _Rect.left;

	float _yValue[ enum_seq_max ];
	tagDTRect _TextRect[ enum_seq_max ];
	float _yFactor = (m_bPositive) ? 1.0 : -1.0;

	HPEN dotPen = ::CreatePen(PS_DOT, 1, ::GetDCPenColor(_hdc));
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_yValue[i] = m_OffsetValue + m_DistanceValue * _yFactor * m_SequenceModel[i];
		_Point.y = _Coord->AbsYtoViewY(_yValue[i]);
		_Horizon.MoveTo(_Point);
		_Horizon.Render(_hdc, _Rect);

		_TextRect[i].left = _Point.x + m_LabelSpan + m_LabelSpan;
		_TextRect[i].top = _Point.y - m_LabelFontSize/* - m_LabelSpan*/;
		_TextRect[i].right = _Point.x + 120;
		_TextRect[i].bottom = _Point.y - m_LabelSpan;
	}
	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		for(int i = 0; i < enum_seq_max; ++i)
		{
			sprintf(_OutText, "%s %0.3f", m_SequenceLabel[i], _yValue[i]);
			::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect[i], DT_LEFT);
		}
		::SetBkMode(_hdc, oldBkMode);
	}

	return RenderBase(_hdc, _Rect);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Percentage Horizon Equation On Absolute Coordinate

float DTPercentageHorizonAbsEquation::m_SequenceModel[enum_seq_max] = 
{
	 25.0e-2,	 33.3e-2,	 50.0e-2
};

char* DTPercentageHorizonAbsEquation::m_SequenceLabel[enum_seq_max] = 
{
	"25.0%",	"33.3%",	"50.0%"
};

DTPercentageHorizonAbsEquation::DTPercentageHorizonAbsEquation(DTCoordinateTransform* Coord)
: _Coord(Coord)
{
	m_OffsetValue = 0;
	m_DistanceValue = 0;
	m_bPositive = true;
}

int DTPercentageHorizonAbsEquation::HitTest(const tagDTAbsPoint& _Point)
{
	float _yValue;
	float _yUnit = max(1.f, _Coord->UnitHeight());
	float _yFactor = (m_bPositive) ? 1.0 : -1.0;
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_yValue =  m_OffsetValue + m_DistanceValue * _yFactor * m_SequenceModel[i];
		if(CMP_WITH_PERMISSIBLE_ERROR(_yValue, _Point.yValue, _yUnit))
		{
			return ENUM_ASSIST_HITTEST_TARGET;
		}
	}
	if(CMP_WITH_PERMISSIBLE_ERROR(m_OffsetValue, _Point.yValue, _yUnit))
		return ENUM_ASSIST_HITTEST_TARGET;
	_yValue = m_OffsetValue + m_DistanceValue * _yFactor;
	if(CMP_WITH_PERMISSIBLE_ERROR(_yValue, _Point.yValue, _yUnit))
		return ENUM_ASSIST_HITTEST_TARGET;
	return ENUM_ASSIST_HITTEST_NULL;
}

int DTPercentageHorizonAbsEquation::MoveTo(const tagDTAbsPoint& _Point)
{
	m_OffsetValue = _Point.yValue;
	return 0;
}

int DTPercentageHorizonAbsEquation::RenderBase(HDC _hdc, const tagDTRect& _Rect)
{
	DTHorizonEquation _Horizon;
	tagDTPoint _Point;
	tagDTRect _TextRect;
	char _OutText[128];

	float _yFactor = (m_bPositive) ? 1.0 : -1.0;

	_Point.x = _Rect.left;
	_Point.y = _Coord->AbsYtoViewY(m_OffsetValue);
	_Horizon.MoveTo(_Point);
	_Horizon.Render(_hdc, _Rect);

	_TextRect.left = _Point.x;
	_TextRect.top = _Point.y - m_LabelFontSize - m_LabelSpan;
	_TextRect.right = _Point.x + 120;
	_TextRect.bottom = _Point.y - m_LabelSpan;
	sprintf(_OutText, "Base %0.3f", m_OffsetValue);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
		::SetBkMode(_hdc, oldBkMode);
	}

	_Point.x = _Rect.left;
	_Point.y = _Coord->AbsYtoViewY(m_OffsetValue + m_DistanceValue * _yFactor);
	_Horizon.MoveTo(_Point);
	_Horizon.Render(_hdc, _Rect);

	_TextRect.left = _Point.x;
	_TextRect.top = _Point.y - m_LabelFontSize - m_LabelSpan;
	_TextRect.right = _Point.x + 120;
	_TextRect.bottom = _Point.y - m_LabelSpan;
	sprintf(_OutText, "100%% %0.3f", m_OffsetValue + m_DistanceValue * _yFactor);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
		::SetBkMode(_hdc, oldBkMode);
	}

	return 0;
}

int DTPercentageHorizonAbsEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	DTHorizonEquation _Horizon;
	tagDTPoint _Point;
	char _OutText[128];

	_Point.x = _Rect.left;

	float _yValue[ enum_seq_max ];
	tagDTRect _TextRect[ enum_seq_max ];
	float _yFactor = (m_bPositive) ? 1.0 : -1.0;

	HPEN dotPen = ::CreatePen(PS_DOT, 1, ::GetDCPenColor(_hdc));
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_yValue[i] = m_OffsetValue + m_DistanceValue * _yFactor * m_SequenceModel[i];
		_Point.y = _Coord->AbsYtoViewY(_yValue[i]);
		_Horizon.MoveTo(_Point);
		_Horizon.Render(_hdc, _Rect);

		_TextRect[i].left = _Point.x + m_LabelSpan + m_LabelSpan;
		_TextRect[i].top = _Point.y - m_LabelFontSize/* - m_LabelSpan*/;
		_TextRect[i].right = _Point.x + 120;
		_TextRect[i].bottom = _Point.y - m_LabelSpan;
	}
	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		for(int i = 0; i < enum_seq_max; ++i)
		{
			sprintf(_OutText, "%s %0.3f", m_SequenceLabel[i], _yValue[i]);
			::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect[i], DT_LEFT);
		}
		::SetBkMode(_hdc, oldBkMode);
	}

	return RenderBase(_hdc, _Rect);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Percentage Horizon Equation On Absolute Coordinate

float DTOctBandHorizonAbsEquation::m_SequenceModel[enum_seq_max] = 
{
	12.5e-2,	 25.0e-2,	 37.5e-2,	 50.0e-2,	 62.5e-2,	 75.0e-2,	 87.5e-2
};

char* DTOctBandHorizonAbsEquation::m_SequenceLabel[enum_seq_max] = 
{
	"12.5%",	"25.0%",	"37.5%",	"50.0%",	"62.5%",	"75.0%",	"87.5%"
};

DTOctBandHorizonAbsEquation::DTOctBandHorizonAbsEquation(DTCoordinateTransform* Coord)
: _Coord(Coord)
{
	m_OffsetValue = 0;
	m_DistanceValue = 0;
	m_bPositive = true;
}

int DTOctBandHorizonAbsEquation::HitTest(const tagDTAbsPoint& _Point)
{
	float _yValue;
	float _yUnit = max(1.f, _Coord->UnitHeight());
	float _yFactor = (m_bPositive) ? 1.0 : -1.0;
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_yValue =  m_OffsetValue + m_DistanceValue * _yFactor * m_SequenceModel[i];
		if(CMP_WITH_PERMISSIBLE_ERROR(_yValue, _Point.yValue, _yUnit))
		{
			return ENUM_ASSIST_HITTEST_TARGET;
		}
	}
	if(CMP_WITH_PERMISSIBLE_ERROR(m_OffsetValue, _Point.yValue, _yUnit))
		return ENUM_ASSIST_HITTEST_TARGET;
	_yValue = m_OffsetValue + m_DistanceValue * _yFactor;
	if(CMP_WITH_PERMISSIBLE_ERROR(_yValue, _Point.yValue, _yUnit))
		return ENUM_ASSIST_HITTEST_TARGET;
	return ENUM_ASSIST_HITTEST_NULL;
}

int DTOctBandHorizonAbsEquation::MoveTo(const tagDTAbsPoint& _Point)
{
	m_OffsetValue = _Point.yValue;
	return 0;
}

int DTOctBandHorizonAbsEquation::RenderBase(HDC _hdc, const tagDTRect& _Rect)
{
	DTHorizonEquation _Horizon;
	tagDTPoint _Point;
	tagDTRect _TextRect;
	char _OutText[128];

	float _yFactor = (m_bPositive) ? 1.0 : -1.0;

	_Point.x = _Rect.left;
	_Point.y = _Coord->AbsYtoViewY(m_OffsetValue);
	_Horizon.MoveTo(_Point);
	_Horizon.Render(_hdc, _Rect);

	_TextRect.left = _Point.x;
	_TextRect.top = _Point.y - m_LabelFontSize - m_LabelSpan;
	_TextRect.right = _Point.x + 120;
	_TextRect.bottom = _Point.y - m_LabelSpan;
	sprintf(_OutText, "Base %0.3f", m_OffsetValue);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
		::SetBkMode(_hdc, oldBkMode);
	}

	_Point.x = _Rect.left;
	_Point.y = _Coord->AbsYtoViewY(m_OffsetValue + m_DistanceValue * _yFactor);
	_Horizon.MoveTo(_Point);
	_Horizon.Render(_hdc, _Rect);

	_TextRect.left = _Point.x;
	_TextRect.top = _Point.y - m_LabelFontSize - m_LabelSpan;
	_TextRect.right = _Point.x + 120;
	_TextRect.bottom = _Point.y - m_LabelSpan;
	sprintf(_OutText, "100%% %0.3f", m_OffsetValue + m_DistanceValue * _yFactor);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect, DT_LEFT);
		::SetBkMode(_hdc, oldBkMode);
	}

	return 0;
}

int DTOctBandHorizonAbsEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	DTHorizonEquation _Horizon;
	tagDTPoint _Point;
	char _OutText[128];

	_Point.x = _Rect.left;

	float _yValue[ enum_seq_max ];
	tagDTRect _TextRect[ enum_seq_max ];
	float _yFactor = (m_bPositive) ? 1.0 : -1.0;

	HPEN dotPen = ::CreatePen(PS_DOT, 1, ::GetDCPenColor(_hdc));
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_yValue[i] = m_OffsetValue + m_DistanceValue * _yFactor * m_SequenceModel[i];
		_Point.y = _Coord->AbsYtoViewY(_yValue[i]);
		_Horizon.MoveTo(_Point);
		_Horizon.Render(_hdc, _Rect);

		_TextRect[i].left = _Point.x + m_LabelSpan + m_LabelSpan;
		_TextRect[i].top = _Point.y - m_LabelFontSize/* - m_LabelSpan*/;
		_TextRect[i].right = _Point.x + 120;
		_TextRect[i].bottom = _Point.y - m_LabelSpan;
	}
	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);

	if(IsShowLabel())
	{
		int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
		for(int i = 0; i < enum_seq_max; ++i)
		{
			sprintf(_OutText, "%s %0.3f", m_SequenceLabel[i], _yValue[i]);
			::DrawTextA(_hdc, _OutText, strlen(_OutText), &_TextRect[i], DT_LEFT);
		}
		::SetBkMode(_hdc, oldBkMode);
	}

	return RenderBase(_hdc, _Rect);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gould Rays

float DTGouldRayAbsEquation::m_SequenceModel[enum_seq_max] = 
{
	1.0 / 3.0,	2.0 / 3.0
};

char* DTGouldRayAbsEquation::m_SequenceLabel[enum_seq_max] = 
{
	"1:3",	"2:3"
};

DTGouldRayAbsEquation::DTGouldRayAbsEquation(DTCoordinateTransform* Coord)
: _Coord(Coord)
{
	m_OffsetDate = 0;
	m_OffsetValue = 0;
	m_DistanceDate = 0;
	m_DistanceValue = 0;
	m_bPositive = true;
}

int DTGouldRayAbsEquation::HitTest(const tagDTAbsPoint& _Point)
{
	DTLineEquation _RenderLine;
	tagDTPoint _PointA;
	tagDTPoint _PointB;
	tagDTPoint _TargetPoint;
	long _Index_XDate = _Coord->IndexOfDate(m_OffsetDate);

	_TargetPoint.x = _Coord->AbsXtoViewX(_Point.xDate);
	_TargetPoint.y = _Coord->AbsXtoViewX(_Point.yValue);

	_PointA.x = _Coord->AbsXtoViewX(m_OffsetDate);
	_PointA.y = _Coord->AbsYtoViewY(m_OffsetValue);

	_PointB.x = _Coord->ViewXByIndex(_Index_XDate + m_DistanceDate);
	_PointB.y = _Coord->AbsYtoViewY(m_OffsetValue + m_DistanceValue);

	_RenderLine.Build(_PointA, _PointB);
	if(_RenderLine.HitTest(_TargetPoint))
		return ENUM_ASSIST_HITTEST_TARGET;
	
	float _yFactor = (m_bPositive) ? 1.0 : -1.0;
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_PointB.y = _Coord->AbsYtoViewY(m_OffsetValue + _yFactor * (1 + m_SequenceModel[i]) * m_DistanceValue);
		_RenderLine.Build(_PointA, _PointB);
		if(_RenderLine.HitTest(_TargetPoint))
		{
			return ENUM_ASSIST_HITTEST_TARGET;
		}
	}

	return ENUM_ASSIST_HITTEST_NULL;
}

int DTGouldRayAbsEquation::MoveTo(const tagDTAbsPoint& _Point)
{
	m_OffsetDate = _Point.xDate;
	m_OffsetValue = _Point.yValue;
	return 0;
}

int DTGouldRayAbsEquation::Render(HDC _hdc, const tagDTRect& _Rect)
{
	DTLineEquation _RenderBaseLine;
	DTLineEquation _RenderLine;
	tagDTPoint _PointA;
	tagDTPoint _PointB;
	tagDTRect _TextRect;
	char _OutText[128];
	long _Index_XDate = _Coord->IndexOfDate(m_OffsetDate);

	float _yFactor = (m_bPositive) ? 1.0 : -1.0;

	_PointA.x = _Coord->AbsXtoViewX(m_OffsetDate);
	_PointA.y = _Coord->AbsYtoViewY(m_OffsetValue);

	_PointB.x = _Coord->ViewXByIndex(_Index_XDate + m_DistanceDate);
	_PointB.y = _Coord->AbsYtoViewY(m_OffsetValue + m_DistanceValue);

	_RenderBaseLine.Build(_PointA, _PointB);

	HPEN dotPen = ::CreatePen(PS_DOT, 1, ::GetDCPenColor(_hdc));
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);
	for(int i = 0; i < enum_seq_max; ++i)
	{
		_PointB.y = _Coord->AbsYtoViewY(m_OffsetValue + _yFactor * (1 + m_SequenceModel[i]) * m_DistanceValue);
		_RenderLine.Build(_PointA, _PointB);
		_RenderLine.Render(_hdc, _Rect);
	}
	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);
	
	_RenderBaseLine.Render(_hdc, _Rect);
	return 0;
}