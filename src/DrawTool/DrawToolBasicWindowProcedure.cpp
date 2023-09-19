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
#include "DrawToolBasic.h"
#include "DrawToolBasicUtil.h"
#include "DrawToolTextInput.h"
#include "DrawToolPropertyInput.h"


LRESULT DTAssistGraphics::OnPaint(HDC _hdc, BOOL& bHandleForward)
{
	bHandleForward = FALSE;
	//::OutputDebugStringA("DTAssistGraphics::OnPaint()");
	std::list<DTAssistGraph*>::iterator it = m_AssistGraph.begin();
	std::list<DTAssistGraph*>::iterator ite = m_AssistGraph.end();

	ForgetFullViewBitmap();
#if 0
	HDC _MemDC = ::CreateCompatibleDC(_hdc);
	m_FullViewBitmap = ::CreateCompatibleBitmap(_hdc, m_FullViewRect.right - m_FullViewRect.left, m_FullViewRect.bottom - m_FullViewRect.top);
	HBITMAP oldBitmap = (HBITMAP) ::SelectObject(_MemDC, m_FullViewBitmap);
	::BitBlt(_MemDC, 0, 0, m_FullViewRect.right - m_FullViewRect.left, m_FullViewRect.bottom - m_FullViewRect.top, _hdc, 0, 0, SRCCOPY);
#else
	HDC _MemDC = _hdc;
#endif

	int nSaveDC = ::SaveDC(_MemDC);
	HPEN oldPen = (HPEN) ::SelectObject(_MemDC, ::GetStockObject(DC_PEN));
	HBRUSH oldBrush = (HBRUSH) ::SelectObject(_MemDC, ::GetStockObject(DC_BRUSH));
	::SelectObject(_MemDC, (m_LabelFont) ? m_LabelFont : (::GetStockObject(ANSI_VAR_FONT)));
	::SetTextColor(_MemDC, m_FgColor);
	::SetBkColor(_MemDC, m_BgColor);
	::SetDCPenColor(_MemDC, m_FgColor);
	::SetDCBrushColor(_MemDC, m_BgColor);
	::SetROP2(_MemDC, R2_COPYPEN);

	for(; it != ite; ++it)
	{
		if(*it == NULL)
			continue;

        //::OutputDebugStringA("DTAssistGraphics::OnPaint(), a graphic");
		DTAssistGraph* _AssistGraph = *it;
		if(_AssistGraph->IsAvaiable())
		{
			tagDTRect _ClipRect = _AssistGraph->ViewRect();
			HRGN _Rgn = ::CreateRectRgn(_ClipRect.left, _ClipRect.top, _ClipRect.right, _ClipRect.bottom);
			if(_Rgn)
			{
				::SelectClipRgn(_MemDC, _Rgn);
				_AssistGraph->Render(_MemDC);
				::SelectClipRgn(_MemDC, NULL);
				::DeleteObject(_Rgn);
			}
		}
	}

	::SelectObject(_MemDC, oldBrush);
	::SelectObject(_MemDC, oldPen);
#if 0
	::BitBlt(_hdc, 0, 0, m_FullViewRect.right - m_FullViewRect.left, m_FullViewRect.bottom - m_FullViewRect.top, _MemDC, 0, 0, SRCCOPY);
#endif
	::RestoreDC(_MemDC, nSaveDC);
#if 0
	::DeleteDC(_MemDC);
#else
	_MemDC = NULL;
#endif
	return 0;
}

LRESULT DTAssistGraphics::OnDynamicLayerPaint(HDC _hdc, BOOL& bHandleForward)
{
	int nSaveDC = ::SaveDC(_hdc);
	HPEN oldPen = (HPEN) ::SelectObject(_hdc, ::GetStockObject(DC_PEN));
	HBRUSH oldBrush = (HBRUSH) ::SelectObject(_hdc, ::GetStockObject(DC_BRUSH));
	::SelectObject(_hdc, ::GetStockObject(DEFAULT_GUI_FONT));
	::SetTextColor(_hdc, m_FgColor);
	::SetBkColor(_hdc, m_BgColor);
	::SetDCPenColor(_hdc, m_FgColor);
	::SetDCBrushColor(_hdc, m_BgColor);
	::SetROP2(_hdc, R2_COPYPEN);

	//::OutputDebugStringA("OnDynamicLayerPaint");
	if(ENUM_ASSIST_PRE_EDIT_MODE_LINE_P2 == m_CurMode)
	{
		tagDTRect _ClipRect = m_SpotViewCoord.ViewRect();
		HRGN _Rgn = ::CreateRectRgn(_ClipRect.left, _ClipRect.top, _ClipRect.right, _ClipRect.bottom);
		if(_Rgn)
		{
			::SelectClipRgn(_hdc, _Rgn);
			::MoveToEx(_hdc, m_CreationP1.x, m_CreationP1.y, NULL);
			::LineTo(_hdc, m_CreationP2.x, m_CreationP2.y);
			::SelectClipRgn(_hdc, NULL);
			::DeleteObject(_Rgn);
		}
		//::OutputDebugStringA("OnDynamicLayerPaint, ENUM_ASSIST_PRE_EDIT_MODE_LINE_P2");
	}
	else
	{
		if(m_ShadowGraph && m_ShadowGraph->IsAvaiable())
		{
			tagDTRect _ClipRect = m_ShadowGraph->ViewRect();
			HRGN _Rgn = ::CreateRectRgn(_ClipRect.left, _ClipRect.top, _ClipRect.right, _ClipRect.bottom);
			if(_Rgn)
			{
				::SelectClipRgn(_hdc, _Rgn);
				if(!m_ActiveGraph)
				{
					m_ShadowGraph->RenderSelectedAnchors(_hdc);
				}
				else
				{
					m_ShadowGraph->RenderShadow(_hdc);
					m_ShadowGraph->RenderShadowAnchors(_hdc);
				}
				::SelectClipRgn(_hdc, NULL);
				::DeleteObject(_Rgn);
			}
		}
		if(m_ActiveGraph && m_ActiveGraph->IsAvaiable())
		{
			tagDTRect _ClipRect = m_ActiveGraph->ViewRect();
			HRGN _Rgn = ::CreateRectRgn(_ClipRect.left, _ClipRect.top, _ClipRect.right, _ClipRect.bottom);
			if(_Rgn)
			{
				::SelectClipRgn(_hdc, _Rgn);
				m_ActiveGraph->RenderEditing(_hdc);
				m_ActiveGraph->RenderEditingAnchors(_hdc);
				::SelectClipRgn(_hdc, NULL);
				::DeleteObject(_Rgn);
			}
		}
		//::OutputDebugStringA("OnDynamicLayerPaint, else");
	}

	::SelectObject(_hdc, oldBrush);
	::SelectObject(_hdc, oldPen);
	::RestoreDC(_hdc, nSaveDC);
	return 0;
}

LRESULT DTAssistGraphics::OnSelectedPaint(HDC _hdc)
{
	//if(!m_FullViewBitmap)
	//	return -1;
	bool bNeedRenderAnchors = m_ShadowGraph && m_ShadowGraph->IsAvaiable() && m_ShadowGraph->Description().m_Identity == m_ActiveGraphId;

	//HBRUSH _Brush = ::CreatePatternBrush(m_FullViewBitmap);
	int nSaveDC = ::SaveDC(_hdc);
	HPEN oldPen = (HPEN) ::SelectObject(_hdc, ::GetStockObject(DC_PEN));
	HBRUSH oldBrush = (HBRUSH) ::SelectObject(_hdc, ::GetStockObject(DC_BRUSH));
	::SetDCPenColor(_hdc, m_FgColor);
	::SetROP2(_hdc, R2_COPYPEN);
	if(bNeedRenderAnchors)
	{
		// ::OutputDebugStringA("DTAssistGraphics::OnSelectedPaint(), bNeedRenderAnchors");
		//::FillRect(_hdc, &m_ShadowGraph->ViewRect(), _Brush);
		m_ShadowGraph->RenderSelectedAnchors(_hdc);
	}
	else
	{
		// ::OutputDebugStringA("DTAssistGraphics::OnSelectedPaint(), NOT bNeedRenderAnchors");
		//::FillRect(_hdc, &m_RecoverShadowRect, _Brush);
	}
	::RestoreDC(_hdc, nSaveDC);
	//::DeleteObject(_Brush);
	return 0;
}

LRESULT DTAssistGraphics::OnHitTest(DWORD _Period, DWORD _ViewId, tagDTPoint& _Point, BOOL& bHandleForward)
{
	bHandleForward = FALSE;
	LRESULT& _HitTest = m_LastHitTest;
	_HitTest = ENUM_ASSIST_HITTEST_NULL;
	long _xDate = m_AbsCoord.ViewXtoAbsX(_Point.x);
	m_HitTestId.reset();

	if(m_ShadowGraph && m_ShadowGraph->IsAvaiable()
			&& m_ShadowGraph->Description().m_Identity.nPeriod == _Period
			&& m_ShadowGraph->Description().m_Identity.nViewId == _ViewId)
	{
		_HitTest = m_ShadowGraph->HitTest(_Point, _xDate);
		if(_HitTest != ENUM_ASSIST_HITTEST_NULL)
		{
			m_HitTestId = m_ShadowGraph->Description().m_Identity;
		}
	}

	std::list<DTAssistGraph*>::iterator it = m_AssistGraph.begin();
	std::list<DTAssistGraph*>::iterator ite = m_AssistGraph.end();
	for(; it != ite && _HitTest == ENUM_ASSIST_HITTEST_NULL; ++it)
	{
		if(*it == NULL)
			continue;

		DTAssistGraph* _AssistGraph = *it;
		if(_AssistGraph->IsAvaiable()
			&& _AssistGraph->Description().m_Identity.nPeriod == _Period
			&& _AssistGraph->Description().m_Identity.nViewId == _ViewId)
		{
			_HitTest = _AssistGraph->HitTest(_Point, _xDate);
			if(_HitTest != ENUM_ASSIST_HITTEST_NULL)
			{
				m_HitTestId = _AssistGraph->Description().m_Identity;
				break;
			}
		}
	}

	return _HitTest;
}

LRESULT DTAssistGraphics::OnLButtonDown(UINT _Flags, DWORD _Period, tagDTPoint _Point, BOOL bShiftPressed, BOOL bCtrlPressed, BOOL& bHandleForward)
{
	bHandleForward = FALSE;
	LRESULT _Res = m_CurMode;
	LRESULT _HitTest = ENUM_ASSIST_HITTEST_NULL;
	long _ViewId = HitTestViewId(_Point);
	////////////////////////////////////////////////////////////////////////
	//// <!--- Begin editing
	////////////////////////////////////////////////////////////////////////
	if(m_CurMode == ENUM_ASSIST_EDIT_MODE_NULL)
	{
		_HitTest = OnHitTest(_Period, _ViewId, _Point, bHandleForward);
		if(_HitTest != ENUM_ASSIST_HITTEST_NULL)
		{
			std::list<DTAssistGraph*>::iterator it = m_AssistGraph.begin();
			std::list<DTAssistGraph*>::iterator ite = m_AssistGraph.end();
			for(; it != ite; ++it)
			{
				if(*it == NULL)
					continue;
				DTAssistGraph* _AssistGraph = *it;
				if(_AssistGraph->IsAvaiable() && _AssistGraph->Description().m_Identity == m_HitTestId)
				{
					m_SpotPoint = _Point;
					m_ActiveGraphId = m_HitTestId;
					//m_AssistGraph.erase(it);
					if(m_ShadowGraph)
					{
						m_RecoverShadowRect = m_ShadowGraph->ViewRect();
						m_DiscardAssistGraph.push_back(m_ShadowGraph);
					}
					m_ShadowGraph = CreateAssistGraphInstance(_AssistGraph);
					break;
				}
			}

			if(m_ShadowGraph)
			{
				switch(_HitTest)
				{
				case ENUM_ASSIST_HITTEST_LOCATION_P1:
					PushEditMode(ENUM_ASSIST_EDIT_MODE_BEGINNING_P1); break;
				case ENUM_ASSIST_HITTEST_LOCATION_P2:
					PushEditMode(ENUM_ASSIST_EDIT_MODE_BEGINNING_P2); break;
				case ENUM_ASSIST_HITTEST_LOCATION_P3:
					PushEditMode(ENUM_ASSIST_EDIT_MODE_BEGINNING_P3); break;
				case ENUM_ASSIST_HITTEST_TARGET:
					PushEditMode(ENUM_ASSIST_EDIT_MODE_BEGINNING_OFFSET); break;
				}
				m_SpotPoint = _Point;
				std::map<DWORD, DTCoordinateTransform*>::iterator it = m_ViewCoord.find(_ViewId);
				std::map<DWORD, DTCoordinateTransform*>::iterator ite = m_ViewCoord.end();
				if(it != ite && it->second)
					m_SpotViewCoord = *it->second;
			}
			_Res = 1;
		}
		else
		{
			m_ActiveGraphId.reset();
			m_CreateGraphId.reset();
			if(m_ShadowGraph)
			{
				m_RecoverShadowRect = m_ShadowGraph->ViewRect();
				//m_AssistGraph.push_back(m_ShadowGraph);
				m_DiscardAssistGraph.push_back(m_ShadowGraph);
				_Res = 1;
			}
			m_ShadowGraph = NULL;
		}
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- editing or moving, need to commit.
	////////////////////////////////////////////////////////////////////////
	else if(ENUM_ASSIST_EDIT_MODE_OFFSET == m_CurMode
			|| ENUM_ASSIST_EDIT_MODE_EDIT_P1 == m_CurMode
			|| ENUM_ASSIST_EDIT_MODE_EDIT_P2 == m_CurMode
			|| ENUM_ASSIST_EDIT_MODE_EDIT_P3 == m_CurMode)
	{
		if(m_ActiveGraph && m_ActiveGraph->IsAvaiable())
		{
			PushEditMode((ENUM_ASSIST_EDIT_MODE)m_ActiveGraph->NextEditMode(m_CurMode));
			if(ENUM_ASSIST_EDIT_MODE_COMMIT == m_CurMode)
			{
				if(m_CommitGraph)
				{
					m_DiscardAssistGraph.push_back(m_CommitGraph);
					m_CommitGraph = NULL;
				}
				m_CommitGraph = m_ActiveGraph;
				m_ActiveGraph = NULL;
				m_CommitGraph->Complete();
			}
		}
		else
		{
			ClearEditMode();
			//SetCursor(_HitTest);
		}
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- create.
	////////////////////////////////////////////////////////////////////////
	else if(ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING == m_CurMode)
	{
		if(m_ActiveGraph)
			m_DiscardAssistGraph.push_back(m_ActiveGraph);
		if(m_ShadowGraph)
			m_DiscardAssistGraph.push_back(m_ShadowGraph);
		if(m_NewGraph)
			m_DiscardAssistGraph.push_back(m_NewGraph);
		m_ActiveGraph = NULL;
		m_ShadowGraph = NULL;
		m_LastHitTest = ENUM_ASSIST_HITTEST_NULL;
		m_HitTestId.reset();
		m_ActiveGraphId.reset();
		m_CreateGraphId.reset();

		PushEditMode(NextEditMode(m_ToolSelected, m_CurMode));

		if(ENUM_ASSIST_PRE_EDIT_MODE_SET_P1 == m_CurMode)
		{
			m_CreationP1 = _Point;
			m_CreationP2 = _Point;
			PushEditMode(NextEditMode(m_ToolSelected, m_CurMode));

			m_SpotPoint = _Point;
			std::map<DWORD, DTCoordinateTransform*>::iterator it = m_ViewCoord.find(_ViewId);
			std::map<DWORD, DTCoordinateTransform*>::iterator ite = m_ViewCoord.end();
			if(it != ite && it->second)
				m_SpotViewCoord = *it->second;
		}

		if(ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P1_P1 == m_CurMode)
		{
			DTAssistGraphDescription _NewDescription;
			_NewDescription.m_Identity.upgrade(m_StkCode, _Period, _ViewId, m_ToolSelected);
			_NewDescription.m_propLine.flagStyle = 0;
			_NewDescription.m_propLine.nWidth = 1;
			_NewDescription.m_propLine.rgbColor = 0;
			_NewDescription.m_propLine.nMode = 0;

			DTTextInputBox _InputBox;
			LRESULT _Res_DoModal = 0;
			if(m_ToolSelected == ENUM_ASSIST_GRAPH_TYPE_Text)
			{
				_Res_DoModal = _InputBox.DoModal();
				//// <!!!! NODE !!!!> the hook of keyboard will causes the repaint window procedure, even if now we are doing modal dialog.
				m_ToolSelected = ENUM_ASSIST_GRAPH_TYPE_Text;
				PushEditMode(ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P1_P1);
			}

			m_ActiveGraph = CreateAssistGraphInstance(m_ToolSelected, _NewDescription);

			//// <!--- Text ---->
			if(m_ToolSelected == ENUM_ASSIST_GRAPH_TYPE_Text
				&& _Res_DoModal == IDOK)
			{
				m_ActiveGraph->Description().m_propLine.nWidth = _InputBox.RetrieveFontSize();
				m_ActiveGraph->Description().m_propLine.rgbColor = _InputBox.RetrieveColor();
				//// <!--- NODE ----> Text must be assigned before Graph building.
				m_ActiveGraph->CopyText(_InputBox.RetrieveText());
			}

			m_ActiveGraph->BuildAccordToViewPoint(&_Point, &_Point, &_Point);
			m_ShadowGraph = CreateAssistGraphInstance(m_ActiveGraph);

			PushEditMode((ENUM_ASSIST_EDIT_MODE) m_ActiveGraph->NextEditMode(m_CurMode));
			m_ToolSelected = ENUM_ASSIST_GRAPH_TYPE_NULL;

			switch(m_CurMode)
			{
			case ENUM_ASSIST_EDIT_MODE_OFFSET:
				m_LastHitTest = ENUM_ASSIST_HITTEST_TARGET; break;
			case ENUM_ASSIST_EDIT_MODE_EDIT_P1:
				m_LastHitTest = ENUM_ASSIST_HITTEST_LOCATION_P1; break;
			case ENUM_ASSIST_EDIT_MODE_EDIT_P2:
				m_LastHitTest = ENUM_ASSIST_HITTEST_LOCATION_P2; break;
			case ENUM_ASSIST_EDIT_MODE_EDIT_P3:
				m_LastHitTest = ENUM_ASSIST_HITTEST_LOCATION_P3; break;
			}

			if(ENUM_ASSIST_EDIT_MODE_COMMIT == m_CurMode)
			{
				if(m_CommitGraph)
				{
					m_DiscardAssistGraph.push_back(m_CommitGraph);
					m_CommitGraph = NULL;
				}
				m_CommitGraph = m_ActiveGraph;
				m_ActiveGraph = NULL;
				m_CommitGraph->Complete();
			}
			else
			{
				m_HitTestId = m_ActiveGraph->Description().m_Identity;
				m_ActiveGraphId = m_HitTestId;
				m_CreateGraphId = m_HitTestId;

				m_SpotPoint = _Point;
				std::map<DWORD, DTCoordinateTransform*>::iterator it = m_ViewCoord.find(_ViewId);
				std::map<DWORD, DTCoordinateTransform*>::iterator ite = m_ViewCoord.end();
				if(it != ite && it->second)
					m_SpotViewCoord = *it->second;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- igore creating.
	////////////////////////////////////////////////////////////////////////
	else if(ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING < m_CurMode && m_CurMode < ENUM_ASSIST_PRE_EDIT_MODE_END)
	{
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- unexpected
	////////////////////////////////////////////////////////////////////////
	else
	{
	    m_CurMode = ENUM_ASSIST_EDIT_MODE_UNEXPECTED;
		ClearEditMode();
		::SetCursor(m_hcrsrNormal);
	}

	return m_CurMode;
}

LRESULT DTAssistGraphics::OnLButtonUp(UINT _Flags, DWORD _Period, tagDTPoint _Point, BOOL bShiftPressed, BOOL bCtrlPressed, BOOL& bHandleForward)
{
	bHandleForward = FALSE;
	LRESULT _Res = 0;
	long _ViewId = HitTestViewId(_Point);
	//LRESULT _HitTest = OnHitTest(_Period, _ViewId, _Point, bHandleForward);

	////////////////////////////////////////////////////////////////////////
	//// <!--- just click left button (down, no move and up) to select a graph, not be able to edit it.
	////////////////////////////////////////////////////////////////////////
	if(ENUM_ASSIST_EDIT_MODE_TEND_BEGIN < m_CurMode && m_CurMode < ENUM_ASSIST_EDIT_MODE_BEGINNING)
	{
		ClearEditMode();
		//SetCursor(_HitTest);
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- editing or moving, need to commit.
	////////////////////////////////////////////////////////////////////////
	else if(ENUM_ASSIST_EDIT_MODE_OFFSET == m_CurMode
			|| ENUM_ASSIST_EDIT_MODE_EDIT_P1 == m_CurMode
			|| ENUM_ASSIST_EDIT_MODE_EDIT_P2 == m_CurMode
			|| ENUM_ASSIST_EDIT_MODE_EDIT_P3 == m_CurMode)
	{
		if(m_ActiveGraph && m_ActiveGraph->IsAvaiable())
		{
			PushEditMode((ENUM_ASSIST_EDIT_MODE)m_ActiveGraph->NextEditMode(m_CurMode));
			if(ENUM_ASSIST_EDIT_MODE_COMMIT == m_CurMode)
			{
				if(m_CommitGraph)
				{
					m_DiscardAssistGraph.push_back(m_CommitGraph);
					m_CommitGraph = NULL;
				}
				m_CommitGraph = m_ActiveGraph;
				m_ActiveGraph = NULL;
				m_CommitGraph->Complete();
			}
		}
		else
		{
			ClearEditMode();
			//SetCursor(_HitTest);
		}
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- create.
	////////////////////////////////////////////////////////////////////////
	else if(ENUM_ASSIST_PRE_EDIT_MODE_LINE_P2 == m_CurMode)
	{
		m_CreationP2 = _Point;
		PushEditMode((ENUM_ASSIST_EDIT_MODE) NextEditMode(m_ToolSelected, m_CurMode));
		if(ENUM_ASSIST_PRE_EDIT_MODE_BUILD_P1_P2_P2 == m_CurMode)
		{
			if(m_ActiveGraph)
				m_DiscardAssistGraph.push_back(m_ActiveGraph);
			if(m_ShadowGraph)
				m_DiscardAssistGraph.push_back(m_ShadowGraph);
			if(m_NewGraph)
				m_DiscardAssistGraph.push_back(m_NewGraph);

			DTAssistGraphDescription _NewDescription;
			_NewDescription.m_Identity.upgrade(m_StkCode, _Period, _ViewId, m_ToolSelected);
			_NewDescription.m_propLine.flagStyle = 0;
			_NewDescription.m_propLine.nWidth = 1;
			_NewDescription.m_propLine.rgbColor = 0;
			_NewDescription.m_propLine.nMode = 0;

			m_ActiveGraph = CreateAssistGraphInstance(m_ToolSelected, _NewDescription);
			m_ActiveGraph->BuildAccordToViewPoint(&m_CreationP1, &m_CreationP2, &m_CreationP2);
			m_ShadowGraph = CreateAssistGraphInstance(m_ActiveGraph);
			PushEditMode((ENUM_ASSIST_EDIT_MODE) m_ActiveGraph->NextEditMode(m_CurMode));
			m_ToolSelected = ENUM_ASSIST_GRAPH_TYPE_NULL;

			switch(m_CurMode)
			{
			case ENUM_ASSIST_EDIT_MODE_OFFSET:
				m_LastHitTest = ENUM_ASSIST_HITTEST_TARGET; break;
			case ENUM_ASSIST_EDIT_MODE_EDIT_P1:
				m_LastHitTest = ENUM_ASSIST_HITTEST_LOCATION_P1; break;
			case ENUM_ASSIST_EDIT_MODE_EDIT_P2:
				m_LastHitTest = ENUM_ASSIST_HITTEST_LOCATION_P2; break;
			case ENUM_ASSIST_EDIT_MODE_EDIT_P3:
				m_LastHitTest = ENUM_ASSIST_HITTEST_LOCATION_P3; break;
			}

			m_HitTestId = m_ActiveGraph->Description().m_Identity;
			m_ActiveGraphId = m_HitTestId;
			m_CreateGraphId = m_HitTestId;

			m_SpotPoint = _Point;
			std::map<DWORD, DTCoordinateTransform*>::iterator it = m_ViewCoord.find(_ViewId);
			std::map<DWORD, DTCoordinateTransform*>::iterator ite = m_ViewCoord.end();
			if(it != ite && it->second)
				m_SpotViewCoord = *it->second;
		}
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- unexpected
	////////////////////////////////////////////////////////////////////////
	else
	{
		ClearEditMode();
		//SetCursor(_HitTest);
	}
	return m_CurMode;
}

LRESULT DTAssistGraphics::OnLButtonDblClk(UINT nFlags, DWORD _Period, tagDTPoint _Point, BOOL bShiftPressed, BOOL bCtrlPressed, BOOL& bHandleForward)
{
	bHandleForward = FALSE;
	if(!m_ActiveGraph && m_ShadowGraph)
	{
		long _ViewId = HitTestViewId(_Point);
		LRESULT _HitTest = OnHitTest(_Period, _ViewId, _Point, bHandleForward);
		if(m_ShadowGraph->Description().m_Identity != m_HitTestId)
		{
			CancelSelected();
		}
	}
	return 0;
}

LRESULT DTAssistGraphics::OnMouseMove(UINT nFlags, DWORD _Period, tagDTPoint _Point, BOOL& bHandleForward)
{
	bHandleForward = FALSE;
	LRESULT _HitTest = ENUM_ASSIST_HITTEST_NULL;
	long _ViewId = HitTestViewId(_Point);
	_HitTest = OnHitTest(_Period, _ViewId, _Point, bHandleForward);

	////////////////////////////////////////////////////////////////////////
	//// <!--- offset or editing happen
	////////////////////////////////////////////////////////////////////////
	switch(m_CurMode)
	{
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_OFFSET:
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_P1:
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_P2:
	case ENUM_ASSIST_EDIT_MODE_BEGINNING_P3:
		if(!m_ShadowGraph)
		{
			ClearEditMode();
			::SetCursor(m_hcrsrNormal);
		}
		else if(m_ActiveGraph)
		{
			ReleaseAssistGraphInstance(m_ActiveGraph);
			m_ActiveGraph = NULL;
		}

		if(m_ShadowGraph && !m_ActiveGraph)
		{
			m_ActiveGraph = CreateAssistGraphInstance(m_ShadowGraph);
			PushEditMode((ENUM_ASSIST_EDIT_MODE) m_ShadowGraph->NextEditMode(m_CurMode));
			::SetCursor(m_hcrsrMove);
		}
		return m_CurMode;
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- while offseting
	////////////////////////////////////////////////////////////////////////
	if(m_CurMode == ENUM_ASSIST_EDIT_MODE_OFFSET)
	{
		if(!m_ShadowGraph)
		{
			ClearEditMode();
			::SetCursor(m_hcrsrNormal);
		}
		else if(m_ActiveGraph)
		{
			m_ActiveGraph->Clone(*m_ShadowGraph);
			m_ActiveGraph->MoveTo(m_SpotPoint, _Point);
			::SetCursor(m_hcrsrMove);
		}
		else if(!m_ActiveGraph)
		{
			ClearEditMode();
			::SetCursor(m_hcrsrNormal);
		}
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- while editing P1
	////////////////////////////////////////////////////////////////////////
	else if(m_CurMode == ENUM_ASSIST_EDIT_MODE_EDIT_P1)
	{
		if(!m_ShadowGraph)
		{
			ClearEditMode();
			::SetCursor(m_hcrsrNormal);
		}
		else if(m_ActiveGraph)
		{
			m_ActiveGraph->Clone(*m_ShadowGraph);
			m_ActiveGraph->MoveP1To(_Point);
			::SetCursor(m_hcrsrAnchors);
		}
		else if(!m_ActiveGraph)
		{
			ClearEditMode();
			::SetCursor(m_hcrsrNormal);
		}
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- while editing P2
	////////////////////////////////////////////////////////////////////////
	else if(m_CurMode == ENUM_ASSIST_EDIT_MODE_EDIT_P2)
	{
		if(!m_ShadowGraph)
		{
			ClearEditMode();
			::SetCursor(m_hcrsrNormal);
		}
		else if(m_ActiveGraph)
		{
			m_ActiveGraph->Clone(*m_ShadowGraph);
			m_ActiveGraph->MoveP2To(_Point);
			::SetCursor(m_hcrsrAnchors);
		}
		else if(!m_ActiveGraph)
		{
			ClearEditMode();
			::SetCursor(m_hcrsrNormal);
		}
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- while editing P3
	////////////////////////////////////////////////////////////////////////
	else if(m_CurMode == ENUM_ASSIST_EDIT_MODE_EDIT_P3)
	{
		if(!m_ShadowGraph)
		{
			ClearEditMode();
			::SetCursor(m_hcrsrNormal);
		}
		else if(m_ActiveGraph)
		{
			m_ActiveGraph->Clone(*m_ShadowGraph);
			m_ActiveGraph->MoveP3To(_Point);
			::SetCursor(m_hcrsrAnchors);
		}
		else if(!m_ActiveGraph)
		{
			ClearEditMode();
			::SetCursor(m_hcrsrNormal);
		}
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- create
	////////////////////////////////////////////////////////////////////////
	else if(ENUM_ASSIST_PRE_EDIT_MODE_BEGINNING == m_CurMode)
	{
		::SetCursor(m_hcrsrCreate);
	}
	else if(ENUM_ASSIST_PRE_EDIT_MODE_LINE_P2 == m_CurMode)
	{
		m_CreationP2 = _Point;
		::SetCursor(m_hcrsrCreate);
	}

	////////////////////////////////////////////////////////////////////////
	//// <!--- unexpected
	////////////////////////////////////////////////////////////////////////
	else
	{
		ClearEditMode();
		::SetCursor(m_hcrsrNormal);
	}
	return m_CurMode;
}

LRESULT DTAssistGraphics::OnRButtonDown(UINT _Flags, DWORD _Period, tagDTPoint _Point, BOOL bShiftPressed, BOOL bCtrlPressed, BOOL& bHandleForward)
{
	bHandleForward = FALSE;
	LRESULT _Res = m_CurMode;
	LRESULT _HitTest = ENUM_ASSIST_HITTEST_NULL;
	long _ViewId = HitTestViewId(_Point);
	////////////////////////////////////////////////////////////////////////
	//// <!--- Begin editing
	////////////////////////////////////////////////////////////////////////
	if(m_CurMode == ENUM_ASSIST_EDIT_MODE_NULL)
	{

	}

	return m_CurMode;
}


LRESULT DTAssistGraphics::OnRButtonUp(UINT _Flags, DWORD _Period, tagDTPoint _Point, BOOL bShiftPressed, BOOL bCtrlPressed, BOOL& bHandleForward)
{
	bHandleForward = FALSE;
	LRESULT _Res = m_CurMode;
	LRESULT _HitTest = ENUM_ASSIST_HITTEST_NULL;
	long _ViewId = HitTestViewId(_Point);
	////////////////////////////////////////////////////////////////////////
	//// <!--- Begin editing
	////////////////////////////////////////////////////////////////////////
	if(m_CurMode == ENUM_ASSIST_EDIT_MODE_NULL)
	{
		_HitTest = OnHitTest(_Period, _ViewId, _Point, bHandleForward);
		if(_HitTest != ENUM_ASSIST_HITTEST_NULL)
		{
			std::list<DTAssistGraph*>::iterator it = m_AssistGraph.begin();
			std::list<DTAssistGraph*>::iterator ite = m_AssistGraph.end();
			for(; it != ite; ++it)
			{
				if(*it == NULL)
					continue;
				DTAssistGraph* _AssistGraph = *it;
				if(_AssistGraph->IsAvaiable() && _AssistGraph->Description().m_Identity == m_HitTestId)
				{
					m_SpotPoint = _Point;
					m_ActiveGraphId = m_HitTestId;
					//m_AssistGraph.erase(it);
					if(m_ShadowGraph)
					{
						m_RecoverShadowRect = m_ShadowGraph->ViewRect();
						m_DiscardAssistGraph.push_back(m_ShadowGraph);
					}
					m_ShadowGraph = CreateAssistGraphInstance(_AssistGraph);
					break;
				}
			}

			if(m_ShadowGraph)
			{
				PushEditMode((ENUM_ASSIST_EDIT_MODE) ENUM_ASSIST_EDIT_MODE_POPUPMENU);
				m_SpotPoint = _Point;
				std::map<DWORD, DTCoordinateTransform*>::iterator it = m_ViewCoord.find(_ViewId);
				std::map<DWORD, DTCoordinateTransform*>::iterator ite = m_ViewCoord.end();
				if(it != ite && it->second)
					m_SpotViewCoord = *it->second;
			}
			_Res = 1;
		}
	}

	return m_CurMode;
}

LRESULT DTAssistGraphics::OnSelectedEdit()
{
	if(!m_ShadowGraph)
		return -1;

	LRESULT _Res_DoModal = 0;
	DTPropertyInputBox _dlg;
	DTAssistGraphDescriptionScript _Script = m_ShadowGraph->DescriptionScript();
	DTAssistGraphDescription _Description = m_ShadowGraph->Description();

	_dlg.SetDescriptionScript(_Script);
	_dlg.SetDescription(_Description);
	_dlg.SetText(m_ShadowGraph->Text());

	_Res_DoModal = _dlg.DoModal();

	if(_Res_DoModal != IDOK)
		return -1;

	_Description = _dlg.Description();
	if(_Script.m_Location1)
	{
		m_ShadowGraph->Description().m_ptLocations.ptAbsoluteP1.yValue = _Description.m_ptLocations.ptAbsoluteP1.yValue;
	}
	if(_Script.m_Location2)
	{
		m_ShadowGraph->Description().m_ptLocations.ptAbsoluteP2.yValue = _Description.m_ptLocations.ptAbsoluteP2.yValue;
	}
	if(_Script.m_Location3)
	{
		m_ShadowGraph->Description().m_ptLocations.ptAbsoluteP3.yValue = _Description.m_ptLocations.ptAbsoluteP3.yValue;
	}
	if(_Script.m_Text)
	{
		m_ShadowGraph->CopyText(_dlg.RetrieveText());
	}
	if(_Script.m_Custom)
	{
		m_ShadowGraph->Description().m_propLine = _Description.m_propLine;
	}

	if(m_ActiveGraph)
	{
		m_DiscardAssistGraph.push_back(m_ActiveGraph);
		m_ActiveGraph = NULL;
	}
	if(m_CommitGraph)
	{
		m_DiscardAssistGraph.push_back(m_CommitGraph);
		m_CommitGraph = NULL;
	}
	m_CommitGraph = m_ShadowGraph;
	m_ShadowGraph = NULL;
	m_CommitGraph->BuildAccordToDescription();
	PushEditMode(ENUM_ASSIST_EDIT_MODE_COMMIT);
	return 0;
}
