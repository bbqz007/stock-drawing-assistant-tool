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
// 黄金价位线 Golden Price Horizons
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTGoldenPriceHorizonsGraph::DTGoldenPriceHorizonsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph), m_AbsEquationBase(&m_coorTransform)
{
}

DTGoldenPriceHorizonsGraph::DTGoldenPriceHorizonsGraph(DTGoldenPriceHorizonsGraph& _other)
: DTAssistGraphI(_other), m_AbsEquationBase(&m_coorTransform)
{
	m_AbsEquationBase = _other.m_AbsEquationBase;
	EndClone(_other);
}

int DTGoldenPriceHorizonsGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSinglePoint(_CurMode);
}

int DTGoldenPriceHorizonsGraph::NextEditMode(int _CurMode)
{
	return DTGoldenPriceHorizonsGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTGoldenPriceHorizonsGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSinglePoint();
}

DTAssistGraphDescriptionScript DTGoldenPriceHorizonsGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTGoldenPriceHorizonsGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	tagDTAbsPoint _AbsPoint;
	tagDTPoint _AnchorPoint1;
	_AnchorPoint1.x = (m_coorTransform.ViewRect().left + m_coorTransform.ViewRect().right) / 2;
	_AnchorPoint1.y = m_ptViewLocationP1.y;

	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(_AnchorPoint1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;

	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
	{
		_AbsPoint.xDate = _xDate;
		_AbsPoint.yValue = m_coorTransform.ViewYtoAbsY(_Point.y);

		_HitTest = m_AbsEquationBase.HitTest(_AbsPoint);
	}
	return _HitTest;
}

int DTGoldenPriceHorizonsGraph::Render(HDC _hdc)
{
	/*if(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate > m_Graphics->Coordinate().BoundDate())
		return 0;*/
	return m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
}

int DTGoldenPriceHorizonsGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	//// <!-- render -->
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGoldenPriceHorizonsGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_AbsEquationBase.HideLabel();
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	m_AbsEquationBase.ShowLabel();
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGoldenPriceHorizonsGraph::RenderAnchors(HDC _hdc)
{
	//// <!-- prepare anchors -->
	tagDTPoint _AnchorPoint1;
	_AnchorPoint1.x = (m_coorTransform.ViewRect().left + m_coorTransform.ViewRect().right) / 2;
	_AnchorPoint1.y = m_ptViewLocationP1.y;
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

int DTGoldenPriceHorizonsGraph::RenderShadowAnchors(HDC _hdc)
{

	return RenderAnchors(_hdc);
}

int DTGoldenPriceHorizonsGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTGoldenPriceHorizonsGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGoldenPriceHorizonsGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	m_bAvaiable = true;
	return Build();
}

int DTGoldenPriceHorizonsGraph::Build()
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

	if(!IsAvaiable())
		return -1;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	return m_AbsEquationBase.MoveTo(m_descGraph.m_ptLocations.ptAbsoluteP1);
}

int DTGoldenPriceHorizonsGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	return BuildAccordToDescription();
}

int DTGoldenPriceHorizonsGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTGoldenPriceHorizonsGraph& _Clone = reinterpret_cast<DTGoldenPriceHorizonsGraph&>(_Other);
	m_AbsEquationBase = _Clone.m_AbsEquationBase;
	return DTAssistGraph::Clone(_Other);
}

int DTGoldenPriceHorizonsGraph::EndClone(DTAssistGraph& _Other)
{
	m_AbsEquationBase.CoordinateTransform(&m_coorTransform);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 黄金价位线 Golden Price Horizons
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 黄金目标线 Golden Aim Horizons
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTGoldenAimHorizonsGraph::DTGoldenAimHorizonsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph), m_AbsEquationBase(&m_coorTransform)
{
}

DTGoldenAimHorizonsGraph::DTGoldenAimHorizonsGraph(DTGoldenAimHorizonsGraph& _other)
: DTAssistGraphI(_other), m_AbsEquationBase(&m_coorTransform)
{
	m_AbsEquationBase = _other.m_AbsEquationBase;
	EndClone(_other);
}

int DTGoldenAimHorizonsGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfParallelLine(_CurMode);;
}

int DTGoldenAimHorizonsGraph::NextEditMode(int _CurMode)
{
	return DTGoldenAimHorizonsGraph::_NextEditMode(_CurMode);;
}

DTAssistGraphDescriptionScript DTGoldenAimHorizonsGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfParallelLine();
}

DTAssistGraphDescriptionScript DTGoldenAimHorizonsGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTGoldenAimHorizonsGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	tagDTAbsPoint _AbsPoint;
	tagDTRect _HelpRect1;
	tagDTRect _HelpRect2;
	_HelpRect1.left = min(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	_HelpRect1.top = min(m_ptViewLocationP1.y, m_ptViewLocationP2.y);
	_HelpRect1.right = max(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	_HelpRect1.bottom = max(m_ptViewLocationP1.y, m_ptViewLocationP2.y);
	_HelpRect2.left = min(m_ptViewLocationP2.x, m_ptViewLocationP3.x);
	_HelpRect2.top = min(m_ptViewLocationP2.y, m_ptViewLocationP3.y);
	_HelpRect2.right = max(m_ptViewLocationP2.x, m_ptViewLocationP3.x);
	_HelpRect2.bottom = max(m_ptViewLocationP2.y, m_ptViewLocationP3.y);

	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP3, _Point, ENUM_ASSIST_HITTEST_LOCATION_P3);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;

	if(_HitTest == ENUM_ASSIST_HITTEST_NULL && ::PtInRect(&_HelpRect1, _Point))
		_HitTest = m_HelperLine1.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL && ::PtInRect(&_HelpRect2, _Point))
		_HitTest = m_HelperLine2.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
	{
		_AbsPoint.xDate = _xDate;
		_AbsPoint.yValue = m_coorTransform.ViewYtoAbsY(_Point.y);

		_HitTest = m_AbsEquationBase.HitTest(_AbsPoint);
	}

	return _HitTest;
}

int DTGoldenAimHorizonsGraph::Render(HDC _hdc)
{
	::MoveToEx(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y, NULL);
	::LineTo(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y);
	::LineTo(_hdc, m_ptViewLocationP3.x, m_ptViewLocationP3.y);
	return m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
}

int DTGoldenAimHorizonsGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	//// <!-- render -->
	::MoveToEx(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y, NULL);
	::LineTo(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y);
	::LineTo(_hdc, m_ptViewLocationP3.x, m_ptViewLocationP3.y);
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGoldenAimHorizonsGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	::MoveToEx(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y, NULL);
	::LineTo(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y);
	::LineTo(_hdc, m_ptViewLocationP3.x, m_ptViewLocationP3.y);
	m_AbsEquationBase.HideLabel();
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	m_AbsEquationBase.ShowLabel();
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGoldenAimHorizonsGraph::RenderAnchors(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrAnchors = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrAnchors);
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrAnchors);
	//// <!-- render -->
	::Rectangle(_hdc, m_ptViewLocationP1.x - m_nAnchorThickness, m_ptViewLocationP1.y - m_nAnchorThickness, m_ptViewLocationP1.x + m_nAnchorThickness, m_ptViewLocationP1.y + m_nAnchorThickness);
	::Rectangle(_hdc, m_ptViewLocationP2.x - m_nAnchorThickness, m_ptViewLocationP2.y - m_nAnchorThickness, m_ptViewLocationP2.x + m_nAnchorThickness, m_ptViewLocationP2.y + m_nAnchorThickness);
	::Rectangle(_hdc, m_ptViewLocationP3.x - m_nAnchorThickness, m_ptViewLocationP3.y - m_nAnchorThickness, m_ptViewLocationP3.x + m_nAnchorThickness, m_ptViewLocationP3.y + m_nAnchorThickness);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCBrushColor(_hdc, clrOfBrush);
	return 0;
}

int DTGoldenAimHorizonsGraph::RenderShadowAnchors(HDC _hdc)
{

	return RenderAnchors(_hdc);
}

int DTGoldenAimHorizonsGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTGoldenAimHorizonsGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGoldenAimHorizonsGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	m_ptViewLocationP2.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);
	m_ptViewLocationP2.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP2.yValue);

	m_ptViewLocationP3.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP3.xDate);
	m_ptViewLocationP3.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP3.yValue);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x) 
		|| !CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP2.x)
		|| !CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP3.x) )
	{
		m_bAvaiable = false;
		return -1;
	}

	m_bAvaiable = true;
	return Build();
}

int DTGoldenAimHorizonsGraph::Build()
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

	if(!IsAvaiable())
		return -1;

	m_descGraph.m_ptLocations.ptAbsoluteP3.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP3.x);
	m_descGraph.m_ptLocations.ptAbsoluteP3.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP3.y);

	float _delta_value = (m_ptViewLocationP2.y - m_ptViewLocationP1.y) * m_coorTransform.UnitHeight();
	m_AbsEquationBase.MoveTo(m_descGraph.m_ptLocations.ptAbsoluteP3);
	m_AbsEquationBase.PositiveOnYValAxis(IS_POSITIVE_ON_VIEW_Y_AXIS(m_ptViewLocationP2.y, m_ptViewLocationP1.y));
	m_AbsEquationBase.DistanceOnYValAxis(_delta_value);

	m_HelperLine1.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	m_HelperLine2.Build(m_ptViewLocationP2, m_ptViewLocationP3);
	return 0;
}

int DTGoldenAimHorizonsGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	m_descGraph.m_ptLocations.ptAbsoluteP3.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP3.x);
	m_descGraph.m_ptLocations.ptAbsoluteP3.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP3.y);

	return BuildAccordToDescription();
}

int DTGoldenAimHorizonsGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTGoldenAimHorizonsGraph& _Clone = reinterpret_cast<DTGoldenAimHorizonsGraph&>(_Other);
	m_AbsEquationBase = _Clone.m_AbsEquationBase;
	m_HelperLine1 = _Clone.m_HelperLine1;
	m_HelperLine2 = _Clone.m_HelperLine2;
	return DTAssistGraph::Clone(_Other);
}

int DTGoldenAimHorizonsGraph::EndClone(DTAssistGraph& _Other)
{
	m_AbsEquationBase.CoordinateTransform(&m_coorTransform);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 黄金目标线 Golden Aim Horizons
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 黄金分割 Golden Secion Horizons
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTGoldenSecionHorizonsGraph::DTGoldenSecionHorizonsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph), m_AbsEquationBase(&m_coorTransform)
{
}

DTGoldenSecionHorizonsGraph::DTGoldenSecionHorizonsGraph(DTGoldenSecionHorizonsGraph& _other)
: DTAssistGraphI(_other), m_AbsEquationBase(&m_coorTransform)
{
	m_AbsEquationBase = _other.m_AbsEquationBase;
	EndClone(_other);
}

int DTGoldenSecionHorizonsGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTGoldenSecionHorizonsGraph::NextEditMode(int _CurMode)
{
	return DTGoldenSecionHorizonsGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTGoldenSecionHorizonsGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTGoldenSecionHorizonsGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTGoldenSecionHorizonsGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	tagDTAbsPoint _AbsPoint;
	tagDTPoint _AnchorPoint1;
	tagDTPoint _AnchorPoint2;
	_AnchorPoint1.x = (m_coorTransform.ViewRect().left + m_coorTransform.ViewRect().right) / 2;
	_AnchorPoint1.y = m_ptViewLocationP1.y;
	_AnchorPoint2.x = _AnchorPoint1.x;
	_AnchorPoint2.y = m_ptViewLocationP2.y;

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
		_AbsPoint.yValue = m_coorTransform.ViewYtoAbsY(_Point.y);

		_HitTest = m_AbsEquationBase.HitTest(_AbsPoint);
	}
	return _HitTest;
}

int DTGoldenSecionHorizonsGraph::Render(HDC _hdc)
{
	return m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
}

int DTGoldenSecionHorizonsGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	//// <!-- render -->
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGoldenSecionHorizonsGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_AbsEquationBase.HideLabel();
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	m_AbsEquationBase.ShowLabel();
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGoldenSecionHorizonsGraph::RenderAnchors(HDC _hdc)
{
	//// <!-- prepare anchors -->
	tagDTPoint _AnchorPoint1;
	tagDTPoint _AnchorPoint2;
	_AnchorPoint1.x = (m_coorTransform.ViewRect().left + m_coorTransform.ViewRect().right) / 2;
	_AnchorPoint1.y = m_ptViewLocationP1.y;
	_AnchorPoint2.x = _AnchorPoint1.x;
	_AnchorPoint2.y = m_ptViewLocationP2.y;
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

int DTGoldenSecionHorizonsGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGoldenSecionHorizonsGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTGoldenSecionHorizonsGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGoldenSecionHorizonsGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	m_ptViewLocationP2.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);
	m_ptViewLocationP2.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP2.yValue);

	m_bAvaiable = true;
	return Build();
}

int DTGoldenSecionHorizonsGraph::Build()
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

	if(!IsAvaiable())
		return -1;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	float _delta_value = (m_ptViewLocationP2.y - m_ptViewLocationP1.y) * m_coorTransform.UnitHeight();
	m_AbsEquationBase.MoveTo(m_descGraph.m_ptLocations.ptAbsoluteP1);
	m_AbsEquationBase.PositiveOnYValAxis(IS_POSITIVE_ON_VIEW_Y_AXIS(m_ptViewLocationP2.y, m_ptViewLocationP1.y));
	m_AbsEquationBase.DistanceOnYValAxis(_delta_value);

	return 0;
}

int DTGoldenSecionHorizonsGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTGoldenSecionHorizonsGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTGoldenSecionHorizonsGraph& _Clone = reinterpret_cast<DTGoldenSecionHorizonsGraph&>(_Other);
	m_AbsEquationBase = _Clone.m_AbsEquationBase;
	return DTAssistGraph::Clone(_Other);
}

int DTGoldenSecionHorizonsGraph::EndClone(DTAssistGraph& _Other)
{
	m_AbsEquationBase.CoordinateTransform(&m_coorTransform);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 黄金分割 Golden Secion Horizons
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 百分比线 Percentage Horizons
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTPercentageHorizonsGraph::DTPercentageHorizonsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph), m_AbsEquationBase(&m_coorTransform)
{
}

DTPercentageHorizonsGraph::DTPercentageHorizonsGraph(DTPercentageHorizonsGraph& _other)
: DTAssistGraphI(_other), m_AbsEquationBase(&m_coorTransform)
{
	m_AbsEquationBase = _other.m_AbsEquationBase;
	EndClone(_other);
}

int DTPercentageHorizonsGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTPercentageHorizonsGraph::NextEditMode(int _CurMode)
{
	return DTPercentageHorizonsGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTPercentageHorizonsGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTPercentageHorizonsGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTPercentageHorizonsGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	tagDTAbsPoint _AbsPoint;
	tagDTPoint _AnchorPoint1;
	tagDTPoint _AnchorPoint2;
	_AnchorPoint1.x = (m_coorTransform.ViewRect().left + m_coorTransform.ViewRect().right) / 2;
	_AnchorPoint1.y = m_ptViewLocationP1.y;
	_AnchorPoint2.x = _AnchorPoint1.x;
	_AnchorPoint2.y = m_ptViewLocationP2.y;

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
		_AbsPoint.yValue = m_coorTransform.ViewYtoAbsY(_Point.y);

		_HitTest = m_AbsEquationBase.HitTest(_AbsPoint);
	}
	return _HitTest;
}

int DTPercentageHorizonsGraph::Render(HDC _hdc)
{
	return m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
}

int DTPercentageHorizonsGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	//// <!-- render -->
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTPercentageHorizonsGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_AbsEquationBase.HideLabel();
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	m_AbsEquationBase.ShowLabel();
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTPercentageHorizonsGraph::RenderAnchors(HDC _hdc)
{
	//// <!-- prepare anchors -->
	tagDTPoint _AnchorPoint1;
	tagDTPoint _AnchorPoint2;
	_AnchorPoint1.x = (m_coorTransform.ViewRect().left + m_coorTransform.ViewRect().right) / 2;
	_AnchorPoint1.y = m_ptViewLocationP1.y;
	_AnchorPoint2.x = _AnchorPoint1.x;
	_AnchorPoint2.y = m_ptViewLocationP2.y;
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

int DTPercentageHorizonsGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTPercentageHorizonsGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTPercentageHorizonsGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTPercentageHorizonsGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	m_ptViewLocationP2.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);
	m_ptViewLocationP2.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP2.yValue);

	m_bAvaiable = true;
	return Build();
}

int DTPercentageHorizonsGraph::Build()
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

	if(!IsAvaiable())
		return -1;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	float _delta_value = (m_ptViewLocationP2.y - m_ptViewLocationP1.y) * m_coorTransform.UnitHeight();
	m_AbsEquationBase.MoveTo(m_descGraph.m_ptLocations.ptAbsoluteP1);
	m_AbsEquationBase.PositiveOnYValAxis(IS_POSITIVE_ON_VIEW_Y_AXIS(m_ptViewLocationP2.y, m_ptViewLocationP1.y));
	m_AbsEquationBase.DistanceOnYValAxis(_delta_value);

	return 0;
}

int DTPercentageHorizonsGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTPercentageHorizonsGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTPercentageHorizonsGraph& _Clone = reinterpret_cast<DTPercentageHorizonsGraph&>(_Other);
	m_AbsEquationBase = _Clone.m_AbsEquationBase;
	return DTAssistGraph::Clone(_Other);
}

int DTPercentageHorizonsGraph::EndClone(DTAssistGraph& _Other)
{
	m_AbsEquationBase.CoordinateTransform(&m_coorTransform);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 百分比线 Percentage Horizons
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 波段线 Oct Band Horizons
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTOctBandHorizonsGraph::DTOctBandHorizonsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph), m_AbsEquationBase(&m_coorTransform)
{
}

DTOctBandHorizonsGraph::DTOctBandHorizonsGraph(DTOctBandHorizonsGraph& _other)
: DTAssistGraphI(_other), m_AbsEquationBase(&m_coorTransform)
{
	m_AbsEquationBase = _other.m_AbsEquationBase;
	EndClone(_other);
}

int DTOctBandHorizonsGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTOctBandHorizonsGraph::NextEditMode(int _CurMode)
{
	return DTOctBandHorizonsGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTOctBandHorizonsGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTOctBandHorizonsGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTOctBandHorizonsGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	tagDTAbsPoint _AbsPoint;
	tagDTPoint _AnchorPoint1;
	tagDTPoint _AnchorPoint2;
	_AnchorPoint1.x = (m_coorTransform.ViewRect().left + m_coorTransform.ViewRect().right) / 2;
	_AnchorPoint1.y = m_ptViewLocationP1.y;
	_AnchorPoint2.x = _AnchorPoint1.x;
	_AnchorPoint2.y = m_ptViewLocationP2.y;

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
		_AbsPoint.yValue = m_coorTransform.ViewYtoAbsY(_Point.y);

		_HitTest = m_AbsEquationBase.HitTest(_AbsPoint);
	}
	return _HitTest;
}

int DTOctBandHorizonsGraph::Render(HDC _hdc)
{
	return m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
}

int DTOctBandHorizonsGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	//// <!-- render -->
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTOctBandHorizonsGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_AbsEquationBase.HideLabel();
	m_AbsEquationBase.Render(_hdc, m_rcValidateRect);
	m_AbsEquationBase.ShowLabel();
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTOctBandHorizonsGraph::RenderAnchors(HDC _hdc)
{
	//// <!-- prepare anchors -->
	tagDTPoint _AnchorPoint1;
	tagDTPoint _AnchorPoint2;
	_AnchorPoint1.x = (m_coorTransform.ViewRect().left + m_coorTransform.ViewRect().right) / 2;
	_AnchorPoint1.y = m_ptViewLocationP1.y;
	_AnchorPoint2.x = _AnchorPoint1.x;
	_AnchorPoint2.y = m_ptViewLocationP2.y;
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

int DTOctBandHorizonsGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTOctBandHorizonsGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTOctBandHorizonsGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTOctBandHorizonsGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	m_ptViewLocationP2.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);
	m_ptViewLocationP2.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP2.yValue);

	m_bAvaiable = true;
	return Build();
}

int DTOctBandHorizonsGraph::Build()
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

	if(!IsAvaiable())
		return -1;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	float _delta_value = (m_ptViewLocationP2.y - m_ptViewLocationP1.y) * m_coorTransform.UnitHeight();
	m_AbsEquationBase.MoveTo(m_descGraph.m_ptLocations.ptAbsoluteP1);
	m_AbsEquationBase.PositiveOnYValAxis(IS_POSITIVE_ON_VIEW_Y_AXIS(m_ptViewLocationP2.y, m_ptViewLocationP1.y));
	m_AbsEquationBase.DistanceOnYValAxis(_delta_value);

	return 0;
}

int DTOctBandHorizonsGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTOctBandHorizonsGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTOctBandHorizonsGraph& _Clone = reinterpret_cast<DTOctBandHorizonsGraph&>(_Other);
	m_AbsEquationBase = _Clone.m_AbsEquationBase;
	return DTAssistGraph::Clone(_Other);
}

int DTOctBandHorizonsGraph::EndClone(DTAssistGraph& _Other)
{
	m_AbsEquationBase.CoordinateTransform(&m_coorTransform);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 波段线 Oct Band Horizons
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////