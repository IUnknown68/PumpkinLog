// Server.h : Declaration of the Server

#pragma once
#include <unordered_map>
#include <string>

#include "resource.h"       // main symbols
#include "PumpkinLog.h"
#include "Logger.h"
#include "LogWindow.h"

// Server

namespace PumpkinLog {

class ATL_NO_VTABLE Server :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Server, &CLSID_Server>,
	public IDispatchImpl<IServer, &IID_IServer, &LIBID_PumpkinLogLib, 1, 0>,
  public ILogServerInternal
{
public:
	Server()
	{
	}

  DECLARE_REGISTRY_RESOURCEID(IDR_PUMPKINLOGSERVER)
	DECLARE_PROTECT_FINAL_CONSTRUCT()
  DECLARE_CLASSFACTORY_SINGLETON(Server)

  BEGIN_COM_MAP(Server)
	  COM_INTERFACE_ENTRY(ILogServerInternal)
	  COM_INTERFACE_ENTRY(IServer)
	  COM_INTERFACE_ENTRY(IDispatch)
  END_COM_MAP()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
  STDMETHOD(createLogger)(BSTR aName, VARIANT aOptions, IDispatch ** aRetVal);

  STDMETHOD(onLoggerQuit)(BSTR aName);
  //STDMETHOD(onWindowClose)(BSTR aName);
  STDMETHOD(getBucket)(LPCWSTR aUri, ILogBucketContainer ** aRetVal);
  STDMETHOD(onBucketGone)(LPCWSTR aUri);

private:
  //HRESULT getLogWindow(VARIANT aOptions, ILogBucket ** aRetVal, BOOL aEnsureExists = FALSE);
  HRESULT createBucket(LPCWSTR aUri, CComPtr<ILogBucketContainer> & aRetVal);

  // Here we hold weak pointers because we don't want to influence the refcount.
  // It's safe to do so because Logger and LogWindow tells us before the kill themselfs.
  typedef std::unordered_map<std::wstring, PumpkinLog::Logger* > LoggerMap;
  //typedef std::unordered_map<std::wstring, PumpkinLog::LogWindow* > LogWindowMap;
  typedef std::unordered_map<std::wstring, ILogBucketContainer* > LogBucketMap;

  LoggerMap mLoggers;
  //LogWindowMap mWindows;
  LogBucketMap mBuckets;
};

OBJECT_ENTRY_AUTO(CLSID_Server, Server)

} // namespace PumpkinLog


