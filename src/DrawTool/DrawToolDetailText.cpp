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
// 文本 Text
// <!--
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DTTextGraph::DTTextGraph(DTAssistGraphics* _Graphics, const DTAssistGraphDescription& _descGraph)
: DTAssistGraphI(_Graphics, _descGraph)
{
}

DTTextGraph::DTTextGraph(DTTextGraph& _other)
: DTAssistGraphI(_other)
{
	m_Text = _other.m_Text;
	m_TextRect = _other.m_TextRect;
}

int DTTextGraph::_NextEditMode(int _CurMode)
{
	return DTAssistGraph::_NextEditModeOfText(_CurMode);
}

int DTTextGraph::NextEditMode(int _CurMode)
{
	return DTTextGraph::_NextEditMode(_CurMode);
}

DTAssistGraphDescriptionScript DTTextGraph::_DescriptionScript()
{
	return DTAssistGraph::_DescriptionScriptOfText();
}

DTAssistGraphDescriptionScript DTTextGraph::DescriptionScript()
{
	return _DescriptionScript();
}

int DTTextGraph::HitTest(const tagDTPoint& _Point, long _xDate)
{
	int _HitTest = ENUM_ASSIST_HITTEST_NULL;

	if(!::PtInRect(&m_rcValidateRect, _Point))
		return _HitTest;
	
	if(_HitTest == ENUM_ASSIST_HITTEST_NULL)
		_HitTest = (::PtInRect(&m_TextRect, _Point)) ? ENUM_ASSIST_HITTEST_TARGET : ENUM_ASSIST_HITTEST_NULL;
	return _HitTest;
}

int DTTextGraph::Render_i(HDC _hdc)
{
	//// <!-- prepare fnot -->
	HFONT _TextFont = ::CreateFont(m_descGraph.m_propLine.nWidth, 0,
										0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
										OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, ANTIALIASED_QUALITY,
										VARIABLE_PITCH | FF_DONTCARE, _T("Arial"));
	HFONT oldTextFont = (HFONT) ::SelectObject(_hdc, _TextFont);

	m_Text.Revise();
	//// <!-- render text -->
	::DrawTextA(_hdc, m_Text.m_Text, ::strlen(m_Text.m_Text), &m_TextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	//// <!-- restore fnot -->
	::SelectObject(_hdc, oldTextFont);
	::DeleteObject(_TextFont);
	return 0;
}

int DTTextGraph::Render(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, RGB(0, 255, 0));
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, RGB(0, 255, 0));
	COLORREF clrOfText = ::SetTextColor(_hdc, m_descGraph.m_propLine.rgbColor);
	int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetBkMode(_hdc, oldBkMode);
	::SetTextColor(_hdc, clrOfText);
	::SetDCBrushColor(_hdc, clrOfBrush);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTTextGraph::RenderShadow(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrShadow = m_Graphics->ShadowColor();
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, clrShadow);
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrShadow);
	COLORREF clrOfText = ::SetTextColor(_hdc, clrShadow);
	int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetBkMode(_hdc, oldBkMode);
	::SetTextColor(_hdc, clrOfText);
	::SetDCBrushColor(_hdc, clrOfBrush);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTTextGraph::RenderEditing(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, RGB(0, 255, 0));
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, RGB(0, 255, 0));
	COLORREF clrOfText = ::SetTextColor(_hdc, m_descGraph.m_propLine.rgbColor);
	int oldBkMode = ::SetBkMode(_hdc, TRANSPARENT);
	//// <!-- render -->
	Render_i(_hdc);
	//// <!-- restore colors -->
	::SetBkMode(_hdc, oldBkMode);
	::SetTextColor(_hdc, clrOfText);
	::SetDCBrushColor(_hdc, clrOfBrush);
	::SetDCPenColor(_hdc, clrOfPen);
	return 0;
}

int DTTextGraph::RenderAnchors(HDC _hdc)
{
	//// <!-- prepare colors -->
	COLORREF clrAnchors = m_Graphics->ShadowBackGroupColor();
	COLORREF clrOfBrush = ::SetDCBrushColor(_hdc, clrAnchors);
	COLORREF clrOfPen = ::SetDCPenColor(_hdc, m_Graphics->ShadowColor());
	//// <!-- render -->
	::MoveToEx(_hdc, m_TextRect.left, m_TextRect.top, NULL);
	::LineTo(_hdc, m_TextRect.right, m_TextRect.top);
	::LineTo(_hdc, m_TextRect.right, m_TextRect.bottom);
	::LineTo(_hdc, m_TextRect.left, m_TextRect.bottom);
	::LineTo(_hdc, m_TextRect.left, m_TextRect.top);
	//// <!-- restore colors -->
	::SetDCPenColor(_hdc, clrOfPen);
	::SetDCBrushColor(_hdc, clrOfBrush);
	return 0;
}

int DTTextGraph::RenderShadowAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTTextGraph::RenderEditingAnchors(HDC _hdc)
{
	return 0;
}

int DTTextGraph::RenderSelectedAnchors(HDC _hdc)
{
	return RenderAnchors(_hdc);
}

int DTTextGraph::Build()
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

	return 0;
}

int DTTextGraph::BuildAccordToDescription()
{
	m_ptViewLocationP1.x = m_coorTransform.AbsXtoViewX(m_descGraph.m_ptLocations.ptAbsoluteP1.xDate);
	m_ptViewLocationP1.y = m_coorTransform.AbsYtoViewY(m_descGraph.m_ptLocations.ptAbsoluteP1.yValue);

	if(!CHECK_VIEW_X_AXIS_AVAIABLE(m_ptViewLocationP1.x))
	{
		m_bAvaiable = false;
		return -1;
	}

	HDC _hdc0 = ::GetDC(NULL);
	HDC _memdc = ::CreateCompatibleDC(_hdc0);
	::ReleaseDC(NULL, _hdc0);

	HFONT _TextFont = ::CreateFont(m_descGraph.m_propLine.nWidth, 0,
										0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
										OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, ANTIALIASED_QUALITY,
										VARIABLE_PITCH | FF_DONTCARE, _T("Arial"));
	HFONT oldTextFont = (HFONT) ::SelectObject(_memdc, _TextFont);

	SIZE _TextSize;
	::GetTextExtentPoint32A(_memdc, m_Text.m_Text, ::strlen(m_Text.m_Text), &_TextSize);

	::SelectObject(_memdc, oldTextFont);
	::DeleteObject(_TextFont);
	::DeleteDC(_memdc);

	m_TextRect.left = m_ptViewLocationP1.x;
	m_TextRect.top = m_ptViewLocationP1.y;
	m_TextRect.right = m_TextRect.left + _TextSize.cx;
	m_TextRect.bottom = m_TextRect.top + _TextSize.cy;
	
	m_bAvaiable = true;
	Build();
	return 0;
}

int DTTextGraph::EndMove(long _delta_X, long _delta_Y)
{
	return DTAssistGraph::EndMove();
}

int DTTextGraph::Complete()
{
	m_descGraph.m_ptLocations.ptAbsoluteP1.xDate = m_coorTransform.ViewXtoAbsX2(m_ptViewLocationP1.x);
	m_descGraph.m_ptLocations.ptAbsoluteP1.yValue = m_coorTransform.ViewYtoAbsY(m_ptViewLocationP1.y);

	return BuildAccordToDescription();
}

int DTTextGraph::Clone(DTAssistGraph& _Other)
{
	if(Type() != _Other.Type())
		return -1;
	DTTextGraph& _Clone = reinterpret_cast<DTTextGraph&>(_Other);
	m_Text = _Clone.m_Text;
	m_TextRect = _Clone.m_TextRect;
	return DTAssistGraph::Clone(_Other);
}

int DTTextGraph::CopyText(DTAssistGraphPersistText& _Text)
{
	m_Text.CopyText(_Text);
	return ::strlen(m_Text.m_Text);
}

int DTTextGraph::CopyText(char* _Text)
{
	m_Text.CopyText(_Text);
	return ::strlen(m_Text.m_Text);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --->
// 文本 Text
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////