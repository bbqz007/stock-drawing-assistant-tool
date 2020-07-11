/**
MIT License
Copyright (c) 2015-2020 bbqz007 <https://github.com/bbqz007, http://www.cnblogs.com/bbqzsl>
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

#ifndef _DRAWTOOL_FLOATBAR__H_
#define _DRAWTOOL_FLOATBAR__H_

#include <windows.h>
#include <commctrl.h>
#include "DrawToolDef.h"

class DTFloatBar
{
public:
	static LRESULT CALLBACK WindowProcNil(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static DWORD WINAPI ThreadProc(LPVOID lpPara);

	DTFloatBar(HINSTANCE _hInst, HWND _hWndParent);
	~DTFloatBar();

	HWND Create();
	BOOL Destroy();

	operator HWND ();

	BOOL ShowFloatBar(HWND hWndAfter, BOOL bShowWindow, BOOL bDefaultPos);
	BOOL IsShowFloatBar();
	void SetFloatBarCharChain(HWND hWndCharChain);
	void EndProcessWindowMessage();

	BOOL RetrieveMaskSignal();

	ENUM_ASSIST_GRAPH_TYPE RetrieveGraphTool(ENUM_FLTBAR_GRAPH_TYPE _ToolType);
	ENUM_ASSIST_GRAPH_TYPE RetrievePickedGraphTool();
	ENUM_ASSIST_GRAPH_TYPE PickGraphTool();
protected:
	LRESULT MessageLoop();
	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBtnClicked(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseLeave(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCharChain(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void SetButtonClicked(HWND hWndCtl);

	void DisabledButton(HDC hdc, int idx);
	void EnabledButton(HDC hdc);
	void RecoverButton(HDC hdc, int idx);
	void SelectedButton(HDC hdc, int idx);
	void PushedButton(HDC hdc, int idx);
	void DisabledButtonExcept(HDC hdc, int idx);

	void RecoverUnselected();
private:
	bool m_bExit;
	HINSTANCE m_hInst;

	HWND m_hWnd;
	HWND m_hWndParent;
	HWND m_hWndCharChain;
	SUBCLASSPROC m_pfnSubclass;
	UINT_PTR m_uIdSubclass;
	DWORD_PTR m_dwRefData;

	HANDLE m_hEventMessageLoopThreadFinished;
	HANDLE m_hMessageLoopThread;
	DWORD m_dwThreadId;

	HWND m_hIconButton[ ENUM_FLTBAR_GRAPH_TYPE_TOTAL ];
	HICON m_hIcon[ ENUM_FLTBAR_GRAPH_TYPE_TOTAL ];
	RECT m_rcButton[ ENUM_FLTBAR_GRAPH_TYPE_TOTAL ];

	HIMAGELIST m_hImageList;

	RECT m_rcFltBar;
	RECT m_rcFltBarWindow;
	DWORD m_nStyle;
	DWORD m_nStyleEx;

	long m_nButtonSize;
	long m_nButtonSpan;
	long m_nNumOfButtonEachRow;

	long m_nIdxSelected;
	long m_nIdxPushed;

	HWND m_hToolBar;
	BOOL m_bIsShowWindow;

	HWND m_hToolTip;
};

class DTFloatBarGhostMoveTracker
{
public:
	enum { ENUM_GHOST_RECT_SIDE_SIZE = 3 };
	DTFloatBarGhostMoveTracker(HWND _hWnd, POINT& _Point, RECT& _RectGhost);
	~DTFloatBarGhostMoveTracker();
	void BeginDrag();
	void EndDrag(bool _Canceled);
	void OnMove(long _x, long _y);
	void OnCancelDrag(long _x, long _y);
	bool OnDrop(long _x, long _y);
	bool OnDropRightButton(long _x, long _y);
	bool OnDropLeftButton(long _x, long _y);
	bool ProcessWindowMessage(MSG* _Msg);

	void AdjustCursor(POINT& _Point);

	bool TrackDragAndDrop(HWND _hWnd);
protected:
	void DrawGhostRect(HDC _hdc, RECT& _Rect);
	void CleanGhostRect(HDC _hdc, RECT& _Rect);
	void CreateHalftoneBrush();
protected:
	HDC m_hdc;
	HWND m_hWndTracker;
	HBRUSH m_hBrushHalftone;
	HBITMAP m_hBmpGray;

	RECT m_rcGhost;
	SIZE m_Size;
	SIZE m_Offset;
};


#endif // _DRAWTOOL_FLOATBAR__H_
