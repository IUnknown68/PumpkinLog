#include "StdAfx.h"
#include "FileBucket.h"

namespace PumpkinLog {
namespace LogBucket {


HRESULT FileBucket::FinalConstruct()
{
  return S_OK;
}

void FileBucket::FinalRelease()
{
  mFile.Close();
}

//----------------------------------------------------------------------------
//  init
STDMETHODIMP FileBucket::init(LPCWSTR aUri, ILogBucket * aContainer, ILogServerInternal * aLogServer)
{
  mName = aUri;
  CStringW filePath;
  DWORD resultLength = 0;
  IF_FAILED_RET_HR(CoInternetParseUrl(aUri, PARSE_PATH_FROM_URL, 0, filePath.GetBuffer(MAX_PATH), MAX_PATH, &resultLength, 0));
  filePath.ReleaseBuffer();

  IF_FAILED_RET_HR(mFile.Create(filePath, GENERIC_WRITE, FILE_SHARE_WRITE, OPEN_ALWAYS));
  mFile.Seek(0, FILE_END);

  return S_OK;
}

//----------------------------------------------------------------------------
//  addRefLogger
STDMETHODIMP_(ULONG) FileBucket::addRefLogger(LPCWSTR aName)
{
  return 1;
}

//----------------------------------------------------------------------------
//  removeRefLogger
STDMETHODIMP_(ULONG) FileBucket::removeRefLogger(LPCWSTR aName)
{
  return 0;
}

//----------------------------------------------------------------------------
//  onLoggerLog
STDMETHODIMP FileBucket::onLoggerLog(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals, LPDISPATCH pOptions)
{
  EXPECTED_(mFile);

  ATLASSERT(0 == pVals->rgsabound[0].lLbound);
  VARIANT* pVariants = (VARIANT*)pVals->pvData;
  for(ULONG n = 0; n < pVals->rgsabound[0].cElements; n++)
  {
    CComVariant vt;
    HRESULT hr = vt.ChangeType(VT_BSTR, &pVariants[pVals->rgsabound[0].cElements - n - 1]);
    if (FAILED(hr)) {
      return S_OK;
    }
    CStringA logStr = CW2A(vt.bstrVal, CP_UTF8);
    hr = mFile.Write(logStr.GetBuffer(logStr.GetLength()), logStr.GetLength());
    if (FAILED(hr)) {
      return S_OK;
    }
    hr = mFile.Write(" ", 1);
    if (FAILED(hr)) {
      return S_OK;
    }
  }

  mFile.Write("\n", 1);

  return S_OK;
}

} // namespace LogBucket
} // namespace PumpkinLog
