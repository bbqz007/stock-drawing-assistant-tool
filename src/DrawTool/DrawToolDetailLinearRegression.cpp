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
#include <deque>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 线形回归线 Linear Regression Area
// <!---
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTLinearRegressionAreaGraph::DTLinearRegressionAreaGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTLinearRegressionAreaGraph::DTLinearRegressionAreaGraph(DTLinearRegressionAreaGraph& _other)
: DTAssistGraphI(_other)
{
	m_EquationBase = _other.m_EquationBase;
	m_EquationRegressionHigh = _other.m_EquationRegressionHigh;
	m_EquationRegressionLow = _other.m_EquationRegressionLow;
	m_Distance = _other.m_Distance;
}

int DTLinearRegressionAreaGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTLinearRegressionAreaGraph::NextEditMode(int _CurMode)
{
	return DTLinearRegressionAreaGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTLinearRegressionAreaGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTLinearRegressionAreaGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTLinearRegressionAreaGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
	{
		if((_HitTest = m_EquationBase.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
		else if((_HitTest = m_EquationRegressionHigh.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
		else if((_HitTest = m_EquationRegressionLow.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
	}
	return _HitTest;
}

int DTLinearRegressionAreaGraph::Render(HDC _hdc)
{
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	m_EquationRegressionHigh.Render(_hdc, m_rcValidateRect);
	m_EquationRegressionLow.Render(_hdc, m_rcValidateRect);
	return 0;
}

int DTLinearRegressionAreaGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	m_EquationRegressionHigh.Render(_hdc, m_rcValidateRect);
	m_EquationRegressionLow.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTLinearRegressionAreaGraph::RenderEditing(HDC _hdc)
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
	m_EquationRegressionHigh.Render(_hdc, m_rcValidateRect);
	m_EquationRegressionLow.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTLinearRegressionAreaGraph::RenderAnchors(HDC _hdc)
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

int DTLinearRegressionAreaGraph::RenderShadowAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y) )
	{
		DTVerticalEquation _AssistLineEquation;
		//// <!-- prepare colors -->
		COLORREF clrShadow = m_Graphics->ShadowColor();
		COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
		//// <!-- render -->
		_AssistLineEquation.MoveTo(m_ptViewLocationP1);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		_AssistLineEquation.MoveTo(m_ptViewLocationP2);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		//// <!-- restore colors -->
		::SetDCPenColor(_hdc, clrOfPen);
	}
	return RenderAnchors(_hdc);
}

int DTLinearRegressionAreaGraph::RenderEditingAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y) )
	{
		DTVerticalEquation _AssistLineEquation;
		//// <!-- prepare colors -->
		COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
		COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
		int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
		//// <!-- render -->
		_AssistLineEquation.MoveTo(m_ptViewLocationP1);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		_AssistLineEquation.MoveTo(m_ptViewLocationP2);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		//// <!-- restore colors -->
		::SetROP2(_hdc, nOldRop2);
		::SetDCPenColor(_hdc, clrOfPen);
	}
	return 0;
}

int DTLinearRegressionAreaGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTLinearRegressionAreaGraph::BuildAccordToDescription()
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

int DTLinearRegressionAreaGraph::Build()
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

	double _Mean_ViewX = 0.0;
	double _Mean_ViewY = 0.0;
	double _Sum_ViewXViewY = 0.0;
	double _Sum_SquareViewX = 0.0;
	double _Sum_ViewX = 0.0;
	double eCoefficentA = 0.0;
	double eCoefficentB = 0.0;
	double _UnitHeight = m_coorTransform.UnitHeight();
	double _YVirtualO = m_coorTransform.ViewYtoAbsY(0);

	long _index_xDate1 = m_coorTransform.IndexOfDate(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	long _index_xDate2 = m_coorTransform.IndexOfDate(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);

	if(!CHECK_VIEW_XINDEX_AXIS_AVAIABLE(_index_xDate1)
		|| !CHECK_VIEW_XINDEX_AXIS_AVAIABLE(_index_xDate2)
		|| ::abs(_UnitHeight) < 1e-10 )
	{
		m_bAvaiable = false;
		return -1;
	}

	std::deque<tagKValue> _dqKValues;
	std::deque<long> _dqViewX;

	long _index_xLeft = min(_index_xDate1, _index_xDate2);
	long _index_xRight = max(_index_xDate1, _index_xDate2);
	long _n = 0;
	for(long i = _index_xLeft; i <= _index_xRight; ++i, ++_n)
	{
		_dqKValues.push_back(m_coorTransform.KValueByIndex(i));
		_dqViewX.push_back(m_coorTransform.ViewXByIndex(i));
		double _ViewX = (double) _dqViewX.back();
		double _ValueY = _YVirtualO - (double) _dqKValues.back().m_fClose;
		_Mean_ViewX += _ViewX;
		_Mean_ViewY += _ValueY;
		_Sum_ViewXViewY += _ViewX * _ValueY;
		_Sum_SquareViewX += _ViewX * _ViewX;
	}

	if(_n == 0)
	{
		m_bAvaiable = false;
		return -1;
	}

	_Sum_ViewX = _Mean_ViewX;
	_Mean_ViewX /= _dqViewX.size();
	_Mean_ViewY /= _dqKValues.size();
	
	double eCoefficent_lxy = 0.0;
	double eCoefficent_lxx = 0.0;

	/*for(size_t i = 0; i < _dqViewX.size(); ++i)
	{
		eCoefficent_lxy += ((double)_dqViewX[i] - _Mean_ViewX) * ((_YVirtualO - _dqKValues[i].m_fClose) - _Mean_ViewY);
		eCoefficent_lxx += ::pow(((double)_dqViewX[i] - _Mean_ViewX), 2);
	}*/
	eCoefficent_lxy = _Sum_ViewXViewY - (double)_n * _Mean_ViewX * _Mean_ViewY;
	eCoefficent_lxx = _Sum_SquareViewX - (double)_n * ::pow(_Mean_ViewX, 2);

	if(::abs(eCoefficent_lxx) < 1e-4)
	{
		m_bAvaiable = false;
		return -1;
	}
	eCoefficent_lxy /= _UnitHeight;

	eCoefficentA = eCoefficent_lxy / eCoefficent_lxx;
	eCoefficentB = _Mean_ViewY / _UnitHeight - eCoefficentA * _Mean_ViewX;

	m_ptViewLocationP1.x = m_coorTransform.ViewXByIndex(_index_xDate1);
	m_ptViewLocationP1.y = (long) (eCoefficentA * (double)m_ptViewLocationP1.x + eCoefficentB);
	m_ptViewLocationP2.x = m_coorTransform.ViewXByIndex(_index_xDate2);
	m_ptViewLocationP2.y = (long) (eCoefficentA * (double)m_ptViewLocationP2.x + eCoefficentB);
	
	m_rcVirtualRect.left = min(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.right = max(m_ptViewLocationP1.x, m_ptViewLocationP2.x);

	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	m_EquationBase.Build(eCoefficentA, eCoefficentB);

	double eHighCoefficentB = eCoefficentB;
	double eLowCoefficentB = eCoefficentB;
	for(long i = 0; i < _dqViewX.size(); ++i)
	{
		double eTmpCoefficentB;
		eTmpCoefficentB = (double) m_coorTransform.AbsYtoViewY(_dqKValues[i].m_fHigh) - eCoefficentA * (double) _dqViewX[i]; 
		if(eTmpCoefficentB < eHighCoefficentB)
			eHighCoefficentB = eTmpCoefficentB;
		eTmpCoefficentB = (double) m_coorTransform.AbsYtoViewY(_dqKValues[i].m_fLow) - eCoefficentA * (double) _dqViewX[i]; 
		if(eTmpCoefficentB > eLowCoefficentB)
			eLowCoefficentB = eTmpCoefficentB;
	}

	m_EquationRegressionHigh.Build(eCoefficentA, eHighCoefficentB);
	m_EquationRegressionLow.Build(eCoefficentA, eLowCoefficentB);

	m_bAvaiable = !m_EquationBase.IsVertical() && !m_EquationRegressionHigh.IsVertical() && !m_EquationRegressionLow.IsVertical();
	return 0;
}

void DTLinearRegressionAreaGraph::MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint)
{
	long _delta_X = _ToPoint.x - _FromPoint.x;
	long _delta_Y = _ToPoint.y - _FromPoint.y;

	m_ptViewLocationP1.x += _delta_X;
	m_ptViewLocationP1.y += _delta_Y;

	m_ptViewLocationP2.x += _delta_X;
	m_ptViewLocationP2.y += _delta_Y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	Build();
}

void DTLinearRegressionAreaGraph::MoveP1To(const tagDTPoint& _Point)
{
	m_ptViewLocationP1.x = _Point.x;
	m_ptViewLocationP1.y = _Point.y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	Build();
}

void DTLinearRegressionAreaGraph::MoveP2To(const tagDTPoint& _Point)
{
	m_ptViewLocationP2.x = _Point.x;
	m_ptViewLocationP2.y = _Point.y;

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	Build();
}

int DTLinearRegressionAreaGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTLinearRegressionAreaGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTLinearRegressionAreaGraph& _Clone = reinterpret_cast<DTLinearRegressionAreaGraph&>(_Other);
	m_EquationBase = _Clone.m_EquationBase;
	m_EquationRegressionHigh = _Clone.m_EquationRegressionHigh;
	m_EquationRegressionLow = _Clone.m_EquationRegressionLow;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 线形回归线 Linear Regression Area
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 延长线形回归线 Linear Regression Extension Area
// <!---
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTLinearRegressionExtensionAreaGraph::DTLinearRegressionExtensionAreaGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTLinearRegressionExtensionAreaGraph::DTLinearRegressionExtensionAreaGraph(DTLinearRegressionExtensionAreaGraph& _other)
: DTAssistGraphI(_other)
{
	m_EquationBase = _other.m_EquationBase;
	m_EquationRegressionHigh = _other.m_EquationRegressionHigh;
	m_EquationRegressionLow = _other.m_EquationRegressionLow;
	m_rcVirtualExRect = _other.m_rcVirtualExRect;
	m_rcValidateExRect = _other.m_rcValidateExRect;
	m_Distance = _other.m_Distance;
}

int DTLinearRegressionExtensionAreaGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTLinearRegressionExtensionAreaGraph::NextEditMode(int _CurMode)
{
	return DTLinearRegressionExtensionAreaGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTLinearRegressionExtensionAreaGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTLinearRegressionExtensionAreaGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTLinearRegressionExtensionAreaGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point)
		&& !::PtInRect(&m_rcValidateExRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
	{
		if((_HitTest = m_EquationBase.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
		else if((_HitTest = m_EquationRegressionHigh.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
		else if((_HitTest = m_EquationRegressionLow.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
	}
	return _HitTest;
}

int DTLinearRegressionExtensionAreaGraph::Render(HDC _hdc)
{
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	m_EquationRegressionHigh.Render(_hdc, m_rcValidateRect);
	m_EquationRegressionLow.Render(_hdc, m_rcValidateRect);
	return RenderExtension(_hdc);
}

int DTLinearRegressionExtensionAreaGraph::RenderShadow(HDC _hdc)
{
	if(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y)
		return 0;

	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	m_EquationRegressionHigh.Render(_hdc, m_rcValidateRect);
	m_EquationRegressionLow.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	return RenderShadowExtension(_hdc);
}

int DTLinearRegressionExtensionAreaGraph::RenderEditing(HDC _hdc)
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
	m_EquationRegressionHigh.Render(_hdc, m_rcValidateRect);
	m_EquationRegressionLow.Render(_hdc, m_rcValidateRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetDCPenColor(_hdc, clrOfPen);
	return RenderEditingExtension(_hdc);
}

int DTLinearRegressionExtensionAreaGraph::RenderAnchors(HDC _hdc)
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

int DTLinearRegressionExtensionAreaGraph::RenderShadowAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y) )
	{
		DTVerticalEquation _AssistLineEquation;
		//// <!-- prepare colors -->
		COLORREF clrShadow = m_Graphics->ShadowColor();
		COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
		//// <!-- render -->
		_AssistLineEquation.MoveTo(m_ptViewLocationP1);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		_AssistLineEquation.MoveTo(m_ptViewLocationP2);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		//// <!-- restore colors -->
		::SetDCPenColor(_hdc, clrOfPen);
	}
	return RenderAnchors(_hdc);
}

int DTLinearRegressionExtensionAreaGraph::RenderEditingAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y) )
	{
		DTVerticalEquation _AssistLineEquation;
		//// <!-- prepare colors -->
		COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
		COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
		int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
		//// <!-- render -->
		_AssistLineEquation.MoveTo(m_ptViewLocationP1);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		_AssistLineEquation.MoveTo(m_ptViewLocationP2);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		//// <!-- restore colors -->
		::SetROP2(_hdc, nOldRop2);
		::SetDCPenColor(_hdc, clrOfPen);
	}
	return 0;
}

int DTLinearRegressionExtensionAreaGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTLinearRegressionExtensionAreaGraph::RenderExtension(HDC _hdc)
{
	//// <!-- prepare colors -->
	HPEN dotPen = ::CreatePen(PS_DOT, 1, ::GetDCPenColor(_hdc));
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateExRect);
	m_EquationRegressionHigh.Render(_hdc, m_rcValidateExRect);
	m_EquationRegressionLow.Render(_hdc, m_rcValidateExRect);
	//// <!-- restore colors -->
	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);
	return 0;
}

int DTLinearRegressionExtensionAreaGraph::RenderShadowExtension(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	HPEN dotPen = ::CreatePen(PS_DOT, 1, clrShadow);
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateExRect);
	m_EquationRegressionHigh.Render(_hdc, m_rcValidateExRect);
	m_EquationRegressionLow.Render(_hdc, m_rcValidateExRect);
	//// <!-- restore colors -->
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);
	return 0;
}

int DTLinearRegressionExtensionAreaGraph::RenderEditingExtension(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
	COLORREF clrShadowBkGrnd = m_Graphics->ShadowBackGroupColor();
	HPEN dotPen = ::CreatePen(PS_DOT, 1, clrShadowFgGrnd);
	HPEN oldPen = (HPEN)::SelectObject(_hdc, dotPen);
	COLORREF clrOfBkGrnd = ::SetBkColor(_hdc, clrShadowBkGrnd);
	int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateExRect);
	m_EquationRegressionHigh.Render(_hdc, m_rcValidateExRect);
	m_EquationRegressionLow.Render(_hdc, m_rcValidateExRect);
	//// <!-- restore colors -->
	::SetROP2(_hdc, nOldRop2);
	::SetBkColor(_hdc, clrOfBkGrnd);
	::SelectObject(_hdc, oldPen);
	::DeleteObject(dotPen);
	return 0;
}


int DTLinearRegressionExtensionAreaGraph::BuildAccordToDescription()
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

int DTLinearRegressionExtensionAreaGraph::Build()
{
	m_rcVirtualRect.left = LONG_INFINITE_LEFT;
	m_rcVirtualRect.top = LONG_INFINITE_TOP;
	m_rcVirtualRect.right = LONG_INFINITE_RIGHT;
	m_rcVirtualRect.bottom = LONG_INFINITE_BOTTOM;
	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);
	m_rcVirtualExRect = m_rcVirtualRect;

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	m_rcValidateExRect.left = m_rcValidateRect.left;
	m_rcValidateExRect.top = m_rcValidateRect.top;
	m_rcValidateExRect.right = m_rcValidateRect.right;
	m_rcValidateExRect.bottom = m_rcValidateRect.bottom;

	double _Mean_ViewX = 0.0;
	double _Mean_ViewY = 0.0;
	double _Sum_ViewXViewY = 0.0;
	double _Sum_SquareViewX = 0.0;
	double _Sum_ViewX = 0.0;
	double eCoefficentA = 0.0;
	double eCoefficentB = 0.0;
	double _UnitHeight = m_coorTransform.UnitHeight();
	double _YVirtualO = m_coorTransform.ViewYtoAbsY(0);

	long _index_xDate1 = m_coorTransform.IndexOfDate(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	long _index_xDate2 = m_coorTransform.IndexOfDate(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);

	if(!CHECK_VIEW_XINDEX_AXIS_AVAIABLE(_index_xDate1)
		|| !CHECK_VIEW_XINDEX_AXIS_AVAIABLE(_index_xDate2)
		|| ::abs(_UnitHeight) < 1e-10 )
	{
		m_bAvaiable = false;
		return -1;
	}

	std::deque<tagKValue> _dqKValues;
	std::deque<long> _dqViewX;

	long _index_xLeft = min(_index_xDate1, _index_xDate2);
	long _index_xRight = max(_index_xDate1, _index_xDate2);
	long _n = 0;
	for(long i = _index_xLeft; i <= _index_xRight; ++i, ++_n)
	{
		_dqKValues.push_back(m_coorTransform.KValueByIndex(i));
		_dqViewX.push_back(m_coorTransform.ViewXByIndex(i));
		double _ViewX = (double) _dqViewX.back();
		double _ValueY = _YVirtualO - (double) _dqKValues.back().m_fClose;
		_Mean_ViewX += _ViewX;
		_Mean_ViewY += _ValueY;
		_Sum_ViewXViewY += _ViewX * _ValueY;
		_Sum_SquareViewX += _ViewX * _ViewX;
	}

	if(_n == 0)
	{
		m_bAvaiable = false;
		return -1;
	}

	_Sum_ViewX = _Mean_ViewX;
	_Mean_ViewX /= _dqViewX.size();
	_Mean_ViewY /= _dqKValues.size();
	
	double eCoefficent_lxy = 0.0;
	double eCoefficent_lxx = 0.0;

	/*for(size_t i = 0; i < _dqViewX.size(); ++i)
	{
		eCoefficent_lxy += ((double)_dqViewX[i] - _Mean_ViewX) * ((_YVirtualO - _dqKValues[i].m_fClose) - _Mean_ViewY);
		eCoefficent_lxx += ::pow(((double)_dqViewX[i] - _Mean_ViewX), 2);
	}*/
	eCoefficent_lxy = _Sum_ViewXViewY - (double)_n * _Mean_ViewX * _Mean_ViewY;
	eCoefficent_lxx = _Sum_SquareViewX - (double)_n * ::pow(_Mean_ViewX, 2);

	if(::abs(eCoefficent_lxx) < 1e-4)
	{
		m_bAvaiable = false;
		return -1;
	}
	eCoefficent_lxy /= _UnitHeight;

	eCoefficentA = eCoefficent_lxy / eCoefficent_lxx;
	eCoefficentB = _Mean_ViewY / _UnitHeight - eCoefficentA * _Mean_ViewX;

	m_ptViewLocationP1.x = m_coorTransform.ViewXByIndex(_index_xDate1);
	m_ptViewLocationP1.y = (long) (eCoefficentA * (double)m_ptViewLocationP1.x + eCoefficentB);
	m_ptViewLocationP2.x = m_coorTransform.ViewXByIndex(_index_xDate2);
	m_ptViewLocationP2.y = (long) (eCoefficentA * (double)m_ptViewLocationP2.x + eCoefficentB);
	
	m_rcVirtualRect.left = min(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.right = max(m_ptViewLocationP1.x, m_ptViewLocationP2.x);

	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	m_rcVirtualExRect.left = m_rcVirtualRect.right;

	if(!::IntersectRect(&m_rcValidateExRect, &m_rcVirtualExRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateExRect.left = LONG_INFINITE_LEFT;
		m_rcValidateExRect.top = LONG_INFINITE_TOP;
		m_rcValidateExRect.right = LONG_INFINITE_LEFT;
		m_rcValidateExRect.bottom = LONG_INFINITE_TOP;
	}

	m_EquationBase.Build(eCoefficentA, eCoefficentB);

	double eHighCoefficentB = eCoefficentB;
	double eLowCoefficentB = eCoefficentB;
	for(long i = 0; i < _dqViewX.size(); ++i)
	{
		double eTmpCoefficentB;
		eTmpCoefficentB = (double) m_coorTransform.AbsYtoViewY(_dqKValues[i].m_fHigh) - eCoefficentA * (double) _dqViewX[i]; 
		if(eTmpCoefficentB < eHighCoefficentB)
			eHighCoefficentB = eTmpCoefficentB;
		eTmpCoefficentB = (double) m_coorTransform.AbsYtoViewY(_dqKValues[i].m_fLow) - eCoefficentA * (double) _dqViewX[i]; 
		if(eTmpCoefficentB > eLowCoefficentB)
			eLowCoefficentB = eTmpCoefficentB;
	}

	m_EquationRegressionHigh.Build(eCoefficentA, eHighCoefficentB);
	m_EquationRegressionLow.Build(eCoefficentA, eLowCoefficentB);

	m_bAvaiable = !m_EquationBase.IsVertical() && !m_EquationRegressionHigh.IsVertical() && !m_EquationRegressionLow.IsVertical();
	return 0;
}

void DTLinearRegressionExtensionAreaGraph::MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint)
{
	long _delta_X = _ToPoint.x - _FromPoint.x;
	long _delta_Y = _ToPoint.y - _FromPoint.y;

	m_ptViewLocationP1.x += _delta_X;
	m_ptViewLocationP1.y += _delta_Y;

	m_ptViewLocationP2.x += _delta_X;
	m_ptViewLocationP2.y += _delta_Y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	Build();
}

void DTLinearRegressionExtensionAreaGraph::MoveP1To(const tagDTPoint& _Point)
{
	m_ptViewLocationP1.x = _Point.x;
	m_ptViewLocationP1.y = _Point.y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	Build();
}

void DTLinearRegressionExtensionAreaGraph::MoveP2To(const tagDTPoint& _Point)
{
	m_ptViewLocationP2.x = _Point.x;
	m_ptViewLocationP2.y = _Point.y;

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	Build();
}

int DTLinearRegressionExtensionAreaGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTLinearRegressionExtensionAreaGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTLinearRegressionExtensionAreaGraph& _Clone = reinterpret_cast<DTLinearRegressionExtensionAreaGraph&>(_Other);
	m_EquationBase = _Clone.m_EquationBase;
	m_EquationRegressionHigh = _Clone.m_EquationRegressionHigh;
	m_EquationRegressionLow = _Clone.m_EquationRegressionLow;
	m_rcVirtualExRect = _Clone.m_rcVirtualExRect;
	m_rcValidateExRect = _Clone.m_rcValidateExRect;
	m_Distance = _Clone.m_Distance;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 延长线形回归线 Linear Regression Extension Area
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 线性回归 Linear Regression Line
// <!---
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTLinearRegressionLineGraph::DTLinearRegressionLineGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTLinearRegressionLineGraph::DTLinearRegressionLineGraph(DTLinearRegressionLineGraph& _other)
: DTAssistGraphI(_other)
{
	m_EquationBase = _other.m_EquationBase;
	m_Distance = _other.m_Distance;
}

int DTLinearRegressionLineGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfSimpleLine(_CurMode);
}

int DTLinearRegressionLineGraph::NextEditMode(int _CurMode)
{
	return DTLinearRegressionLineGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTLinearRegressionLineGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfSimpleLine();
}

DTAssistGraphDescriptionScript DTLinearRegressionLineGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTLinearRegressionLineGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP1, _Point, ENUM_ASSIST_HITTEST_LOCATION_P1);
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = HitAnchor(m_ptViewLocationP2, _Point, ENUM_ASSIST_HITTEST_LOCATION_P2);

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
	{
		if((_HitTest = m_EquationBase.HitTest(_Point)) != ENUM_ASSIST_HITTEST_NULL) ;
	}
	return _HitTest;
}

int DTLinearRegressionLineGraph::Render(HDC _hdc)
{
	//// <!-- render -->
	m_EquationBase.Render(_hdc, m_rcValidateRect);
	return 0;
}

int DTLinearRegressionLineGraph::RenderShadow(HDC _hdc)
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

int DTLinearRegressionLineGraph::RenderEditing(HDC _hdc)
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

int DTLinearRegressionLineGraph::RenderAnchors(HDC _hdc)
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

int DTLinearRegressionLineGraph::RenderShadowAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y) )
	{
		DTVerticalEquation _AssistLineEquation;
		//// <!-- prepare colors -->
		COLORREF clrShadow = m_Graphics->ShadowColor();
		COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
		//// <!-- render -->
		_AssistLineEquation.MoveTo(m_ptViewLocationP1);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		_AssistLineEquation.MoveTo(m_ptViewLocationP2);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		//// <!-- restore colors -->
		::SetDCPenColor(_hdc, clrOfPen);
	}
	return RenderAnchors(_hdc);
}

int DTLinearRegressionLineGraph::RenderEditingAnchors(HDC _hdc)
{
	if(!(m_ptViewLocationP1.x == m_ptViewLocationP2.x 
		&& m_ptViewLocationP1.y == m_ptViewLocationP2.y) )
	{
		DTVerticalEquation _AssistLineEquation;
		//// <!-- prepare colors -->
		COLORREF clrShadowFgGrnd = m_Graphics->ShadowForeGroupColor();
		COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadowFgGrnd);
		int nOldRop2 = ::SetROP2(_hdc, R2_XORPEN);
		//// <!-- render -->
		_AssistLineEquation.MoveTo(m_ptViewLocationP1);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		_AssistLineEquation.MoveTo(m_ptViewLocationP2);
		_AssistLineEquation.Render(_hdc, m_rcValidateRect);
		//// <!-- restore colors -->
		::SetROP2(_hdc, nOldRop2);
		::SetDCPenColor(_hdc, clrOfPen);
	}
	return 0;
}

int DTLinearRegressionLineGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTLinearRegressionLineGraph::BuildAccordToDescription()
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

int DTLinearRegressionLineGraph::Build()
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

	double _Mean_ViewX = 0.0;
	double _Mean_ViewY = 0.0;
	double _Sum_ViewXViewY = 0.0;
	double _Sum_SquareViewX = 0.0;
	double _Sum_ViewX = 0.0;
	double eCoefficentA = 0.0;
	double eCoefficentB = 0.0;
	double _UnitHeight = m_coorTransform.UnitHeight();
	double _YVirtualO = m_coorTransform.ViewYtoAbsY(0);

	long _index_xDate1 = m_coorTransform.IndexOfDate(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	long _index_xDate2 = m_coorTransform.IndexOfDate(m_descGraph.m_ptLocations.ptAbsoluteP2.xDate);

	if(!CHECK_VIEW_XINDEX_AXIS_AVAIABLE(_index_xDate1)
		|| !CHECK_VIEW_XINDEX_AXIS_AVAIABLE(_index_xDate2)
		|| ::abs(_UnitHeight) < 1e-10 )
	{
		m_bAvaiable = false;
		return -1;
	}

	std::deque<tagKValue> _dqKValues;
	std::deque<long> _dqViewX;

	long _index_xLeft = min(_index_xDate1, _index_xDate2);
	long _index_xRight = max(_index_xDate1, _index_xDate2);
	long _n = 0;
	for(long i = _index_xLeft; i <= _index_xRight; ++i, ++_n)
	{
		_dqKValues.push_back(m_coorTransform.KValueByIndex(i));
		_dqViewX.push_back(m_coorTransform.ViewXByIndex(i));
		double _ViewX = (double) _dqViewX.back();
		double _ValueY = _YVirtualO - (double) _dqKValues.back().m_fClose;
		_Mean_ViewX += _ViewX;
		_Mean_ViewY += _ValueY;
		_Sum_ViewXViewY += _ViewX * _ValueY;
		_Sum_SquareViewX += _ViewX * _ViewX;
	}

	if(_n == 0)
	{
		m_bAvaiable = false;
		return -1;
	}

	_Sum_ViewX = _Mean_ViewX;
	_Mean_ViewX /= _dqViewX.size();
	_Mean_ViewY /= _dqKValues.size();
	
	double eCoefficent_lxy = 0.0;
	double eCoefficent_lxx = 0.0;

	eCoefficent_lxy = _Sum_ViewXViewY - (double)_n * _Mean_ViewX * _Mean_ViewY;
	eCoefficent_lxx = _Sum_SquareViewX - (double)_n * ::pow(_Mean_ViewX, 2);

	if(::abs(eCoefficent_lxx) < 1e-4)
	{
		m_bAvaiable = false;
		return -1;
	}
	eCoefficent_lxy /= _UnitHeight;

	eCoefficentA = eCoefficent_lxy / eCoefficent_lxx;
	eCoefficentB = _Mean_ViewY / _UnitHeight - eCoefficentA * _Mean_ViewX;

	m_ptViewLocationP1.x = m_coorTransform.ViewXByIndex(_index_xDate1);
	m_ptViewLocationP1.y = (long) (eCoefficentA * (double)m_ptViewLocationP1.x + eCoefficentB);
	m_ptViewLocationP2.x = m_coorTransform.ViewXByIndex(_index_xDate2);
	m_ptViewLocationP2.y = (long) (eCoefficentA * (double)m_ptViewLocationP2.x + eCoefficentB);
	
	m_rcVirtualRect.left = min(m_ptViewLocationP1.x, m_ptViewLocationP2.x);
	m_rcVirtualRect.right = max(m_ptViewLocationP1.x, m_ptViewLocationP2.x);

	DTEquationHelper::AdjustNarrowRect(m_rcVirtualRect);

	if(!::IntersectRect(&m_rcValidateRect, &m_rcVirtualRect, &m_coorTransform.ViewRect()))
	{
		m_rcValidateRect.left = LONG_INFINITE_LEFT;
		m_rcValidateRect.top = LONG_INFINITE_TOP;
		m_rcValidateRect.right = LONG_INFINITE_LEFT;
		m_rcValidateRect.bottom = LONG_INFINITE_TOP;
	}

	m_EquationBase.Build(eCoefficentA, eCoefficentB);

	m_bAvaiable = !m_EquationBase.IsVertical();
	return 0;
}

void DTLinearRegressionLineGraph::MoveTo(const tagDTPoint& _FromPoint, const tagDTPoint& _ToPoint)
{
	long _delta_X = _ToPoint.x - _FromPoint.x;
	long _delta_Y = _ToPoint.y - _FromPoint.y;

	m_ptViewLocationP1.x += _delta_X;
	m_ptViewLocationP1.y += _delta_Y;

	m_ptViewLocationP2.x += _delta_X;
	m_ptViewLocationP2.y += _delta_Y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	Build();
}

void DTLinearRegressionLineGraph::MoveP1To(const tagDTPoint& _Point)
{
	m_ptViewLocationP1.x = _Point.x;
	m_ptViewLocationP1.y = _Point.y;

	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	Build();
}

void DTLinearRegressionLineGraph::MoveP2To(const tagDTPoint& _Point)
{
	m_ptViewLocationP2.x = _Point.x;
	m_ptViewLocationP2.y = _Point.y;

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	Build();
}

int DTLinearRegressionLineGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	m_descGraph.m_ptLocations.ptAbsoluteP2.xDate = m_coorTransform.ViewXtoAbsX(m_ptViewLocationP2.x);
	m_descGraph.m_ptLocations.ptAbsoluteP2.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP2.y);

	return BuildAccordToDescription();
}

int DTLinearRegressionLineGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTLinearRegressionLineGraph& _Clone = reinterpret_cast<DTLinearRegressionLineGraph&>(_Other);
	m_EquationBase = _Clone.m_EquationBase;
	m_Distance = _Clone.m_Distance;
	return DTAssistGraph::Clone(_Other);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 线性回归 Linear Regression Line
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////