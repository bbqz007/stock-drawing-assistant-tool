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
// 红色上箭头 Red Up Arrow
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTRedUpArrowGraph::DTRedUpArrowGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTRedUpArrowGraph::DTRedUpArrowGraph(DTRedUpArrowGraph& _other)
: DTAssistGraphI(_other)
{
	m_ArrowRect = _other.m_ArrowRect;
}

int DTRedUpArrowGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSinglePoint(_CurMode);
}

int DTRedUpArrowGraph::NextEditMode(int _CurMode)
{
	return DTRedUpArrowGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTRedUpArrowGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSinglePoint();
}

DTAssistGraphDescriptionScript DTRedUpArrowGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTRedUpArrowGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = (::PtInRect(&m_ArrowRect, _Point)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
	return _HitTest;
}

int DTRedUpArrowGraph::Render_i(HDC _hdc)
{
	tagDTPoint _Path[8];
	
	_Path[0] = m_ptViewLocationP1;
	_Path[7] = m_ptViewLocationP1;
	_Path[1].x = m_ptViewLocationP1.x - 7;
	_Path[1].y = m_ptViewLocationP1.y + 7;
	_Path[6].x = m_ptViewLocationP1.x + 7;
	_Path[6].y = m_ptViewLocationP1.y + 7;
	_Path[2].x = m_ptViewLocationP1.x - 3;
	_Path[2].y = m_ptViewLocationP1.y + 7;
	_Path[5].x = m_ptViewLocationP1.x + 3;
	_Path[5].y = m_ptViewLocationP1.y + 7;
	_Path[3].x = m_ptViewLocationP1.x - 3;
	_Path[3].y = m_ptViewLocationP1.y + 14;
	_Path[4].x = m_ptViewLocationP1.x + 3;
	_Path[4].y = m_ptViewLocationP1.y + 14;

	
	::Polygon(_hdc, _Path, 8);

	return 0;
}

int DTRedUpArrowGraph::Render(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, RGB(255, 0, 0));
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, RGB(255, 0, 0));
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetDCBrushColor(_hdc, clrOfBrush);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTRedUpArrowGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrShadow);
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCPenColor(_hdc, clrOfBrush);
	return 0;
}

int DTRedUpArrowGraph::RenderEditing(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, RGB(255, 0, 0));
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, RGB(255, 0, 0));
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetDCBrushColor(_hdc, clrOfBrush);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTRedUpArrowGraph::RenderAnchors(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrAnchors = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrAnchors);
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrAnchors);
	//// <!-- render -->
	::Rectangle(_hdc, m_ptViewLocationP1.x - m_nAnchorThickness, m_ptViewLocationP1.y - m_nAnchorThickness, m_ptViewLocationP1.x + m_nAnchorThickness, m_ptViewLocationP1.y + m_nAnchorThickness);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCBrushColor(_hdc, clrOfBrush);
	return 0;
}

int DTRedUpArrowGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTRedUpArrowGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTRedUpArrowGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTRedUpArrowGraph::Build()
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

	m_ArrowRect.top = m_ptViewLocationP1.y;
	m_ArrowRect.bottom = m_ptViewLocationP1.y + 14; 
	m_ArrowRect.left = m_ptViewLocationP1.x - 7;
	m_ArrowRect.right = m_ptViewLocationP1.x + 7;
	return 0;
}

int DTRedUpArrowGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x))
	{
		m_bAvaiable = false;
		return -1;
	}

	m_bAvaiable = true;
	Build();
	return 0;
}

int DTRedUpArrowGraph::EndMove(long _delta_X, long _delta_Y)
{
	return DTAssistGraph::EndMove();
}

int DTRedUpArrowGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	return BuildAccordToDescription();
}

int DTRedUpArrowGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTRedUpArrowGraph& _Clone = reinterpret_cast<DTRedUpArrowGraph&>(_Other);
	m_ArrowRect = _Clone.m_ArrowRect;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 红色上箭头 Red Up Arrow
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 绿色下箭头 Green Down Arrow
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTGreenDownArrowGraph::DTGreenDownArrowGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTGreenDownArrowGraph::DTGreenDownArrowGraph(DTGreenDownArrowGraph& _other)
: DTAssistGraphI(_other)
{
	m_ArrowRect = _other.m_ArrowRect;
}

int DTGreenDownArrowGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSinglePoint(_CurMode);
}

int DTGreenDownArrowGraph::NextEditMode(int _CurMode)
{
	return DTGreenDownArrowGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTGreenDownArrowGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSinglePoint();
}

DTAssistGraphDescriptionScript DTGreenDownArrowGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTGreenDownArrowGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = (::PtInRect(&m_ArrowRect, _Point)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
	return _HitTest;
}

int DTGreenDownArrowGraph::Render_i(HDC _hdc)
{
	tagDTPoint _Path[8];
	
	_Path[0] = m_ptViewLocationP1;
	_Path[7] = m_ptViewLocationP1;
	_Path[1].x = m_ptViewLocationP1.x - 7;
	_Path[1].y = m_ptViewLocationP1.y - 7;
	_Path[6].x = m_ptViewLocationP1.x + 7;
	_Path[6].y = m_ptViewLocationP1.y - 7;
	_Path[2].x = m_ptViewLocationP1.x - 3;
	_Path[2].y = m_ptViewLocationP1.y - 7;
	_Path[5].x = m_ptViewLocationP1.x + 3;
	_Path[5].y = m_ptViewLocationP1.y - 7;
	_Path[3].x = m_ptViewLocationP1.x - 3;
	_Path[3].y = m_ptViewLocationP1.y - 14;
	_Path[4].x = m_ptViewLocationP1.x + 3;
	_Path[4].y = m_ptViewLocationP1.y - 14;

	::Polygon(_hdc, _Path, 8);

	return 0;
}

int DTGreenDownArrowGraph::Render(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, RGB(0, 255, 0));
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, RGB(0, 255, 0));
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetDCBrushColor(_hdc, clrOfBrush);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGreenDownArrowGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrShadow);
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetDCBrushColor(_hdc, clrOfBrush);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGreenDownArrowGraph::RenderEditing(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, RGB(0, 255, 0));
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, RGB(0, 255, 0));
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetDCBrushColor(_hdc, clrOfBrush);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTGreenDownArrowGraph::RenderAnchors(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrAnchors = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrAnchors);
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrAnchors);
	//// <!-- render -->
	::Rectangle(_hdc, m_ptViewLocationP1.x - m_nAnchorThickness, m_ptViewLocationP1.y - m_nAnchorThickness, m_ptViewLocationP1.x + m_nAnchorThickness, m_ptViewLocationP1.y + m_nAnchorThickness);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCBrushColor(_hdc, clrOfBrush);
	return 0;
}

int DTGreenDownArrowGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGreenDownArrowGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTGreenDownArrowGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTGreenDownArrowGraph::Build()
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

	m_ArrowRect.top = m_ptViewLocationP1.y - 14;
	m_ArrowRect.bottom = m_ptViewLocationP1.y; 
	m_ArrowRect.left = m_ptViewLocationP1.x - 7;
	m_ArrowRect.right = m_ptViewLocationP1.x + 7;
	return 0;
}

int DTGreenDownArrowGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x))
	{
		m_bAvaiable = false;
		return -1;
	}

	m_bAvaiable = true;
	Build();
	return 0;
}

int DTGreenDownArrowGraph::EndMove(long _delta_X, long _delta_Y)
{
	return DTAssistGraph::EndMove();
}

int DTGreenDownArrowGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	return BuildAccordToDescription();
}

int DTGreenDownArrowGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTGreenDownArrowGraph& _Clone = reinterpret_cast<DTGreenDownArrowGraph&>(_Other);
	m_ArrowRect = _Clone.m_ArrowRect;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 绿色下箭头 Green Down Arrow
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 矩形框 Rectangle
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTRectangleGraph::DTRectangleGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTRectangleGraph::DTRectangleGraph(DTRectangleGraph& _other)
: DTAssistGraphI(_other)
{
	m_Rect = _other.m_Rect;
}

int DTRectangleGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTRectangleGraph::NextEditMode(int _CurMode)
{
	return DTRectangleGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTRectangleGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTRectangleGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTRectangleGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = ((CMP_WITH_PERMISSIBLE_ERROR(m_ptViewLocationP1.x, _Point.x, 2.0) || CMP_WITH_PERMISSIBLE_ERROR(m_ptViewLocationP2.x, _Point.x, 2.0)) 
						&& (m_Rect.top <= _Point.y && _Point.y <= m_Rect.bottom)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = ((CMP_WITH_PERMISSIBLE_ERROR(m_ptViewLocationP1.y, _Point.y, 2.0) || CMP_WITH_PERMISSIBLE_ERROR(m_ptViewLocationP2.y, _Point.y, 2.0)) 
						&& (m_Rect.left <= _Point.x && _Point.x <= m_Rect.right)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
	return _HitTest;
}

int DTRectangleGraph::Render_i(HDC _hdc)
{
	tagDTPoint _Path[4] = 
	{
		{ m_Rect.left, m_Rect.top },
		{ m_Rect.right, m_Rect.top },
		{ m_Rect.right, m_Rect.bottom },
		{ m_Rect.left, m_Rect.bottom }
	};
	
	::MoveToEx(_hdc, _Path[0].x, _Path[0].y, NULL);
	::LineTo(_hdc, _Path[1].x, _Path[1].y);
	::LineTo(_hdc, _Path[2].x, _Path[2].y);
	::LineTo(_hdc, _Path[3].x, _Path[3].y);
	::LineTo(_hdc, _Path[0].x, _Path[0].y);

	return 0;
}

int DTRectangleGraph::Render(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- render -->
	return Render_i(_hdc);;
}

int DTRectangleGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrShadow);
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCPenColor(_hdc, clrOfBrush);
	return 0;
}

int DTRectangleGraph::RenderEditing(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

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

int DTRectangleGraph::RenderAnchors(HDC _hdc)
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

int DTRectangleGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTRectangleGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTRectangleGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTRectangleGraph::Build()
{
	m_rcVirtualRect.left = min(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.top = min(m_ptViewLocationP1.y, m_ptViewLocationP2.y);
	m_rcVirtualRect.right = max(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.bottom = max(m_ptViewLocationP1.y, m_ptViewLocationP2.y);
	m_Rect = m_rcVirtualRect;
	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	return 0;
}

int DTRectangleGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	m_ptViewLocationP2.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);
	m_ptViewLocationP2.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP2.yValue);


	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x))
	{
		m_bAvaiable = false;
		return -1;
	}

	m_bAvaiable = true;
	Build();
	return 0;
}

int DTRectangleGraph::EndMove(long _delta_X, long _delta_Y)
{
	return DTAssistGraph::EndMove();
}

int DTRectangleGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTRectangleGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTRectangleGraph& _Clone = reinterpret_cast<DTRectangleGraph&>(_Other);
	m_Rect = _Clone.m_Rect;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 矩形框 Rectangle
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////