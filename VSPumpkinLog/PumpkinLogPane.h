#pragma once

#include "..\PumpkinLog\PumpkinLog.h"

class VSPumpkinLogWindowPane :
	public CComObjectRootEx<CComSingleThreadModel>,
  public CWindowImpl<VSPumpkinLogWindowPane, CRichEditCtrl>,
  public IVsWindowPaneImpl<VSPumpkinLogWindowPane>,
	public VsWindowFrameEventSink<VSPumpkinLogWindowPane>,
	public VSL::ISupportErrorInfoImpl<
		InterfaceSupportsErrorInfoList<IVsWindowPane,
		InterfaceSupportsErrorInfoList<IVsWindowFrameNotify,
		InterfaceSupportsErrorInfoList<IVsWindowFrameNotify3> > > >,
	public IVsBroadcastMessageEvents,
  public ILogBucket
{
	VSL_DECLARE_NOT_COPYABLE(VSPumpkinLogWindowPane)

protected:

	// Protected constructor called by CComObject<VSPumpkinLogWindowPane>::CreateInstance.
	VSPumpkinLogWindowPane();

	~VSPumpkinLogWindowPane();
	
public:

  DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())

  BEGIN_COM_MAP(VSPumpkinLogWindowPane)
	  COM_INTERFACE_ENTRY(IVsWindowPane)
	  COM_INTERFACE_ENTRY(IVsWindowFrameNotify)
	  COM_INTERFACE_ENTRY(IVsWindowFrameNotify3)
	  COM_INTERFACE_ENTRY(ISupportErrorInfo)
	  COM_INTERFACE_ENTRY(IVsBroadcastMessageEvents)
  END_COM_MAP()

  BEGIN_MSG_MAP(VSPumpkinLogWindowPane)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
  END_MSG_MAP()

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	STDMETHOD(CreatePaneWindow)(HWND hwndParent, int x, int y, int cx, int cy, HWND *phwnd);
	STDMETHOD(TranslateAccelerator)(LPMSG lpmsg);
  STDMETHOD(OnBroadcastMessage)(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/);

  STDMETHOD(init)(LPCWSTR aUri, ILogBucket * aContainer, ILogServerInternal * aLogServer);
  STDMETHOD_(ULONG, addRefLogger)(LPCWSTR aName);
  STDMETHOD_(ULONG, removeRefLogger)(LPCWSTR aName);
  STDMETHOD(onLoggerLog)(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals);

private:
  void LogIntro(LogFacility logType, LPCWSTR aName);
  void LogExtro();
  void LogVariant(VARIANT & value);
  void Log(LogFacility logType, LPCWSTR aName, SAFEARRAY* pVals);
  void Log(LogFacility logType, LPCWSTR aName, VARIANT vtValue);
  void ClearLog();

  CFont mFont;
  VSCOOKIE m_BroadcastCookie;
//  CComQIPtr<ILogServerInternal> mPumpkinLog;
};
