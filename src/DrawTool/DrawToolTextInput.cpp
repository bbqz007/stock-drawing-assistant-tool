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
#include "DrawTool.h"
#include "DrawToolTextInput.h"
#include <string.h>
#include <algorithm>
#include <time.h>
#include <sys/timeb.h>
#include <atlstr.h>
#include "resource.h"

LRESULT CALLBACK SubClassFontSizeEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if(uMsg == WM_CHAR)
	{
		int nStartChar = 0;
		int nEndChar = 0;

		if(wParam < '0' || wParam > '9')
		{
			return 0;
		}
		else if(wParam == '0')
		{
			::SendMessage(hWnd, EM_GETSEL, (WPARAM) &nStartChar, (LPARAM) &nEndChar);
			if(nStartChar == 0 || nEndChar == 0)
			{
				return 0;
			}
		}
	}
	switch(uMsg)
	{
	case WM_COPY:
	case WM_CUT:
	case WM_CLEAR:
	case WM_CONTEXTMENU:
		return 0;
	}
	return ::CallWindowProc((WNDPROC) dwRefData, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK DTTextInputBox::StartDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT _LRES = 0;
	DTTextInputBox* _TextInput = NULL;
	if(uMsg == WM_INITDIALOG)
	{
		BOOL bHandle = FALSE;
		_TextInput = (DTTextInputBox*) lParam;
		_TextInput->Attach(hWnd);
		_TextInput->m_DefTextInputBoxProc = (WNDPROC) ::GetClassLongPtr(hWnd, DWLP_DLGPROC);
		::SetLayeredWindowAttributes(hWnd, 0, 240, LWA_ALPHA);
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, lParam);
	}
	else
	{
		_TextInput = (DTTextInputBox*) ::GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}

	if(_TextInput)
	{
		_TextInput->ProcessWindowMessage(hWnd, uMsg, wParam, lParam, _LRES);
	}
	return _LRES;
}

BOOL CALLBACK DTTextInputBox_StartDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DTTextInputBox::StartDlgProc(hDlg, uMsg, wParam, lParam);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DTTextInputBox::DTTextInputBox()
: m_Width(21), m_TextFont(NULL),
  m_Color(RGB(255, 255, 255))
{
	m_IDD = IDD_TEXT_INPUT;
	m_hWnd = NULL;
	m_DefTextInputBoxProc = NULL;
}

DTTextInputBox::~DTTextInputBox()
{
	if(m_TextFont)
		::DeleteObject(m_TextFont);
	m_TextFont = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT DTTextInputBox::DoModal()
{

	return ::DialogBoxParam(DTApiGetAppInstance(), MAKEINTRESOURCE(m_IDD), ::GetActiveWindow(), (DLGPROC) DTTextInputBox_StartDlgProc, (LPARAM) this);
}

BOOL DTTextInputBox::ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
	BOOL bHandled = TRUE;
	if(uMsg == WM_INITDIALOG)
	{
		bHandled = TRUE;
		lResult = OnInitDialog(uMsg, wParam, lParam, bHandled);
		if(bHandled)
			return TRUE;
	}
	if(uMsg == WM_DRAWITEM)
	{
		bHandled = TRUE;
		lResult = OnDrawItem(uMsg, wParam, lParam, bHandled);
		if(bHandled)
			return TRUE;
	}
	if(uMsg == WM_COMMAND && IDC_BTN_UP == LOWORD(wParam))
	{
		bHandled = TRUE;
		lResult = OnWidthBtnUp(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled);
		if(bHandled)
			return TRUE;
	}
	if(uMsg == WM_COMMAND && IDC_BTN_DOWN == LOWORD(wParam))
	{
		bHandled = TRUE;
		lResult = OnWidthBtnDown(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled);
		if(bHandled)
			return TRUE;
	}
	if(uMsg == WM_COMMAND && IDOK == LOWORD(wParam))
	{
		bHandled = TRUE;
		lResult = OnCloseCmd(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled);
		if(bHandled)
			return TRUE;
	}
	if(uMsg == WM_COMMAND && IDCANCEL == LOWORD(wParam))
	{
		bHandled = TRUE;
		lResult = OnCloseCmd(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled);
		if(bHandled)
			return TRUE;
	}
	if(uMsg == WM_COMMAND && IDC_BTN_COLOR == LOWORD(wParam))
	{
		bHandled = TRUE;
		lResult = OnColorPicker(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled);
		if(bHandled)
			return TRUE;
	}
	if(uMsg == WM_COMMAND && IDC_EDIT_TEXT_INPUT == LOWORD(wParam))
	{
		if(EN_UPDATE == HIWORD(wParam))
		{
			bHandled = TRUE;
			lResult = OnTextInputUpdate(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled);
			if(bHandled)
				return TRUE;
		}
	}
	return ::CallWindowProc(m_DefTextInputBoxProc, hWnd, uMsg, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT DTTextInputBox::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	::SetWindowSubclass(::GetDlgItem(m_hWnd, IDC_EDIT_WIDTH_PICKER),
							SubClassFontSizeEditProc, 0,
							::GetWindowLongPtr(::GetDlgItem(m_hWnd, IDC_EDIT_WIDTH_PICKER), GWLP_WNDPROC));

	::SendDlgItemMessage(m_hWnd, IDC_COMBO_LINE_STYLE, CB_ADDSTRING, 0, (LPARAM)_T(""));
	::SendDlgItemMessage(m_hWnd, IDC_COMBO_LINE_STYLE, CB_ADDSTRING, 0, (LPARAM)_T(""));
	::SendDlgItemMessage(m_hWnd, IDC_COMBO_LINE_STYLE, CB_SETCURSEL, 1, 0L);

	::SendDlgItemMessage(m_hWnd, IDC_EDIT_WIDTH_PICKER, EM_SETLIMITTEXT, 2, 0L);

	m_TextFont = ::CreateFont(12, 6, 0,0, FW_THIN, 0,0,0, DEFAULT_CHARSET,
										OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, ANTIALIASED_QUALITY,
										VARIABLE_PITCH | FF_DONTCARE, _T("ËÎÌו"));

	::SendDlgItemMessage(m_hWnd, IDC_EDIT_TEXT_INPUT, WM_SETFONT, (WPARAM) m_TextFont, 0L);

	::SendDlgItemMessage(m_hWnd, IDC_EDIT_TEXT_INPUT, EM_SETLIMITTEXT, m_Text.ENUM_TEXT_SIZE, 0L);

	::SetDlgItemInt(m_hWnd, IDC_EDIT_WIDTH_PICKER, m_Width + 1, FALSE);

	::SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM) ::GetDlgItem(m_hWnd, IDC_EDIT_TEXT_INPUT), 1L);
	::OutputDebugStringA("DTTextInputBox::OnInitDialog");

	return TRUE;
}

LRESULT DTTextInputBox::OnDrawItem(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	DRAWITEMSTRUCT* item = (DRAWITEMSTRUCT*) lParam;
	if(item->CtlType == ODT_COMBOBOX)
	{

		if(item->CtlID == IDC_COMBO_LINE_STYLE && item->itemID != -1)
		{
			HPEN hPen = NULL;
			HPEN oldPen = NULL;
			if(item->itemState & ODS_SELECTED)
			{
				::FillRect(item->hDC, &item->rcItem, ::GetSysColorBrush(COLOR_HIGHLIGHT));
			}
			else if(item->itemState & ODS_DISABLED)
			{
				::FillRect(item->hDC, &item->rcItem, ::GetSysColorBrush(COLOR_3DFACE));
			}
			else
			{
				::FillRect(item->hDC, &item->rcItem, ::GetSysColorBrush(COLOR_WINDOW));
			}

			if(item->itemID == 0)
			{
				hPen = ::CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			}
			else if(item->itemID == 1)
			{
				hPen = ::CreatePen(PS_DASH, 1, RGB(0, 0, 0));
			}
			//::SetDCPenColor(item->hDC, RGB(0, 0, 0));
			if(hPen)
			{
				int oldMode = ::SetBkMode(item->hDC, TRANSPARENT);
				oldPen = (HPEN) ::SelectObject(item->hDC, hPen);
				::MoveToEx(item->hDC, item->rcItem.left, (item->rcItem.top + item->rcItem.bottom) / 2, NULL);
				::LineTo(item->hDC, item->rcItem.right, (item->rcItem.top + item->rcItem.bottom) / 2);
				::SelectObject(item->hDC, oldPen);
				::SetBkMode(item->hDC, oldMode);
				::DeleteObject(hPen);
			}
		}
	}
	else if(item->CtlType == ODT_BUTTON)
	{
		if(item->CtlID == IDC_BTN_UP
			|| item->CtlID == IDC_BTN_DOWN)
		{
			::FillRect(item->hDC, &item->rcItem, ::GetSysColorBrush(COLOR_3DFACE));

			COLORREF oldDCPenColor = NULL;
			COLORREF oldDCBrushColor = NULL;
			HBRUSH oldBrush = NULL;
			HPEN oldPen = NULL;
			if(item->itemState & ODS_DISABLED)
			{
				oldDCBrushColor = ::SetDCBrushColor(item->hDC, RGB(172, 168, 153));
				oldDCPenColor = ::SetDCPenColor(item->hDC, RGB(172, 168, 153));
			}
			else
			{
				oldDCBrushColor = ::SetDCBrushColor(item->hDC, RGB(0, 0, 0));
				oldDCPenColor = ::SetDCPenColor(item->hDC, RGB(0, 0, 0));
			}
			oldBrush = (HBRUSH) ::SelectObject(item->hDC, ::GetStockObject(DC_BRUSH));
			oldPen = (HPEN) ::SelectObject(item->hDC, ::GetStockObject(DC_PEN));

			if(item->CtlID == IDC_BTN_UP)
			{
				POINT ptPath[4] =
				{
					(item->rcItem.left + item->rcItem.right) / 2, (item->rcItem.top + item->rcItem.bottom) / 2 - 2,
					(item->rcItem.left + item->rcItem.right) / 2 - 2, (item->rcItem.top + item->rcItem.bottom) / 2 + 1,
					(item->rcItem.left + item->rcItem.right) / 2 + 2, (item->rcItem.top + item->rcItem.bottom) / 2 + 1,
					(item->rcItem.left + item->rcItem.right) / 2, (item->rcItem.top + item->rcItem.bottom) / 2 - 2
				};
				::Polygon(item->hDC, ptPath, 3);
			}
			else if(item->CtlID == IDC_BTN_DOWN)
			{
				POINT ptPath[4] =
				{
					(item->rcItem.left + item->rcItem.right) / 2, (item->rcItem.top + item->rcItem.bottom) / 2 + 1,
					(item->rcItem.left + item->rcItem.right) / 2 - 2, (item->rcItem.top + item->rcItem.bottom) / 2 - 2,
					(item->rcItem.left + item->rcItem.right) / 2 + 2, (item->rcItem.top + item->rcItem.bottom) / 2 - 2,
					(item->rcItem.left + item->rcItem.right) / 2, (item->rcItem.top + item->rcItem.bottom) / 2 + 1
				};
				::Polygon(item->hDC, ptPath, 3);
			}
			::SelectObject(item->hDC, oldBrush);
			::SelectObject(item->hDC, oldPen);
			::SetDCBrushColor(item->hDC, oldDCBrushColor);
			::SetDCBrushColor(item->hDC, oldDCPenColor);

			if(item->itemState & ODS_SELECTED)
			{
				::DrawEdge(item->hDC, &item->rcItem, BDR_SUNKEN, BF_RECT | BF_ADJUST | BF_SOFT);
			}
			else
			{
				::DrawEdge(item->hDC, &item->rcItem, BDR_RAISED, BF_RECT | BF_ADJUST | BF_SOFT);
			}
		}
		else if(item->CtlID == IDC_BTN_COLOR)
		{
			COLORREF oldDCBrushColor = ::SetDCBrushColor(item->hDC, m_Color);
			::FillRect(item->hDC, &item->rcItem, (HBRUSH) ::GetStockObject(DC_BRUSH));
			::DrawEdge(item->hDC, &item->rcItem, EDGE_ETCHED, BF_RECT | BF_FLAT | BF_SOFT);
			::SetDCBrushColor(item->hDC, oldDCBrushColor);
		}
	}
	return TRUE;
}

LRESULT DTTextInputBox::OnWidthBtnUp(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	RetrieveWidth();

	++m_Width;
	m_Width %= 50;
	::SetDlgItemInt(m_hWnd, IDC_EDIT_WIDTH_PICKER, m_Width + 1, FALSE);
	return 0;
}

LRESULT DTTextInputBox::OnWidthBtnDown(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	RetrieveWidth();

	m_Width += 50 - 1;
	m_Width %= 50;
	::SetDlgItemInt(m_hWnd, IDC_EDIT_WIDTH_PICKER, m_Width + 1, FALSE);
	return 0;
}

LRESULT DTTextInputBox::OnColorPicker(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
{
	if(wNotifyCode == STN_CLICKED)
	{
		CHOOSECOLOR m_cc;
		COLORREF acrCustClr[16];
		memset(&m_cc, 0, sizeof(m_cc));
		m_cc.lStructSize = sizeof(m_cc);
		m_cc.hwndOwner = ::GetActiveWindow();
		m_cc.lpCustColors = (LPDWORD) acrCustClr;
		m_cc.Flags = CC_RGBINIT;

		acrCustClr[0] = m_Color;

		if(ChooseColor(&m_cc))
		{
			m_Color = m_cc.rgbResult;
		}

		::InvalidateRect(hWndCtl, NULL, TRUE);
	}
	return 0;
}

LRESULT DTTextInputBox::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	RetrieveWidth();

	memset(m_Text.m_Text, 0, m_Text.ENUM_TEXT_SIZE);
	::GetDlgItemTextA(m_hWnd, IDC_EDIT_TEXT_INPUT, m_Text.m_Text, m_Text.ENUM_TEXT_SIZE + 2);
	m_Text.Revise();
	EndDialog(m_hWnd, wID);
	return 0;
}

LRESULT DTTextInputBox::OnTextInputUpdate(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	RetrieveWidth();
	return 0;
}


void DTTextInputBox::RetrieveWidth()
{
	BOOL bTranslated = FALSE;
	UINT nWidth = ::GetDlgItemInt(m_hWnd, IDC_EDIT_WIDTH_PICKER, &bTranslated, FALSE);
	if(bTranslated)
	{
		if(nWidth >= 1 && nWidth <= 50)
			m_Width = nWidth - 1;
		else if(nWidth > 50)
			m_Width = 49;
	}
}
