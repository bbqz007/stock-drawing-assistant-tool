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
// 圆弧线 Ellipse Arc
// <!-- 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTEllipseArcGraph::DTEllipseArcGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTEllipseArcGraph::DTEllipseArcGraph(DTEllipseArcGraph& _other)
: DTAssistGraphI(_other)
{
	m_EquationBase = _other.m_EquationBase;
}

int DTEllipseArcGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTEllipseArcGraph::NextEditMode(int _CurMode)
{
	return DTEllipseArcGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTEllipseArcGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTEllipseArcGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTEllipseArcGraph::HitTest(const tagDTPoint& _Point, long _xDate)
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
	return _HitTest;
}

int DTEllipseArcGraph::Render(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	if(!m_bUseDefault)
	{
		int _Ret = 0;
		HPEN usrPen = ::CreatePen(m_descGraph.m_propLine.flagStyle, m_descGraph.m_propLine.nWidth, m_descGraph.m_propLine.rgbColor);
		HPEN oldPen = (HPEN) ::SelectObject(_hdc, usrPen);
		//// <!-- render -->
		_Ret = m_EquationBase.Render(_hdc, m_rcValidateRect);
		::SelectObject(_hdc, oldPen);
		::DeleteObject(usrPen);
		return _Ret;
	}
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	return 0;
}

int DTEllipseArcGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTEllipseArcGraph::RenderEditing(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTEllipseArcGraph::RenderAnchors(HDC _hdc)
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

int DTEllipseArcGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTEllipseArcGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTEllipseArcGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTEllipseArcGraph::BuildAccordToDescription()
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
	Build();
	return 0;
}

int DTEllipseArcGraph::Build()
{
	m_rcVirtualRect.left = min(m_ptViewLocationP1.x, m_ptViewLocationP2.x) - ::abs(m_ptViewLocationP1.x - m_ptViewLocationP2.x);
	m_rcVirtualRect.top = min(m_ptViewLocationP1.y, m_ptViewLocationP2.y) - ::abs(m_ptViewLocationP1.y - m_ptViewLocationP2.y);
	m_rcVirtualRect.right = max(m_ptViewLocationP1.x, m_ptViewLocationP2.x) + ::abs(m_ptViewLocationP1.x - m_ptViewLocationP2.x);
	m_rcVirtualRect.bottom = max(m_ptViewLocationP1.y, m_ptViewLocationP2.y) + ::abs(m_ptViewLocationP1.y - m_ptViewLocationP2.y);

	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);
	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	m_EquationBase.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	return 0;
}

int DTEllipseArcGraph::EndMove(long _delta_X, long _delta_Y)
{
	m_EquationBase.Offset(_delta_X, _delta_Y);
	return DTAssistGraph::EndMove();
}

int DTEllipseArcGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTEllipseArcGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTEllipseArcGraph& _Clone = reinterpret_cast<DTEllipseArcGraph&>(_Other);
	m_EquationBase = _Clone.m_EquationBase;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 圆弧线 Ellipse Arc
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 圆 Circle
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTCircleGraph::DTCircleGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTCircleGraph::DTCircleGraph(DTCircleGraph& _other)
: DTAssistGraphI(_other)
{
	m_EquationBase = _other.m_EquationBase;
}

int DTCircleGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTCircleGraph::NextEditMode(int _CurMode)
{
	return DTCircleGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTCircleGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTCircleGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTCircleGraph::HitTest(const tagDTPoint& _Point, long _xDate)
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
	return _HitTest;
}

int DTCircleGraph::Render(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	if(!m_bUseDefault)
	{
		int _Ret = 0;
		HPEN usrPen = ::CreatePen(m_descGraph.m_propLine.flagStyle, m_descGraph.m_propLine.nWidth, m_descGraph.m_propLine.rgbColor);
		HPEN oldPen = (HPEN) ::SelectObject(_hdc, usrPen);
		//// <!-- render -->
		_Ret = m_EquationBase.Render(_hdc, m_rcValidateRect);
		::SelectObject(_hdc, oldPen);
		::DeleteObject(usrPen);
		return _Ret;
	}
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	return 0;
}

int DTCircleGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTCircleGraph::RenderEditing(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTCircleGraph::RenderAnchors(HDC _hdc)
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

int DTCircleGraph::RenderShadowAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y))
	{
		//// <!-- prepare colors -->
		COLORREF clrShadow = m_Graphics->ShadowColor();
		COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
		//// <!-- render -->
		::MoveToEx(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y, NULL);
		::LineTo(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y);
		//// <!-- restore colors -->
		::SetDCPenColor(_hdc, clrOfPen);
	}
	return RenderAnchors(_hdc);
}

int DTCircleGraph::RenderEditingAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y))
	{
		//// <!-- prepare colors -->
		COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
		COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
		int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
		//// <!-- render -->
		::MoveToEx(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y, NULL);
		::LineTo(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y);
		//// <!-- restore colors -->
		::SetROP2(_hdc, nOldRop2);
		::SetDCPenColor(_hdc, clrOfPen);
	}
	return 0;
}

int DTCircleGraph::RenderSelectedAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y))
	{
		//// <!-- render -->
		::MoveToEx(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y, NULL);
		::LineTo(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y);
	}
	return RenderAnchors(_hdc);
}

int DTCircleGraph::BuildAccordToDescription()
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
	Build();
	return 0;
}

int DTCircleGraph::Build()
{
	float _R = DTEquationHelper::DistanceBetweenTowPoints(m_ptViewLocationP1, m_ptViewLocationP2);
	m_rcVirtualRect.left = m_ptViewLocationP1.x - _R;
	m_rcVirtualRect.top = m_ptViewLocationP1.y - _R;
	m_rcVirtualRect.right = m_ptViewLocationP1.x + _R;
	m_rcVirtualRect.bottom = m_ptViewLocationP1.y + _R;

	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);
	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	m_EquationBase.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	return 0;
}

int DTCircleGraph::EndMove(long _delta_X, long _delta_Y)
{
	
	return m_EquationBase.Offset(_delta_X, _delta_Y);
}

int DTCircleGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTCircleGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTCircleGraph& _Clone = reinterpret_cast<DTCircleGraph&>(_Other);
	m_EquationBase = _Clone.m_EquationBase;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 圆 Circle
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 外接圆 Circumcircle
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTCircumcircleGraph::DTCircumcircleGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTCircumcircleGraph::DTCircumcircleGraph(DTCircumcircleGraph& _other)
: DTAssistGraphI(_other)
{
	m_EquationBase = _other.m_EquationBase;
}

int DTCircumcircleGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfParallelLine(_CurMode);
}

int DTCircumcircleGraph::NextEditMode(int _CurMode)
{
	return DTCircumcircleGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTCircumcircleGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfParallelLine();
}

DTAssistGraphDescriptionScript DTCircumcircleGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTCircumcircleGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP3, _Point, ENUM_ASSIST_HITTEST_LOCATION_P3);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;

	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_EquationBase.HitTest(_Point);
	return _HitTest;
}

int DTCircumcircleGraph::Render(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	if(!m_bUseDefault)
	{
		int _Ret = 0;
		HPEN usrPen = ::CreatePen(m_descGraph.m_propLine.flagStyle, m_descGraph.m_propLine.nWidth, m_descGraph.m_propLine.rgbColor);
		HPEN oldPen = (HPEN) ::SelectObject(_hdc, usrPen);
		//// <!-- render -->
		_Ret = m_EquationBase.Render(_hdc, m_rcValidateRect);
		::SelectObject(_hdc, oldPen);
		::DeleteObject(usrPen);
		return _Ret;
	}
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	return 0;
}

int DTCircumcircleGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTCircumcircleGraph::RenderEditing(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTCircumcircleGraph::RenderAnchors(HDC _hdc)
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

int DTCircumcircleGraph::RenderShadowAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y))
	{
		//// <!-- prepare colors -->
		COLORREF clrShadow = m_Graphics->ShadowColor();
		COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
		//// <!-- render -->
		::MoveToEx(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y, NULL);
		::LineTo(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y);
		::LineTo(_hdc, m_ptViewLocationP3.x, m_ptViewLocationP3.y);
		::LineTo(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y);
		//// <!-- restore colors -->
		::SetDCPenColor(_hdc, clrOfPen);
	}
	return RenderAnchors(_hdc);
}

int DTCircumcircleGraph::RenderEditingAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y))
	{
		//// <!-- prepare colors -->
		COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
		COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
		int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
		//// <!-- render -->
		::MoveToEx(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y, NULL);
		::LineTo(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y);
		::LineTo(_hdc, m_ptViewLocationP3.x, m_ptViewLocationP3.y);
		::LineTo(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y);
		//// <!-- restore colors -->
		::SetROP2(_hdc, nOldRop2);
		::SetDCPenColor(_hdc, clrOfPen);
	}
	return 0;
}

int DTCircumcircleGraph::RenderSelectedAnchors(HDC _hdc)
{
	if(!((m_ptViewLocationP1.x == m_ptViewLocationP2.x && m_ptViewLocationP1.x == m_ptViewLocationP3.x)
		&& (m_ptViewLocationP1.y == m_ptViewLocationP2.y && m_ptViewLocationP1.y == m_ptViewLocationP3.y)))
	{
		//// <!-- render -->
		::MoveToEx(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y, NULL);
		::LineTo(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y);
		::LineTo(_hdc, m_ptViewLocationP3.x, m_ptViewLocationP3.y);
		::LineTo(_hdc, m_ptViewLocationP1.x, m_ptViewLocationP1.y);
	}
	return RenderAnchors(_hdc);
}

int DTCircumcircleGraph::BuildAccordToDescription()
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
	Build();
	return 0;
}

int DTCircumcircleGraph::Build()
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

	m_EquationBase.Build(m_ptViewLocationP1, m_ptViewLocationP2, m_ptViewLocationP3);
	return 0;
}

int DTCircumcircleGraph::EndMove(long _delta_X, long _delta_Y)
{
	return m_EquationBase.Offset(_delta_X, _delta_Y);
}

int DTCircumcircleGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	m_descGraph.m_ptLocations.ptAbsoluteP3.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP3.x);
	m_descGraph.m_ptLocations.ptAbsoluteP3.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP3.y);

	return BuildAccordToDescription();
}

int DTCircumcircleGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTCircumcircleGraph& _Clone = reinterpret_cast<DTCircumcircleGraph&>(_Other);
	m_EquationBase = _Clone.m_EquationBase;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 外接圆 Circumcircle
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////