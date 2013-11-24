// Server.cpp : Implementation of Server

#include "stdafx.h"
#include "Server.h"

// Server
namespace PumpkinLog {

using namespace LogBucket;

STDMETHODIMP Server::createLogger(BSTR aName, VARIANT aOptions, IDispatch ** aRetVal)
{
  if (!aRetVal) {
    return E_POINTER;
  }

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
  LogWindow::_ComObject * newWindow = NULL;
  IF_FAILED_RET(LogWindow::_ComObject::CreateInstance(&newWindow));
  CComPtr<ILogBucket> windowOwner(newWindow);

  // init container
  IF_FAILED_RET(newContainer->init(aUri, windowOwner, this));

  aRetVal = containerOwner;
  return S_OK;
}

//----------------------------------------------------------------------------
//  createBucket
HRESULT Server::createBucket(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal)
{
  CComPtr<IUri> uri;
  IF_FAILED_RET(CreateUri(aUri, Uri_CREATE_CANONICALIZE, 0, &uri.p));

  CComBSTR scheme;
  IF_FAILED_RET(uri->GetSchemeName(&scheme));
  if (scheme == L"window") {
    /////////////////////// WINDOW
    // create
    IF_FAILED_RET(createLogWindow(aUri, aRetVal));
    // store
    mBuckets[aUri] = aRetVal;
    return S_OK;
    /////////////////////// WINDOW
  }

  return INET_E_UNKNOWN_PROTOCOL;
}


} // namespace PumpkinLog
