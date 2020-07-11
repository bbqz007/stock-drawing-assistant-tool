/**
MIT License
Copyright (c) 2015-2020 bbqz007 <https://github.com/bbqz007, http://www.cnblogs.com/bbqzsl>
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

#ifndef _DRAWTOOL_BASIC_UTIL__H_
#define _DRAWTOOL_BASIC_UTIL__H_

extern const double g_ConstPi;
extern double g_CirlceQuadrant [4] [4] [3];
extern double g_AngleQuadrant [4] [8];
extern int g_AngleQuadrantMapArrow [4] [8] [2];

#define CMP_WITH_PERMISSIBLE_ERROR(base, comp, err) (( ( (base) - (err) ) <= (comp) ) && ( (comp) < ( (base) + (err) ) ))
#define FLOAT_NEGATIVE_LIMIT	-1e+38
#define FLOAT_POSITIVE_LIMIT	1e+38
#define FLOAT_INFINITE_LEFT		FLOAT_NEGATIVE_LIMIT
#define FLOAT_INFINITE_TOP		FLOAT_NEGATIVE_LIMIT
#define FLOAT_INFINITE_RIGHT	FLOAT_POSITIVE_LIMIT
#define FLOAT_INFINITE_BOTTOM	FLOAT_POSITIVE_LIMIT

#define LONG_NEGATIVE_LIMIT	-1048575	// 0x800FFFFF
#define LONG_POSITIVE_LIMIT	1048575		// 0x000FFFFF
#define LONG_NEGATIVE_INFINITE	-65535	// 0x8000FFFF
#define LONG_POSITIVE_INFINITE	65535	// 0x0000FFFF
#define LONG_INFINITE_LEFT		LONG_NEGATIVE_INFINITE
#define LONG_INFINITE_TOP		LONG_NEGATIVE_INFINITE
#define LONG_INFINITE_RIGHT		LONG_POSITIVE_INFINITE
#define LONG_INFINITE_BOTTOM	LONG_POSITIVE_INFINITE
#define LONG_NEGATIVE_LIMIT_RESULT	-2097151	// 0x801FFFFF
#define LONG_POSITIVE_LIMIT_RESULT	2097151		// 0x001FFFFF
#define CHECK_VIEW_X_AXIS_AVAIABLE(x) (LONG_NEGATIVE_LIMIT < (x) && (x) < LONG_POSITIVE_LIMIT)
#define CHECK_VIEW_XDATE_AXIS_AVAIABLE(x) ((x) > 19900000)
#define CHECK_VIEW_XINDEX_AXIS_AVAIABLE(x) ((x) >= 0)

#define IS_POSITIVE_ON_VIEW_Y_AXIS(y2, y1) (y2 <= y1)
#define IS_POSITIVE_ON_VALUE_Y_AXIS(y2, y1) (y2 >= y1)

namespace DTEquationHelper
{
	inline float DistanceBetweenTowPoints(const tagDTPoint& _PointA, const tagDTPoint& _PointB)
	{
		double ret = 0.0;
		ret = ::pow((double)(_PointA.x - _PointB.x), 2) + ::pow((double)(_PointA.y - _PointB.y), 2);
		ret = ::sqrt(ret);
		return (float) ret;
	}

	inline void AdjustNarrowRect(tagDTRect& _Rect)
	{
		if(_Rect.left == _Rect.right)
			_Rect.right += 1;
		if(_Rect.top == _Rect.bottom)
			_Rect.bottom += 1;
	}
}

#pragma warning(disable:4305)
#pragma warning(disable:4996)
#pragma warning(disable:4390)

namespace DTHelper
{
	int OutputDebugStringA(const char* fmt, ...);
	void ConvertLongStkCodeToShortStkCode(char (&_StkCode)[8], DWORD _LMarketType, char* _LStkCode);
}

#endif // _DRAWTOOL_BASIC_UTIL__H_
