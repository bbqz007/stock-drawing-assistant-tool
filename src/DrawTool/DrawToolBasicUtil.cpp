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
#include <math.h>

extern const double g_ConstPi = 3.1415926535;

#define CONST_PI_VALUE g_ConstPi
#define CONST_PI_VALUE_OCT (CONST_PI_VALUE / 8)
#define CONST_PI_VALUE_HEX (CONST_PI_VALUE / 16)

extern double g_CirlceQuadrant [4] [4] [3] =
{
	// +x +y
	::tan(0.0),								::cos(0.0),									::sin(0.0),
	::tan(CONST_PI_VALUE_OCT),				::cos(CONST_PI_VALUE_OCT),					::sin(CONST_PI_VALUE_OCT),
	::tan(CONST_PI_VALUE_OCT * 2),			::cos(CONST_PI_VALUE_OCT * 2),				::sin(CONST_PI_VALUE_OCT * 2),
	::tan(CONST_PI_VALUE_OCT * 3),			::cos(CONST_PI_VALUE_OCT * 3),				::sin(CONST_PI_VALUE_OCT * 3),

	// -x +y
	::tan(CONST_PI_VALUE_OCT * 4),			::cos(CONST_PI_VALUE_OCT * 4),				::sin(CONST_PI_VALUE_OCT * 4),
	::tan(CONST_PI_VALUE_OCT * 5),			::cos(CONST_PI_VALUE_OCT * 5),				::sin(CONST_PI_VALUE_OCT * 5),
	::tan(CONST_PI_VALUE_OCT * 6),			::cos(CONST_PI_VALUE_OCT * 6),				::sin(CONST_PI_VALUE_OCT * 6),
	::tan(CONST_PI_VALUE_OCT * 7),			::cos(CONST_PI_VALUE_OCT * 7),				::sin(CONST_PI_VALUE_OCT * 7),

	// -x -y
	::tan(CONST_PI_VALUE_OCT * 8),			::cos(CONST_PI_VALUE_OCT * 8),				::sin(CONST_PI_VALUE_OCT * 8),
	::tan(CONST_PI_VALUE_OCT * 9),			::cos(CONST_PI_VALUE_OCT * 9),				::sin(CONST_PI_VALUE_OCT * 9),
	::tan(CONST_PI_VALUE_OCT * 10),			::cos(CONST_PI_VALUE_OCT * 10),				::sin(CONST_PI_VALUE_OCT * 10),
	::tan(CONST_PI_VALUE_OCT * 11),			::cos(CONST_PI_VALUE_OCT * 11),				::sin(CONST_PI_VALUE_OCT * 11),

	// +x -y
	::tan(CONST_PI_VALUE_OCT * 12),			::cos(CONST_PI_VALUE_OCT * 12),				::sin(CONST_PI_VALUE_OCT * 12),
	::tan(CONST_PI_VALUE_OCT * 13),			::cos(CONST_PI_VALUE_OCT * 13),				::sin(CONST_PI_VALUE_OCT * 13),
	::tan(CONST_PI_VALUE_OCT * 14),			::cos(CONST_PI_VALUE_OCT * 14),				::sin(CONST_PI_VALUE_OCT * 14),
	::tan(CONST_PI_VALUE_OCT * 15),			::cos(CONST_PI_VALUE_OCT * 15),				::sin(CONST_PI_VALUE_OCT * 15)
};

extern double g_AngleQuadrant [4] [8] = 
{
	// +x +y
	::tan(0.0),							::tan(CONST_PI_VALUE_HEX),			::tan(CONST_PI_VALUE_HEX * 2),		::tan(CONST_PI_VALUE_HEX * 3),		
	::tan(CONST_PI_VALUE_HEX * 4),		::tan(CONST_PI_VALUE_HEX * 5),		::tan(CONST_PI_VALUE_HEX * 6),		::tan(CONST_PI_VALUE_HEX * 7),		
	
	// -x +y
	0.0-::abs(::tan(CONST_PI_VALUE_HEX * 8)),	::tan(CONST_PI_VALUE_HEX * 9),		::tan(CONST_PI_VALUE_HEX * 10),		::tan(CONST_PI_VALUE_HEX * 11),	
	::tan(CONST_PI_VALUE_HEX * 12),		::tan(CONST_PI_VALUE_HEX * 13),		::tan(CONST_PI_VALUE_HEX * 14),		::tan(CONST_PI_VALUE_HEX * 15),	

	// -x -y
	::tan(CONST_PI_VALUE_HEX * 16),		::tan(CONST_PI_VALUE_HEX * 17),		::tan(CONST_PI_VALUE_HEX * 18),		::tan(CONST_PI_VALUE_HEX * 19),	
	::tan(CONST_PI_VALUE_HEX * 20),		::tan(CONST_PI_VALUE_HEX * 21),		::tan(CONST_PI_VALUE_HEX * 22),		::tan(CONST_PI_VALUE_HEX * 23),	
	
	// +x -y
	0.0-::abs(::tan(CONST_PI_VALUE_HEX * 24)),	::tan(CONST_PI_VALUE_HEX * 25),		::tan(CONST_PI_VALUE_HEX * 26),		::tan(CONST_PI_VALUE_HEX * 27),	
	::tan(CONST_PI_VALUE_HEX * 28),		::tan(CONST_PI_VALUE_HEX * 29),		::tan(CONST_PI_VALUE_HEX * 30),		::tan(CONST_PI_VALUE_HEX * 31)
};	


extern int g_AngleQuadrantMapArrow [4] [8] [2] =
{
	// +x +y
	15, 1,		0, 2,		0, 2,		1, 3,
	1, 3,		2, 4,		2, 4,		3, 5,

	// -x +y
	3, 5,		4, 6,		4, 6,		5, 7,
	5, 7,		6, 8,		6, 8,		7, 9,

	// -x -y
	7, 9,		8, 10,		8, 10,		9, 11,
	9, 11,		10, 12,		10, 12,		11, 13,

	// +x -y
	11, 13,		12, 14,		12, 14,		13, 15,
	13, 15,		14, 0,		14, 0,		15, 1,
};

int DTHelper::OutputDebugStringA(const char* fmt, ...)
{
#define MAX_OUTPUT_BUFFER_SIZE 1024
	int ret = 0;
	char buf[MAX_OUTPUT_BUFFER_SIZE];
	memset(buf, 0, MAX_OUTPUT_BUFFER_SIZE);

	va_list params;
	va_start(params, fmt);

	ret = vsprintf_s(buf, MAX_OUTPUT_BUFFER_SIZE - 1, fmt, params);
	va_end(params);

	::OutputDebugStringA(buf);
	return ret;
}

void DTHelper::ConvertLongStkCodeToShortStkCode(char (&_StkCode)[8], DWORD _LMarketType, char* _LStkCode)
{
	memset(_StkCode, 0, sizeof(_StkCode));

	if(_LMarketType == 'ESHS')
	{
		_StkCode[0] = 'S';
		_StkCode[1] = 'H';
	}
	else if(_LMarketType == 'ESZS')
	{
		_StkCode[0] = 'S';
		_StkCode[1] = 'Z';
	}
	else if(_LMarketType == 'IBNC')
	{
		_StkCode[0] = 'B';
		_StkCode[1] = 'I';
	}

	if(_StkCode[0] != 0 && _StkCode[1] != 0 && _LStkCode)
	{
		memcpy(&_StkCode[2], _LStkCode, max(0, min(6, strlen(_LStkCode))));
	}
}