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

#ifndef _DRAWTOOL_DEF__H_
#define _DRAWTOOL_DEF__H_

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 线段 Line Segment
// 直线 Line
// 箭头 Line Segment with Arrow
// 射线 Line Extension
// 价格通道线 Price Channels
// 平行直线 Parallel Lines
// 圆弧线 Arc
// 圆 Circle
// 外接圆 Circumcircle
// 黄金价位线 Golden Price Horizons
// 黄金目标线 Golden Aim Horizons
// 黄金分割 Golden Secion Horizons
// 百分比线 Percentage Horizons
// 波段线 Bond Horizons
// 线形回归线 Linear Regression Area
// 延长线形回归线 Linear Regression Extension Area
// 线性回归 Linear Regression Line
// 周期线 Cycle Verticals
// 斐波拉契线 Fibonacci Verticals
// 江恩时序列 Gann Sequence Verticals
// 阻速线 Gould Rays
// 江恩角度线 Gann Rays
enum ENUM_ASSIST_GRAPH_TYPE
{
	ENUM_ASSIST_GRAPH_TYPE_NULL,
	ENUM_ASSIST_GRAPH_TYPE_Segment,
	ENUM_ASSIST_GRAPH_TYPE_Line,
	ENUM_ASSIST_GRAPH_TYPE_Segment_with_Arrow,
	ENUM_ASSIST_GRAPH_TYPE_Line_Extension,
	ENUM_ASSIST_GRAPH_TYPE_Price_Channels,
	ENUM_ASSIST_GRAPH_TYPE_Parallel_Lines,
	ENUM_ASSIST_GRAPH_TYPE_Arc,
	ENUM_ASSIST_GRAPH_TYPE_Circle,
	ENUM_ASSIST_GRAPH_TYPE_Circumcircle,
	ENUM_ASSIST_GRAPH_TYPE_GPHs,
	ENUM_ASSIST_GRAPH_TYPE_GAHs,
	ENUM_ASSIST_GRAPH_TYPE_GSHs,
	ENUM_ASSIST_GRAPH_TYPE_PHs,
	ENUM_ASSIST_GRAPH_TYPE_BHs,
	ENUM_ASSIST_GRAPH_TYPE_LRA,
	ENUM_ASSIST_GRAPH_TYPE_LREA,
	ENUM_ASSIST_GRAPH_TYPE_LRL,
	ENUM_ASSIST_GRAPH_TYPE_Cycle,
	ENUM_ASSIST_GRAPH_TYPE_Fibonacci,
	ENUM_ASSIST_GRAPH_TYPE_Gann,
	ENUM_ASSIST_GRAPH_TYPE_Gould_Rays,
	ENUM_ASSIST_GRAPH_TYPE_Gann_Rays,
	ENUM_ASSIST_GRAPH_TYPE_Rectangle,
	ENUM_ASSIST_GRAPH_TYPE_Red_Up_Arrow = 100,
	ENUM_ASSIST_GRAPH_TYPE_Green_Down_Arrow,
	ENUM_ASSIST_GRAPH_TYPE_Text
};

enum ENUM_ASSIST_EDIT_MODE
{
	ENUM_ASSIST_EDIT_MODE_NULL = 0,
	ENUM_ASSIST_EDIT_MODE_UNKNOWN,
	ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING,
	ENUM_ASSIST_PRE_EDIT_MODE_SET_P1,
	ENUM_ASSIST_PRE_EDIT_MODE_LINE_P2,
	ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P1_P1,
	ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P2_P2,
	ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P2_P3,
	ENUM_ASSIST_PRE_EDIT_MODE_END,
	ENUM_ASSIST_EDIT_MODE_TEND_BEGIN,
	ENUM_ASSIST_EDIT_MODE_BEGINNING_OFFSET,
	ENUM_ASSIST_EDIT_MODE_BEGINNING_P1,
	ENUM_ASSIST_EDIT_MODE_BEGINNING_P2,
	ENUM_ASSIST_EDIT_MODE_BEGINNING_P3,
	ENUM_ASSIST_EDIT_MODE_BEGINNING,
	ENUM_ASSIST_EDIT_MODE_OFFSET,
	ENUM_ASSIST_EDIT_MODE_EDIT_P1,
	ENUM_ASSIST_EDIT_MODE_EDIT_P2,
	ENUM_ASSIST_EDIT_MODE_EDIT_P3,
	ENUM_ASSIST_EDIT_MODE_END,
	ENUM_ASSIST_EDIT_MODE_COMMIT,
	ENUM_ASSIST_EDIT_MODE_POPUPMENU,
	ENUM_ASSIST_EDIT_MODE_UNEXPECTED
};

enum ENUM_ASSIST_HITTEST
{
	ENUM_ASSIST_HITTEST_NULL,
	ENUM_ASSIST_HITTEST_TARGET,
	ENUM_ASSIST_HITTEST_LOCATION_P1,
	ENUM_ASSIST_HITTEST_LOCATION_P2,
	ENUM_ASSIST_HITTEST_LOCATION_P3
};

enum ENUM_ASSIST_DIRECT
{
	ENUM_ASSIST_DIRECT_UNKNOWN,
	ENUM_ASSIST_DIRECT_LEFT,
	ENUM_ASSIST_DIRECT_LEFTUP,
	ENUM_ASSIST_DIRECT_UP,
	ENUM_ASSIST_DIRECT_RIGHTUP,
	ENUM_ASSIST_DIRECT_RIGHT,
	ENUM_ASSIST_DIRECT_RIGHTDOWN,
	ENUM_ASSIST_DIRECT_DOWN,
	ENUM_ASSIST_DIRECT_LEFTDOWN
};

enum ENUM_FLTBAR_GRAPH_TYPE
{
	ENUM_FLTBAR_GRAPH_TYPE_NULL,
	ENUM_FLTBAR_GRAPH_TYPE_Segment,
	ENUM_FLTBAR_GRAPH_TYPE_Line,
	ENUM_FLTBAR_GRAPH_TYPE_Segment_with_Arrow,
	ENUM_FLTBAR_GRAPH_TYPE_Line_Extension,
	ENUM_FLTBAR_GRAPH_TYPE_Price_Channels,
	ENUM_FLTBAR_GRAPH_TYPE_Parallel_Lines,
	ENUM_FLTBAR_GRAPH_TYPE_Arc,
	ENUM_FLTBAR_GRAPH_TYPE_Circle,
	ENUM_FLTBAR_GRAPH_TYPE_Circumcircle,
	ENUM_FLTBAR_GRAPH_TYPE_GPHs,
	ENUM_FLTBAR_GRAPH_TYPE_GAHs,
	ENUM_FLTBAR_GRAPH_TYPE_GSHs,
	ENUM_FLTBAR_GRAPH_TYPE_PHs,
	ENUM_FLTBAR_GRAPH_TYPE_BHs,
	ENUM_FLTBAR_GRAPH_TYPE_LRA,
	ENUM_FLTBAR_GRAPH_TYPE_LREA,
	ENUM_FLTBAR_GRAPH_TYPE_LRL,
	ENUM_FLTBAR_GRAPH_TYPE_Cycle,
	ENUM_FLTBAR_GRAPH_TYPE_Fibonacci,
	ENUM_FLTBAR_GRAPH_TYPE_Gann,
	ENUM_FLTBAR_GRAPH_TYPE_Gould_Rays,
	ENUM_FLTBAR_GRAPH_TYPE_Gann_Rays,
	ENUM_FLTBAR_GRAPH_TYPE_Rectangle,
	ENUM_FLTBAR_GRAPH_TYPE_Red_Up_Arrow,
	ENUM_FLTBAR_GRAPH_TYPE_Green_Down_Arrow,
	ENUM_FLTBAR_GRAPH_TYPE_Text,
	ENUM_FLTBAR_GRAPH_TYPE_Delete,
	ENUM_FLTBAR_GRAPH_TYPE_Mask,
	ENUM_FLTBAR_GRAPH_TYPE_TOTAL
};


#define WM_DT_FLTBAR_DELETE (WM_USER + 400)
#define WM_DT_FLTBAR_DELETE_TARGET (WM_USER + 401)
#define WM_DT_FLTBAR_MASK (WM_USER + 402)
#define WM_DT_FLTBAR_EDIT_TARGET (WM_USER + 403)

#endif // _DRAWTOOL_DEF__H_
