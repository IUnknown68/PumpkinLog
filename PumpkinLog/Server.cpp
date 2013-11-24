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
    instance = it->second;
    logger = it->second;
  }
  else {
    Logger::_ComObject * newLogger = NULL;
    HRESULT hr = Logger::_ComObject::CreateInstance(&newLogger);
    if (FAILED(hr)) {
      return hr;
    }
    instance = newLogger;
    logger = newLogger;
/*
    CComPtr<ILogBucketContainer> logDestination;
    hr = getBucket(aName, &logDestination.p);
    //hr = getLogWindow(aOptions, &logDestination.p, TRUE);
    if (FAILED(hr)) {
      return hr;
    }
*/
    IF_FAILED_RET(logger->init(aName, this));
    mLoggers[aName] = logger;
  }
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
STDMETHODIMP Server::getBucket(LPCWSTR aUri, ILogBucketContainer ** aRetVal)
{
  if (!aRetVal) {
    return E_POINTER;
  }
  CComPtr<ILogBucketContainer> bucket;
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

/*
//----------------------------------------------------------------------------
//  onWindowClose
STDMETHODIMP Server::onWindowClose(BSTR aName)
{
  mWindows.erase(aName);
  return S_OK;
}
*/

/*
HRESULT Server::getLogWindow(VARIANT aOptions, ILogBucket ** aRetVal, BOOL aEnsureExists)
{
  if (!aRetVal) {
    return E_POINTER;
  }

  CComPtr<ILogBucket> instance;
  CComVariant vtName;
  vtName.ChangeType(VT_BSTR, &aOptions);
  std::wstring name = (VT_BSTR == vtName.vt) ? vtName.bstrVal : L"";

  auto it = mWindows.find(name);
  if (it != mWindows.end()) {
    instance = it->second;
  }
  else {
    LogWindow::_ComObject * window = NULL;
    HRESULT hr = LogWindow::_ComObject::CreateInstance(&window);
    if (FAILED(hr)) {
      return hr;
    }
    instance = window;

    hr = window->init(name.c_str(), this);
    if (FAILED(hr)) {
      return hr;
    }
    mWindows[name] = window;
  }
  (*aRetVal) = instance.Detach();
  return S_OK;
}
*/

HRESULT Server::createBucket(LPCWSTR aUri, CComPtr<ILogBucketContainer> & aRetVal)
{
  CComPtr<IUri> uri;
  IF_FAILED_RET(CreateUri(aUri, Uri_CREATE_CANONICALIZE, 0, &uri.p));

  CComBSTR scheme;
  IF_FAILED_RET(uri->GetSchemeName(&scheme));
  if (scheme == L"window") {
    /////////////////////// WINDOW
    // create container
    Container::_ComObject * newContainer = NULL;
    IF_FAILED_RET(Container::_ComObject::CreateInstance(&newContainer));
    CComPtr<ILogBucketContainer> containerOwner(newContainer);


    // create target
    LogWindow::_ComObject * newWindow = NULL;
    IF_FAILED_RET(LogWindow::_ComObject::CreateInstance(&newWindow));
    CComPtr<ILogBucket> windowOwner(newWindow);

    // init container
    IF_FAILED_RET(newContainer->init(aUri, windowOwner, this));

    // init window
    IF_FAILED_RET(newWindow->init(aUri, containerOwner, this));

    // store
    mBuckets[aUri] = containerOwner;
    aRetVal = containerOwner;
    return S_OK;

    /////////////////////// WINDOW
  }

  return INET_E_UNKNOWN_PROTOCOL;
}


} // namespace PumpkinLog
