#include "stdafx.h"
#include "../PumpkinLog.h"
#include "libPumpkinLog.h"
#include "ErrorStrings.h"

namespace PumpkinLog
{

const Logger::_log Logger::log;
const Logger::_debug Logger::debug;
const Logger::_info Logger::info;
const Logger::_warn Logger::warn;
const Logger::_error Logger::error;

/*============================================================================
 * class LoggerImplementation
 *  The actual implementation of the logger wrapper
 */
class LoggerImplementation : public Logger::IImplementation
{
public:
  virtual void DoLog(Logger::LogFacility aFacility)
  {
    if (!mLogger) {
      return;
    }

    LONG count = (LONG)mEntries.size();
    CComSafeArray<VARIANT> options(count);
    for (auto it = mEntries.begin(); it != mEntries.end(); ++it) {
      options[(LONG)(it - mEntries.begin())] = CComVariant(*it);
    }
    mEntries.clear();

    switch(aFacility) {
/* not used
      case LT_CUSTOM:
        mLogger->llLog(nullptr, options);
        break;
*/
      case LT_LOG:
        mLogger->log(options);
        break;
      case LT_DEBUG:
        mLogger->debug(options);
        break;
      case LT_INFO:
        mLogger->info(options);
        break;
      case LT_WARN:
        mLogger->warn(options);
        break;
      case LT_ERROR:
        mLogger->error(options);
        break;
    }
  }

  virtual void AddValue(CComVariant & aValue)
  {
    if (!mLogger) {
      return;
    }
    mEntries.push_back(aValue);
  }

  virtual void AddLRESULT(LRESULT res)
  {
    if (!mLogger) {
      return;
    }
    CStringW s(DErrWin32(res));
    if (s.IsEmpty()) {
      s.Format(_T("%i"), res);
    }
    mEntries.push_back((LPCWSTR)s);
  }

  virtual void AddHRESULT(HRESULT hr)
  {
    if (!mLogger) {
      return;
    }
    CStringW s(DErrHRESULT(hr));
    if (s.IsEmpty()) {
      s.Format(_T("0x%08x"), hr);
    }
    mEntries.push_back((LPCWSTR)s);
  }

  virtual HRESULT Init(LPCWSTR aName, LPCWSTR * aOptions, LONG aOptionsCount)
  {
    if (mLogger) {
      return S_FALSE; // already initialized
    }

    CComPtr<IServer> server;
    HRESULT hr = server.CoCreateInstance(CLSID_Server);
    if (FAILED(hr)) {
      return hr;
    }

    if (aOptionsCount < 0) {
      return E_INVALIDARG;
    }

    CComSafeArray<VARIANT> options(aOptionsCount);
    for (LONG n = 0; n < aOptionsCount; n++) {
      options[n] = CComVariant(aOptions[n]);
    }

    CComPtr<IDispatch> disp;
    hr = server->createLogger(CComBSTR(aName), options, &disp);
    if (FAILED(hr)) {
      return hr;
    }
    mLogger = disp;

    return S_OK;
  }
private:
  // our logger
  CComQIPtr<ILoggerDispatch> mLogger;

  // array with entries to send to logger when the next terminator is streamed
  std::vector<CComVariant> mEntries;

};

/*============================================================================
 * class Logger
 *  Wrapper
 */
Logger::Logger()
{
  mImplementation = new LoggerImplementation();
}

} // end namespace PumpkinLog
