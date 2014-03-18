

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Mar 18 10:23:39 2014
 */
/* Compiler settings for PumpkinLog.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __PumpkinLog_h__
#define __PumpkinLog_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ILoggerDispatch_FWD_DEFINED__
#define __ILoggerDispatch_FWD_DEFINED__
typedef interface ILoggerDispatch ILoggerDispatch;
#endif 	/* __ILoggerDispatch_FWD_DEFINED__ */


#ifndef __ILogger_FWD_DEFINED__
#define __ILogger_FWD_DEFINED__
typedef interface ILogger ILogger;
#endif 	/* __ILogger_FWD_DEFINED__ */


#ifndef __IServer_FWD_DEFINED__
#define __IServer_FWD_DEFINED__
typedef interface IServer IServer;
#endif 	/* __IServer_FWD_DEFINED__ */


#ifndef __ILogBucket_FWD_DEFINED__
#define __ILogBucket_FWD_DEFINED__
typedef interface ILogBucket ILogBucket;
#endif 	/* __ILogBucket_FWD_DEFINED__ */


#ifndef __ILogServerInternal_FWD_DEFINED__
#define __ILogServerInternal_FWD_DEFINED__
typedef interface ILogServerInternal ILogServerInternal;
#endif 	/* __ILogServerInternal_FWD_DEFINED__ */


#ifndef __Server_FWD_DEFINED__
#define __Server_FWD_DEFINED__

#ifdef __cplusplus
typedef class Server Server;
#else
typedef struct Server Server;
#endif /* __cplusplus */

#endif 	/* __Server_FWD_DEFINED__ */


#ifndef __Logger_FWD_DEFINED__
#define __Logger_FWD_DEFINED__

#ifdef __cplusplus
typedef class Logger Logger;
#else
typedef struct Logger Logger;
#endif /* __cplusplus */

#endif 	/* __Logger_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_PumpkinLog_0000_0000 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_PumpkinLog_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_PumpkinLog_0000_0000_v0_0_s_ifspec;

#ifndef __ILoggerDispatch_INTERFACE_DEFINED__
#define __ILoggerDispatch_INTERFACE_DEFINED__

/* interface ILoggerDispatch */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ILoggerDispatch;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E034DA10-3781-40D4-8B3F-9A47700F3490")
    ILoggerDispatch : public IDispatch
    {
    public:
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE log( 
            /* [in] */ SAFEARRAY * pVals) = 0;
        
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE debug( 
            /* [in] */ SAFEARRAY * pVals) = 0;
        
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE info( 
            /* [in] */ SAFEARRAY * pVals) = 0;
        
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE warn( 
            /* [in] */ SAFEARRAY * pVals) = 0;
        
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE error( 
            /* [in] */ SAFEARRAY * pVals) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILoggerDispatchVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILoggerDispatch * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILoggerDispatch * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILoggerDispatch * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILoggerDispatch * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILoggerDispatch * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILoggerDispatch * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILoggerDispatch * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *log )( 
            ILoggerDispatch * This,
            /* [in] */ SAFEARRAY * pVals);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *debug )( 
            ILoggerDispatch * This,
            /* [in] */ SAFEARRAY * pVals);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *info )( 
            ILoggerDispatch * This,
            /* [in] */ SAFEARRAY * pVals);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *warn )( 
            ILoggerDispatch * This,
            /* [in] */ SAFEARRAY * pVals);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *error )( 
            ILoggerDispatch * This,
            /* [in] */ SAFEARRAY * pVals);
        
        END_INTERFACE
    } ILoggerDispatchVtbl;

    interface ILoggerDispatch
    {
        CONST_VTBL struct ILoggerDispatchVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILoggerDispatch_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILoggerDispatch_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILoggerDispatch_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILoggerDispatch_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ILoggerDispatch_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ILoggerDispatch_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ILoggerDispatch_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define ILoggerDispatch_log(This,pVals)	\
    ( (This)->lpVtbl -> log(This,pVals) ) 

#define ILoggerDispatch_debug(This,pVals)	\
    ( (This)->lpVtbl -> debug(This,pVals) ) 

#define ILoggerDispatch_info(This,pVals)	\
    ( (This)->lpVtbl -> info(This,pVals) ) 

#define ILoggerDispatch_warn(This,pVals)	\
    ( (This)->lpVtbl -> warn(This,pVals) ) 

#define ILoggerDispatch_error(This,pVals)	\
    ( (This)->lpVtbl -> error(This,pVals) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILoggerDispatch_INTERFACE_DEFINED__ */



#ifndef __PumpkinLogLib_LIBRARY_DEFINED__
#define __PumpkinLogLib_LIBRARY_DEFINED__

/* library PumpkinLogLib */
/* [helpstring][version][uuid] */ 

typedef /* [uuid] */  DECLSPEC_UUID("55B9D045-A4C5-4BD3-828B-433505D5F953") 
enum LogFacility
    {	LT_INTERNAL	= 0,
	LT_LOG	= 1,
	LT_DEBUG	= 2,
	LT_INFO	= 3,
	LT_WARN	= 4,
	LT_ERROR	= 5
    } 	LogFacility;


EXTERN_C const IID LIBID_PumpkinLogLib;

#ifndef __ILogger_INTERFACE_DEFINED__
#define __ILogger_INTERFACE_DEFINED__

/* interface ILogger */
/* [unique][oleautomation][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ILogger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E2C38BBC-21CC-4071-8C37-FD2FC21CC5AF")
    ILogger : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE log1( 
            VARIANT aVal1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE debug1( 
            VARIANT aVal1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE info1( 
            VARIANT aVal1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE warn1( 
            VARIANT aVal1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE error1( 
            VARIANT aVal1) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE log2( 
            VARIANT aVal1,
            VARIANT aVal2) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE debug2( 
            VARIANT aVal1,
            VARIANT aVal2) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE info2( 
            VARIANT aVal1,
            VARIANT aVal2) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE warn2( 
            VARIANT aVal1,
            VARIANT aVal2) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE error2( 
            VARIANT aVal1,
            VARIANT aVal2) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE log3( 
            VARIANT aVal1,
            VARIANT aVal2,
            VARIANT aVal3) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE debug3( 
            VARIANT aVal1,
            VARIANT aVal2,
            VARIANT aVal3) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE info3( 
            VARIANT aVal1,
            VARIANT aVal2,
            VARIANT aVal3) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE warn3( 
            VARIANT aVal1,
            VARIANT aVal2,
            VARIANT aVal3) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE error3( 
            VARIANT aVal1,
            VARIANT aVal2,
            VARIANT aVal3) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILoggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogger * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogger * This);
        
        HRESULT ( STDMETHODCALLTYPE *log1 )( 
            ILogger * This,
            VARIANT aVal1);
        
        HRESULT ( STDMETHODCALLTYPE *debug1 )( 
            ILogger * This,
            VARIANT aVal1);
        
        HRESULT ( STDMETHODCALLTYPE *info1 )( 
            ILogger * This,
            VARIANT aVal1);
        
        HRESULT ( STDMETHODCALLTYPE *warn1 )( 
            ILogger * This,
            VARIANT aVal1);
        
        HRESULT ( STDMETHODCALLTYPE *error1 )( 
            ILogger * This,
            VARIANT aVal1);
        
        HRESULT ( STDMETHODCALLTYPE *log2 )( 
            ILogger * This,
            VARIANT aVal1,
            VARIANT aVal2);
        
        HRESULT ( STDMETHODCALLTYPE *debug2 )( 
            ILogger * This,
            VARIANT aVal1,
            VARIANT aVal2);
        
        HRESULT ( STDMETHODCALLTYPE *info2 )( 
            ILogger * This,
            VARIANT aVal1,
            VARIANT aVal2);
        
        HRESULT ( STDMETHODCALLTYPE *warn2 )( 
            ILogger * This,
            VARIANT aVal1,
            VARIANT aVal2);
        
        HRESULT ( STDMETHODCALLTYPE *error2 )( 
            ILogger * This,
            VARIANT aVal1,
            VARIANT aVal2);
        
        HRESULT ( STDMETHODCALLTYPE *log3 )( 
            ILogger * This,
            VARIANT aVal1,
            VARIANT aVal2,
            VARIANT aVal3);
        
        HRESULT ( STDMETHODCALLTYPE *debug3 )( 
            ILogger * This,
            VARIANT aVal1,
            VARIANT aVal2,
            VARIANT aVal3);
        
        HRESULT ( STDMETHODCALLTYPE *info3 )( 
            ILogger * This,
            VARIANT aVal1,
            VARIANT aVal2,
            VARIANT aVal3);
        
        HRESULT ( STDMETHODCALLTYPE *warn3 )( 
            ILogger * This,
            VARIANT aVal1,
            VARIANT aVal2,
            VARIANT aVal3);
        
        HRESULT ( STDMETHODCALLTYPE *error3 )( 
            ILogger * This,
            VARIANT aVal1,
            VARIANT aVal2,
            VARIANT aVal3);
        
        END_INTERFACE
    } ILoggerVtbl;

    interface ILogger
    {
        CONST_VTBL struct ILoggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogger_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogger_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogger_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogger_log1(This,aVal1)	\
    ( (This)->lpVtbl -> log1(This,aVal1) ) 

#define ILogger_debug1(This,aVal1)	\
    ( (This)->lpVtbl -> debug1(This,aVal1) ) 

#define ILogger_info1(This,aVal1)	\
    ( (This)->lpVtbl -> info1(This,aVal1) ) 

#define ILogger_warn1(This,aVal1)	\
    ( (This)->lpVtbl -> warn1(This,aVal1) ) 

#define ILogger_error1(This,aVal1)	\
    ( (This)->lpVtbl -> error1(This,aVal1) ) 

#define ILogger_log2(This,aVal1,aVal2)	\
    ( (This)->lpVtbl -> log2(This,aVal1,aVal2) ) 

#define ILogger_debug2(This,aVal1,aVal2)	\
    ( (This)->lpVtbl -> debug2(This,aVal1,aVal2) ) 

#define ILogger_info2(This,aVal1,aVal2)	\
    ( (This)->lpVtbl -> info2(This,aVal1,aVal2) ) 

#define ILogger_warn2(This,aVal1,aVal2)	\
    ( (This)->lpVtbl -> warn2(This,aVal1,aVal2) ) 

#define ILogger_error2(This,aVal1,aVal2)	\
    ( (This)->lpVtbl -> error2(This,aVal1,aVal2) ) 

#define ILogger_log3(This,aVal1,aVal2,aVal3)	\
    ( (This)->lpVtbl -> log3(This,aVal1,aVal2,aVal3) ) 

#define ILogger_debug3(This,aVal1,aVal2,aVal3)	\
    ( (This)->lpVtbl -> debug3(This,aVal1,aVal2,aVal3) ) 

#define ILogger_info3(This,aVal1,aVal2,aVal3)	\
    ( (This)->lpVtbl -> info3(This,aVal1,aVal2,aVal3) ) 

#define ILogger_warn3(This,aVal1,aVal2,aVal3)	\
    ( (This)->lpVtbl -> warn3(This,aVal1,aVal2,aVal3) ) 

#define ILogger_error3(This,aVal1,aVal2,aVal3)	\
    ( (This)->lpVtbl -> error3(This,aVal1,aVal2,aVal3) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILogger_INTERFACE_DEFINED__ */


#ifndef __IServer_INTERFACE_DEFINED__
#define __IServer_INTERFACE_DEFINED__

/* interface IServer */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("021A77D9-2B94-4E3E-8919-8C3729C13A27")
    IServer : public IDispatch
    {
    public:
        virtual /* [vararg][helpstring][id] */ HRESULT STDMETHODCALLTYPE createLogger( 
            /* [in] */ BSTR aName,
            /* [in] */ SAFEARRAY * pVals,
            /* [retval][out] */ IDispatch **aRetVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IServer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IServer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IServer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IServer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IServer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IServer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [vararg][helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *createLogger )( 
            IServer * This,
            /* [in] */ BSTR aName,
            /* [in] */ SAFEARRAY * pVals,
            /* [retval][out] */ IDispatch **aRetVal);
        
        END_INTERFACE
    } IServerVtbl;

    interface IServer
    {
        CONST_VTBL struct IServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IServer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IServer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IServer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IServer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IServer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IServer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IServer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IServer_createLogger(This,aName,pVals,aRetVal)	\
    ( (This)->lpVtbl -> createLogger(This,aName,pVals,aRetVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IServer_INTERFACE_DEFINED__ */


#ifndef __ILogBucket_INTERFACE_DEFINED__
#define __ILogBucket_INTERFACE_DEFINED__

/* interface ILogBucket */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ILogBucket;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CF69F643-A8D9-4155-8EA1-FB0EFD2985C5")
    ILogBucket : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE init( 
            /* [in] */ LPCWSTR aUri,
            /* [in] */ ILogBucket *aContainer,
            /* [in] */ ILogServerInternal *aLogServer) = 0;
        
        virtual ULONG STDMETHODCALLTYPE addRefLogger( 
            /* [in] */ LPCWSTR aName) = 0;
        
        virtual ULONG STDMETHODCALLTYPE removeRefLogger( 
            /* [in] */ LPCWSTR aName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onLoggerLog( 
            /* [in] */ LogFacility aFacility,
            /* [in] */ LPCWSTR aName,
            /* [in] */ SAFEARRAY *pVals) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILogBucketVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogBucket * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogBucket * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogBucket * This);
        
        HRESULT ( STDMETHODCALLTYPE *init )( 
            ILogBucket * This,
            /* [in] */ LPCWSTR aUri,
            /* [in] */ ILogBucket *aContainer,
            /* [in] */ ILogServerInternal *aLogServer);
        
        ULONG ( STDMETHODCALLTYPE *addRefLogger )( 
            ILogBucket * This,
            /* [in] */ LPCWSTR aName);
        
        ULONG ( STDMETHODCALLTYPE *removeRefLogger )( 
            ILogBucket * This,
            /* [in] */ LPCWSTR aName);
        
        HRESULT ( STDMETHODCALLTYPE *onLoggerLog )( 
            ILogBucket * This,
            /* [in] */ LogFacility aFacility,
            /* [in] */ LPCWSTR aName,
            /* [in] */ SAFEARRAY *pVals);
        
        END_INTERFACE
    } ILogBucketVtbl;

    interface ILogBucket
    {
        CONST_VTBL struct ILogBucketVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogBucket_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogBucket_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogBucket_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogBucket_init(This,aUri,aContainer,aLogServer)	\
    ( (This)->lpVtbl -> init(This,aUri,aContainer,aLogServer) ) 

#define ILogBucket_addRefLogger(This,aName)	\
    ( (This)->lpVtbl -> addRefLogger(This,aName) ) 

#define ILogBucket_removeRefLogger(This,aName)	\
    ( (This)->lpVtbl -> removeRefLogger(This,aName) ) 

#define ILogBucket_onLoggerLog(This,aFacility,aName,pVals)	\
    ( (This)->lpVtbl -> onLoggerLog(This,aFacility,aName,pVals) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILogBucket_INTERFACE_DEFINED__ */


#ifndef __ILogServerInternal_INTERFACE_DEFINED__
#define __ILogServerInternal_INTERFACE_DEFINED__

/* interface ILogServerInternal */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ILogServerInternal;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D3D815CB-965C-42C6-A0EF-FF9A6D71D476")
    ILogServerInternal : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE onLoggerQuit( 
            /* [in] */ BSTR aName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE getBucket( 
            /* [in] */ LPCWSTR aUri,
            /* [retval][out] */ ILogBucket **aRetVal) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onBucketGone( 
            /* [in] */ LPCWSTR aUri) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILogServerInternalVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILogServerInternal * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILogServerInternal * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILogServerInternal * This);
        
        HRESULT ( STDMETHODCALLTYPE *onLoggerQuit )( 
            ILogServerInternal * This,
            /* [in] */ BSTR aName);
        
        HRESULT ( STDMETHODCALLTYPE *getBucket )( 
            ILogServerInternal * This,
            /* [in] */ LPCWSTR aUri,
            /* [retval][out] */ ILogBucket **aRetVal);
        
        HRESULT ( STDMETHODCALLTYPE *onBucketGone )( 
            ILogServerInternal * This,
            /* [in] */ LPCWSTR aUri);
        
        END_INTERFACE
    } ILogServerInternalVtbl;

    interface ILogServerInternal
    {
        CONST_VTBL struct ILogServerInternalVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILogServerInternal_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ILogServerInternal_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ILogServerInternal_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ILogServerInternal_onLoggerQuit(This,aName)	\
    ( (This)->lpVtbl -> onLoggerQuit(This,aName) ) 

#define ILogServerInternal_getBucket(This,aUri,aRetVal)	\
    ( (This)->lpVtbl -> getBucket(This,aUri,aRetVal) ) 

#define ILogServerInternal_onBucketGone(This,aUri)	\
    ( (This)->lpVtbl -> onBucketGone(This,aUri) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILogServerInternal_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Server;

#ifdef __cplusplus

class DECLSPEC_UUID("1D41AC82-70B0-4C33-9A1F-C4FFEF93AA55")
Server;
#endif

EXTERN_C const CLSID CLSID_Logger;

#ifdef __cplusplus

class DECLSPEC_UUID("C643D408-593B-48B3-970B-52C08F554EF4")
Logger;
#endif
#endif /* __PumpkinLogLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  LPSAFEARRAY_UserSize(     unsigned long *, unsigned long            , LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserMarshal(  unsigned long *, unsigned char *, LPSAFEARRAY * ); 
unsigned char * __RPC_USER  LPSAFEARRAY_UserUnmarshal(unsigned long *, unsigned char *, LPSAFEARRAY * ); 
void                      __RPC_USER  LPSAFEARRAY_UserFree(     unsigned long *, LPSAFEARRAY * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


