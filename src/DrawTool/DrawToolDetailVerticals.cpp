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
#include "DrawToolDetail.h"
#include "DrawToolBasicUtil.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 周期线 Cycle Verticals
// <!---
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DTCycleVerticalsGraph::DTCycleVerticalsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph), m_AbsEquationBase(&m_coorTransform)
{
}

DTCycleVerticalsGraph::DTCycleVerticalsGraph(DTCycleVerticalsGraph& _other)
: DTAssistGraphI(_other), m_AbsEquationBase(&m_coorTransform)
{
	m_AbsEquationBase = _other.m_AbsEquationBase;
	EndClone(_other);
}

int DTCycleVerticalsGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTCycleVerticalsGraph::NextEditMode(int _CurMode)
{
	return DTCycleVerticalsGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTCycleVerticalsGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTCycleVerticalsGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTCycleVerticalsGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	tagDTAbsPoint _AbsPoint;
	tagDTPoint _AnchorPoint1;
	tagDTPoint _AnchorPoint2;
	_AnchorPoint1.x = m_ptViewLocationP1.x;
	_AnchorPoint1.y = (m_coorTransform.ViewRect().top + m_coorTransform.ViewRect().bottom) / 2;
	_AnchorPoint2.x = m_ptViewLocationP2.x;
	_AnchorPoint2.y = _AnchorPoint1.y;

	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(_AnchorPoint1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(_AnchorPoint2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
	{
		_AbsPoint.xDate = _xDate;
		_AbsPoint.yValue = m_coorTransform.ViewYtoAbsY(_Point.y);			// igore

		_HitTest = m_AbsEquationBase.HitTest(_AbsPoint);
	}

	return _HitTest;
}

int DTCycleVerticalsGraph::Render(HDC _hdc)
{
	return m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
}

int DTCycleVerticalsGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTCycleVerticalsGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_AbsEquationBase.HideLabel();
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	m_AbsEquationBase.ShowLabel();
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTCycleVerticalsGraph::RenderAnchors(HDC _hdc)
{
	tagDTPoint _AnchorPoint1;
	tagDTPoint _AnchorPoint2;
	_AnchorPoint1.x = m_ptViewLocationP1.x;
	_AnchorPoint1.y = (m_coorTransform.ViewRect().top + m_coorTransform.ViewRect().bottom) / 2;
	_AnchorPoint2.x = m_ptViewLocationP2.x;
	_AnchorPoint2.y = _AnchorPoint1.y;
	//// <!-- prepare colors -->
	COLORREF clrAnchors = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrAnchors);
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrAnchors);
	//// <!-- render -->
	::Rectangle(_hdc, _AnchorPoint1.x - m_nAnchorThickness, _AnchorPoint1.y - m_nAnchorThickness, _AnchorPoint1.x + m_nAnchorThickness, _AnchorPoint1.y + m_nAnchorThickness);
	::Rectangle(_hdc, _AnchorPoint2.x - m_nAnchorThickness, _AnchorPoint2.y - m_nAnchorThickness, _AnchorPoint2.x + m_nAnchorThickness, _AnchorPoint2.y + m_nAnchorThickness);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCBrushColor(_hdc, clrOfBrush);
	return 0;
}

int DTCycleVerticalsGraph::RenderShadowAnchors(HDC _hdc)
{

	return RenderAnchors(_hdc);
}

int DTCycleVerticalsGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTCycleVerticalsGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTCycleVerticalsGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	m_ptViewLocationP2.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);
	m_ptViewLocationP2.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP2.yValue);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x) 
		|| !CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP2.x) )
	{
		m_bAvaiable = false;
		return -1;
	}

	m_bAvaiable = true;
	return Build();
}

int DTCycleVerticalsGraph::Build()
{
	m_rcVirtualRect.left = LONG_INFINITE_LEFT;
	m_rcVirtualRect.top = LONG_INFINITE_TOP;
	m_rcVirtualRect.right = LONG_INFINITE_RIGHT;
	m_rcVirtualRect.bottom = LONG_INFINITE_BOTTOM;
	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	long _index_xDate1 = m_coorTransform.IndexOfDate(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	long _index_xDate2 = m_coorTransform.IndexOfDate(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);

	if(!CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate)
		|| !CHECK_VIEW_XINDEX_AXIS_AVAIABLE(_index_xDate1)
		|| !CHECK_VIEW_XINDEX_AXIS_AVAIABLE(_index_xDate2) )
	{
		m_bAvaiable = false;
		return -1;
	}

	if(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate <= m_descGraph.m_ptLocations.ptAbsoluteP2.xDate)
		m_AbsEquationBase.MoveTo(m_descGraph.m_ptLocations.ptAbsoluteP1);
	else
		m_AbsEquationBase.MoveTo(m_descGraph.m_ptLocations.ptAbsoluteP2);
	long _Diff = ::abs(_index_xDate2 - _index_xDate1);
	m_AbsEquationBase.EqualDifferenceOnXDateAxis(_Diff);
	return 0;
}

void DTCycleVerticalsGraph::MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint)
{
	long _delta_X = _ToPoint.x - _FromPoint.x;
	long _delta_Y = _ToPoint.y - _FromPoint.y;

	m_ptViewLocationP1.x += _delta_X;
	m_ptViewLocationP1.y += _delta_Y;

	m_ptViewLocationP2.x += _delta_X;
	m_ptViewLocationP2.y += _delta_Y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x)
		|| !CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP2.x)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate) )
	{
		m_bAvaiable = false;
		return ;
	}

	m_bAvaiable = true;
	Build();
}

void DTCycleVerticalsGraph::MoveP1To(const tagDTPoint& _Point)
{
	m_ptViewLocationP1.x = _Point.x;
	m_ptViewLocationP1.y = _Point.y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate) )
	{
		m_bAvaiable = false;
		return ;
	}

	m_bAvaiable = true;
	Build();
}

void DTCycleVerticalsGraph::MoveP2To(const tagDTPoint& _Point)
{
	m_ptViewLocationP2.x = _Point.x;
	m_ptViewLocationP2.y = _Point.y;

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP2.x)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate) )
	{
		m_bAvaiable = false;
		return ;
	}

	m_bAvaiable = true;
	Build();
}

int DTCycleVerticalsGraph::Complete()
{
	return BuildAccordToDescription();
}

int DTCycleVerticalsGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTCycleVerticalsGraph& _Clone = reinterpret_cast<DTCycleVerticalsGraph&>(_Other);
	m_AbsEquationBase = _Clone.m_AbsEquationBase;
	return DTAssistGraph::Clone(_Other);
}

int DTCycleVerticalsGraph::EndClone(DTAssistGraph& _Other)
{
	m_AbsEquationBase.CoordinateTransform(&m_coorTransform);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 周期线 Cycle Verticals
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 斐波那契线 Fibonacci Verticals
// <!---
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DTFibonacciVerticalsGraph::DTFibonacciVerticalsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph), m_AbsEquationBase(&m_coorTransform)
{
}

DTFibonacciVerticalsGraph::DTFibonacciVerticalsGraph(DTFibonacciVerticalsGraph& _other)
: DTAssistGraphI(_other), m_AbsEquationBase(&m_coorTransform)
{
	m_AbsEquationBase = _other.m_AbsEquationBase;
	EndClone(_other);
}

int DTFibonacciVerticalsGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSinglePoint(_CurMode);
}

int DTFibonacciVerticalsGraph::NextEditMode(int _CurMode)
{
	return DTFibonacciVerticalsGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTFibonacciVerticalsGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSinglePoint();
}

DTAssistGraphDescriptionScript DTFibonacciVerticalsGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTFibonacciVerticalsGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	tagDTAbsPoint _AbsPoint;
	tagDTPoint _AnchorPoint1;
	_AnchorPoint1.x = m_ptViewLocationP1.x;
	_AnchorPoint1.y = (m_coorTransform.ViewRect().top + m_coorTransform.ViewRect().bottom) / 2;

	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(_AnchorPoint1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
	{
		_AbsPoint.xDate = _xDate;
		_AbsPoint.yValue = m_coorTransform.ViewYtoAbsY(_Point.y);			// igore

		_HitTest = m_AbsEquationBase.HitTest(_AbsPoint);
	}

	return _HitTest;
}

int DTFibonacciVerticalsGraph::Render(HDC _hdc)
{
	return m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
}

int DTFibonacciVerticalsGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTFibonacciVerticalsGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_AbsEquationBase.HideLabel();
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	m_AbsEquationBase.ShowLabel();
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTFibonacciVerticalsGraph::RenderAnchors(HDC _hdc)
{
	tagDTAbsPoint _AbsPoint;
	tagDTPoint _AnchorPoint1;
	_AnchorPoint1.x = m_ptViewLocationP1.x;
	_AnchorPoint1.y = (m_coorTransform.ViewRect().top + m_coorTransform.ViewRect().bottom) / 2;

	//// <!-- prepare colors -->
	COLORREF clrAnchors = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrAnchors);
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrAnchors);
	//// <!-- render -->
	::Rectangle(_hdc, _AnchorPoint1.x - m_nAnchorThickness, _AnchorPoint1.y - m_nAnchorThickness, _AnchorPoint1.x + m_nAnchorThickness, _AnchorPoint1.y + m_nAnchorThickness);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCBrushColor(_hdc, clrOfBrush);
	return 0;
}

int DTFibonacciVerticalsGraph::RenderShadowAnchors(HDC _hdc)
{

	return RenderAnchors(_hdc);
}

int DTFibonacciVerticalsGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTFibonacciVerticalsGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTFibonacciVerticalsGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x))
	{
		m_bAvaiable = false;
		return -1;
	}
	
	m_bAvaiable = true;
	return Build();
}

int DTFibonacciVerticalsGraph::Build()
{
	m_rcVirtualRect.left = LONG_INFINITE_LEFT;
	m_rcVirtualRect.top = LONG_INFINITE_TOP;
	m_rcVirtualRect.right = LONG_INFINITE_RIGHT;
	m_rcVirtualRect.bottom = LONG_INFINITE_BOTTOM;
	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	return m_AbsEquationBase.MoveTo(m_descGraph.m_ptLocations.ptAbsoluteP1);
}

void DTFibonacciVerticalsGraph::MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint)
{
	long _delta_X = _ToPoint.x - _FromPoint.x;
	long _delta_Y = _ToPoint.y - _FromPoint.y;

	m_ptViewLocationP1.x += _delta_X;
	m_ptViewLocationP1.y += _delta_Y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate) )
	{
		m_bAvaiable = false;
		return ;
	}

	m_bAvaiable = true;
	Build();
}

void DTFibonacciVerticalsGraph::MoveP1To(const tagDTPoint& _Point)
{
	m_ptViewLocationP1.x = _Point.x;
	m_ptViewLocationP1.y = _Point.y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate) )
	{
		m_bAvaiable = false;
		return ;
	}

	m_bAvaiable = true;
	Build();
}

int DTFibonacciVerticalsGraph::Complete()
{
	return BuildAccordToDescription();
}

int DTFibonacciVerticalsGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTFibonacciVerticalsGraph& _Clone = reinterpret_cast<DTFibonacciVerticalsGraph&>(_Other);
	m_AbsEquationBase = _Clone.m_AbsEquationBase;
	return DTAssistGraph::Clone(_Other);
}

int DTFibonacciVerticalsGraph::EndClone(DTAssistGraph& _Other)
{
	m_AbsEquationBase.CoordinateTransform(&m_coorTransform);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 斐波那契线 Fibonacci Verticals
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 江恩时序列 Gann Sequence Verticals
// <!---
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTGannSequenceVerticalsGraph::DTGannSequenceVerticalsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph), m_AbsEquationBase(&m_coorTransform)
{
}

DTGannSequenceVerticalsGraph::DTGannSequenceVerticalsGraph(DTGannSequenceVerticalsGraph& _other)
: DTAssistGraphI(_other), m_AbsEquationBase(&m_coorTransform)
{
	m_AbsEquationBase = _other.m_AbsEquationBase;
	EndClone(_other);
}

int DTGannSequenceVerticalsGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSinglePoint(_CurMode);
}

int DTGannSequenceVerticalsGraph::NextEditMode(int _CurMode)
{
	return DTGannSequenceVerticalsGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTGannSequenceVerticalsGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSinglePoint();
}

DTAssistGraphDescriptionScript DTGannSequenceVerticalsGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTGannSequenceVerticalsGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	tagDTAbsPoint _AbsPoint;
	tagDTPoint _AnchorPoint1;
	_AnchorPoint1.x = m_ptViewLocationP1.x;
	_AnchorPoint1.y = (m_coorTransform.ViewRect().top + m_coorTransform.ViewRect().bottom) / 2;

	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(_AnchorPoint1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
	{
		_AbsPoint.xDate = _xDate;
		_AbsPoint.yValue = m_coorTransform.ViewYtoAbsY(_Point.y);			// igore

		_HitTest = m_AbsEquationBase.HitTest(_AbsPoint);
	}

	return _HitTest;
}

int DTGannSequenceVerticalsGraph::Render(HDC _hdc)
{
	return m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
}

int DTGannSequenceVerticalsGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGannSequenceVerticalsGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_AbsEquationBase.HideLabel();
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	m_AbsEquationBase.ShowLabel();
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGannSequenceVerticalsGraph::RenderAnchors(HDC _hdc)
{
	tagDTAbsPoint _AbsPoint;
	tagDTPoint _AnchorPoint1;
	_AnchorPoint1.x = m_ptViewLocationP1.x;
	_AnchorPoint1.y = (m_coorTransform.ViewRect().top + m_coorTransform.ViewRect().bottom) / 2;

	//// <!-- prepare colors -->
	COLORREF clrAnchors = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrAnchors);
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrAnchors);
	//// <!-- render -->
	::Rectangle(_hdc, _AnchorPoint1.x - m_nAnchorThickness, _AnchorPoint1.y - m_nAnchorThickness, _AnchorPoint1.x + m_nAnchorThickness, _AnchorPoint1.y + m_nAnchorThickness);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCBrushColor(_hdc, clrOfBrush);
	return 0;
}

int DTGannSequenceVerticalsGraph::RenderShadowAnchors(HDC _hdc)
{

	return RenderAnchors(_hdc);
}

int DTGannSequenceVerticalsGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTGannSequenceVerticalsGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGannSequenceVerticalsGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x))
	{
		m_bAvaiable = false;
		return -1;
	}

	m_bAvaiable = true;
	return Build();
}

int DTGannSequenceVerticalsGraph::Build()
{
	m_rcVirtualRect.left = LONG_INFINITE_LEFT;
	m_rcVirtualRect.top = LONG_INFINITE_TOP;
	m_rcVirtualRect.right = LONG_INFINITE_RIGHT;
	m_rcVirtualRect.bottom = LONG_INFINITE_BOTTOM;
	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	return m_AbsEquationBase.MoveTo(m_descGraph.m_ptLocations.ptAbsoluteP1);
}

void DTGannSequenceVerticalsGraph::MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint)
{
	long _delta_X = _ToPoint.x - _FromPoint.x;
	long _delta_Y = _ToPoint.y - _FromPoint.y;

	m_ptViewLocationP1.x += _delta_X;
	m_ptViewLocationP1.y += _delta_Y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate) )
	{
		m_bAvaiable = false;
		return ;
	}

	m_bAvaiable = true;
	Build();
}

void DTGannSequenceVerticalsGraph::MoveP1To(const tagDTPoint& _Point)
{
	m_ptViewLocationP1.x = _Point.x;
	m_ptViewLocationP1.y = _Point.y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate) )
	{
		m_bAvaiable = false;
		return ;
	}

	m_bAvaiable = true;
	Build();
}

int DTGannSequenceVerticalsGraph::Complete()
{
	return BuildAccordToDescription();
}

int DTGannSequenceVerticalsGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTGannSequenceVerticalsGraph& _Clone = reinterpret_cast<DTGannSequenceVerticalsGraph&>(_Other);
	m_AbsEquationBase = _Clone.m_AbsEquationBase;
	return DTAssistGraph::Clone(_Other);
}

int DTGannSequenceVerticalsGraph::EndClone(DTAssistGraph& _Other)
{
	m_AbsEquationBase.CoordinateTransform(&m_coorTransform);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 江恩时序列 Gann Sequence Verticals
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////