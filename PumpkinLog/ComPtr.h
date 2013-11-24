#pragma once

#include "PumpkinLog.h"

namespace PumpkinLog {

  /*
class LogBucketPtr
{
public:
  LogBucketPtr() throw() : mRefCount(0), mP(NULL)
  {
  }
  ~LogBucketPtr()
  {
    if (mRefCountPtr) {
      (*mRefCountPtr)--;
      if (0 == (*mRefCountPtr)) {
        delete mRefCountPtr;
        CComPtr<ILogServerInternal> server;
        HRESULT hr = server.CoCreateInstance(CLSID_Server);
        ATLVERIFY(SUCCEEDED(hr));
        server->onBucketFinalRelease();
      }
    }
    if (mP) {
      mP->Release();
    }
  }
private:
  ULONG mRefCount;
  CComPtr<ILogBucket> mP;
  CComPtr<ILogServerInternal> mServer;
};

*/

} // namespace PumpkinLog


