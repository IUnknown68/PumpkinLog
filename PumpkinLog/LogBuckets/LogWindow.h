// Server.h : Declaration of the Server

#pragma once
#include "resource.h"       // main symbols
#include "PumpkinLog.h"
#include "Container.h"

namespace PumpkinLog {
namespace LogBucket {

class LogWindow :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CFrameWindowImpl<LogWindow>,
  public CMessageFilter,
  public ILogWindow
{
public:
  typedef CComObject<LogWindow>  _ComObject;
  typedef CComPtr<ILogWindow>  _ComPtr;

  LogWindow()
  {
  }

	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

  virtual void OnFinalMessage(HWND);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  BEGIN_COM_MAP(LogWindow)
    COM_INTERFACE_ENTRY(ILogWindow)
  END_COM_MAP()

  BEGIN_MSG_MAP(LogWindow)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDCLOSE, OnCmdClose)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnCmdExit)
		COMMAND_ID_HANDLER(ID_FILE_SAVE_AS, OnCmdFileSaveAs)
		COMMAND_ID_HANDLER(ID_EDIT_COPY, OnCmdCopy)
		COMMAND_RANGE_HANDLER(ID_WINDOW_TABFIRST, ID_WINDOW_TABLAST, OnWindowActivate)
    CHAIN_MSG_MAP(CFrameWindowImpl<LogWindow>)
  END_MSG_MAP()

  HRESULT FinalConstruct();
	void PenultimateRelease();
  void FinalRelease();

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnCmdClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCmdExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCmdFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCmdCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnWindowActivate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
  STDMETHOD(init)(LPCWSTR aName);
  STDMETHOD(getHWND)(HWND * aRetVal);
  STDMETHOD(addTab)(HWND aHWND, LPCWSTR aName);
  STDMETHOD(removeTab)(HWND aHWND);

private:
  void UpdateStatusbar();
  void BringToFront();
  int FindTab(HWND aHWND);
  ULONG GetLoggerRefCount();

private:
	CCommandBarCtrl m_CmdBar;
  CMultiPaneStatusBarCtrl m_wndStatusBar;
	CTabView m_view;
  //LogView m_view;
  CComPtr<ILogServerInternal> mServer;
  CComPtr<ILogBucket> mContainer;
  CStringW  mName;
};

} // namespace LogBucket
} // namespace PumpkinLog
