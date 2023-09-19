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
// 阻速线 Gould Rays
// <!---
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DTGouldRaysGraph::DTGouldRaysGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTGouldRaysGraph::DTGouldRaysGraph(DTGouldRaysGraph& _other)
: DTAssistGraphI(_other)
{
	m_EquationBase = _other.m_EquationBase;
	m_Equation1 = _other.m_Equation1;
	m_Equation2 = _other.m_Equation2;
}

int DTGouldRaysGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTGouldRaysGraph::NextEditMode(int _CurMode)
{
	return DTGouldRaysGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTGouldRaysGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTGouldRaysGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTGouldRaysGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationBase.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_Equation1.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_Equation2.HitTest(_Point);

	return _HitTest;
}

int DTGouldRaysGraph::Render_i(HDC _hdc)
{
	m_EquationBase.Render(_hdc, m_rcValidateRect);

	HPEN dotPen = ::CreatePen(PS_DOT, 1, ::GetDCPenColor(_hdc));
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);
	m_Equation1.Render(_hdc, m_rcValidateRect);
	m_Equation2.Render(_hdc, m_rcValidateRect);
	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);

	return 0;
}

int DTGouldRaysGraph::Render(HDC _hdc)
{
	return Render_i(_hdc);
}

int DTGouldRaysGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGouldRaysGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGouldRaysGraph::RenderAnchors(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrAnchors = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrAnchors);
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrAnchors);
	//// <!-- render -->
	::Rectangle(_hdc, m_ptViewLocationP1.x - m_nAnchorThickness, m_ptViewLocationP1.y - m_nAnchorThickness, m_ptViewLocationP1.x + m_nAnchorThickness, m_ptViewLocationP1.y + m_nAnchorThickness);
	::Rectangle(_hdc, m_ptViewLocationP2.x - m_nAnchorThickness, m_ptViewLocationP2.y - m_nAnchorThickness, m_ptViewLocationP2.x + m_nAnchorThickness, m_ptViewLocationP2.y + m_nAnchorThickness);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCBrushColor(_hdc, clrOfBrush);
	return 0;
}

int DTGouldRaysGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGouldRaysGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTGouldRaysGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGouldRaysGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	m_ptViewLocationP2.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);
	m_ptViewLocationP2.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP2.yValue);

	if(!CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate)
		|| !CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x)
		|| !CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP2.x)
		|| m_ptViewLocationP1.x >= m_ptViewLocationP2.x)
	{
		m_bAvaiable = false;
		return -1;
	}

	m_bAvaiable = true;
	return Build();
}

int DTGouldRaysGraph::Build()
{
	m_rcVirtualRect.left = m_ptViewLocationP1.x;
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

	long _delta_X = m_ptViewLocationP2.x - m_ptViewLocationP1.x;
	long _delta_Y = m_ptViewLocationP2.y - m_ptViewLocationP1.y;
	tagDTPoint _PointB = m_ptViewLocationP2;

	if(_delta_X <= 0)
	{
		m_bAvaiable = false;
		return -1;
	}

	m_bAvaiable = true;

	m_EquationBase.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	_PointB.y = m_ptViewLocationP1.y + (1.0 / 3.0) * _delta_Y;
	m_Equation1.Build(m_ptViewLocationP1, _PointB);
	_PointB.y = m_ptViewLocationP1.y + (2.0 / 3.0) * _delta_Y;
	m_Equation2.Build(m_ptViewLocationP1, _PointB);
	return 0;
}

void DTGouldRaysGraph::MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint)
{
	long _delta_X = _ToPoint.x - _FromPoint.x;
	long _delta_Y = _ToPoint.y - _FromPoint.y;

	m_ptViewLocationP1.x += _delta_X;
	m_ptViewLocationP1.y += _delta_Y;

	m_ptViewLocationP2.x += _delta_X;
	m_ptViewLocationP2.y += _delta_Y;

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x)
		|| !CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP2.x)
		|| m_ptViewLocationP1.x >= m_ptViewLocationP2.x )
	{
		m_bAvaiable = false;
		return ;
	}

	m_bAvaiable = true;
	Build();
}

void DTGouldRaysGraph::MoveP1To(const tagDTPoint& _Point)
{
	m_ptViewLocationP1.x = _Point.x;
	m_ptViewLocationP1.y = _Point.y;

	Build();
}

void DTGouldRaysGraph::MoveP2To(const tagDTPoint& _Point)
{
	m_ptViewLocationP2.x = _Point.x;
	m_ptViewLocationP2.y = _Point.y;

	Build();
}

int DTGouldRaysGraph::Complete()
{
	m_ptViewLocationP1.x = m_coorTransform.AdjustViewX(m_ptViewLocationP1.x);
	m_ptViewLocationP2.x = m_coorTransform.AdjustViewX(m_ptViewLocationP2.x);

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTGouldRaysGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTGouldRaysGraph& _Clone = reinterpret_cast<DTGouldRaysGraph&>(_Other);
	m_EquationBase = _Clone.m_EquationBase;
	m_Equation1 = _Clone.m_Equation1;
	m_Equation2 = _Clone.m_Equation2;
	return DTAssistGraph::Clone(_Other);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 阻速线 Gould Rays
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 江恩角度线 Gann Rays
// <!---
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DTGannRaysGraph::DTGannRaysGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
	m_bShowLabel = true;
}

DTGannRaysGraph::DTGannRaysGraph(DTGannRaysGraph& _other)
: DTAssistGraphI(_other)
{
	m_EquationBase = _other.m_EquationBase;
	m_EquationHorizon = _other.m_EquationHorizon;
	m_EquationVertical = _other.m_EquationVertical;
	m_EquationH1 = _other.m_EquationH1;
	m_EquationH2 = _other.m_EquationH2;
	m_EquationH3 = _other.m_EquationH3;
	m_EquationH4 = _other.m_EquationH4;
	m_EquationV1 = _other.m_EquationV1;
	m_EquationV2 = _other.m_EquationV2;
	m_EquationV3 = _other.m_EquationV3;
	m_EquationV4 = _other.m_EquationV4;
	m_bShowLabel = _other.m_bShowLabel;
}

int DTGannRaysGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTGannRaysGraph::NextEditMode(int _CurMode)
{
	return DTGannRaysGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTGannRaysGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTGannRaysGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTGannRaysGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationBase.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationHorizon.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationVertical.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationH1.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationH2.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationH3.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationH4.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationV1.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationV2.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationV3.HitTest(_Point);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationV4.HitTest(_Point);

	return _HitTest;
}

int DTGannRaysGraph::Render_i(HDC _hdc)
{
	long _delta_Y = m_ptViewLocationP2.y - m_ptViewLocationP1.y;
	long _delta_X = m_ptViewLocationP2.x - m_ptViewLocationP1.x;

	m_EquationBase.Render(_hdc, m_rcValidateRect);
	m_EquationHorizon.Render(_hdc, m_rcValidateRect);
	m_EquationVertical.Render(_hdc, m_rcValidateRect);

	HPEN dotPen = ::CreatePen(PS_DOT, 1, ::GetDCPenColor(_hdc));
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);

	m_EquationH1.Render(_hdc, m_rcValidateRect);
	m_EquationH2.Render(_hdc, m_rcValidateRect);
	m_EquationH3.Render(_hdc, m_rcValidateRect);
	m_EquationH4.Render(_hdc, m_rcValidateRect);

	m_EquationV1.Render(_hdc, m_rcValidateRect);
	m_EquationV2.Render(_hdc, m_rcValidateRect);
	m_EquationV3.Render(_hdc, m_rcValidateRect);
	m_EquationV4.Render(_hdc, m_rcValidateRect);

	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);

	if(IsShowLabel())
	{
		long _yTopAdjust = (_delta_Y >= 0) ? (-1 * DTAbsEquation::m_LabelFontSize) : 0;
		tagDTRect _TextRect;

		_TextRect.left = m_ptViewLocationP2.x;
		_TextRect.right = _TextRect.left + 40;

		_TextRect.top = m_ptViewLocationP1.y + (1.0 / 2.0) * _delta_Y + _yTopAdjust;
		_TextRect.bottom = _TextRect.top + DTAbsEquation::m_LabelFontSize;
		::DrawTextA(_hdc, "2:1", strlen("2:1"), &_TextRect, DT_LEFT);

		_TextRect.top = m_ptViewLocationP1.y + (1.0 / 3.0) * _delta_Y + _yTopAdjust;
		_TextRect.bottom = _TextRect.top + DTAbsEquation::m_LabelFontSize;
		::DrawTextA(_hdc, "3:1", strlen("3:1"), &_TextRect, DT_LEFT);

		_TextRect.top = m_ptViewLocationP1.y + (1.0 / 4.0) * _delta_Y + _yTopAdjust;
		_TextRect.bottom = _TextRect.top + DTAbsEquation::m_LabelFontSize;
		::DrawTextA(_hdc, "4:1", strlen("4:1"), &_TextRect, DT_LEFT);

		_TextRect.top = m_ptViewLocationP1.y + (1.0 / 8.0) * _delta_Y + _yTopAdjust;
		_TextRect.bottom = _TextRect.top + DTAbsEquation::m_LabelFontSize;
		::DrawTextA(_hdc, "8:1", strlen("8:1"), &_TextRect, DT_LEFT);

		_TextRect.top = m_ptViewLocationP2.y + _yTopAdjust;
		_TextRect.bottom = _TextRect.top + DTAbsEquation::m_LabelFontSize;

		_TextRect.left = m_ptViewLocationP1.x + (1.0 / 2.0) * _delta_X;
		_TextRect.right = _TextRect.left + 40;
		::DrawTextA(_hdc, "1:2", strlen("1:2"), &_TextRect, DT_LEFT);

		_TextRect.left = m_ptViewLocationP1.x + (1.0 / 3.0) * _delta_X;
		_TextRect.right = _TextRect.left + 40;
		::DrawTextA(_hdc, "1:3", strlen("1:3"), &_TextRect, DT_LEFT);

		_TextRect.left = m_ptViewLocationP1.x + (1.0 / 4.0) * _delta_X;
		_TextRect.right = _TextRect.left + 40;
		::DrawTextA(_hdc, "1:4", strlen("1:4"), &_TextRect, DT_LEFT);
	
		_TextRect.left = m_ptViewLocationP1.x + (1.0 / 8.0) * _delta_X;
		_TextRect.right = _TextRect.left + 40;
		::DrawTextA(_hdc, "1:8", strlen("1:8"), &_TextRect, DT_LEFT);
	}

	return 0;
}

int DTGannRaysGraph::Render(HDC _hdc)
{
	return Render_i(_hdc);
}

int DTGannRaysGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	HideLabel();
	Render_i(_hdc);
	ShowLabel();
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGannRaysGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGannRaysGraph::RenderAnchors(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrAnchors = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrAnchors);
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrAnchors);
	//// <!-- render -->
	::Rectangle(_hdc, m_ptViewLocationP1.x - m_nAnchorThickness, m_ptViewLocationP1.y - m_nAnchorThickness, m_ptViewLocationP1.x + m_nAnchorThickness, m_ptViewLocationP1.y + m_nAnchorThickness);
	::Rectangle(_hdc, m_ptViewLocationP2.x - m_nAnchorThickness, m_ptViewLocationP2.y - m_nAnchorThickness, m_ptViewLocationP2.x + m_nAnchorThickness, m_ptViewLocationP2.y + m_nAnchorThickness);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCBrushColor(_hdc, clrOfBrush);
	return 0;
}

int DTGannRaysGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGannRaysGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTGannRaysGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGannRaysGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	m_ptViewLocationP2.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);
	m_ptViewLocationP2.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP2.yValue);

	if(!CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate)
		|| !CHECK_VIEW_XDATE_AXIS_AVAIABLE(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate)
		|| !CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x)
		|| !CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP2.x)
		|| m_ptViewLocationP1.x >= m_ptViewLocationP2.x)
	{
		m_bAvaiable = false;
		return -1;
	}

	m_bAvaiable = true;
	return Build();
}

int DTGannRaysGraph::Build()
{
	m_rcVirtualRect.left = m_ptViewLocationP1.x;
	m_rcVirtualRect.top = LONG_INFINITE_TOP;
	m_rcVirtualRect.right = LONG_INFINITE_RIGHT;
	m_rcVirtualRect.bottom = LONG_INFINITE_BOTTOM;

	long _delta_X = m_ptViewLocationP2.x - m_ptViewLocationP1.x;
	long _delta_Y = m_ptViewLocationP2.y - m_ptViewLocationP1.y;

	if(_delta_Y < 0)
		m_rcVirtualRect.bottom = m_ptViewLocationP1.y;
	else
		m_rcVirtualRect.top = m_ptViewLocationP1.y;

	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	tagDTPoint _PointA = m_ptViewLocationP2;
	tagDTPoint _PointB = m_ptViewLocationP2;

	if(_delta_X <= 0)
	{
		m_bAvaiable = false;
		return -1;
	}

	m_bAvaiable = true;

	m_EquationBase.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	m_EquationHorizon.MoveTo(m_ptViewLocationP1);
	m_EquationVertical.MoveTo(m_ptViewLocationP1);

	_PointB.y = m_ptViewLocationP1.y + (1.0 / 2.0) * _delta_Y;
	m_EquationH1.Build(m_ptViewLocationP1, _PointB);
	_PointB.y = m_ptViewLocationP1.y + (1.0 / 3.0) * _delta_Y;
	m_EquationH2.Build(m_ptViewLocationP1, _PointB);
	_PointB.y = m_ptViewLocationP1.y + (1.0 / 4.0) * _delta_Y;
	m_EquationH3.Build(m_ptViewLocationP1, _PointB);
	_PointB.y = m_ptViewLocationP1.y + (1.0 / 8.0) * _delta_Y;
	m_EquationH4.Build(m_ptViewLocationP1, _PointB);

	_PointA.x = m_ptViewLocationP1.x + (1.0 / 2.0) * _delta_X;
	m_EquationV1.Build(m_ptViewLocationP1, _PointA);
	_PointA.x = m_ptViewLocationP1.x + (1.0 / 3.0) * _delta_X;
	m_EquationV2.Build(m_ptViewLocationP1, _PointA);
	_PointA.x = m_ptViewLocationP1.x + (1.0 / 4.0) * _delta_X;
	m_EquationV3.Build(m_ptViewLocationP1, _PointA);
	_PointA.x = m_ptViewLocationP1.x + (1.0 / 8.0) * _delta_X;
	m_EquationV4.Build(m_ptViewLocationP1, _PointA);
	return 0;
}

void DTGannRaysGraph::MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint)
{
	long _delta_X = _ToPoint.x - _FromPoint.x;
	long _delta_Y = _ToPoint.y - _FromPoint.y;

	m_ptViewLocationP1.x += _delta_X;
	m_ptViewLocationP1.y += _delta_Y;

	m_ptViewLocationP2.x += _delta_X;
	m_ptViewLocationP2.y += _delta_Y;

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x)
		|| !CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP2.x)
		|| m_ptViewLocationP1.x >= m_ptViewLocationP2.x )
	{
		m_bAvaiable = false;
		return ;
	}

	m_bAvaiable = true;
	Build();
}

void DTGannRaysGraph::MoveP1To(const tagDTPoint& _Point)
{
	m_ptViewLocationP1.x = _Point.x;
	m_ptViewLocationP1.y = _Point.y;

	Build();
}

void DTGannRaysGraph::MoveP2To(const tagDTPoint& _Point)
{
	m_ptViewLocationP2.x = _Point.x;
	m_ptViewLocationP2.y = _Point.y;

	Build();
}

int DTGannRaysGraph::Complete()
{
	m_ptViewLocationP1.x = m_coorTransform.AdjustViewX(m_ptViewLocationP1.x);
	m_ptViewLocationP2.x = m_coorTransform.AdjustViewX(m_ptViewLocationP2.x);

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTGannRaysGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTGannRaysGraph& _Clone = reinterpret_cast<DTGannRaysGraph&>(_Other);
	m_EquationBase = _Clone.m_EquationBase;
	m_EquationHorizon = _Clone.m_EquationHorizon;
	m_EquationVertical = _Clone.m_EquationVertical;
	m_EquationH1 = _Clone.m_EquationH1;
	m_EquationH2 = _Clone.m_EquationH2;
	m_EquationH3 = _Clone.m_EquationH3;
	m_EquationH4 = _Clone.m_EquationH4;
	m_EquationV1 = _Clone.m_EquationV1;
	m_EquationV2 = _Clone.m_EquationV2;
	m_EquationV3 = _Clone.m_EquationV3;
	m_EquationV4 = _Clone.m_EquationV4;
	m_bShowLabel = _Clone.m_bShowLabel;
	return DTAssistGraph::Clone(_Other);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 江恩角度线 Gann Rays
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////