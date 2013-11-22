#pragma once

//Base is the user's class that derives from CComObjectRoot and whatever
//interfaces the user wants to support on the object
// CComObjectCached is used primarily for class factories in DLL's
// but it is useful anytime you want to cache an object
template <class Base, LONG refCountForRelease = 1>
class CComObjectCachedEx : 
	public Base
{
public:
	typedef Base _BaseClass;
	CComObjectCachedEx(_In_opt_ void* = NULL)
	{
	}
	// Set refcount to -(LONG_MAX/2) to protect destruction and 
	// also catch mismatched Release in debug builds
	virtual ~CComObjectCachedEx()
	{
		m_dwRef = -(LONG_MAX/2);
		FinalRelease();
#ifdef _ATL_DEBUG_INTERFACES
		_AtlDebugInterfacesModule.DeleteNonAddRefThunk(_GetRawUnknown());
#endif
	}
	//If InternalAddRef or InternalRelease is undefined then your class
	//doesn't derive from CComObjectRoot
	STDMETHOD_(ULONG, AddRef)() throw()
	{
		ULONG l = InternalAddRef();
		if (l == 2)
			_pAtlModule->Lock();
		return l;
	}
	STDMETHOD_(ULONG, Release)() throw()
	{
		ULONG l = InternalRelease();
		if (l == 0)
			delete this;
    else if (l == refCountForRelease) {
      static_cast<_BaseClass*>(this)->PenultimateRelease();
			_pAtlModule->Unlock();
    }
		return l;
	}
	//if _InternalQueryInterface is undefined then you forgot BEGIN_COM_MAP
	STDMETHOD(QueryInterface)(
		_In_ REFIID iid, 
		_Deref_out_ void** ppvObject) throw()
	{
		return _InternalQueryInterface(iid, ppvObject);
	}
	static HRESULT WINAPI CreateInstance(
		_Deref_out_ CComObjectCachedEx<Base, refCountForRelease>** pp) throw();
};

ATLPREFAST_SUPPRESS(6387)
template <class Base, LONG refCountForRelease>
_Success_(return == S_OK) HRESULT WINAPI CComObjectCachedEx<Base, refCountForRelease>::CreateInstance(
	_Deref_out_ CComObjectCachedEx<Base, refCountForRelease>** pp) throw()
{
	ATLASSERT(pp != NULL);
	if (pp == NULL)
		return E_POINTER;
	*pp = NULL;

	HRESULT hRes = E_OUTOFMEMORY;
	CComObjectCachedEx<Base, refCountForRelease>* p = NULL;
	ATLTRY((p = new CComObjectCachedEx<Base, refCountForRelease>()))
	if (p != NULL)
	{
		p->SetVoid(NULL);
		p->InternalFinalConstructAddRef();
		hRes = p->_AtlInitialConstruct();
		if (SUCCEEDED(hRes))
			hRes = p->FinalConstruct();
		if (SUCCEEDED(hRes))
			hRes = p->_AtlFinalConstruct();
		p->InternalFinalConstructRelease();
		if (hRes != S_OK)
		{
			delete p;
			p = NULL;
		}
	}
	*pp = p;
	return hRes;
}
ATLPREFAST_UNSUPPRESS()

