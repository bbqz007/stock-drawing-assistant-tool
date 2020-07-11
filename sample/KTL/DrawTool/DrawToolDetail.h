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

#ifndef _DRAWTOOL_DETAIL__H_
#define _DRAWTOOL_DETAIL__H_

#include "DrawToolBasic.h"

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
// 斐波那契线 Fibonacci Verticals
// 江恩时序列 Gann Sequence Verticals
// 阻速线 Gould Rays
// 江恩角度线 Gann Rays
// 红色上箭头 Red Up Arrow
// 绿色下箭头 Green Down Arrow
// 矩形框 Rectangle
// 文本 Text

class DTAssistGraphI : public DTAssistGraph
{
public:
	DTAssistGraphI(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
	: DTAssistGraph(_Graphics, _descGraph)
	{
	}
	DTAssistGraphI(DTAssistGraphI& _other)
	: DTAssistGraph(_other)
	{
	}
	~DTAssistGraphI()
	{
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 线段 Line Segment
class DTLineSegmentGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTLineSegmentGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTLineSegmentGraph(DTLineSegmentGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Segment; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
private:
	DTLineEquation m_Equation;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 直线 Line
class DTLineGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTLineGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTLineGraph(DTLineGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Line; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
private:
	DTLineEquation m_Equation;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 射线 Line Extension
class DTLineExtensionGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTLineExtensionGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTLineExtensionGraph(DTLineExtensionGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Line_Extension; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
private:
	DTLineEquation m_Equation;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 箭头 Line Segment with Arrow
class DTArrowSegmentGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTArrowSegmentGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTArrowSegmentGraph(DTArrowSegmentGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Segment_with_Arrow; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
protected:
	int RenderArrow(HDC _hdc);
	int RenderArrowByCne(HDC _hdc);
private:
	DTLineEquation m_Equation;
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 价格通道线 Price Channels
class DTPriceChannelsGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTPriceChannelsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTPriceChannelsGraph(DTPriceChannelsGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Price_Channels; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
private:
	DTLineEquation m_EquationBase;
	DTLineEquation m_EquationParallel1;
	DTLineEquation m_EquationParallel2;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 平行直线 Parallel Lines
class DTParallelLinesGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTParallelLinesGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTParallelLinesGraph(DTParallelLinesGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Parallel_Lines; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
private:
	DTLineEquation m_EquationBase;
	DTLineEquation m_EquationParallel1;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 圆弧线 Ellipse Arc
class DTEllipseArcGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTEllipseArcGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTEllipseArcGraph(DTEllipseArcGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Arc; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
private:
	DTEllipseArcEquation m_EquationBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 圆 Circle
class DTCircleGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTCircleGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTCircleGraph(DTCircleGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Circle; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
private:
	DTCircleEquation m_EquationBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 外接圆 Circumcircle
class DTCircumcircleGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTCircumcircleGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTCircumcircleGraph(DTCircumcircleGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Circumcircle; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
private:
	DTCircleEquation m_EquationBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 黄金价位线 Golden Price Horizons
class DTGoldenPriceHorizonsGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTGoldenPriceHorizonsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTGoldenPriceHorizonsGraph(DTGoldenPriceHorizonsGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_GPHs; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int Complete();
	int Clone(DTAssistGraph& _Other);
	int EndClone(DTAssistGraph& _Other);
private:
	DTGoldenPriceHorizonAbsEquation m_AbsEquationBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 黄金目标线 Golden Aim Horizons
class DTGoldenAimHorizonsGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTGoldenAimHorizonsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTGoldenAimHorizonsGraph(DTGoldenAimHorizonsGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_GAHs; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int Complete();
	int Clone(DTAssistGraph& _Other);
	int EndClone(DTAssistGraph& _Other);
private:
	DTGoldenAimHorizonAbsEquation m_AbsEquationBase;
	DTLineEquation m_HelperLine1;
	DTLineEquation m_HelperLine2;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 黄金分割 Golden Secion Horizons
class DTGoldenSecionHorizonsGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTGoldenSecionHorizonsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTGoldenSecionHorizonsGraph(DTGoldenSecionHorizonsGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_GSHs; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int Complete();
	int Clone(DTAssistGraph& _Other);
	int EndClone(DTAssistGraph& _Other);
private:
	DTGoldenSectionHorizonAbsEquation m_AbsEquationBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 百分比线 Percentage Horizons
class DTPercentageHorizonsGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTPercentageHorizonsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTPercentageHorizonsGraph(DTPercentageHorizonsGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_PHs; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int Complete();
	int Clone(DTAssistGraph& _Other);
	int EndClone(DTAssistGraph& _Other);
private:
	DTPercentageHorizonAbsEquation m_AbsEquationBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 波段线 Oct Band Horizons
class DTOctBandHorizonsGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTOctBandHorizonsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTOctBandHorizonsGraph(DTOctBandHorizonsGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_BHs; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int Complete();
	int Clone(DTAssistGraph& _Other);
	int EndClone(DTAssistGraph& _Other);
private:
	DTOctBandHorizonAbsEquation m_AbsEquationBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 周期线 Cycle Verticals
class DTCycleVerticalsGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTCycleVerticalsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTCycleVerticalsGraph(DTCycleVerticalsGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Cycle; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	void MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint);
	void MoveP1To(const tagDTPoint& _Point);
	void MoveP2To(const tagDTPoint& _Point);
	int Complete();
	int Clone(DTAssistGraph& _Other);
	int EndClone(DTAssistGraph& _Other);
private:
	DTEqualDifferenceVerticalAbsEquation m_AbsEquationBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 斐波那契线 Fibonacci Verticals
class DTFibonacciVerticalsGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTFibonacciVerticalsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTFibonacciVerticalsGraph(DTFibonacciVerticalsGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Fibonacci; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	void MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint);
	void MoveP1To(const tagDTPoint& _Point);
	int Complete();
	int Clone(DTAssistGraph& _Other);
	int EndClone(DTAssistGraph& _Other);
private:
	DTFibonacciSequenceVerticalAbsEquation m_AbsEquationBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 江恩时序列 Gann Sequence Verticals
class DTGannSequenceVerticalsGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTGannSequenceVerticalsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTGannSequenceVerticalsGraph(DTGannSequenceVerticalsGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Gann; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	void MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint);
	void MoveP1To(const tagDTPoint& _Point);
	int Complete();
	int Clone(DTAssistGraph& _Other);
	int EndClone(DTAssistGraph& _Other);
private:
	DTGannSequenceVerticalAbsEquation m_AbsEquationBase;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 线形回归线 Linear Regression Area
class DTLinearRegressionAreaGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTLinearRegressionAreaGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTLinearRegressionAreaGraph(DTLinearRegressionAreaGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_LRA; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	void MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint);
	void MoveP1To(const tagDTPoint& _Point);
	void MoveP2To(const tagDTPoint& _Point);
	int Complete();
	int Clone(DTAssistGraph& _Other);
private:
	DTLineEquation m_EquationBase;
	DTLineEquation m_EquationRegressionHigh;
	DTLineEquation m_EquationRegressionLow;
	long m_Distance;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 延长线形回归线 Linear Regression Extension Area
class DTLinearRegressionExtensionAreaGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTLinearRegressionExtensionAreaGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTLinearRegressionExtensionAreaGraph(DTLinearRegressionExtensionAreaGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_LREA; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	void MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint);
	void MoveP1To(const tagDTPoint& _Point);
	void MoveP2To(const tagDTPoint& _Point);
	int Complete();
	int Clone(DTAssistGraph& _Other);
protected:
	int RenderExtension(HDC _hdc);
	int RenderShadowExtension(HDC _hdc);
	int RenderEditingExtension(HDC _hdc);
private:
	DTLineEquation m_EquationBase;
	DTLineEquation m_EquationRegressionHigh;
	DTLineEquation m_EquationRegressionLow;
	tagDTRect m_rcVirtualExRect;
	tagDTRect m_rcValidateExRect;
	long m_Distance;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 线性回归 Linear Regression Line
class DTLinearRegressionLineGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTLinearRegressionLineGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTLinearRegressionLineGraph(DTLinearRegressionLineGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_LRL; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	void MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint);
	void MoveP1To(const tagDTPoint& _Point);
	void MoveP2To(const tagDTPoint& _Point);
	int Complete();
	int Clone(DTAssistGraph& _Other);
private:
	DTLineEquation m_EquationBase;
	long m_Distance;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 阻速线 Gould Rays
class DTGouldRaysGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTGouldRaysGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTGouldRaysGraph(DTGouldRaysGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Gould_Rays; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	void MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint);
	void MoveP1To(const tagDTPoint& _Point);
	void MoveP2To(const tagDTPoint& _Point);
	int Complete();
	int Clone(DTAssistGraph& _Other);
protected:
	int Render_i(HDC _hdc);
private:
	DTLineEquation m_EquationBase;
	DTLineEquation m_Equation1;
	DTLineEquation m_Equation2;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 江恩角度线 Gann Rays
class DTGannRaysGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTGannRaysGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTGannRaysGraph(DTGannRaysGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Gann_Rays; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	void MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint);
	void MoveP1To(const tagDTPoint& _Point);
	void MoveP2To(const tagDTPoint& _Point);
	int Complete();
	int Clone(DTAssistGraph& _Other);
protected:
	int Render_i(HDC _hdc);
	void ShowLabel() { m_bShowLabel = true; }
	void HideLabel() { m_bShowLabel = false; }
	bool IsShowLabel() { return m_bShowLabel; }
private:
	bool m_bShowLabel;
	DTLineEquation m_EquationBase;
	DTHorizonEquation m_EquationHorizon;
	DTVerticalEquation m_EquationVertical;
	DTLineEquation m_EquationH1;
	DTLineEquation m_EquationH2;
	DTLineEquation m_EquationH3;
	DTLineEquation m_EquationH4;
	DTLineEquation m_EquationV1;
	DTLineEquation m_EquationV2;
	DTLineEquation m_EquationV3;
	DTLineEquation m_EquationV4;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 矩形框 Rectangle
class DTRectangleGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTRectangleGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTRectangleGraph(DTRectangleGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Rectangle; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
protected:
	int Render_i(HDC _hdc);
private:
	tagDTRect m_Rect;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 红色上箭头 Red Up Arrow
class DTRedUpArrowGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTRedUpArrowGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTRedUpArrowGraph(DTRedUpArrowGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Red_Up_Arrow; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
protected:
	int Render_i(HDC _hdc);
private:
	tagDTRect m_ArrowRect;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 绿色下箭头 Green Down Arrow
class DTGreenDownArrowGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTGreenDownArrowGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTGreenDownArrowGraph(DTGreenDownArrowGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Green_Down_Arrow; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
protected:
	int Render_i(HDC _hdc);
private:
	tagDTRect m_ArrowRect;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 文本 Text
class DTTextGraph : public DTAssistGraphI
{
public:
	static int _NextEditMode(int _CurMode);
	static DTAssistGraphDescriptionScript _DescriptionScript();
public:
	DTTextGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph);
	DTTextGraph(DTTextGraph& _other);
	ENUM_ASSIST_GRAPH_TYPE Type() { return ENUM_ASSIST_GRAPH_TYPE_Text; }
	DTAssistGraphDescriptionScript DescriptionScript();
	int HitTest(const tagDTPoint& _Point, long _xDate = 0);
	int Render(HDC _hdc);
	int RenderShadow(HDC _hdc);
	int RenderEditing(HDC _hdc);
	int RenderAnchors(HDC _hdc);
	int RenderShadowAnchors(HDC _hdc);
	int RenderEditingAnchors(HDC _hdc);
	int RenderSelectedAnchors(HDC _hdc);
	int NextEditMode(int _CurMode);
	int BuildAccordToDescription();
	int Build();
	int EndMove(long _delta_X, long _delta_Y);
	int Complete();
	int Clone(DTAssistGraph& _Other);
	int CopyText(DTAssistGraphPersistText& _Text);
	int CopyText(char* _Text);

	char* Text() { m_Text.Revise(); return m_Text.m_Text; }
protected:
	int Render_i(HDC _hdc);
private:
	DTAssistGraphPersistText m_Text;
	tagDTRect m_TextRect;
};


#endif // _DRAWTOOL_DETAIL__H_
