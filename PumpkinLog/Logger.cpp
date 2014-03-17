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

HRESULT Logger::init(LPCWSTR aName, ILogServerInternal * aLogServer)
{
  mName = aName;
  mServer = aLogServer;
  return S_OK;
}

HRESULT Logger::setOptions(VARIANT aOptions)
{
  if (VT_EMPTY == aOptions.vt) {
    return S_OK;
  }
  EXPECTED_(mServer);
/*
  CStringW bucketUri;
  CComVariant vtOptions;
  HRESULT hr = vtOptions.ChangeType(VT_BSTR, &aOptions);
  if (SUCCEEDED(hr)) {
    bucketUri = vtOptions.bstrVal;
  }
  else {
    bucketUri = L"window://Default";
  }
  CComPtr<ILogBucket> container;
  IF_FAILED_RET(mServer->getBucket(bucketUri, &container));

  if (mLogDestination.IsEqualObject(container)) {
    // no change
    return S_OK;
  }

  // cleanup old bucket
  if (mLogDestination) {
    mLogDestination->removeRefLogger(mName);
  }

  mLogDestination = container;
  mLogDestination->addRefLogger(mName);
*/
  LPCWSTR bucketURIs[] = {
    L"file://U:\\Users\\Hans\\Documents\\Visual Studio 2010\\Projects\\PumpkinLog\\test.log",
    L"window://Default"
  };
  LogBucketMap newBuckets;
  for (int n = 0; n < 2; n++) {
    auto it = mBuckets.find(bucketURIs[n]);
    if (it == mBuckets.end()) {
      CComPtr<ILogBucket> container;
      IF_FAILED_RET(mServer->getBucket(bucketURIs[n], &container));
      newBuckets[bucketURIs[n]] = container;
      container->addRefLogger(mName);
    }
    else {
      newBuckets[bucketURIs[n]] = it->second;
    }
  }
  mBuckets = newBuckets;
  return S_OK;
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
  ar[1] = aVal1;
  ar[0] = aVal2;
  return doLog(LT_LOG, ar);
}

//----------------------------------------------------------------------------
//  debug2
STDMETHODIMP Logger::debug2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[1] = aVal1;
  ar[0] = aVal2;
  return doLog(LT_DEBUG, ar);
}

//----------------------------------------------------------------------------
//  info2
STDMETHODIMP Logger::info2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[1] = aVal1;
  ar[0] = aVal2;
  return doLog(LT_INFO, ar);
}

//----------------------------------------------------------------------------
//  warn2
STDMETHODIMP Logger::warn2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[1] = aVal1;
  ar[0] = aVal2;
  return doLog(LT_WARN, ar);
}

//----------------------------------------------------------------------------
//  error2
STDMETHODIMP Logger::error2(
  VARIANT aVal1, VARIANT aVal2)
{
  CComSafeArray<VARIANT> ar(2);
  ar[1] = aVal1;
  ar[0] = aVal2;
  return doLog(LT_ERROR, ar);
}

//----------------------------------------------------------------------------
//  log3
STDMETHODIMP Logger::log3(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3)
{
  CComSafeArray<VARIANT> ar(3);
  ar[2] = aVal1;
  ar[1] = aVal2;
  ar[0] = aVal3;
  return doLog(LT_LOG, ar);
}

//----------------------------------------------------------------------------
//  debug3
STDMETHODIMP Logger::debug3(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3)
{
  CComSafeArray<VARIANT> ar(3);
  ar[2] = aVal1;
  ar[1] = aVal2;
  ar[0] = aVal3;
  return doLog(LT_DEBUG, ar);
}

//----------------------------------------------------------------------------
//  info3
STDMETHODIMP Logger::info3(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3)
{
  CComSafeArray<VARIANT> ar(3);
  ar[2] = aVal1;
  ar[1] = aVal2;
  ar[0] = aVal3;
  return doLog(LT_INFO, ar);
}

//----------------------------------------------------------------------------
//  warn3
STDMETHODIMP Logger::warn3(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3)
{
  CComSafeArray<VARIANT> ar(3);
  ar[2] = aVal1;
  ar[1] = aVal2;
  ar[0] = aVal3;
  return doLog(LT_WARN, ar);
}

//----------------------------------------------------------------------------
//  error3
STDMETHODIMP Logger::error3(
  VARIANT aVal1, VARIANT aVal2, VARIANT aVal3)
{
  CComSafeArray<VARIANT> ar(3);
  ar[2] = aVal1;
  ar[1] = aVal2;
  ar[0] = aVal3;
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
