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
// 线段 Line Segment
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTLineSegmentGraph::DTLineSegmentGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTLineSegmentGraph::DTLineSegmentGraph(DTLineSegmentGraph& _other)
: DTAssistGraphI(_other)
{
	m_Equation = _other.m_Equation;
}

int DTLineSegmentGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTLineSegmentGraph::NextEditMode(int _CurMode)
{
	return DTLineSegmentGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTLineSegmentGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTLineSegmentGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTLineSegmentGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_Equation.HitTest(_Point);
	return _HitTest;
}

int DTLineSegmentGraph::Render(HDC _hdc)
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
		_Ret = m_Equation.Render(_hdc, m_rcValidateRect);
		::SelectObject(_hdc, oldPen);
		::DeleteObject(usrPen);
		return _Ret;
	}
	//// <!-- render -->
	return m_Equation.Render(_hdc, m_rcValidateRect);
}

int DTLineSegmentGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_Equation.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTLineSegmentGraph::RenderEditing(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_Equation.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTLineSegmentGraph::RenderAnchors(HDC _hdc)
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

int DTLineSegmentGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTLineSegmentGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTLineSegmentGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTLineSegmentGraph::Build()
{
	m_rcVirtualRect.left = min(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.top = min(m_ptViewLocationP1.y, m_ptViewLocationP2.y);
	m_rcVirtualRect.right = max(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.bottom = max(m_ptViewLocationP1.y, m_ptViewLocationP2.y);
	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	m_Equation.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	return 0;
}

int DTLineSegmentGraph::BuildAccordToDescription()
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

int DTLineSegmentGraph::EndMove(long _delta_X, long _delta_Y)
{
	m_Equation.Offset(_delta_X, _delta_Y);
	return DTAssistGraph::EndMove();
}

int DTLineSegmentGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTLineSegmentGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTLineSegmentGraph& _Clone = reinterpret_cast<DTLineSegmentGraph&>(_Other);
	m_Equation = _Clone.m_Equation;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 线段 Line Segment
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 直线 Line
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTLineGraph::DTLineGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTLineGraph::DTLineGraph(DTLineGraph& _other)
: DTAssistGraphI(_other)
{
	m_Equation = _other.m_Equation;
}

int DTLineGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTLineGraph::NextEditMode(int _CurMode)
{
	return DTLineGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTLineGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTLineGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTLineGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_Equation.HitTest(_Point);
	return _HitTest;
}

int DTLineGraph::Render(HDC _hdc)
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
		_Ret = m_Equation.Render(_hdc, m_rcValidateRect);
		::SelectObject(_hdc, oldPen);
		::DeleteObject(usrPen);
		return _Ret;
	}
	//// <!-- render -->
	return m_Equation.Render(_hdc, m_rcValidateRect);
}

int DTLineGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_Equation.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTLineGraph::RenderEditing(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_Equation.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTLineGraph::RenderAnchors(HDC _hdc)
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

int DTLineGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTLineGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTLineGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}


int DTLineGraph::Build()
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

	m_Equation.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	return 0;
}

int DTLineGraph::BuildAccordToDescription()
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

int DTLineGraph::EndMove(long _delta_X, long _delta_Y)
{
	m_Equation.Offset(_delta_X, _delta_Y);
	return DTAssistGraph::EndMove();
}

int DTLineGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTLineGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTLineGraph& _Clone = reinterpret_cast<DTLineGraph&>(_Other);
	m_Equation = _Clone.m_Equation;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 直线 Line
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 射线 Line Extension
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTLineExtensionGraph::DTLineExtensionGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTLineExtensionGraph::DTLineExtensionGraph(DTLineExtensionGraph& _other)
: DTAssistGraphI(_other)
{
	m_Equation = _other.m_Equation;
}

int DTLineExtensionGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTLineExtensionGraph::NextEditMode(int _CurMode)
{
	return DTLineExtensionGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTLineExtensionGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTLineExtensionGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTLineExtensionGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_Equation.HitTest(_Point);
	return _HitTest;
}

int DTLineExtensionGraph::Render(HDC _hdc)
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
		_Ret = m_Equation.Render(_hdc, m_rcValidateRect);
		::SelectObject(_hdc, oldPen);
		::DeleteObject(usrPen);
		return _Ret;
	}
	//// <!-- render -->
	return m_Equation.Render(_hdc, m_rcValidateRect);
}

int DTLineExtensionGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_Equation.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTLineExtensionGraph::RenderEditing(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_Equation.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTLineExtensionGraph::RenderAnchors(HDC _hdc)
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

int DTLineExtensionGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTLineExtensionGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTLineExtensionGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}


int DTLineExtensionGraph::Build()
{
	long _delta_x = m_ptViewLocationP1.x - m_ptViewLocationP2.x;
	long _delta_y = m_ptViewLocationP1.y - m_ptViewLocationP2.y;

	m_rcVirtualRect.left = min(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.top = min(m_ptViewLocationP1.y, m_ptViewLocationP2.y);
	m_rcVirtualRect.right = max(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.bottom = max(m_ptViewLocationP1.y, m_ptViewLocationP2.y);

	if(_delta_x < 0)
		m_rcVirtualRect.left = LONG_INFINITE_LEFT;
	else if(_delta_x > 0)
		m_rcVirtualRect.right = LONG_INFINITE_RIGHT;
	
	if(_delta_y < 0)
		m_rcVirtualRect.top = LONG_INFINITE_TOP;
	else if(_delta_y > 0)
		m_rcVirtualRect.bottom = LONG_INFINITE_BOTTOM;
	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);


	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	m_Equation.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	return 0;
}

int DTLineExtensionGraph::BuildAccordToDescription()
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

int DTLineExtensionGraph::EndMove(long _delta_X, long _delta_Y)
{
	m_Equation.Offset(_delta_X, _delta_Y);
	return DTAssistGraph::EndMove();
}

int DTLineExtensionGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTLineExtensionGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTLineExtensionGraph& _Clone = reinterpret_cast<DTLineExtensionGraph&>(_Other);
	m_Equation = _Clone.m_Equation;
	return DTAssistGraph::Clone(_Other);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 射线 Line Extension
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 箭头 Line Segment with Arrow
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTArrowSegmentGraph::DTArrowSegmentGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTArrowSegmentGraph::DTArrowSegmentGraph(DTArrowSegmentGraph& _other)
: DTAssistGraphI(_other)
{
	m_Equation = _other.m_Equation;
}

int DTArrowSegmentGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTArrowSegmentGraph::NextEditMode(int _CurMode)
{
	return DTArrowSegmentGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTArrowSegmentGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTArrowSegmentGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTArrowSegmentGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;

	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = m_Equation.HitTest(_Point);
	return _HitTest;
}

int DTArrowSegmentGraph::RenderArrow(HDC _hdc)
{
	long _delta_x = m_ptViewLocationP1.x - m_ptViewLocationP2.x;
	long _delta_y = m_ptViewLocationP1.y - m_ptViewLocationP2.y;
	long _pick = 0;
	long _left = 0;
	long _right = 0;
	
	//// <!-- prepare arrow -->
	if(_delta_x < 0)
	{
		long _base = 8;
		double _theta = (double)_delta_y / (double)_delta_x;
		for(long i = 0; i < 16; ++i)
		{
			_pick = (_base + i) % 32;
			double* _value = (double*)g_AngleQuadrant;
			if(_theta < *(_value + _pick))
			{
				break;
			}
		}
	}
	else if(_delta_x == 0)
	{
		if(_delta_y < 0)
		{
			_pick = 24;
		}
		else if(_delta_y == 0)
		{
			return 0;
		}
		else
		{
			_pick = 8;
		}
	}
	else
	{
		long _base = 24;
		double _theta = (double)_delta_y / (double)_delta_x;
		for(long i = 0; i < 16; ++i)
		{
			_pick = (_base + i) % 32;
			double* _value = (double*)g_AngleQuadrant;
			if(_theta < *(_value + _pick))
			{
				break;
			}
		}
	}

	_left = g_AngleQuadrantMapArrow[ _pick / 8 ] [ _pick % 8 ][0];
	_right = g_AngleQuadrantMapArrow[ _pick / 8 ] [ _pick % 8 ][1];

	//// <!-- render -->
	::MoveToEx(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y, NULL);
	::LineTo(_hdc, m_ptViewLocationP2.x + g_CirlceQuadrant[ _left / 4 ] [ _left % 4 ][1] * 9,
					m_ptViewLocationP2.y + g_CirlceQuadrant[ _left / 4 ] [ _left % 4 ][2] * 9);

	::MoveToEx(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y, NULL);
	::LineTo(_hdc, m_ptViewLocationP2.x + g_CirlceQuadrant[ _right / 4 ] [ _right % 4 ][1] * 9,
					m_ptViewLocationP2.y + g_CirlceQuadrant[ _right / 4 ] [ _right % 4 ][2] * 9);

	return 0;
}

int DTArrowSegmentGraph::RenderArrowByCne(HDC _hdc)
{
	struct _Cn
	{
		double _r;
		double _i;

		_Cn operator * (_Cn _R)
		{
			_Cn& _L = *this;
			_Cn _Ret = _L;
			_Ret._r = _L._r * _R._r;
			_Ret._r -= _L._i * _R._i;
			_Ret._i = _L._r * _R._i + _L._i * _R._r;
			return _Ret;
		}

		_Cn operator * (double _M)
		{
			_Cn& _L = *this;
			_Cn _Ret = _L;
			_Ret._r = _L._r * _M;
			_Ret._i = _L._i * _M;
			return _Ret;
		}

		_Cn operator + (_Cn _R)
		{
			_Cn& _L = *this;
			_Cn _Ret = _L;
			_Ret._r = _L._r + _R._r;
			_Ret._i = _L._i + _R._i;
			return _Ret;
		}

		_Cn normal()
		{
			double _M = module();
			_Cn _Ret = { _r / _M, _i / _M };
			return _Ret;
		}

		double module()
		{
			return ::sqrt(_r * _r + _i * _i);
		}
	};

	_Cn _Start = { m_ptViewLocationP1.x, m_ptViewLocationP1.y };
	_Cn _End = { m_ptViewLocationP2.x, m_ptViewLocationP2.y };
	_Cn _Vector = { _End._r - _Start._r, _End._i - _Start._i };
	_Cn _Reverse = { _Vector._r * -1.0, _Vector._i * -1.0 };

	_Reverse = _Reverse.normal();

	_Cn _Pos_Side = { g_CirlceQuadrant[0][1][1], g_CirlceQuadrant[0][1][2] };
	_Cn _Neg_Side = { g_CirlceQuadrant[3][3][1], g_CirlceQuadrant[3][3][2] };

	_Pos_Side = _Reverse * _Pos_Side;
	_Neg_Side = _Reverse * _Neg_Side;

	_Pos_Side = _Pos_Side * 9;
	_Neg_Side = _Neg_Side * 9;

	_Pos_Side = _Pos_Side + _End;
	_Neg_Side = _Neg_Side + _End;

	//// <!-- render -->
	::MoveToEx(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y, NULL);
	::LineTo(_hdc, _Pos_Side._r, _Pos_Side._i);

	::MoveToEx(_hdc, m_ptViewLocationP2.x, m_ptViewLocationP2.y, NULL);
	::LineTo(_hdc, _Neg_Side._r, _Neg_Side._i);

	return 0;
}

int DTArrowSegmentGraph::Render(HDC _hdc)
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
		_Ret = m_Equation.Render(_hdc, m_rcValidateRect);
		RenderArrowByCne(_hdc);
		::SelectObject(_hdc, oldPen);
		::DeleteObject(usrPen);
		return _Ret;
	}

	//// <!-- render -->
	m_Equation.Render(_hdc, m_rcValidateRect);
	RenderArrowByCne(_hdc);
	return 0;
}

int DTArrowSegmentGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_Equation.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTArrowSegmentGraph::RenderEditing(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_Equation.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTArrowSegmentGraph::RenderAnchors(HDC _hdc)
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

int DTArrowSegmentGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTArrowSegmentGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTArrowSegmentGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}


int DTArrowSegmentGraph::Build()
{
	m_rcVirtualRect.left = min(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.top = min(m_ptViewLocationP1.y, m_ptViewLocationP2.y);
	m_rcVirtualRect.right = max(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.bottom = max(m_ptViewLocationP1.y, m_ptViewLocationP2.y);
	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	m_Equation.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	return 0;
}

int DTArrowSegmentGraph::BuildAccordToDescription()
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

int DTArrowSegmentGraph::EndMove(long _delta_X, long _delta_Y)
{
	m_Equation.Offset(_delta_X, _delta_Y);
	return DTAssistGraph::EndMove();
}

int DTArrowSegmentGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTArrowSegmentGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTArrowSegmentGraph& _Clone = reinterpret_cast<DTArrowSegmentGraph&>(_Other);
	m_Equation = _Clone.m_Equation;
	return DTAssistGraph::Clone(_Other);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 箭头 Line Segment with Arrow
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

