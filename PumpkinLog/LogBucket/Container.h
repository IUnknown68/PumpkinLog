// Server.h : Declaration of the Server

#pragma once
#include "resource.h"       // main symbols
#include "PumpkinLog.h"

namespace PumpkinLog {
namespace LogBucket {

















class Container :
  public CComObjectRootEx<CComSingleThreadModel>,
  public ILogBucketContainer
{
public:
  typedef CComObject<Container>  _ComObject;

  Container()
  {
  }

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  BEGIN_COM_MAP(Container)
    COM_INTERFACE_ENTRY(ILogBucketContainer)
  END_COM_MAP()

public:
  HRESULT FinalConstruct();
  void FinalRelease();
  HRESULT init(LPCWSTR aName, ILogBucket * aTarget, ILogServerInternal * aLogServer);

public:
  STDMETHOD_(ULONG, addRefLogger)(LPCWSTR aName);
  STDMETHOD_(ULONG, removeRefLogger)(LPCWSTR aName);
  STDMETHOD(onLoggerLog)(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals);

private:
  CStringW  mName;
  CComPtr<ILogServerInternal> mServer;
  CComPtr<ILogBucket> mTarget;
};

} // namespace LogBucket
} // namespace PumpkinLog
