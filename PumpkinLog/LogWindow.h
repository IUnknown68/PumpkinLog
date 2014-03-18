// Server.h : Declaration of the Server

#pragma once
#include "resource.h"       // main symbols
#include "PumpkinLog.h"
#include "LogBucket/Container.h"

#include "LogViewRE.h"

namespace PumpkinLog {

typedef LogViewRE LogView;

class LogWindow :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CFrameWindowImpl<LogWindow>,
  public CMessageFilter,
  public ILogBucket
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
    COM_INTERFACE_ENTRY(ILogBucket)
  END_COM_MAP()

  BEGIN_MSG_MAP(LogWindow)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(ID_EDIT_CLEAR_ALL, OnCmdClearLog)
		COMMAND_ID_HANDLER(IDCLOSE, OnCmdClose)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnCmdExit)
		COMMAND_ID_HANDLER(ID_FILE_SAVE_AS, OnCmdFileSaveAs)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnCmdCopy)
    CHAIN_MSG_MAP(CFrameWindowImpl<LogWindow>)
  END_MSG_MAP()

  HRESULT FinalConstruct();
	void PenultimateRelease();
  void FinalRelease();

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnCmdClearLog(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCmdClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCmdExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCmdFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCmdCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
  STDMETHOD(init)(LPCWSTR aUri, ILogBucket * aContainer, ILogServerInternal * aLogServer);
  STDMETHOD_(ULONG, addRefLogger)(LPCWSTR aName);
  STDMETHOD_(ULONG, removeRefLogger)(LPCWSTR aName);
  STDMETHOD(onLoggerLog)(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals);

private:
  ULONG mLoggerRefcount;
	CCommandBarCtrl m_CmdBar;
  LogView m_view;
  CComPtr<ILogServerInternal> mServer;
  CComPtr<ILogBucket> mContainer;
  CStringW  mName;
};


} // namespace PumpkinLog
