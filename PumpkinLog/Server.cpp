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
    IF_FAILED_RET(logger->init(aName, this));
    // set options: only one time!
    IF_FAILED_RET(logger->setOptions(pVals));
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
  CComPtr<ILogBucket> bucket;
  auto it = mBuckets.find(aUri);
  if (it == mBuckets.end()) {
    // not found, create
    // Get the scheme name.
    // Basic part of every configuration is a URI. It provides the follwing information:
    // 1) Scheme
    //    

    CComBSTR schemeName;
    HRESULT hr = E_FAIL;
    CComPtr<IUri> uri;
    if (aUri) {
      // parse URI
      hr = CreateUri(aUri, CREATE_URI_FLAGS, 0, &uri);
      if (FAILED(hr)) {
        return hr;
      }
      // get scheme
      hr = uri->GetSchemeName(&schemeName);
      if (FAILED(hr)) {
        return hr;
      }
    }

    // handle scheme
    TCreateLogBucket CreateLogBucket = GetLogBucketCreator(schemeName);
    hr = (*this.*CreateLogBucket)(aUri, bucket);
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
//  CreateLogBucket_window
HRESULT Server::CreateLogBucket_window(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal) {
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
//  CreateLogBucket_xml
HRESULT Server::CreateLogBucket_xml(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal) {
  return S_OK;
}

//----------------------------------------------------------------------------
//  CreateLogBucket_file
HRESULT Server::CreateLogBucket_file(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal) {
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
//  CreateLogBucket__default
HRESULT Server::CreateLogBucket__default(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal) {
  return CreateLogBucket_window(aUri, aRetVal);
}

} // namespace PumpkinLog
