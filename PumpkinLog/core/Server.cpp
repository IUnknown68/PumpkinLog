// Server.cpp : Implementation of Server

#include "stdafx.h"
#include "Server.h"

// Server
namespace PumpkinLog {

#ifdef Uri_CREATE_NORMALIZE_INTL_CHARACTERS
#  define CREATE_URI_FLAGS  (Uri_CREATE_CANONICALIZE|Uri_CREATE_DECODE_EXTRA_INFO|Uri_CREATE_CRACK_UNKNOWN_SCHEMES|Uri_CREATE_NO_IE_SETTINGS|Uri_CREATE_NORMALIZE_INTL_CHARACTERS)
#else
#  define CREATE_URI_FLAGS  (Uri_CREATE_CANONICALIZE|Uri_CREATE_DECODE_EXTRA_INFO|Uri_CREATE_CRACK_UNKNOWN_SCHEMES|Uri_CREATE_NO_IE_SETTINGS)
#endif

using namespace LogBucket;

STDMETHODIMP Server::createLogger(BSTR aName, SAFEARRAY* pVals, IDispatch ** aRetVal)
{
  if (!aRetVal) {
    return E_POINTER;
  }

  CComPtr<IDispatch> instance;
  std::wstring name = aName;

  auto it = mLoggers.find(name);
  Logger * logger = NULL;
  if (it != mLoggers.end()) {
    // logger found
    instance = it->second;
    logger = it->second;
  }
  else {
    // not found, create new logger
    Logger::_ComObject * newLogger = NULL;
    HRESULT hr = Logger::_ComObject::CreateInstance(&newLogger);
    if (FAILED(hr)) {
      return hr;
    }
    instance = newLogger;
    logger = newLogger;

    // one-time init
    IF_FAILED_RET_HR(logger->init(aName, pVals, this));
    mLoggers[name] = logger;
  }
  (*aRetVal) = instance.Detach();
  return S_OK;
}

//----------------------------------------------------------------------------
//  onLoggerQuit
STDMETHODIMP Server::onLoggerQuit(BSTR aName)
{
  mLoggers.erase(aName);
  return S_OK;
}

//----------------------------------------------------------------------------
//  getBucket
STDMETHODIMP Server::getBucket(LPCWSTR aUri, ILogBucket ** aRetVal)
{
  if (!aRetVal) {
    return E_POINTER;
  }
  std::wstring uri = aUri;
  CComPtr<ILogBucket> bucket;
  auto it = mBuckets.find(uri);
  if (it == mBuckets.end()) {
    // not found, create

    CComPtr<IUri> parsedURI;
    CComBSTR schemeName;
    HRESULT hr = E_FAIL;
    if (!aUri) {
      return E_INVALIDARG;
    }

    // parse URI
    hr = CreateUri(aUri, CREATE_URI_FLAGS, 0, &parsedURI);
    if (FAILED(hr)) {
      return hr;
    }

    // get scheme
    hr = parsedURI->GetSchemeName(&schemeName);
    if (FAILED(hr)) {
      return hr;
    }

    // find appropriate creator
    TCreateBucket CreateBucket = &Server::CreateBucket<LogWindow>;
    for (LogBucketEntry * entry = GetBucketCreatorMap(); entry->CreateBucket; entry++) {
      if (0 == wcscmp(entry->name, schemeName)) {
        CreateBucket = entry->CreateBucket;
        break;  // found
      }
    }

    // and create bucket
    hr = ((*this).*CreateBucket)(aUri, bucket);
    if (FAILED(hr)) {
      return hr;
    }
    mBuckets[uri] = bucket;
  }
  else {
    // found
    bucket = it->second;
  }
  (*aRetVal) = bucket.Detach();
  return (*aRetVal) ? S_OK : E_FAIL;
}

//----------------------------------------------------------------------------
//  onBucketGone
STDMETHODIMP Server::onBucketGone(LPCWSTR aUri)
{
  mBuckets.erase(aUri);
  return S_OK;
}

} // namespace PumpkinLog
