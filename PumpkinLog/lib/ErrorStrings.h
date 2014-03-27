#pragma once

LPCTSTR DErrFACILITY(HRESULT aError);
LPCTSTR DErrWin32(LRESULT aError);
LPCTSTR DErrSEVERITY(HRESULT aError);
LPCTSTR DErrHRESULT(HRESULT aError);

typedef LPCTSTR (*DErr_type)(DWORD);

extern DErr_type DErrFACILITYDefault;
extern DErr_type DErrWin32Default;
extern DErr_type DErrSEVERITYDefault;
extern DErr_type DErrHRESULTDefault;
