// Server.cpp : Implementation of Server

#include "stdafx.h"
#include "Server.h"

// Server
namespace PumpkinLog {

STDMETHODIMP Server::createLogger(BSTR aName, VARIANT aWindowName, IDispatch ** aRetVal)
{
  if (!aRetVal) {
    return E_POINTER;
  }

  CComPtr<IDispatch> instance;

  auto it = mLoggers.find(aName);
  if (it != mLoggers.end()) {
    instance = it->second;
  }
  else {
    Logger::_ComObject * logger = NULL;
    HRESULT hr = Logger::_ComObject::CreateInstance(&logger);
    if (FAILED(hr)) {
      return hr;
    }
    instance = logger;

    CComPtr<ILoggerInternal> logDestination;
    hr = getLogWindow(aWindowName, &logDestination.p, TRUE);
    if (FAILED(hr)) {
      return hr;
    }

    hr = logger->init(aName, logDestination, this);
    if (FAILED(hr)) {
      return hr;
    }
    mLoggers[aName] = logger;
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
//  onWindowClose
STDMETHODIMP Server::onWindowClose(BSTR aName)
{
  mWindows.erase(aName);
  return S_OK;
}

HRESULT Server::getLogWindow(VARIANT aWindowName, ILoggerInternal ** aRetVal, BOOL aEnsureExists)
{
  if (!aRetVal) {
    return E_POINTER;
  }

  CComPtr<ILoggerInternal> instance;
  CComVariant vtName;
  vtName.ChangeType(VT_BSTR, &aWindowName);
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

} // namespace PumpkinLog
