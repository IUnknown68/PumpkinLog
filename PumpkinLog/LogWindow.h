// Server.h : Declaration of the Server

#pragma once
#include "resource.h"       // main symbols
#include "PumpkinLog.h"
#include "CComObjectCachedEx.h"

#include "LogViewRE.h"

namespace PumpkinLog {

typedef LogViewRE LogView;

class LogWindow :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CFrameWindowImpl<LogWindow>,
  public CMessageFilter,
  public ILoggerInternal
{
public:
  typedef CComObject<LogWindow>  _ComObject;

  LogWindow() : mLoggerRefcount(0)
  {
  }

	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

  virtual void OnFinalMessage(HWND);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  BEGIN_COM_MAP(LogWindow)
    COM_INTERFACE_ENTRY(ILoggerInternal)
  END_COM_MAP()

  BEGIN_MSG_MAP(LogWindow)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(ID_CLEAR_LOG, OnClearLog)
		COMMAND_ID_HANDLER(ID_FILE_CLOSE, OnFileClose)
		COMMAND_ID_HANDLER(ID_FILE_SAVE_AS, OnFileSaveAs)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnCopy)
    CHAIN_MSG_MAP(CFrameWindowImpl<LogWindow>)
  END_MSG_MAP()

  HRESULT FinalConstruct();
	void PenultimateRelease();
  void FinalRelease();

  HRESULT init(LPCWSTR aName, ILogServerInternal * aLogServer);

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnClearLog(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
  STDMETHOD(addRefLogger)(LPCWSTR aName);
  STDMETHOD(removeRefLogger)(LPCWSTR aName);
  STDMETHOD(onLoggerLog)(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals);

private:
  ULONG mLoggerRefcount;
	CCommandBarCtrl m_CmdBar;
  LogView m_view;
  CComPtr<ILogServerInternal> mServer;
  CStringW  mName;
};


} // namespace PumpkinLog
