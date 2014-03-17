// Logger.h : Declaration of the Logger

#pragma once
#include "resource.h"       // main symbols
#include <map>

#include "PumpkinLog.h"

namespace PumpkinLog {

// Logger

class ATL_NO_VTABLE Logger :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Logger, &CLSID_Logger>,
	public IDispatchImpl<ILoggerDispatch, &IID_ILoggerDispatch, &LIBID_PumpkinLogLib, 1, 0>,
  public ILogger
{
public:
  typedef CComObject<Logger>  _ComObject;

  Logger()
	{
	}

  DECLARE_NO_REGISTRY()
	DECLARE_PROTECT_FINAL_CONSTRUCT()
  BEGIN_COM_MAP(Logger)
	  COM_INTERFACE_ENTRY(ILoggerDispatch)
	  COM_INTERFACE_ENTRY(ILogger)
	  COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()

	HRESULT FinalConstruct();
	void FinalRelease();

  HRESULT init(LPCWSTR aName, ILogServerInternal * aLogServer);
  HRESULT setOptions(SAFEARRAY* pVals);

public:
  STDMETHOD(log)(SAFEARRAY* pVals);
  STDMETHOD(debug)(SAFEARRAY* pVals);
  STDMETHOD(info)(SAFEARRAY* pVals);
  STDMETHOD(warn)(SAFEARRAY* pVals);
  STDMETHOD(error)(SAFEARRAY* pVals);

  STDMETHOD(log1)(VARIANT aVal1);
  STDMETHOD(debug1)(VARIANT aVal1);
  STDMETHOD(info1)(VARIANT aVal1);
  STDMETHOD(warn1)(VARIANT aVal1);
  STDMETHOD(error1)(VARIANT aVal1);

  STDMETHOD(log2)(VARIANT aVal1, VARIANT aVal2);
  STDMETHOD(debug2)(VARIANT aVal1, VARIANT aVal2);
  STDMETHOD(info2)(VARIANT aVal1, VARIANT aVal2);
  STDMETHOD(warn2)(VARIANT aVal1, VARIANT aVal2);
  STDMETHOD(error2)(VARIANT aVal1, VARIANT aVal2);

  STDMETHOD(log3)(VARIANT aVal1, VARIANT aVal2, VARIANT aVal3);
  STDMETHOD(debug3)(VARIANT aVal1, VARIANT aVal2, VARIANT aVal3);
  STDMETHOD(info3)(VARIANT aVal1, VARIANT aVal2, VARIANT aVal3);
  STDMETHOD(warn3)(VARIANT aVal1, VARIANT aVal2, VARIANT aVal3);
  STDMETHOD(error3)(VARIANT aVal1, VARIANT aVal2, VARIANT aVal3);

private:
  HRESULT doLog(LogFacility aFacility, SAFEARRAY * pVals);

private:
  typedef std::map<std::wstring, CComPtr<ILogBucket> > LogBucketMap;

private:
  CComPtr<ILogServerInternal>   mServer;
  LogBucketMap  mBuckets;

  CStringW  mName;
};

} // namespace PumpkinLog
