// Server.cpp : Implementation of Server

#include "stdafx.h"
#include "Server.h"
#include "LogWindow.h"
#include "LogBucket/FileBucket.h"

// Server
namespace PumpkinLog {

#ifdef Uri_CREATE_NORMALIZE_INTL_CHARACTERS
#  define CREATE_URI_FLAGS  (Uri_CREATE_CANONICALIZE|Uri_CREATE_DECODE_EXTRA_INFO|Uri_CREATE_CRACK_UNKNOWN_SCHEMES|Uri_CREATE_NO_IE_SETTINGS|Uri_CREATE_NORMALIZE_INTL_CHARACTERS)
#else
#  define CREATE_URI_FLAGS  (Uri_CREATE_CANONICALIZE|Uri_CREATE_DECODE_EXTRA_INFO|Uri_CREATE_CRACK_UNKNOWN_SCHEMES|Uri_CREATE_NO_IE_SETTINGS)
#endif

using namespace LogBucket;

STDMETHODIMP Server::createLogger(BSTR aName, VARIANT aOptions, IDispatch ** aRetVal)
{
  if (!aRetVal) {
    return E_POINTER;
  }

  // parse URI
  CComPtr<IUri> uri;
  HRESULT hr = CreateUri(aName, CREATE_URI_FLAGS, 0, &uri);
  if (FAILED(hr)) {
    return hr;
  }
  // get scheme
  CComBSTR schemeName;
  hr = uri->GetSchemeName(&schemeName);
  if (FAILED(hr)) {
    return hr;
  }

  // handle scheme
  Scheme scheme = GetScheme(schemeName);
  hr = ((*this).*scheme)();


  CComPtr<IDispatch> instance;

  auto it = mLoggers.find(aName);
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
    IF_FAILED_RET(logger->init(aName, this));
    mLoggers[aName] = logger;
  }
  // set options: multiple times init
  IF_FAILED_RET(logger->setOptions(aOptions));
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
  CComPtr<ILogBucket> bucket;
  auto it = mBuckets.find(aUri);
  if (it == mBuckets.end()) {
    // not found, create
    HRESULT hr = createBucket(aUri, bucket);
    if (FAILED(hr)) {
      return hr;
    }
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

//----------------------------------------------------------------------------
//  createLogWindow
HRESULT Server::createLogWindow(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal)
{
  // create container
  Container::_ComObject * newContainer = NULL;
  IF_FAILED_RET(Container::_ComObject::CreateInstance(&newContainer));
  CComPtr<ILogBucket> containerOwner(newContainer);

  // create target
  LogWindow::_ComObject * newTarget = NULL;
  IF_FAILED_RET(LogWindow::_ComObject::CreateInstance(&newTarget));
  CComPtr<ILogBucket> targetOwner(newTarget);

  // init container
  IF_FAILED_RET(newContainer->init(aUri, targetOwner, this));

  aRetVal = containerOwner;
  return S_OK;
}

//----------------------------------------------------------------------------
//  createLogFile
HRESULT Server::createLogFile(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal)
{
  // create container
  Container::_ComObject * newContainer = NULL;
  IF_FAILED_RET(Container::_ComObject::CreateInstance(&newContainer));
  CComPtr<ILogBucket> containerOwner(newContainer);

  // create target
  FileBucket::_ComObject * newTarget = NULL;
  IF_FAILED_RET(FileBucket::_ComObject::CreateInstance(&newTarget));
  CComPtr<ILogBucket> targetOwner(newTarget);

  // init container
  IF_FAILED_RET(newContainer->init(aUri, targetOwner, this));

  aRetVal = containerOwner;
  return S_OK;
}

//----------------------------------------------------------------------------
//  createBucket
HRESULT Server::createBucket(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal)
{
  aRetVal.Release();
  CComPtr<IUri> uri;
  IF_FAILED_RET(CreateUri(aUri, Uri_CREATE_CANONICALIZE, 0, &uri.p));

  CComBSTR scheme;
  IF_FAILED_RET(uri->GetSchemeName(&scheme));
  if (scheme == L"window") {
    IF_FAILED_RET(createLogWindow(aUri, aRetVal));
  }
  else if (scheme == L"file") {
    IF_FAILED_RET(createLogFile(aUri, aRetVal));
  }
  if (aRetVal) {
    mBuckets[aUri] = aRetVal;
    return S_OK;
  }

  return INET_E_UNKNOWN_PROTOCOL;
}


} // namespace PumpkinLog
