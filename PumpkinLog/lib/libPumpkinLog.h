#pragma once

namespace PumpkinLog
{

struct CLRESULT
{
  LRESULT m;
  CLRESULT(LRESULT _) : m(_) {}
  operator LRESULT() {return m;}
};

struct CHRESULT
{
  HRESULT m;
  CHRESULT(HRESULT _) : m(_) {}
  operator HRESULT() {return m;}
};

class Logger
{
public:
  // terminators
  static const struct _log {} log;
  static const struct _debug {} debug;
  static const struct _info {} info;
  static const struct _warn {} warn;
  static const struct _error {} error;

  // CTOR / DTOR
  Logger();

  ~Logger()
  { Destroy(); }

  // Init method: call once per instance
  HRESULT Init(LPCWSTR aName, LPCWSTR * aOptions, LONG aOptionsCount)
  { return mImplementation->Init(aName, aOptions, aOptionsCount); }

  void Destroy()
  {
    if (mImplementation) {
      delete mImplementation;
    }
    mImplementation = NULL;
  }

  // operator <<
  template<typename T> Logger & operator << (const T aValue)
  {
    mImplementation->AddValue(CComVariant(aValue));
    return *this;
  }

  // operator << overloads for special types
  Logger & operator << (CLRESULT res)
      { mImplementation->AddLRESULT(res); return *this; }
  Logger & operator << (CHRESULT hr)
      { mImplementation->AddHRESULT(hr); return *this; }

  // operator << overloads for terminators
  Logger & operator << (_log)
      { mImplementation->DoLog(LT_LOG); return (*this); };
  Logger & operator << (_debug)
      { mImplementation->DoLog(LT_DEBUG); return (*this); };
  Logger & operator << (_info)
      { mImplementation->DoLog(LT_INFO); return (*this); };
  Logger & operator << (_warn)
      { mImplementation->DoLog(LT_WARN); return (*this); };
  Logger & operator << (_error)
      { mImplementation->DoLog(LT_ERROR); return (*this); };

  enum LogFacility
  {
    LT_LOG      = 1,
    LT_DEBUG    = 2,
    LT_INFO     = 3,
    LT_WARN     = 4,
    LT_ERROR    = 5
  };

  class IImplementation {
  public:
    virtual void DoLog(Logger::LogFacility aFacility) = 0;
    virtual void AddValue(CComVariant & aValue) = 0;
    virtual void AddHRESULT(HRESULT hr) = 0;
    virtual void AddLRESULT(LRESULT res) = 0;
    virtual HRESULT Init(LPCWSTR aName, LPCWSTR * aOptions, LONG aOptionsCount) = 0;
  };

private:
  IImplementation * mImplementation;
};


} // end namespace PumpkinLog
