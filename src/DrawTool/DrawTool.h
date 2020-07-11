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

#ifndef _DRAWTOOL__H_
#define _DRAWTOOL__H_

#include "DrawToolBasic.h"

/// <!-- NOTE!! your program's WM_NCACTIVE should return 1;

BOOL InitDrawToolFloatBar(HINSTANCE _hInst, HWND _hWndParent);
BOOL UninitDrawToolFloatBar();
BOOL PreUninitDrawToolFloatBar();

void DTApiSetFloatBarCharChain(HWND hWndCharChain);
BOOL DTApiIsShowFloatBar();
BOOL DTApiShowFloatBar(HWND hWndAfter, BOOL bShowWindow, BOOL bDefaultPos);
void DTApiSetAppInstance(HINSTANCE _hInst);
HINSTANCE DTApiGetAppInstance();
ENUM_ASSIST_GRAPH_TYPE DTApiRetrievePickedGraphTool();
ENUM_ASSIST_GRAPH_TYPE DTApiPickGraphTool();
BOOL DTApiSetFloatBarCaptionTitle(char* szTitle);
BOOL DTApiIsSignalMaskGraphTool();

void DTApiInsertTypeNameToScript(DWORD _Type, char* _Name);
void DTApiInsertViewNameToScript(DWORD _ViewId, char* _Name);
void DTApiInsertPeriodNameToScript(DWORD _Period, char* _Name, BOOL _ByDay = FALSE);


#endif // _DRAWTOOL__H_
