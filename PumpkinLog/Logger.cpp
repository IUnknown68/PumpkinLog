// Logger.cpp : Implementation of Logger

#include "stdafx.h"
#include "Server.h"
#include "Logger.h"


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
        IF_FAILED_RET(mServer->getBucket(bucketUri.c_str(), &container));
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
STDMETHODIMP Logger::log(
  SAFEARRAY* pVals)
{
  return doLog(LT_LOG, pVals);
}

//----------------------------------------------------------------------------
//  debug
STDMETHODIMP Logger::debug(
  SAFEARRAY* pVals)
{
  return doLog(LT_DEBUG, pVals);
}

//----------------------------------------------------------------------------
//  info
STDMETHODIMP Logger::info(
  SAFEARRAY* pVals)
{
  return doLog(LT_INFO, pVals);
}

//----------------------------------------------------------------------------
//  warn
STDMETHODIMP Logger::warn(
  SAFEARRAY* pVals)
{
  return doLog(LT_WARN, pVals);
}

//----------------------------------------------------------------------------
//  error
STDMETHODIMP Logger::error(
  SAFEARRAY* pVals)
{
  return doLog(LT_ERROR, pVals);
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
  return doLog(LT_LOG, ar);
}

//----------------------------------------------------------------------------
//  debug1
STDMETHODIMP Logger::debug1(
  VARIANT aVal1)
{
  CComSafeArray<VARIANT> ar(1);
  ar[0] = aVal1;
  return doLog(LT_DEBUG, ar);
}

//----------------------------------------------------------------------------
//  info1
STDMETHODIMP Logger::info1(
  VARIANT aVal1)
{
  CComSafeArray<VARIANT> ar(1);
  ar[0] = aVal1;
  return doLog(LT_INFO, ar);
}

//----------------------------------------------------------------------------
//  warn1
STDMETHODIMP Logger::warn1(
  VARIANT aVal1)
{
  CComSafeArray<VARIANT> ar(1);
  ar[0] = aVal1;
  return doLog(LT_WARN, ar);
}

//----------------------------------------------------------------------------
//  error1
STDMETHODIMP Logger::error1(
  VARIANT aVal1)
{
  CComSafeArray<VARIANT> ar(1);
  ar[0] = aVal1;
  return doLog(LT_ERROR, ar);
}

//----------------------------------------------------------------------------
//  log2
STDMETHODIMP Logger::log2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[0] = aVal1;
  ar[1] = aVal2;
  return doLog(LT_LOG, ar);
}

//----------------------------------------------------------------------------
//  debug2
STDMETHODIMP Logger::debug2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[0] = aVal1;
  ar[1] = aVal2;
  return doLog(LT_DEBUG, ar);
}

//----------------------------------------------------------------------------
//  info2
STDMETHODIMP Logger::info2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[0] = aVal1;
  ar[1] = aVal2;
  return doLog(LT_INFO, ar);
}

//----------------------------------------------------------------------------
//  warn2
STDMETHODIMP Logger::warn2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[0] = aVal1;
  ar[1] = aVal2;
  return doLog(LT_WARN, ar);
}

//----------------------------------------------------------------------------
//  error2
STDMETHODIMP Logger::error2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[0] = aVal1;
  ar[1] = aVal2;
  return doLog(LT_ERROR, ar);
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
  return doLog(LT_LOG, ar);
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
  return doLog(LT_DEBUG, ar);
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
  return doLog(LT_INFO, ar);
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
  return doLog(LT_WARN, ar);
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
  return doLog(LT_ERROR, ar);
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
  return doLog(LT_LOG, ar);
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
  return doLog(LT_DEBUG, ar);
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
  return doLog(LT_INFO, ar);
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
  return doLog(LT_WARN, ar);
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
  return doLog(LT_ERROR, ar);
}

//----------------------------------------------------------------------------
//  log
HRESULT Logger::doLog(LogFacility aFacility, SAFEARRAY * pVals)
{
  for (auto it = mBuckets.begin(); it != mBuckets.end(); ++it) {
    it->second->onLoggerLog(aFacility, mName, pVals);
  }
  return S_OK;
}

} // namespace PumpkinLog
