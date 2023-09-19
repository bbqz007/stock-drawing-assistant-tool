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

#include "stdafx.h"
#include "DrawToolDetail.h"
#include "DrawToolBasicUtil.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 价格通道线 Price Channels
// <!---
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTPriceChannelsGraph::DTPriceChannelsGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTPriceChannelsGraph::DTPriceChannelsGraph(DTPriceChannelsGraph& _other)
: DTAssistGraphI(_other)
{
	m_EquationBase = _other.m_EquationBase;
	m_EquationParallel1 = _other.m_EquationParallel1;
	m_EquationParallel2 = _other.m_EquationParallel2;
}

int DTPriceChannelsGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfParallelLine(_CurMode);
}

int DTPriceChannelsGraph::NextEditMode(int _CurMode)
{
	return DTPriceChannelsGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTPriceChannelsGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfParallelLine();
}

DTAssistGraphDescriptionScript DTPriceChannelsGraph::DescriptionScript()
{
	return _DescriptionScript();
}


int DTPriceChannelsGraph::HitTest(const tagDTPoint& _Point, long _xDate)
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
	{
		if((_HitTest = m_EquationBase.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
		else if((_HitTest = m_EquationParallel1.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
		else if((_HitTest = m_EquationParallel2.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
	}
	return _HitTest;
}

int DTPriceChannelsGraph::Render(HDC _hdc)
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
		_Ret = m_EquationParallel1.Render(_hdc, m_rcValidateRect);
		_Ret = m_EquationParallel2.Render(_hdc, m_rcValidateRect);
		::SelectObject(_hdc, oldPen);
		::DeleteObject(usrPen);
		return _Ret;
	}
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	m_EquationParallel1.Render(_hdc, m_rcValidateRect);
	m_EquationParallel2.Render(_hdc, m_rcValidateRect);
	return 0;
}

int DTPriceChannelsGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	m_EquationParallel1.Render(_hdc, m_rcValidateRect);
	m_EquationParallel2.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTPriceChannelsGraph::RenderEditing(HDC _hdc)
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
	m_EquationParallel1.Render(_hdc, m_rcValidateRect);
	m_EquationParallel2.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTPriceChannelsGraph::RenderAnchors(HDC _hdc)
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

int DTPriceChannelsGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTPriceChannelsGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTPriceChannelsGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTPriceChannelsGraph::BuildAccordToDescription()
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

int DTPriceChannelsGraph::Build()
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

	DTLineEquation _EquationNormal;
	tagDTPoint _PointAt;
	m_EquationBase.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	m_EquationParallel1.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	m_EquationParallel1.MoveTo(m_ptViewLocationP3);
	m_EquationParallel2.Build(m_ptViewLocationP1, m_ptViewLocationP2);

	_EquationNormal.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	_EquationNormal.MoveTo(m_ptViewLocationP3);
	_EquationNormal.Perpendicular();
	if(ISDTOK(_EquationNormal.IntersectAt(m_EquationBase, _PointAt)))
	{
		_PointAt.x = _PointAt.x * 2 - m_ptViewLocationP3.x;
		_PointAt.y = _PointAt.y * 2 - m_ptViewLocationP3.y;
		m_EquationParallel2.MoveTo(_PointAt);
	}

	return 0;
}

int DTPriceChannelsGraph::EndMove(long _delta_X, long _delta_Y)
{
	m_EquationParallel1.Offset(_delta_X, _delta_Y);
	m_EquationParallel2.Offset(_delta_X, _delta_Y);
	m_EquationBase.Offset(_delta_X, _delta_Y);
	return DTAssistGraph::EndMove();
}

int DTPriceChannelsGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	m_descGraph.m_ptLocations.ptAbsoluteP3.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP3.x);
	m_descGraph.m_ptLocations.ptAbsoluteP3.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP3.y);

	return BuildAccordToDescription();
}

int DTPriceChannelsGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTPriceChannelsGraph& _Clone = reinterpret_cast<DTPriceChannelsGraph&>(_Other);
	m_EquationBase = _Clone.m_EquationBase;
	m_EquationParallel1 = _Clone.m_EquationParallel1;
	m_EquationParallel2 = _Clone.m_EquationParallel2;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 价格通道线 Price Channels
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 平行直线 Parallel Lines
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTParallelLinesGraph::DTParallelLinesGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTParallelLinesGraph::DTParallelLinesGraph(DTParallelLinesGraph& _other)
: DTAssistGraphI(_other)
{
	m_EquationBase = _other.m_EquationBase;
	m_EquationParallel1 = _other.m_EquationParallel1;
}

int DTParallelLinesGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfParallelLine(_CurMode);
}

int DTParallelLinesGraph::NextEditMode(int _CurMode)
{
	return DTParallelLinesGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTParallelLinesGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfParallelLine();
}

DTAssistGraphDescriptionScript DTParallelLinesGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTParallelLinesGraph::HitTest(const tagDTPoint& _Point, long _xDate)
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
	{
		if((_HitTest = m_EquationBase.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
		else if((_HitTest = m_EquationParallel1.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
	}
	return _HitTest;
}

int DTParallelLinesGraph::Render(HDC _hdc)
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
		_Ret = m_EquationParallel1.Render(_hdc, m_rcValidateRect);
		::SelectObject(_hdc, oldPen);
		::DeleteObject(usrPen);
		return _Ret;
	}
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	m_EquationParallel1.Render(_hdc, m_rcValidateRect);
	return 0;
}

int DTParallelLinesGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	m_EquationParallel1.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTParallelLinesGraph::RenderEditing(HDC _hdc)
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
	m_EquationParallel1.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTParallelLinesGraph::RenderAnchors(HDC _hdc)
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

int DTParallelLinesGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTParallelLinesGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTParallelLinesGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTParallelLinesGraph::BuildAccordToDescription()
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

int DTParallelLinesGraph::Build()
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

	DTLineEquation _EquationNormal;
	m_EquationBase.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	m_EquationParallel1.Build(m_ptViewLocationP1, m_ptViewLocationP2);
	m_EquationParallel1.MoveTo(m_ptViewLocationP3);

	return 0;
}

int DTParallelLinesGraph::EndMove(long _delta_X, long _delta_Y)
{
	m_EquationParallel1.Offset(_delta_X, _delta_Y);
	m_EquationBase.Offset(_delta_X, _delta_Y);
	return DTAssistGraph::EndMove();
}

int DTParallelLinesGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	m_descGraph.m_ptLocations.ptAbsoluteP3.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP3.x);
	m_descGraph.m_ptLocations.ptAbsoluteP3.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP3.y);

	return BuildAccordToDescription();
}

int DTParallelLinesGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTParallelLinesGraph& _Clone = reinterpret_cast<DTParallelLinesGraph&>(_Other);
	m_EquationBase = _Clone.m_EquationBase;
	m_EquationParallel1 = _Clone.m_EquationParallel1;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 平行直线 Parallel Lines
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////