// PumpkinLog.cpp : main source file for PumpkinLog.exe
//

#include "stdafx.h"

#include "resource.h"

// Note: Proxy/Stub Information
//		To build a separate proxy/stub DLL, 
//		run nmake -f PumpkinLogps.mk in the project directory.
#include "initguid.h"
#include "PumpkinLog.h"
#include "PumpkinLog_i.c"

CServerAppModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
#ifdef _DEBUG
    _Module.m_dwPause = 0;
    _Module.m_dwTimeOut = 0;
#endif
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);
	int nRet = theLoop.Run();
	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(ObjectMap, hInstance, &LIBID_PumpkinLogLib);
	ATLASSERT(SUCCEEDED(hRes));

	AtlAxWinInit();

	// Parse command line, register or unregister or run the server
	int nRet = 0;
	TCHAR szTokens[] = _T("-/");
	bool bRun = true;
	bool bAutomation = false;

	LPCTSTR lpszToken = _Module.FindOneOf(::GetCommandLine(), szTokens);
	while(lpszToken != NULL)
	{
		if(lstrcmpi(lpszToken, _T("UnregServer")) == 0)
		{
			_Module.UpdateRegistryFromResource(IDR_PUMPKINLOG, FALSE);
			nRet = _Module.UnregisterServer(TRUE);
			bRun = false;
      bAutomation = false;
			break;
		}
		else if(lstrcmpi(lpszToken, _T("RegServer")) == 0)
		{
			_Module.UpdateRegistryFromResource(IDR_PUMPKINLOG, TRUE);
			nRet = _Module.RegisterServer(TRUE);
			bRun = false;
      bAutomation = false;
			break;
		}
		else if((lstrcmpi(lpszToken, _T("Automation")) == 0) ||
			(lstrcmpi(lpszToken, _T("Embedding")) == 0))
		{
      bRun = true;
			bAutomation = true;
			break;
		}
		lpszToken = _Module.FindOneOf(lpszToken, szTokens);
	}

	if(bRun)
	{
    HMODULE hInstRich = ::LoadLibrary(CRichEditCtrl::GetLibraryName());
    ATLASSERT(hInstRich != NULL);

    _Module.StartMonitor();
		hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
		ATLASSERT(SUCCEEDED(hRes));
		hRes = ::CoResumeClassObjects();
		ATLASSERT(SUCCEEDED(hRes));

		nRet = Run(lpstrCmdLine, nCmdShow);
    ::FreeLibrary(hInstRich);

		_Module.RevokeClassObjects();
    ::Sleep(_Module.m_dwPause);
  }

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
