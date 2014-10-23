// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

#define WINVER		    0x0600
#define _WIN32_WINNT	0x0600
#define _WIN32_IE	    0x0700
#define _RICHEDIT_VER	0x0200

#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
#include <atlstr.h>
#include <atlapp.h>

extern CServerAppModule _Module;

// This is here only to tell VC7 Class Wizard this is an ATL project
#ifdef ___VC7_CLWIZ_ONLY___
CComModule
CExeModule
#endif

#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <atltime.h>
#include <atlsafe.h>
#include <atlfile.h>
#include <Urlmon.h>

#include <string>
#include <map>
#include <unordered_map>
#include <functional>

#include <ajvar.h>

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#define EXPECTED_(_val) \
  if (!_val) { \
    return E_UNEXPECTED; \
  }
