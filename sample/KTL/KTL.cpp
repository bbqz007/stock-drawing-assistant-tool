// KTL.cpp : main source file for KTL.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include "resource.h"

#include "XWView.h"
#include "DrawTool/DrawTool.h"
#include "XW/include/NSAutoreleasePool.h"

CAppModule _Module;
HINSTANCE g_hInstance;

struct XWNSAotureleasePoolIdleHandler : public CIdleHandler
{
	BOOL OnIdle()
	{
		NSAutoreleasePoolDrain();
		return TRUE;
	}
};

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	XWNSAotureleasePoolIdleHandler autodrainIdleHandler;
	theLoop.AddIdleHandler(&autodrainIdleHandler);

	::Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	::Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	int nRet;

	_0_autoreleasepool {

	CXWView wndXW;
	wndXW.Create(0, ATL::CWindow::rcDefault, NULL, WS_VISIBLE | WS_CAPTION | WS_SYSMENU, 0);
	wndXW.SetWindowPos(NULL, 0, 0, 1024, 1024 /16 * 10, SWP_NOMOVE);
	wndXW.ShowWindow(nCmdShow);
	wndXW.CenterWindow();

	InitDrawToolFloatBar(g_hInstance, (HWND)wndXW);
    DTApiShowFloatBar(NULL, TRUE, TRUE);
    DTApiSetFloatBarCaptionTitle("zhelper");
    DTApiSetFloatBarCharChain((HWND)wndXW);
	// @autoreleasepool {
	//NSAutoreleasePoolAttachCurrentThread();
	nRet = theLoop.Run();

	//NSAutoreleasePoolDettachCurrentThread();
	// }

	GCMatrixContextClear();

	}}

	_Module.RemoveMessageLoop();

	::Gdiplus::GdiplusShutdown(gdiplusToken);

	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	PreUninitDrawToolFloatBar();
    UninitDrawToolFloatBar();
	_Module.Term();

	::CoUninitialize();

	return nRet;
}

