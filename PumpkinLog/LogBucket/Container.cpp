#include "StdAfx.h"
#include "Container.h"

namespace PumpkinLog {
namespace LogBucket {


HRESULT Container::FinalConstruct()
{
  return S_OK;
}

void Container::FinalRelease()
{
  if (mServer) {
    mServer->onBucketGone(mName);
  }
}

HRESULT Container::init(LPCWSTR aName, ILogBucket * aTarget, ILogServerInternal * aLogServer)
{
  mName = aName;
  mServer = aLogServer;
  mTarget = aTarget;
  return S_OK;
}

//----------------------------------------------------------------------------
//  addRefLogger
STDMETHODIMP_(ULONG) Container::addRefLogger(LPCWSTR aName)
{
  return (mTarget) ? mTarget->addRefLogger(aName) : 0;
}

//----------------------------------------------------------------------------
//  removeRefLogger
STDMETHODIMP_(ULONG) Container::removeRefLogger(LPCWSTR aName)
{
  return (mTarget) ? mTarget->removeRefLogger(aName) : 0;
}

//----------------------------------------------------------------------------
//  onLoggerLog
STDMETHODIMP Container::onLoggerLog(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals)
{
  return (mTarget) ? mTarget->onLoggerLog(aFacility, aName, pVals) : E_UNEXPECTED;
}

} // namespace LogBucket
} // namespace PumpkinLog
