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

class CCurrentWindowsVersion
{
public:
	CCurrentWindowsVersion()
	{
		version_ = ::GetVersion();
	}
	BOOL IsGreaterThanWin7() const
	{
		WORD ver = LOBYTE(LOWORD(version_));
		ver <<= 8;
		ver |= HIBYTE(LOWORD(version_));
		return ver > 0x601;
	}
	static const CCurrentWindowsVersion& Get()
	{
		static CCurrentWindowsVersion inst;
		return inst;
	}
private:
	DWORD version_;
};

static TCHAR* RetrieveToolTipText(ENUM_FLTBAR_GRAPH_TYPE _ToolType)
{
	TCHAR* _ToolTipText = _T("");

	switch(_ToolType)
	{
	case ENUM_FLTBAR_GRAPH_TYPE_NULL: _ToolTipText = _T("请选定"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Segment: _ToolTipText = _T("线段"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Line: _ToolTipText = _T("直线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Segment_with_Arrow: _ToolTipText = _T("箭头"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Line_Extension: _ToolTipText = _T("射线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Price_Channels: _ToolTipText = _T("价格通道线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Parallel_Lines: _ToolTipText = _T("平行直线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Arc: _ToolTipText = _T("圆弧线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Circle: _ToolTipText = _T("圆"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Circumcircle: _ToolTipText = _T("外接圆"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_GPHs: _ToolTipText = _T("黄金价位线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_GAHs: _ToolTipText = _T("黄金目标线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_GSHs: _ToolTipText = _T("黄金分割"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_PHs: _ToolTipText = _T("百分比线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_BHs: _ToolTipText = _T("波段线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_LRA: _ToolTipText = _T("线形回归带"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_LREA: _ToolTipText = _T("延长线形回归带"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_LRL: _ToolTipText = _T("线形回归线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Cycle: _ToolTipText = _T("周期线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Fibonacci: _ToolTipText = _T("费波拉契线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Gann: _ToolTipText = _T("江恩时间序列"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Gould_Rays: _ToolTipText = _T("阻速线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Gann_Rays: _ToolTipText = _T("江恩角度线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Rectangle: _ToolTipText = _T("矩形"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Red_Up_Arrow: _ToolTipText = _T("涨标记"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Green_Down_Arrow: _ToolTipText = _T("跌标记"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Text: _ToolTipText = _T("文字注释"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Delete: _ToolTipText = _T("删除当前品种的画线"); break;
	case ENUM_FLTBAR_GRAPH_TYPE_Mask: _ToolTipText = _T("隐藏全部自画线"); break;
	}
	return _ToolTipText;
};

ENUM_ASSIST_GRAPH_TYPE DTFloatBar::RetrieveGraphTool(ENUM_FLTBAR_GRAPH_TYPE _ToolType)
{
#define BEGIN_GRAPH_TYPE_PICKER(who)	\
	ENUM_ASSIST_GRAPH_TYPE _TypePicked = ENUM_ASSIST_GRAPH_TYPE_NULL;	\
	switch(who)	\
	{
#define GRAPH_TYPE_PICKER(name)			\
	case ENUM_FLTBAR_GRAPH_TYPE_##name: _TypePicked = ENUM_ASSIST_GRAPH_TYPE_##name; break;
#define GRAPH_NOTYPE_PICKER(name)			\
	case ENUM_FLTBAR_GRAPH_TYPE_##name: _TypePicked = ENUM_ASSIST_GRAPH_TYPE_NULL; break;
#define END_GRAPH_TYPE_PICKER	\
	}	\
	return _TypePicked;

	BEGIN_GRAPH_TYPE_PICKER (_ToolType)
#ifndef PRIVATE_V2
	GRAPH_TYPE_PICKER (Segment)
	GRAPH_TYPE_PICKER (Line)
	GRAPH_TYPE_PICKER (Segment_with_Arrow)
	GRAPH_TYPE_PICKER (Line_Extension)
	GRAPH_TYPE_PICKER (Price_Channels)
	GRAPH_TYPE_PICKER (Parallel_Lines)
	GRAPH_TYPE_PICKER (Arc)
	GRAPH_TYPE_PICKER (Circle)
	GRAPH_TYPE_PICKER (Circumcircle)
	GRAPH_TYPE_PICKER (GPHs)
	GRAPH_TYPE_PICKER (GAHs)
	GRAPH_TYPE_PICKER (GSHs)
	GRAPH_TYPE_PICKER (PHs)
	GRAPH_TYPE_PICKER (BHs)
	GRAPH_TYPE_PICKER (LRA)
	GRAPH_TYPE_PICKER (LREA)
	GRAPH_TYPE_PICKER (LRL)
	GRAPH_TYPE_PICKER (Cycle)
	GRAPH_TYPE_PICKER (Fibonacci)
	GRAPH_TYPE_PICKER (Gann)
	GRAPH_TYPE_PICKER (Gould_Rays)
	GRAPH_TYPE_PICKER (Gann_Rays)
	GRAPH_TYPE_PICKER (Rectangle)
	GRAPH_TYPE_PICKER (Red_Up_Arrow)
	GRAPH_TYPE_PICKER (Green_Down_Arrow)
	GRAPH_TYPE_PICKER (Text)
	GRAPH_NOTYPE_PICKER (Delete)
	GRAPH_NOTYPE_PICKER (Mask)
#endif
	END_GRAPH_TYPE_PICKER
}

ENUM_ASSIST_GRAPH_TYPE DTFloatBar::RetrievePickedGraphTool()
{
	return RetrieveGraphTool((ENUM_FLTBAR_GRAPH_TYPE) m_nIdxPushed);
}

ENUM_ASSIST_GRAPH_TYPE DTFloatBar::PickGraphTool()
{
	ENUM_ASSIST_GRAPH_TYPE _Ret = RetrievePickedGraphTool();
	if(ENUM_FLTBAR_GRAPH_TYPE_Mask != m_nIdxPushed)
	{
		RecoverUnselected();
	}
	return _Ret;
}

BOOL DTFloatBar::RetrieveMaskSignal()
{
	return m_nIdxPushed == ENUM_FLTBAR_GRAPH_TYPE_Mask;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK DTFloatBar::SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	DTFloatBar* _FltBar = (DTFloatBar*) dwRefData;
	LRESULT _LRES = 0;
	_FltBar->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, _LRES);
	return _LRES;
}

DWORD WINAPI DTFloatBar::ThreadProc(LPVOID lpPara)
{
	DTFloatBar* _FltBar = (DTFloatBar*) lpPara;
	_FltBar->Create();
	return _FltBar->MessageLoop();
}

LRESULT CALLBACK DTFloatBar::WindowProcNil(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DTFloatBar::DTFloatBar(HINSTANCE _hInst, HWND _hWndParent)
: m_hInst(_hInst), m_hWndParent(_hWndParent)
{
	m_bExit = false;
	m_hWnd = NULL;
	m_hWndCharChain = NULL;
	m_pfnSubclass = (SUBCLASSPROC) DTFloatBar::SubClassProc;
	m_uIdSubclass = NULL;
	m_dwRefData = (DWORD_PTR) this;

	m_hEventMessageLoopThreadFinished = CreateEvent(NULL, TRUE, TRUE, NULL);
	m_hMessageLoopThread = NULL/*::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) DTFloatBar::ThreadProc, (LPVOID) this, 0, &m_dwThreadId)*/;

	memset(m_hIconButton, 0, sizeof(m_hIconButton));
	memset(m_hIcon, 0, sizeof(m_hIcon));

	m_hImageList = NULL;

	m_nButtonSize = 16;
	m_nButtonSpan = 4;
	m_nNumOfButtonEachRow = 3;

	m_rcFltBar.left = 0;
	m_rcFltBar.top = 0;
	m_rcFltBar.right = m_nNumOfButtonEachRow * (m_nButtonSize + m_nButtonSpan + m_nButtonSpan);
	m_rcFltBar.bottom = (ENUM_FLTBAR_GRAPH_TYPE_TOTAL / m_nNumOfButtonEachRow + ((ENUM_FLTBAR_GRAPH_TYPE_TOTAL % m_nNumOfButtonEachRow) ? 1 : 0)) * (m_nButtonSize + m_nButtonSpan + m_nButtonSpan);

	for(int i = 0; i < ENUM_FLTBAR_GRAPH_TYPE_TOTAL; ++i)
	{
		m_rcButton[i].left = (i % m_nNumOfButtonEachRow) * (m_nButtonSize + m_nButtonSpan + m_nButtonSpan) + m_nButtonSpan;
		m_rcButton[i].top = (i / m_nNumOfButtonEachRow) * (m_nButtonSize + m_nButtonSpan + m_nButtonSpan) + m_nButtonSpan;
		m_rcButton[i].right = m_rcButton[i].left + m_nButtonSize;
		m_rcButton[i].bottom = m_rcButton[i].top + m_nButtonSize;
	}

	m_nIdxSelected = -1;
	m_nIdxPushed = -1;

	m_bIsShowWindow = FALSE;
	m_hToolTip = NULL;
}

DTFloatBar::~DTFloatBar()
{
	m_bExit = true;
	::SendMessage(m_hWnd, WM_QUIT, 0, 0);
	if(m_hEventMessageLoopThreadFinished)
		::WaitForSingleObject(m_hEventMessageLoopThreadFinished, INFINITE);
	/*if(m_hMessageLoopThread)
		::WaitForSingleObject(m_hMessageLoopThread, 1000L);*/
	::CloseHandle(m_hMessageLoopThread);
	m_hMessageLoopThread = NULL;
	::CloseHandle(m_hEventMessageLoopThreadFinished);
	m_hEventMessageLoopThreadFinished = NULL;

	Destroy();

	m_hWnd = NULL;
	m_hWndParent = NULL;
	m_pfnSubclass = NULL;
	m_uIdSubclass = NULL;
	m_dwRefData = NULL;
}

HWND DTFloatBar::Create()
{
	if(m_hWnd && ::IsWindow(m_hWnd))
		return m_hWnd;

	WNDCLASSEX wc;
	wc .cbSize = sizeof (WNDCLASSEX);
	wc .style  = CS_CLASSDC | CS_SAVEBITS | CS_DBLCLKS;
	wc .lpfnWndProc = DTFloatBar::WindowProcNil;
	wc .cbClsExtra  = 0;
	wc .cbWndExtra = 0;
	wc .hInstance = m_hInst;
	wc .hIcon = NULL;
	wc .hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc .hbrBackground = (HBRUSH) ::GetSysColorBrush(COLOR_3DFACE);
	wc .lpszMenuName = NULL;
	wc .lpszClassName = _T("DT_Graph_Picker");
	wc .hIconSm = NULL;
#if (_WIN32_WINNT >= 0x0501) && 0
	if (fIsXP)
	{
		BOOL fDropShadow;
		::SystemParametersInfo (SPI_GETDROPSHADOW, 0, &fDropShadow, FALSE);
		if (fDropShadow)
			wc .style |= CS_DROPSHADOW;
	}
#endif
	ATOM atom = ::RegisterClassEx (&wc);

	RECT rcWindow = m_rcFltBar;
	rcWindow.right += ::GetSystemMetrics(SM_CXDLGFRAME) * 4;
	rcWindow.bottom += ::GetSystemMetrics(SM_CYDLGFRAME) * 4 + ::GetSystemMetrics(SM_CYCAPTION);
	
	if (CCurrentWindowsVersion::Get().IsGreaterThanWin7())
	{
		rcWindow.right += 4.f;
		rcWindow.bottom += 15.f;
	}

	m_hWnd = ::CreateWindowEx(WS_EX_TOOLWINDOW, _T("DT_Graph_Picker"), _T (""),  WS_POPUP | /*WS_VISIBLE |*/ WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
								rcWindow.left, rcWindow.bottom, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top,
								m_hWndParent, NULL, m_hInst, NULL);

	assert(::GetParent(m_hWnd) == m_hWndParent);

	::GetWindowRect(m_hWnd, &m_rcFltBarWindow);

	ShowFloatBar(HWND_TOP, m_bIsShowWindow, TRUE);

	DWORD err = ::GetLastError();

	m_bExit = !::SetWindowSubclass(m_hWnd, m_pfnSubclass, m_uIdSubclass, m_dwRefData);

	m_hToolTip = ::CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
									CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
									m_hWnd, NULL, m_hInst, NULL);

	m_hImageList = ::ImageList_LoadImage(m_hInst, MAKEINTRESOURCE(IDB_DT_FLTBAR_IMAGELIST), m_nButtonSize, 32, RGB(0, 128, 128), IMAGE_BITMAP, LR_DEFAULTCOLOR);

	for(int i = 0; i < ENUM_FLTBAR_GRAPH_TYPE_TOTAL; ++i)
	{
		m_hIcon[i] = ::ImageList_GetIcon(m_hImageList, i, ILD_TRANSPARENT);
		::SendMessage(m_hIconButton[i], STM_SETICON, (WPARAM) m_hIcon[i], 0);
		::ShowWindow(m_hIconButton[i], SW_SHOW);

		TOOLINFO _ti;
		_ti.cbSize = sizeof(TOOLINFO);
		_ti.uFlags = TTF_SUBCLASS;
		_ti.hwnd = m_hWnd;
		_ti.hinst = m_hInst;
		_ti.uId = i;
		_ti.lpszText = RetrieveToolTipText((ENUM_FLTBAR_GRAPH_TYPE) i);
		_ti.rect.left = m_rcButton[i].left - m_nButtonSpan;
		_ti.rect.top = m_rcButton[i].top - m_nButtonSpan;
		_ti.rect.right = m_rcButton[i].right + m_nButtonSpan;
		_ti.rect.bottom = m_rcButton[i].bottom + m_nButtonSpan;

		::SendMessage(m_hToolTip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &_ti);
	}

	return m_hWnd;
}

BOOL DTFloatBar::Destroy()
{
	for(int i = 0; i < ENUM_FLTBAR_GRAPH_TYPE_TOTAL; ++i)
	{
		if(m_hIcon[i])
			::DestroyIcon(m_hIcon[i]);
		if(m_hIconButton[i] && ::IsWindow(m_hIconButton[i]))
			::DestroyWindow(m_hIconButton[i]);
		m_hIcon[i] = NULL;
		m_hIconButton[i] = NULL;
	}

	if(m_hWnd && ::IsWindow(m_hWnd))
	{
		::RemoveWindowSubclass(m_hWnd, m_pfnSubclass, m_uIdSubclass);
		::DestroyWindow(m_hWnd);
	}
	m_hWnd = NULL;

	if(m_hImageList)
		::ImageList_Destroy(m_hImageList);
	m_hImageList = NULL;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DTFloatBar::operator HWND ()
{
	return m_hWnd;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT DTFloatBar::MessageLoop()
{
	LRESULT lRes = 0;
	BOOL bRet;
	MSG msg;
	::ResetEvent(m_hEventMessageLoopThreadFinished);
	while((bRet = ::GetMessage(&msg, 0, 0, 0 )) != 0)
	{
		if (bRet == -1)
		{
			// handle the error and possibly exit
			lRes = -1;
			break;
		}
		else if(msg.hwnd == m_hWnd && msg.message == WM_QUIT)
		{
			break;
		}
		else if(!m_bExit)
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			lRes = -2;
			break;
		}
	}
	::SetEvent(m_hEventMessageLoopThreadFinished);
	::OutputDebugStringA("DTFloatBar::MessageLoop() Quit");
	return lRes;
}

BOOL DTFloatBar::ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
	BOOL bHandled = TRUE;
	if(m_hWnd != hWnd)
		return FALSE;

	if(uMsg == WM_COMMAND)
	{
		lResult = CommandHandler(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled);
	}
	else
	{
		lResult = MessageHandler(uMsg, wParam, lParam, bHandled);
	}

	if(!bHandled)
	{
		lResult = ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

LRESULT DTFloatBar::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	LRESULT lResult = 0;

	if(uMsg == WM_CLOSE)
	{
		bHandled = TRUE;
		ShowFloatBar(NULL, FALSE, TRUE);
	}
	else if(uMsg == WM_PAINT)
	{
		lResult = OnPaint(uMsg, wParam, lParam, bHandled);
	}
	else if(uMsg == WM_MOUSEMOVE)
	{
		lResult = OnMouseMove(uMsg, wParam, lParam, bHandled);
	}
	else if(uMsg == WM_MOUSELEAVE)
	{
		lResult = OnMouseLeave(uMsg, wParam, lParam, bHandled);
	}
	else if(uMsg == WM_SETCURSOR)
	{
		lResult = OnSetCursor(uMsg, wParam, lParam, bHandled);
	}
	else if(uMsg == WM_LBUTTONDOWN)
	{
		lResult = OnLButtonDown(uMsg, wParam, lParam, bHandled);
	}
	else if(uMsg == WM_LBUTTONDBLCLK)
	{
		lResult = OnLButtonDown(uMsg, wParam, lParam, bHandled);
	}
	else if(uMsg == WM_NCLBUTTONDOWN)
	{
		lResult = OnNcLButtonDown(uMsg, wParam, lParam, bHandled);
	}
#ifndef PRIVATE_V2
	else if(uMsg == WM_KEYUP || uMsg == WM_KEYDOWN)
	{
		lResult = OnCharChain(uMsg, wParam, lParam, bHandled);
	}
#endif
	return lResult;
}

LRESULT DTFloatBar::CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	for(int i = 0; i < ENUM_FLTBAR_GRAPH_TYPE_TOTAL; ++i)
	{
		if(m_hIconButton[i] == hWndCtl)
		{
			if(wNotifyCode == STN_CLICKED)
			{
				RECT rect;
				::GetClientRect(m_hWnd, &rect);
				SetButtonClicked(hWndCtl);
				::InvalidateRect(m_hWnd, &rect, TRUE);
			}
		}
	}
	return 0;
}

LRESULT DTFloatBar::NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
{
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
void DTFloatBar::SetButtonClicked(HWND hWndCtl)
{
	for(int i = 0; i < ENUM_FLTBAR_GRAPH_TYPE_TOTAL; ++i)
	{
		if(m_hIconButton[i] == hWndCtl)
		{
			::SetWindowLong(m_hIconButton[i], GWL_STYLE, SS_ICON | SS_NOTIFY | SS_WHITEFRAME | WS_CHILD | WS_VISIBLE);
		}
		else
		{
			::SetWindowLong(m_hIconButton[i], GWL_STYLE, SS_ICON | SS_NOTIFY | WS_CHILD | WS_VISIBLE);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void DTFloatBar::DisabledButton(HDC _hdc, int _idx)
{
	if(0 <= _idx && _idx < ENUM_FLTBAR_GRAPH_TYPE_TOTAL)
	{
		RECT _rcButtonEdge = { m_rcButton[_idx].left - m_nButtonSpan,
								m_rcButton[_idx].top - m_nButtonSpan,
								m_rcButton[_idx].right + m_nButtonSpan,
								m_rcButton[_idx].bottom + m_nButtonSpan };
		::FillRect(_hdc, &_rcButtonEdge, ::GetSysColorBrush(COLOR_3DFACE));
		::DrawState(_hdc, 0, 0, (LPARAM) m_hIcon[_idx], 0,
						m_rcButton[_idx].left, m_rcButton[_idx].top,
						m_rcButton[_idx].right - m_rcButton[_idx].left, m_rcButton[_idx].bottom - m_rcButton[_idx].top,
						DST_ICON | DSS_DISABLED);
	}
}

void DTFloatBar::RecoverButton(HDC _hdc, int _idx)
{
	if(0 <= _idx && _idx < ENUM_FLTBAR_GRAPH_TYPE_TOTAL
		&& _idx != m_nIdxPushed)
	{
		RECT _rcButtonEdge = { m_rcButton[_idx].left - m_nButtonSpan,
								m_rcButton[_idx].top - m_nButtonSpan,
								m_rcButton[_idx].right + m_nButtonSpan,
								m_rcButton[_idx].bottom + m_nButtonSpan };
		::FillRect(_hdc, &_rcButtonEdge, ::GetSysColorBrush(COLOR_3DFACE));
		::ImageList_Draw(m_hImageList, _idx, _hdc, m_rcButton[_idx].left, m_rcButton[_idx].top, ILD_TRANSPARENT);
	}
}

void DTFloatBar::SelectedButton(HDC _hdc, int _idx)
{
	if(0 <= _idx && _idx < ENUM_FLTBAR_GRAPH_TYPE_TOTAL
		&& _idx != m_nIdxPushed)
	{
		RECT _rcButtonEdge = { m_rcButton[_idx].left - m_nButtonSpan,
								m_rcButton[_idx].top - m_nButtonSpan,
								m_rcButton[_idx].right + m_nButtonSpan,
								m_rcButton[_idx].bottom + m_nButtonSpan };
		::ImageList_Draw(m_hImageList, _idx, _hdc, m_rcButton[_idx].left, m_rcButton[_idx].top, ILD_TRANSPARENT);
		::DrawEdge(_hdc, &_rcButtonEdge, BDR_RAISEDOUTER, BF_RECT | BF_ADJUST | BF_SOFT);

	}
}

void DTFloatBar::PushedButton(HDC _hdc, int _idx)
{
	if(0 <= _idx && _idx < ENUM_FLTBAR_GRAPH_TYPE_TOTAL)
	{
		RECT _rcButtonEdge = { m_rcButton[_idx].left - m_nButtonSpan,
								m_rcButton[_idx].top - m_nButtonSpan,
								m_rcButton[_idx].right + m_nButtonSpan,
								m_rcButton[_idx].bottom + m_nButtonSpan };
		::FillRect(_hdc, &_rcButtonEdge, ::GetSysColorBrush(COLOR_BTNHIGHLIGHT));
		::ImageList_DrawEx(m_hImageList, _idx, _hdc,
								m_rcButton[_idx].left, m_rcButton[_idx].top,
								m_rcButton[_idx].right - m_rcButton[_idx].left, m_rcButton[_idx].bottom - m_rcButton[_idx].top,
								CLR_NONE, RGB(255, 255, 255), ILD_TRANSPARENT);
		::DrawEdge(_hdc, &_rcButtonEdge, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST | BF_SOFT);
	}
}

void DTFloatBar::DisabledButtonExcept(HDC _hdc, int _idx)
{
	for(int i = ENUM_FLTBAR_GRAPH_TYPE_NULL; i < ENUM_FLTBAR_GRAPH_TYPE_TOTAL; ++i)
	{
		if(_idx == i)
			continue;

		RECT _rcButtonEdge = { m_rcButton[ i ].left - m_nButtonSpan,
								m_rcButton[ i ].top - m_nButtonSpan,
								m_rcButton[ i ].right + m_nButtonSpan,
								m_rcButton[ i ].bottom + m_nButtonSpan };
		::FillRect(_hdc, &_rcButtonEdge, ::GetSysColorBrush(COLOR_3DFACE));
		::DrawState(_hdc, 0, 0, (LPARAM) m_hIcon[ i ], 0,
						m_rcButton[ i ].left, m_rcButton[ i ].top,
						m_rcButton[ i ].right - m_rcButton[ i ].left, m_rcButton[ i ].bottom - m_rcButton[ i ].top,
						DST_ICON | DSS_DISABLED);
	}
}

void DTFloatBar::EnabledButton(HDC _hdc)
{
	for(int i = ENUM_FLTBAR_GRAPH_TYPE_NULL; i < ENUM_FLTBAR_GRAPH_TYPE_TOTAL; ++i)
	{
		RECT _rcButtonEdge = { m_rcButton[ i ].left - m_nButtonSpan,
								m_rcButton[ i ].top - m_nButtonSpan,
								m_rcButton[ i ].right + m_nButtonSpan,
								m_rcButton[ i ].bottom + m_nButtonSpan };
		::FillRect(_hdc, &_rcButtonEdge, ::GetSysColorBrush(COLOR_3DFACE));
		::ImageList_DrawEx(m_hImageList, i, _hdc,
								m_rcButton[ i ].left, m_rcButton[ i ].top,
								m_rcButton[ i ].right - m_rcButton[ i ].left, m_rcButton[ i ].bottom - m_rcButton[ i ].top,
								CLR_NONE, RGB(255, 255, 255), ILD_TRANSPARENT);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT DTFloatBar::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	PAINTSTRUCT _ps;
	HDC _hdc = ::BeginPaint(m_hWnd, &_ps);

	if(m_nIdxPushed == ENUM_FLTBAR_GRAPH_TYPE_Mask)
	{
		DisabledButtonExcept(_hdc, m_nIdxPushed);
		PushedButton(_hdc, m_nIdxPushed);
	}
	else
	{
		for(int i = 0; i < ENUM_FLTBAR_GRAPH_TYPE_TOTAL; ++i)
		{
			::ImageList_Draw(m_hImageList, i, _hdc, m_rcButton[i].left, m_rcButton[i].top, ILD_TRANSPARENT);
			if(m_nIdxPushed == i)
			{
				PushedButton(_hdc, i);
			}
			else if(m_nIdxSelected == i)
			{
				SelectedButton(_hdc, i);
			}
		}
	}
	::EndPaint(m_hWnd, &_ps);
	return (LRESULT) _hdc;
}

LRESULT DTFloatBar::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	if(HIWORD(lParam) == WM_MOUSEMOVE)
	{
		if(LOWORD(lParam) != HTCLIENT
			&& m_nIdxPushed != ENUM_FLTBAR_GRAPH_TYPE_Mask)
		{
			HDC _hdc = ::GetDC(m_hWnd);
			RecoverButton(_hdc, m_nIdxSelected);
			::ReleaseDC(m_hWnd, _hdc);
		}
	}
	if(HIWORD(lParam) == WM_LBUTTONDOWN)
    {
        if(LOWORD(lParam) == HTCLOSE)
        {
            bHandled = TRUE;
            ShowFloatBar(NULL, FALSE, TRUE);
        }
        else if(LOWORD(lParam) == HTCLIENT)
        {
            POINT pt;
            ::GetCursorPos(&pt);
            ::ScreenToClient(m_hWnd, &pt);
            //OnLButtonDown(WM_LBUTTONDOWN, 0, MAKELONG(pt.x, pt.y), bHandled);
        }
    }
	return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}

LRESULT DTFloatBar::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	long& _idx = m_nIdxSelected;
	long _last = m_nIdxSelected;
	long _row = 0;
	long _col = 0;
	long _x = GET_X_LPARAM(lParam);
	long _y = GET_Y_LPARAM(lParam);
	POINT _pt = { _x, _y };

	if(m_nIdxPushed == ENUM_FLTBAR_GRAPH_TYPE_Mask)
		return 0;

	if(!::PtInRect(&m_rcFltBar, _pt))
		return 0;

	_row = _y / (m_nButtonSize + m_nButtonSpan + m_nButtonSpan);
	_col = _x / (m_nButtonSize + m_nButtonSpan + m_nButtonSpan);

	_idx = _row * m_nNumOfButtonEachRow + _col;

	HDC _hdc = ::GetDC(m_hWnd);

	if(_last != _idx)
	{
		RecoverButton(_hdc, _last);
	}

	SelectedButton(_hdc, _idx);

	::ReleaseDC(m_hWnd, _hdc);
	return 0;
}

LRESULT DTFloatBar::OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;

	if(m_nIdxPushed == ENUM_FLTBAR_GRAPH_TYPE_Mask)
		return 0;

	HDC _hdc = ::GetDC(m_hWnd);
	RecoverButton(_hdc, m_nIdxSelected);
	::ReleaseDC(m_hWnd, _hdc);

	m_nIdxSelected = -1;
	return 0;
}

LRESULT DTFloatBar::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	long _idx = m_nIdxPushed;
	long _last = m_nIdxPushed;
	long _row = 0;
	long _col = 0;
	long _x = GET_X_LPARAM(lParam);
	long _y = GET_Y_LPARAM(lParam);
	POINT _pt = { _x, _y };

	if(!::PtInRect(&m_rcFltBar, _pt))
		return 0;

	_row = _y / (m_nButtonSize + m_nButtonSpan + m_nButtonSpan);
	_col = _x / (m_nButtonSize + m_nButtonSpan + m_nButtonSpan);

	_idx = _row * m_nNumOfButtonEachRow + _col;

	HDC _hdc = ::GetDC(m_hWnd);

	switch(_last)
	{
	case ENUM_FLTBAR_GRAPH_TYPE_Mask:
		if(_idx == _last)
		{
			EnabledButton(_hdc);
			m_nIdxSelected = _idx;
			m_nIdxPushed = -1;
			SelectedButton(_hdc, _idx);
			::PostMessage(m_hWndCharChain, WM_DT_FLTBAR_MASK, (WPARAM) FALSE, 0);
		}
		break;
	default:
		if(m_nIdxSelected != _idx)
		{
			RecoverButton(_hdc, m_nIdxSelected);
			m_nIdxSelected = _idx;
		}
		if(_idx != _last && 0 <= _idx && _idx < ENUM_FLTBAR_GRAPH_TYPE_TOTAL)
		{
			m_nIdxPushed = _idx;
			RecoverButton(_hdc, _last);
			PushedButton(_hdc, _idx);
		}
		break;
	}


	switch(m_nIdxPushed)
	{
	case ENUM_FLTBAR_GRAPH_TYPE_Delete:
		{
			// chain process
#ifndef PRIVATE_V2
			//// <!!! NOTE !!!> it should not be SendMessage, otherwise the m_hWndCharChain will not run while MessageBox
			::PostMessage(m_hWndCharChain, WM_DT_FLTBAR_DELETE, 0, 0);
#endif
			RecoverButton(_hdc, m_nIdxSelected);
			m_nIdxPushed = -1;
		}
		break;
	case ENUM_FLTBAR_GRAPH_TYPE_Mask:
		{
			DisabledButtonExcept(_hdc, ENUM_FLTBAR_GRAPH_TYPE_Mask);
			::PostMessage(m_hWndCharChain, WM_DT_FLTBAR_MASK, (WPARAM) TRUE, 0);
		}
		break;
	}

	::ReleaseDC(m_hWnd, _hdc);
	return 0;
}

LRESULT DTFloatBar::OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = (wParam == HTCAPTION) && !CCurrentWindowsVersion::Get().IsGreaterThanWin7();
	if(bHandled)
	{
		::SetWindowPos(m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		POINT ptCursor = { (short) LOWORD(lParam), (short) HIWORD(lParam) };
		if(::DragDetect(m_hWnd, ptCursor))
		{
			RECT rcGhost = m_rcFltBarWindow;
			::GetWindowRect(m_hWnd, &rcGhost);
			::GetCursorPos(&ptCursor);

			DTFloatBarGhostMoveTracker _Tracker(m_hWnd, ptCursor, rcGhost);
			if(_Tracker.TrackDragAndDrop(m_hWnd))
			{
				::GetCursorPos(&ptCursor);
				_Tracker.AdjustCursor(ptCursor);
				::SetWindowPos(m_hWnd, HWND_TOP, ptCursor.x, ptCursor.y, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE);
				::PostMessage(m_hWndCharChain, WM_DT_FLTBAR_MASK, (WPARAM) RetrieveMaskSignal(), 0);
			}
		}
	}
	return 0;
}

LRESULT DTFloatBar::OnCharChain(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	if(m_hWndCharChain)
	{
		::SendMessage(m_hWndCharChain, uMsg, wParam, lParam);
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL DTFloatBar::ShowFloatBar(HWND hWndAfter, BOOL bShowWindow, BOOL bDefaultPos, RECT* rc)
{
	UINT uFlag = SWP_NOSIZE | SWP_NOZORDER;
	uFlag |= (bShowWindow) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;
	//uFlag |= (bDefaultPos) ? 0 : SWP_NOMOVE;

	long _x = max(0, ::GetSystemMetrics(SM_CXSCREEN) - (m_rcFltBar.right - m_rcFltBar.left) * 1.5);
	long _y = max(0, ::GetSystemMetrics(SM_CYSCREEN) * 0.15);

	if (!bDefaultPos && rc)
	{
		_x = min(_x, rc->left);
		_y = min(_y, rc->top);
	}

	if(::SetWindowPos(m_hWnd, HWND_TOP, _x, _y, 0, 0, uFlag))
		m_bIsShowWindow = bShowWindow;

	return m_bIsShowWindow;
}

BOOL DTFloatBar::IsShowFloatBar()
{
	return m_bIsShowWindow;
}

void DTFloatBar::SetFloatBarCharChain(HWND hWndCharChain)
{
	m_hWndCharChain = hWndCharChain;
}

void DTFloatBar::EndProcessWindowMessage()
{
	::PostMessage(m_hWnd, WM_QUIT, 0, 0);
}

void DTFloatBar::RecoverUnselected()
{
	m_nIdxPushed = ENUM_FLTBAR_GRAPH_TYPE_NULL;
	m_nIdxSelected = ENUM_FLTBAR_GRAPH_TYPE_NULL;
	::InvalidateRect(m_hWnd, &m_rcFltBar, TRUE);
}
