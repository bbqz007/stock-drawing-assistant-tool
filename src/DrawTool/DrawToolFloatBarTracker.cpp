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
//#include "DrawToolBasic.h"
//#include "DrawToolBasicUtil.h"
#include "DrawToolFloatBar.h"
#include <string.h>
#include <algorithm>
#include <time.h>
#include <sys/timeb.h>
#include "resource.h"


DTFloatBarGhostMoveTracker::DTFloatBarGhostMoveTracker(HWND _hWnd, POINT& _Point, RECT& _RectGhost)
: m_hWndTracker(_hWnd)
{
	m_hdc = ::GetDCEx(NULL, NULL, DCX_CACHE);//::GetWindowDC(NULL);
	m_rcGhost = _RectGhost;
	m_Offset.cx = m_rcGhost.left - _Point.x;
	m_Offset.cy = m_rcGhost.top - _Point.y;
	m_Size.cx = m_rcGhost.right - m_rcGhost.left;
	m_Size.cy = m_rcGhost.bottom - m_rcGhost.top;

	m_hBrushHalftone = NULL;
	m_hBmpGray = NULL;

	CreateHalftoneBrush();
}

DTFloatBarGhostMoveTracker::~DTFloatBarGhostMoveTracker()
{
	if(m_hdc)
		::ReleaseDC(NULL, m_hdc);
	if(m_hBrushHalftone)
		::DeleteObject(m_hBrushHalftone);
	if(m_hBmpGray)
		::DeleteObject(m_hBmpGray);
	m_hdc = NULL;
	m_hBrushHalftone = NULL;
	m_hBmpGray = NULL;
}	

void DTFloatBarGhostMoveTracker::CreateHalftoneBrush()
{
	WORD grayPattern[8];
	for(int i = 0; i < 8; i++)
		grayPattern[i] = (WORD)(0x5555 << (i & 1));
	m_hBmpGray = ::CreateBitmap(8, 8, 1, 1, &grayPattern);
	m_hBrushHalftone = ::CreatePatternBrush(m_hBmpGray);
}

void DTFloatBarGhostMoveTracker::BeginDrag()
{
	DrawGhostRect(m_hdc, m_rcGhost);
}

void DTFloatBarGhostMoveTracker::EndDrag(bool _Canceled)
{
	CleanGhostRect(m_hdc, m_rcGhost);
}

void DTFloatBarGhostMoveTracker::OnMove(long _x, long _y)
{	
	CleanGhostRect(m_hdc, m_rcGhost);
	m_rcGhost.left = _x + m_Offset.cx;
	m_rcGhost.top = _y + m_Offset.cy;
	::ClientToScreen(m_hWndTracker, (POINT*) &m_rcGhost);
	m_rcGhost.right = m_rcGhost.left + m_Size.cx;
	m_rcGhost.bottom = m_rcGhost.top + m_Size.cy;
	
	DrawGhostRect(m_hdc, m_rcGhost);
}

void DTFloatBarGhostMoveTracker::OnCancelDrag(long _x, long _y)
{
	
}

bool DTFloatBarGhostMoveTracker::OnDrop(long _x, long _y)
{
	return true;
}

bool DTFloatBarGhostMoveTracker::OnDropRightButton(long _x, long _y)
{
	return OnDrop(_x, _y);
}

bool DTFloatBarGhostMoveTracker::OnDropLeftButton(long _x, long _y)
{
	return OnDrop(_x, _y);
}

bool DTFloatBarGhostMoveTracker::ProcessWindowMessage(MSG* _Msg)
{
	bool bHandled = false;
	switch(_Msg->message)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		if(_Msg->wParam == VK_CONTROL)
		{
			POINT _Point = { _Msg->pt.x, _Msg->pt.y };
			::ScreenToClient(m_hWndTracker, &_Point);
			OnMove(_Point.x, _Point.y);
			bHandled = true;
		}
	}
	return bHandled;
}

void DTFloatBarGhostMoveTracker::DrawGhostRect(HDC _hdc, RECT& _Rect)
{
	if(m_hBrushHalftone)
	{
		HBRUSH hBrushOld = (HBRUSH) ::SelectObject(m_hdc, m_hBrushHalftone);

		::PatBlt(_hdc, _Rect.left, _Rect.top, 
					_Rect.right - _Rect.left, ENUM_GHOST_RECT_SIDE_SIZE, PATINVERT);
		::PatBlt(_hdc, _Rect.left, _Rect.bottom - ENUM_GHOST_RECT_SIDE_SIZE, 
					_Rect.right - _Rect.left, ENUM_GHOST_RECT_SIDE_SIZE, PATINVERT);

		::PatBlt(_hdc, _Rect.left, _Rect.top + ENUM_GHOST_RECT_SIDE_SIZE, 
					ENUM_GHOST_RECT_SIDE_SIZE, _Rect.bottom - _Rect.top - 2 * ENUM_GHOST_RECT_SIDE_SIZE, PATINVERT);
		::PatBlt(_hdc, _Rect.right - ENUM_GHOST_RECT_SIDE_SIZE, _Rect.top + ENUM_GHOST_RECT_SIDE_SIZE, 
					ENUM_GHOST_RECT_SIDE_SIZE, _Rect.bottom - _Rect.top - 2 * ENUM_GHOST_RECT_SIDE_SIZE, PATINVERT);

		::SelectObject(m_hdc, hBrushOld);
	}
}

void DTFloatBarGhostMoveTracker::CleanGhostRect(HDC _hdc, RECT& _Rect)
{
	DrawGhostRect(_hdc, _Rect);
}

void DTFloatBarGhostMoveTracker::AdjustCursor(POINT& _Point)
{
	_Point.x += m_Offset.cx;
	_Point.y += m_Offset.cy;
}

bool DTFloatBarGhostMoveTracker::TrackDragAndDrop(HWND _hWnd)
{
	bool bResult = true;
	BeginDrag();
	::SetCapture(_hWnd);
	MSG msg;
	while((::GetCapture() == _hWnd) 
			&& (GetMessage(&msg, NULL, 0, 0)))
	{
		if(msg.hwnd != _hWnd)
			continue;

		long _x = (long) (short) LOWORD(msg.lParam);
		long _y = (long) (short) HIWORD(msg.lParam);

		if(!ProcessWindowMessage(&msg))
		{
			switch(msg.message)
			{
			case WM_MOUSEMOVE:
				OnMove(_x, _y);
				break;
			case WM_RBUTTONUP:
				::ReleaseCapture();
				bResult = OnDropRightButton(_x, _y);
				break;
			case WM_LBUTTONUP:
				::ReleaseCapture();
				bResult = OnDropLeftButton(_x, _y);
				break;
			case WM_KEYDOWN:
				if(msg.wParam != VK_ESCAPE)
					break;
			case WM_RBUTTONDOWN:
			case WM_LBUTTONDOWN:
				::ReleaseCapture();
				OnCancelDrag(_x, _y);
				bResult = false;
				break;
			case WM_SYSKEYDOWN:
				::ReleaseCapture();
				OnCancelDrag(_x, _y);
				bResult = false;
			default:
				::DispatchMessage(&msg);
			}
		}
	}

	EndDrag(!bResult);
	return bResult;
}
