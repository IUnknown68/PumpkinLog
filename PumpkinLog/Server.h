// Server.h : Declaration of the Server

#pragma once
#include <unordered_map>
#include <string>

#include "resource.h"       // main symbols
#include "PumpkinLog.h"
#include "Logger.h"

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
  STDMETHOD(getBucket)(LPCWSTR aUri, ILogBucket ** aRetVal);
  STDMETHOD(onBucketGone)(LPCWSTR aUri);

private:
  typedef HRESULT (Server::*TCreateScheme)(IUri *);

  HRESULT CreateScheme_window(IUri * aUri) {
    return S_OK;
  }

  HRESULT CreateScheme_xml(IUri * aUri) {
    return S_OK;
  }

  HRESULT CreateScheme__default(IUri * aUri) {
    return CreateScheme_window(aUri);
  }

  struct SchemeEntry
  {
    LPCWSTR name;
    TCreateScheme CreateScheme;
  };

  TCreateScheme GetSchemeCreator(LPCWSTR aScheme) {
    static SchemeEntry schemes[] =
    {
      {L"xml", &Server::CreateScheme_xml},
      {L"window", &Server::CreateScheme_window},
      {NULL, NULL}
    };
    TCreateScheme CreateScheme = &Server::CreateScheme__default;
    if (aScheme) {
      for (SchemeEntry * entry = schemes; entry->CreateScheme; entry++) {
        if (0 == wcscmp(entry->name, aScheme)) {
          CreateScheme = entry->CreateScheme;
          break;  // found
        }
      }
    }
    return CreateScheme;
  }

  HRESULT createLogWindow(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal);
  HRESULT createLogFile(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal);

  HRESULT createBucket(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal);

  // Here we hold weak pointers because we don't want to influence the refcount.
  // It's safe to do so because Logger and LogWindow tells us before they commit suicide.
  typedef std::unordered_map<std::wstring, PumpkinLog::Logger* > LoggerMap;
  typedef std::unordered_map<std::wstring, ILogBucket* > LogBucketMap;

  LoggerMap mLoggers;
  LogBucketMap mBuckets;
};

OBJECT_ENTRY_AUTO(CLSID_Server, Server)

} // namespace PumpkinLog


