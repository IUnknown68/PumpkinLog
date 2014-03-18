// Server.h : Declaration of the Server

#pragma once
#include <unordered_map>
#include <string>

#include "resource.h"       // main symbols
#include "PumpkinLog.h"
#include "Logger.h"
#include "LogWindow.h"
#include "LogBucket/FileBucket.h"

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
  typedef HRESULT (Server::*TCreateBucket)(LPCWSTR, CComPtr<ILogBucket> &);

  template<class T> HRESULT CreateBucket(LPCWSTR aUri, CComPtr<ILogBucket> & aRetVal)
  {
    // create container
    LogBucket::Container::_ComObject * newContainer = NULL;
    IF_FAILED_RET(LogBucket::Container::_ComObject::CreateInstance(&newContainer));
    CComPtr<ILogBucket> containerOwner(newContainer);

    // create target
    T::_ComObject * newTarget = NULL;
    IF_FAILED_RET(T::_ComObject::CreateInstance(&newTarget));
    CComPtr<ILogBucket> targetOwner(newTarget);

    // init container
    IF_FAILED_RET(newContainer->init(aUri, targetOwner, this));

    aRetVal = containerOwner;
    return S_OK;
  }

  struct LogBucketEntry
  {
    LPCWSTR name;
    TCreateBucket CreateBucket;
  };

  LogBucketEntry * GetBucketCreatorMap() {
    static LogBucketEntry bucketCreators[] =
    {
      //{L"xml", &Server::CreateBucket<LogConfig>},
      {L"window", &Server::CreateBucket<LogWindow>},
      {L"file", &Server::CreateBucket<LogBucket::FileBucket>},
      {NULL, NULL}
    };
    return bucketCreators;
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


