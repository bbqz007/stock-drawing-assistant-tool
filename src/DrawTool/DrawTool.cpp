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
#include "DrawToolFloatBar.h"
#include <string.h>
#include <algorithm>
#include <time.h>
#include <sys/timeb.h>

static DTFloatBar* _app_FltBar = NULL;
static HINSTANCE _app_hInstance = NULL;

BOOL InitDrawToolFloatBar(HINSTANCE _hInst, HWND _hWndParent)
{
	if(!_app_FltBar)
	{
		_app_FltBar = new DTFloatBar((_hInst) ? _hInst : _app_hInstance, _hWndParent);
		_app_FltBar->Create();
	}
	return TRUE;
}

BOOL UninitDrawToolFloatBar()
{
	if(_app_FltBar)
		delete _app_FltBar;
	_app_FltBar = NULL;
	return TRUE;
}

BOOL PreUninitDrawToolFloatBar()
{
	if(_app_FltBar)
		_app_FltBar->EndProcessWindowMessage();
	return TRUE;
}


BOOL DTApiShowFloatBar(HWND hWndAfter, BOOL bShowWindow, BOOL bDefaultPos, RECT* rc)
{
	if(_app_FltBar)
		return _app_FltBar->ShowFloatBar(hWndAfter, bShowWindow, bDefaultPos, rc);
	return FALSE;
}

BOOL DTApiIsShowFloatBar()
{
	if(_app_FltBar)
		return _app_FltBar->IsShowFloatBar();
	return FALSE;
}

void DTApiSetAppInstance(HINSTANCE _hInst)
{
	_app_hInstance = _hInst;
}

HINSTANCE DTApiGetAppInstance()
{
	return _app_hInstance;
}

void DTApiSetFloatBarCharChain(HWND hWndCharChain)
{
	if(_app_FltBar)
		_app_FltBar->SetFloatBarCharChain(hWndCharChain);
}

ENUM_ASSIST_GRAPH_TYPE DTApiRetrievePickedGraphTool()
{
	if(_app_FltBar)
		return _app_FltBar->RetrievePickedGraphTool();
	return ENUM_ASSIST_GRAPH_TYPE_NULL;
}

ENUM_ASSIST_GRAPH_TYPE DTApiPickGraphTool()
{
	if(_app_FltBar)
		return _app_FltBar->PickGraphTool();
	return ENUM_ASSIST_GRAPH_TYPE_NULL;
}

BOOL DTApiSetFloatBarCaptionTitle(char* szTitle)
{
	if(_app_FltBar)
		return ::SetWindowTextA((HWND) *_app_FltBar, szTitle);
	return FALSE;
}

BOOL DTApiIsSignalMaskGraphTool()
{
	if(_app_FltBar)
		return _app_FltBar->RetrieveMaskSignal();
	return FALSE;
}

void DTApiInsertTypeNameToScript(DWORD _Type, char* _Name)
{
	DTAssistGraphDescriptionScriptMap::Instance().InsertTypeName(_Type, _Name);
}

void DTApiInsertViewNameToScript(DWORD _ViewId, char* _Name)
{
	DTAssistGraphDescriptionScriptMap::Instance().InsertViewName(_ViewId, _Name);
}

void DTApiInsertPeriodNameToScript(DWORD _Period, char* _Name, BOOL _ByDay = FALSE)
{
	DTAssistGraphDescriptionScriptMap::Instance().InsertPeriodName(_Period, _Name);
	if(_ByDay)
	{
		DTAssistGraphDescriptionScriptMap::Instance().InsertDayPeriod(_Period);
	}
}


