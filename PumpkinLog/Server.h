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
  STDMETHOD(createLogger)(BSTR aName, SAFEARRAY* pVals, IDispatch ** aRetVal);

  STDMETHOD(onLoggerQuit)(BSTR aName);
  STDMETHOD(getBucket)(LPCWSTR aUri, ILogBucket ** aRetVal);
  STDMETHOD(onBucketGone)(LPCWSTR aUri);

private:
  typedef HRESULT (Server::*TCreateLogBucket)(LPCWSTR, CComPtr<ILogBucket> &);

  HRESULT CreateLogBucket_window(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal);
  HRESULT CreateLogBucket_file(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal);
  HRESULT CreateLogBucket_xml(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal);
  HRESULT CreateLogBucket__default(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal);

  struct LogBucketEntry
  {
    LPCWSTR name;
    TCreateLogBucket CreateLogBucket;
  };

  TCreateLogBucket GetLogBucketCreator(LPCWSTR aLogBucket) {
    static LogBucketEntry schemes[] =
    {
      {L"xml", &Server::CreateLogBucket_xml},
      {L"window", &Server::CreateLogBucket_window},
      {L"file", &Server::CreateLogBucket_file},
      {NULL, NULL}
    };
    TCreateLogBucket CreateLogBucket = &Server::CreateLogBucket__default;
    if (aLogBucket) {
      for (LogBucketEntry * entry = schemes; entry->CreateLogBucket; entry++) {
        if (0 == wcscmp(entry->name, aLogBucket)) {
          CreateLogBucket = entry->CreateLogBucket;
          break;  // found
        }
      }
    }
    return CreateLogBucket;
  }

  // Here we hold weak pointers because we don't want to influence the refcount.
  // It's safe to do so because Logger and LogWindow tells us before they commit suicide.
  typedef std::unordered_map<std::wstring, PumpkinLog::Logger* > LoggerMap;
  typedef std::unordered_map<std::wstring, ILogBucket* > LogBucketMap;

  LoggerMap mLoggers;
  LogBucketMap mBuckets;
};

OBJECT_ENTRY_AUTO(CLSID_Server, Server)

} // namespace PumpkinLog


