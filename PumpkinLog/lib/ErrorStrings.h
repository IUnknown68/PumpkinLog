#pragma once


// by default all errorstrings are enabled for debug builds
#ifdef _DEBUG
#  define DISPLAY_ALL_ERRORSTRINGS
#endif
// and disabled enabled for release builds.
// If you want them enabled #define them before including this file

#ifdef DISPLAY_ALL_ERRORSTRINGS
#ifndef DISPLAY_FACILITY
#  define DISPLAY_FACILITY
#endif
#ifndef DISPLAY_FACILITY
#  define DISPLAY_WIN32_ERR
#endif
#ifndef DISPLAY_FACILITY
#  define DISPLAY_SEVERITY
#endif
#ifndef DISPLAY_FACILITY
#  define DISPLAY_HRESULT
#endif
#endif

LPCTSTR DErrFACILITY(HRESULT aError);
LPCTSTR DErrWin32(LRESULT aError);
LPCTSTR DErrSEVERITY(HRESULT aError);
LPCTSTR DErrHRESULT(HRESULT aError);

typedef LPCTSTR (*DErr_type)(DWORD);

extern DErr_type DErrFACILITYDefault;
extern DErr_type DErrWin32Default;
extern DErr_type DErrSEVERITYDefault;
extern DErr_type DErrHRESULTDefault;
