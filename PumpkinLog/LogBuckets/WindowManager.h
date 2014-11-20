// Server.h : Declaration of the Server

#pragma once
#include "resource.h"       // main symbols
#include "PumpkinLog.h"
#include "Container.h"

#include "LogWindow.h"

namespace PumpkinLog {
namespace LogBucket {

class WindowManager
{
private:
  typedef std::map<std::wstring, LogWindow::_ComPtr> WindowMap;

public:
  static WindowManager & instance()
  {
    static WindowManager theInstance;
    return theInstance;
  }

  ~WindowManager()
  {
  }

  HRESULT Get(const std::wstring & aName, bool aCreate, LogWindow::_ComPtr & aRetVal)
  {
    auto it = mWindows.find(aName);
    if (it != mWindows.end()) {
      aRetVal = it->second;
      return S_OK;
    }
    if (!aCreate) {
      return E_FAIL;
    }
    // create
    return CreateNewWindow(aName, aRetVal);
  }

  HRESULT GetDefault(LogWindow::_ComPtr & aRetVal)
  {
    if (mWindows.size()) {
      auto it = mWindows.begin();
      aRetVal = it->second;
      return S_OK;
    }
    // create
    return CreateNewWindow(std::wstring(L"default"), aRetVal);
  }

  void OnClose(const std::wstring & aName)
  {
    mWindows.erase(aName);
  }

private:
  WindowManager()
  {
  }

  HRESULT CreateNewWindow(const std::wstring & aName,  LogWindow::_ComPtr & aRetVal)
  {
    LogWindow::_ComObject * newWindow = NULL;
    IF_FAILED_RET_HR(LogWindow::_ComObject::CreateInstance(&newWindow));
    LogWindow::_ComPtr owner(newWindow);

    IF_FAILED_RET_HR(newWindow->init(aName.c_str()));

    mWindows[aName] = owner;
    aRetVal = owner;
    return S_OK;
  }

  WindowMap mWindows;

};

} // namespace LogBucket
} // namespace PumpkinLog
