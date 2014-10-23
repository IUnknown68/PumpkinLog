// Logger.cpp : Implementation of Logger

#include "stdafx.h"
#include "Server.h"
#include "Logger.h"

using namespace Ajvar::Dispatch;

namespace PumpkinLog {

// Logger

HRESULT Logger::FinalConstruct()
{
	return S_OK;
}

void Logger::FinalRelease()
{
  for (auto it = mBuckets.begin(); it != mBuckets.end(); ++it) {
    it->second->removeRefLogger(mName);
  }
  mBuckets.clear();

  // remove the logger from the server
  if (mServer) {
    mServer->onLoggerQuit(CComBSTR(mName));
  }
}

HRESULT Logger::init(LPCWSTR aName, SAFEARRAY* aLogBucketURIs, ILogServerInternal * aLogServer)
{
  mName = aName;
  mServer = aLogServer;

  ATL::CComSafeArray<VARIANT> values;
  HRESULT hr = values.Attach(aLogBucketURIs);
  if (FAILED(hr)) {
    return hr;
  }

  // lambda to ensure values.Detach() is called
  hr = [&] () -> HRESULT {
    for (ULONG i = 0; i < values.GetCount(); ++i) {
      // get URI i
      auto value = values.GetAt(i);
      // ensure it is a string
      CComVariant vtURI;
      HRESULT hr = vtURI.ChangeType(VT_BSTR, &value);
      if (FAILED(hr)) {
        return DISP_E_TYPEMISMATCH;
      }
      std::wstring bucketUri = vtURI.bstrVal;

      auto it = mBuckets.find(bucketUri);
      if (it == mBuckets.end()) {
        // not found: get from server
        CComPtr<ILogBucket> container;
        // ask server
        IF_FAILED_RET_HR(mServer->getBucket(bucketUri.c_str(), &container));
        mBuckets[bucketUri] = container;
        container->addRefLogger(mName);
      }
    }
    return S_OK;
  }();
  // prevent destruction of aLogBucketURIs
  values.Detach();

  return hr;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// ILoggerDispatch implementation

//----------------------------------------------------------------------------
//  log
STDMETHODIMP Logger::llLog(
  SAFEARRAY* pVals)
{
  return doLog(LT_CUSTOM, pVals, nullptr);
}

//----------------------------------------------------------------------------
//  log
STDMETHODIMP Logger::log(
  SAFEARRAY* pVals)
{
  return doLog(LT_LOG, pVals, nullptr);
}

//----------------------------------------------------------------------------
//  debug
STDMETHODIMP Logger::debug(
  SAFEARRAY* pVals)
{
  return doLog(LT_DEBUG, pVals, nullptr);
}

//----------------------------------------------------------------------------
//  info
STDMETHODIMP Logger::info(
  SAFEARRAY* pVals)
{
  return doLog(LT_INFO, pVals, nullptr);
}

//----------------------------------------------------------------------------
//  warn
STDMETHODIMP Logger::warn(
  SAFEARRAY* pVals)
{
  return doLog(LT_WARN, pVals, nullptr);
}

//----------------------------------------------------------------------------
//  error
STDMETHODIMP Logger::error(
  SAFEARRAY* pVals)
{
  return doLog(LT_ERROR, pVals, nullptr);
}

//----------------------------------------------------------------------------
//  loglow
STDMETHODIMP Logger::loglow(
  LONG nFacility,
  LPDISPATCH pArVals)
{
  Ex::ObjectGet arguments(pArVals);
  if (!arguments) {
    return S_OK;
  }

  auto length = arguments[L"length"];
  length.ChangeType(VT_I4);
  if (!length.lVal) {
    return S_OK;
  }

  CComSafeArray<VARIANT> ar(length.lVal);
  for(long n = 0; n < length.lVal; n++) {
    CStringW nS;
    nS.Format(L"%i", n);
    ar[n] = arguments[nS];
  }

  if (nFacility < LT_LOG || nFacility > LT_ERROR) {
    nFacility = LT_LOG;
  }
  return doLog((LogFacility)nFacility, ar, nullptr);
}

//----------------------------------------------------------------------------
// ILogger implementation

//----------------------------------------------------------------------------
//  log1
STDMETHODIMP Logger::log1(
  VARIANT aVal1)
{
  CComSafeArray<VARIANT> ar(1);
  ar[0] = aVal1;
  return doLog(LT_LOG, ar, nullptr);
}

//----------------------------------------------------------------------------
//  debug1
STDMETHODIMP Logger::debug1(
  VARIANT aVal1)
{
  CComSafeArray<VARIANT> ar(1);
  ar[0] = aVal1;
  return doLog(LT_DEBUG, ar, nullptr);
}

//----------------------------------------------------------------------------
//  info1
STDMETHODIMP Logger::info1(
  VARIANT aVal1)
{
  CComSafeArray<VARIANT> ar(1);
  ar[0] = aVal1;
  return doLog(LT_INFO, ar, nullptr);
}

//----------------------------------------------------------------------------
//  warn1
STDMETHODIMP Logger::warn1(
  VARIANT aVal1)
{
  CComSafeArray<VARIANT> ar(1);
  ar[0] = aVal1;
  return doLog(LT_WARN, ar, nullptr);
}

//----------------------------------------------------------------------------
//  error1
STDMETHODIMP Logger::error1(
  VARIANT aVal1)
{
  CComSafeArray<VARIANT> ar(1);
  ar[0] = aVal1;
  return doLog(LT_ERROR, ar, nullptr);
}

//----------------------------------------------------------------------------
//  log2
STDMETHODIMP Logger::log2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[0] = aVal1;
  ar[1] = aVal2;
  return doLog(LT_LOG, ar, nullptr);
}

//----------------------------------------------------------------------------
//  debug2
STDMETHODIMP Logger::debug2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[0] = aVal1;
  ar[1] = aVal2;
  return doLog(LT_DEBUG, ar, nullptr);
}

//----------------------------------------------------------------------------
//  info2
STDMETHODIMP Logger::info2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[0] = aVal1;
  ar[1] = aVal2;
  return doLog(LT_INFO, ar, nullptr);
}

//----------------------------------------------------------------------------
//  warn2
STDMETHODIMP Logger::warn2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[0] = aVal1;
  ar[1] = aVal2;
  return doLog(LT_WARN, ar, nullptr);
}

//----------------------------------------------------------------------------
//  error2
STDMETHODIMP Logger::error2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[0] = aVal1;
  ar[1] = aVal2;
  return doLog(LT_ERROR, ar, nullptr);
}

//----------------------------------------------------------------------------
//  log3
STDMETHODIMP Logger::log3(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3)
{
  CComSafeArray<VARIANT> ar(3);
  ar[0] = aVal1;
  ar[1] = aVal2;
  ar[2] = aVal3;
  return doLog(LT_LOG, ar, nullptr);
}

//----------------------------------------------------------------------------
//  debug3
STDMETHODIMP Logger::debug3(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3)
{
  CComSafeArray<VARIANT> ar(3);
  ar[0] = aVal1;
  ar[1] = aVal2;
  ar[2] = aVal3;
  return doLog(LT_DEBUG, ar, nullptr);
}

//----------------------------------------------------------------------------
//  info3
STDMETHODIMP Logger::info3(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3)
{
  CComSafeArray<VARIANT> ar(3);
  ar[0] = aVal1;
  ar[1] = aVal2;
  ar[2] = aVal3;
  return doLog(LT_INFO, ar, nullptr);
}

//----------------------------------------------------------------------------
//  warn3
STDMETHODIMP Logger::warn3(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3)
{
  CComSafeArray<VARIANT> ar(3);
  ar[0] = aVal1;
  ar[1] = aVal2;
  ar[2] = aVal3;
  return doLog(LT_WARN, ar, nullptr);
}

//----------------------------------------------------------------------------
//  error3
STDMETHODIMP Logger::error3(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3)
{
  CComSafeArray<VARIANT> ar(3);
  ar[0] = aVal1;
  ar[1] = aVal2;
  ar[2] = aVal3;
  return doLog(LT_ERROR, ar, nullptr);
}

//----------------------------------------------------------------------------
//  log4
STDMETHODIMP Logger::log4(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3, VARIANT aVal4)
{
  CComSafeArray<VARIANT> ar(4);
  ar[0] = aVal1;
  ar[1] = aVal2;
  ar[2] = aVal3;
  ar[3] = aVal4;
  return doLog(LT_LOG, ar, nullptr);
}

//----------------------------------------------------------------------------
//  debug4
STDMETHODIMP Logger::debug4(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3, VARIANT aVal4)
{
  CComSafeArray<VARIANT> ar(4);
  ar[0] = aVal1;
  ar[1] = aVal2;
  ar[2] = aVal3;
  ar[3] = aVal4;
  return doLog(LT_DEBUG, ar, nullptr);
}

//----------------------------------------------------------------------------
//  info4
STDMETHODIMP Logger::info4(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3, VARIANT aVal4)
{
  CComSafeArray<VARIANT> ar(4);
  ar[0] = aVal1;
  ar[1] = aVal2;
  ar[2] = aVal3;
  ar[3] = aVal4;
  return doLog(LT_INFO, ar, nullptr);
}

//----------------------------------------------------------------------------
//  warn4
STDMETHODIMP Logger::warn4(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3, VARIANT aVal4)
{
  CComSafeArray<VARIANT> ar(4);
  ar[0] = aVal1;
  ar[1] = aVal2;
  ar[2] = aVal3;
  ar[3] = aVal4;
  return doLog(LT_WARN, ar, nullptr);
}

//----------------------------------------------------------------------------
//  error4
STDMETHODIMP Logger::error4(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3, VARIANT aVal4)
{
  CComSafeArray<VARIANT> ar(4);
  ar[0] = aVal1;
  ar[1] = aVal2;
  ar[2] = aVal3;
  ar[3] = aVal4;
  return doLog(LT_ERROR, ar, nullptr);
}

//----------------------------------------------------------------------------
//  doLog
HRESULT Logger::doLog(LogFacility aFacility, SAFEARRAY * pVals, LPDISPATCH pOptions)
{
  for (auto it = mBuckets.begin(); it != mBuckets.end(); ++it) {
    it->second->onLoggerLog(aFacility, mName, pVals, pOptions);
  }
  return S_OK;
}

} // namespace PumpkinLog
