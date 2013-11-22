#include "StdAfx.h"
#include "LogWindow.h"

namespace PumpkinLog {

HRESULT LogWindow::FinalConstruct()
{
  return S_OK;
}

void LogWindow::FinalRelease()
{
  // remove the window from the server
  if (mServer) {
    mServer->onWindowClose(CComBSTR(mName));
  }
}

void LogWindow::OnFinalMessage(HWND)
{
  // Window destroyed, Release() it. Paired with AddRef() in OnCreate().
  Release();
}

BOOL LogWindow::PreTranslateMessage(MSG* pMsg)
{
  return CFrameWindowImpl<LogWindow>::PreTranslateMessage(pMsg);
}

LRESULT LogWindow::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

#ifdef USE_HTML_LOGWINDOW
  CString s, sm;
  GetModuleFileName(_Module.GetResourceInstance(), sm.GetBuffer(_MAX_PATH), _MAX_PATH);
  sm.ReleaseBuffer();
  s.Format(_T("res://%s/log.html"), sm);
  m_hWndClient = m_view.Create(m_hWnd, rcDefault, s, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL);
#else
  m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL);
#endif

	UpdateLayout();
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);

  // keep COM object alive. Paired with Release() in OnFinalMessage().
  AddRef();
  return 0;
}

LRESULT LogWindow::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
  return 0;
}

LRESULT LogWindow::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  if (0 == mLoggerRefcount) {
    DestroyWindow();
  }
  else {
    ShowWindow(SW_SHOWMINIMIZED);
  }
  return 0;
}

LRESULT LogWindow::OnClearLog(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  m_view.ClearLog();
  return 0;
}

LRESULT LogWindow::OnFileClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  PostMessage(WM_CLOSE);
  return 0;
}

LRESULT LogWindow::OnFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  m_view.SaveAs();
  return 0;
}

LRESULT LogWindow::OnCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  m_view.Copy();
  return 0;
}

HRESULT LogWindow::init(LPCWSTR aName, ILogServerInternal * aLogServer)
{
  mName = aName;
  mServer = aLogServer;
  if (!CreateEx())
  {
    return E_FAIL;
  }
  CStringW name;
  name.Format(_T("LogWindow - %s"), mName);
  SetWindowText(name);
  ShowWindow(SW_SHOW);
  return S_OK;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// ILoggerInternal implementation

//----------------------------------------------------------------------------
//  addRefLogger
STDMETHODIMP LogWindow::addRefLogger(LPCWSTR aName)
{
  ++mLoggerRefcount;
  CComSafeArray<VARIANT> ar(1);
  CStringW s;
  s.Format(L"Client connected. Have %i clients now.", mLoggerRefcount);
  ar[0] = s;
  m_view.Log(LT_INTERNAL, aName, ar);
  return S_OK;
}

//----------------------------------------------------------------------------
//  removeRefLogger
STDMETHODIMP LogWindow::removeRefLogger(LPCWSTR aName)
{
  --mLoggerRefcount;
  CComSafeArray<VARIANT> ar(1);
  CStringW s;
  s.Format(L"Client disconnected. Have %i clients now.", mLoggerRefcount);
  ar[0] = s;
  m_view.Log(LT_INTERNAL, aName, ar);
  return S_OK;
}

//----------------------------------------------------------------------------
//  onLoggerLog
STDMETHODIMP LogWindow::onLoggerLog(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals)
{
  m_view.Log(aFacility, aName, pVals);
  return S_OK;
}


} // namespace PumpkinLog
