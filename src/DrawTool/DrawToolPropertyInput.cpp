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
#include "DrawToolPropertyInput.h"
#include <string.h>
#include <algorithm>
#include <time.h>
#include <sys/timeb.h>
#include <atlstr.h>
#include "resource.h"

#pragma warning(disable:4996)

LRESULT CALLBACK SubClassValueEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	if(uMsg == WM_CHAR)
	{
		int nStartChar = 0;
		int nEndChar = 0;
		char strText[32];
		char* chAt = NULL;

		switch(wParam)
		{
		case 8:	// back space
			break;
		case '0':
			::SendMessage(hWnd, EM_GETSEL, (WPARAM) &nStartChar, (LPARAM) &nEndChar);
			if(nStartChar == 0 || nEndChar == 0)
			{
				return 0;
			}
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		case '.':
			::memset(strText, 0, sizeof(strText));
			::GetWindowTextA(hWnd, strText, sizeof(strText));
			chAt = ::strchr(strText, '.');
			if(chAt)
			{
				::SendMessage(hWnd, EM_GETSEL, (WPARAM) &nStartChar, (LPARAM) &nEndChar);
				if(chAt < (strText + min(nStartChar, nEndChar))
					|| chAt >= (strText + max(nStartChar, nEndChar)))
				{
					return 0;
				}
			}
			break;
		default:
			return 0;
			break;
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

///////////////////////////////////////////////////////////////////////////////////////////////////////
DTPropertyInputBox::DTPropertyInputBox()
{
	m_IDD = IDD_PROP_INPUT;
	m_Customer = FALSE;
}

DTPropertyInputBox::~DTPropertyInputBox()
{
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void DTPropertyInputBox::SetDescriptionScript(DTAssistGraphDescriptionScript _Script)
{
	m_Script = _Script;
}

void DTPropertyInputBox::SetDescription(DTAssistGraphDescription& _Description)
{
	m_Description = _Description;
}

void DTPropertyInputBox::SetText(char* _Text)
{
	if(_Text)
		m_Text.CopyText(_Text);
}

DTAssistGraphDescription& DTPropertyInputBox::Description()
{
	return m_Description;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL DTPropertyInputBox::ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
	BOOL bHandled = TRUE;
	if(uMsg == WM_INITDIALOG)
	{
		bHandled = TRUE; 
		lResult = OnInitDialog(uMsg, wParam, lParam, bHandled); 
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
	if(uMsg == WM_COMMAND && IDC_CHECK_SYSDEFAULT == LOWORD(wParam)) 
	{ 
		if(HIWORD(wParam) == BN_CLICKED)
		{
			bHandled = TRUE; 
			lResult = OnIgoreCheckBoxClick(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); 
			if(bHandled) 
				return TRUE; 
		}
	}
	
	return __super::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

LRESULT DTPropertyInputBox::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnInitDialog(uMsg, wParam, lParam, bHandled);

	::SetWindowSubclass(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_1_INPUT), 
							SubClassValueEditProc, 0, 
							::GetWindowLongPtr(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_1_INPUT), GWLP_WNDPROC));

	::SetWindowSubclass(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_2_INPUT), 
							SubClassValueEditProc, 0, 
							::GetWindowLongPtr(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_2_INPUT), GWLP_WNDPROC));

	::SetWindowSubclass(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_3_INPUT), 
							SubClassValueEditProc, 0, 
							::GetWindowLongPtr(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_3_INPUT), GWLP_WNDPROC));

	::EnableWindow(::GetDlgItem(m_hWnd, IDC_COMBO_LINE_STYLE), FALSE);
	::EnableWindow(::GetDlgItem(m_hWnd, IDC_COMBO_LINE_SIZE), FALSE);
	//::EnableWindow(::GetDlgItem(m_hWnd, IDC_CHECK_SYSDEFAULT), FALSE);

	// Right Top, yValue Column
	char szFloatValue[32];
	tm tmDate;
	if(m_Script.m_Location1)
	{
		::_snprintf(szFloatValue, sizeof(szFloatValue), "%.3f", m_Description.m_ptLocations.ptAbsoluteP1.yValue);
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_1_INPUT), szFloatValue);
		if(::_localtime32(&m_Description.m_ptLocations.ptAbsoluteP1.xDate))
		{
			tmDate = *::_localtime32(&m_Description.m_ptLocations.ptAbsoluteP1.xDate);
			if(DTAssistGraphDescriptionScriptMap::Instance().IsPeriodByTheDay(m_Description.m_Identity.nPeriod))
			{
				::_snprintf(szFloatValue, sizeof(szFloatValue), "%4d%.2d%.2d", tmDate.tm_year + 1900, tmDate.tm_mon + 1, tmDate.tm_mday);
			}
			else
			{
				::_snprintf(szFloatValue, sizeof(szFloatValue), "%.2d%.2d/%d:%.2d", tmDate.tm_mon + 1, tmDate.tm_mday, tmDate.tm_hour, tmDate.tm_min);
			}
		}
		else
		{
			::_snprintf(szFloatValue, sizeof(szFloatValue), "无法识别");
		}
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_STATIC_XDATE_1), szFloatValue);
	}
	else 
	{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_1_INPUT), FALSE);
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_1_INPUT), "—.——");
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_STATIC_XDATE_1), "————");
	}

	if(m_Script.m_Location2)
	{
		::_snprintf(szFloatValue, sizeof(szFloatValue), "%.3f", m_Description.m_ptLocations.ptAbsoluteP2.yValue);
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_2_INPUT), szFloatValue);
		if(::_localtime32(&m_Description.m_ptLocations.ptAbsoluteP2.xDate))
		{
			tmDate = *::_localtime32(&m_Description.m_ptLocations.ptAbsoluteP2.xDate);
			if(DTAssistGraphDescriptionScriptMap::Instance().IsPeriodByTheDay(m_Description.m_Identity.nPeriod))
			{
				::_snprintf(szFloatValue, sizeof(szFloatValue), "%4d%.2d%.2d", tmDate.tm_year + 1900, tmDate.tm_mon + 1, tmDate.tm_mday);
			}
			else
			{
				::_snprintf(szFloatValue, sizeof(szFloatValue), "%.2d%.2d/%d:%.2d", tmDate.tm_mon + 1, tmDate.tm_mday, tmDate.tm_hour, tmDate.tm_min);
			}
		}
		else
		{
			::_snprintf(szFloatValue, sizeof(szFloatValue), "无法识别");
		}
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_STATIC_XDATE_2), szFloatValue);
	}
	else 
	{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_2_INPUT), FALSE);
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_2_INPUT), "—.——");
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_STATIC_XDATE_2), "————");
	}

	if(m_Script.m_Location3)
	{
		::_snprintf(szFloatValue, sizeof(szFloatValue), "%.3f", m_Description.m_ptLocations.ptAbsoluteP3.yValue);
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_3_INPUT), szFloatValue);
		if(::_localtime32(&m_Description.m_ptLocations.ptAbsoluteP3.xDate))
		{
			tmDate = *::_localtime32(&m_Description.m_ptLocations.ptAbsoluteP3.xDate);
			if(DTAssistGraphDescriptionScriptMap::Instance().IsPeriodByTheDay(m_Description.m_Identity.nPeriod))
			{
				::_snprintf(szFloatValue, sizeof(szFloatValue), "%4d%.2d%.2d", tmDate.tm_year + 1900, tmDate.tm_mon + 1, tmDate.tm_mday);
			}
			else
			{
				::_snprintf(szFloatValue, sizeof(szFloatValue), "%.2d%.2d/%d:%.2d", tmDate.tm_mon + 1, tmDate.tm_mday, tmDate.tm_hour, tmDate.tm_min);
			}
		}
		else
		{
			::_snprintf(szFloatValue, sizeof(szFloatValue), "无法识别");
		}
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_STATIC_XDATE_3), szFloatValue);
	}
	else 
	{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_3_INPUT), FALSE);
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_EDIT_YVALUE_3_INPUT), "—.——");
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_STATIC_XDATE_3), "————");
	}

	//
	::_snprintf(szFloatValue, sizeof(szFloatValue), "%.8s", m_Description.m_Identity.stkCode);
	::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_STATIC_STOCK_BINDING), szFloatValue);
	DTAssistGraphDescriptionScriptMap::Instance().TypeName(m_Description.m_Identity.nType, szFloatValue, sizeof(szFloatValue));
	::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_STATIC_GRAPH_TYPE), szFloatValue);
	DTAssistGraphDescriptionScriptMap::Instance().ViewName(m_Description.m_Identity.nViewId, szFloatValue, sizeof(szFloatValue));
	::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_STATIC_VIEW_BINDING), szFloatValue);
	DTAssistGraphDescriptionScriptMap::Instance().PeriodName(m_Description.m_Identity.nPeriod, szFloatValue, sizeof(szFloatValue));
	::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_STATIC_PERIOD_BINDING), szFloatValue);

	//
	if(!m_Script.m_Custom)
	{
		::SendDlgItemMessage(m_hWnd, IDC_CHECK_SYSDEFAULT, BM_SETCHECK, BST_CHECKED, 0);
	}
	else
	{
		::SendDlgItemMessage(m_hWnd, IDC_CHECK_SYSDEFAULT, BM_SETCHECK, BST_UNCHECKED, 0);
	}

	if(!m_Script.m_Text)
	{
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_BTN_COLOR), FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_TEXT_INPUT), FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_EDIT_WIDTH_PICKER), FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_BTN_UP), FALSE);
		::EnableWindow(::GetDlgItem(m_hWnd, IDC_BTN_DOWN), FALSE);
	}
	else
	{
		::SetWindowTextA(::GetDlgItem(m_hWnd, IDC_EDIT_TEXT_INPUT), m_Text.m_Text);
		m_Width = min(max(m_Description.m_propLine.nWidth - 1, 0), 49);
		::SetDlgItemInt(m_hWnd, IDC_EDIT_WIDTH_PICKER, m_Width + 1, FALSE);
		m_Color = m_Description.m_propLine.rgbColor;
	}

	return TRUE;
}

LRESULT DTPropertyInputBox::OnCloseCmd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	char szFloatValue[32];
	char* szEnd = NULL;

	if(m_Script.m_Location1)
	{
		::memset(szFloatValue, 0, sizeof(szFloatValue));
		::GetDlgItemTextA(m_hWnd, IDC_EDIT_YVALUE_1_INPUT, szFloatValue, sizeof(szFloatValue));
		m_Description.m_ptLocations.ptAbsoluteP1.yValue = ::strtod(szFloatValue, &szEnd);
	}
	if(m_Script.m_Location2)
	{
		::memset(szFloatValue, 0, sizeof(szFloatValue));
		::GetDlgItemTextA(m_hWnd, IDC_EDIT_YVALUE_2_INPUT, szFloatValue, sizeof(szFloatValue));
		m_Description.m_ptLocations.ptAbsoluteP2.yValue = ::strtod(szFloatValue, &szEnd);
	}
	if(m_Script.m_Location3)
	{
		::memset(szFloatValue, 0, sizeof(szFloatValue));
		::GetDlgItemTextA(m_hWnd, IDC_EDIT_YVALUE_3_INPUT, szFloatValue, sizeof(szFloatValue));
		m_Description.m_ptLocations.ptAbsoluteP3.yValue = ::strtod(szFloatValue, &szEnd);
	}

	__super::OnCloseCmd(wNotifyCode, wID, hWndCtl, bHandled);

	if(m_Script.m_Custom)
	{
		m_Description.m_propLine.nWidth = RetrieveFontSize();
		m_Description.m_propLine.rgbColor = RetrieveColor();
	}
	return 0;
}

LRESULT DTPropertyInputBox::OnIgoreCheckBoxClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(!m_Script.m_Custom)
	{
		::SendDlgItemMessage(m_hWnd, IDC_CHECK_SYSDEFAULT, BM_SETCHECK, BST_CHECKED, 0);
	}
	else
	{
		::SendDlgItemMessage(m_hWnd, IDC_CHECK_SYSDEFAULT, BM_SETCHECK, BST_UNCHECKED, 0);
	}
	return 0;
}
