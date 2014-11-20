#include "StdAfx.h"
#include "WindowManager.h"

namespace PumpkinLog {
namespace LogBucket {

HRESULT LogWindow::FinalConstruct()
{
  return S_OK;
}

void LogWindow::FinalRelease()
{
}

void LogWindow::OnFinalMessage(HWND)
{
  std::wstring name(mName);
  // window gone, release container, it will release us
  mContainer.Release();
  WindowManager::instance().OnClose(name);
}

BOOL LogWindow::PreTranslateMessage(MSG* pMsg)
{
  if (m_view.PreTranslateMessage(pMsg)) {
    return TRUE;
  }
  if (pMsg->hwnd != m_hWnd) {
    return FALSE;
  }
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

	m_hWndStatusBar = m_wndStatusBar.Create(*this);
  int anPanes[] = {ID_DEFAULT_PANE, ID_PANENUMCLIENTS};
  m_wndStatusBar.SetPanes(anPanes, 2, false);
  UpdateStatusbar();

  CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

  //m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL);
	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);

	UpdateLayout();
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);

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
  if (0 == GetLoggerRefCount()) {
    DestroyWindow();
  }
  else {
    ShowWindow(SW_SHOWMINIMIZED);
  }
  return 0;
}

LRESULT LogWindow::OnCmdClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  PostMessage(WM_CLOSE);
  return 0;
}

LRESULT LogWindow::OnCmdExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  ULONG loggerRefCount = GetLoggerRefCount();
  if (loggerRefCount > 0) {
    CString s;
    if (1 == loggerRefCount) {
      s = _T("There is still one logger connected.");
    }
    else {
      s.Format(_T("There are still %i loggers connected.\nExiting now will disconnect them ungraceful\n\nAre you sure you want to quit?"), loggerRefCount);
    }
    s += _T("\nExiting now will disconnect ungraceful.\n\nAre you sure you want to quit?");
    if (IDYES != MessageBox(s, L"Exiting PumpkinLog", MB_YESNO|MB_ICONQUESTION)) {
      return 0;
    }
  }
  CoDisconnectObject(this, 0);
  DestroyWindow();
  //::PostQuitMessage(0);
  return 0;
}

LRESULT LogWindow::OnCmdFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  //m_view.SaveAs();
  return 0;
}

LRESULT LogWindow::OnCmdCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  //m_view.Copy();
  return 0;
}

LRESULT LogWindow::OnWindowActivate(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nPage = wID - ID_WINDOW_TABFIRST;
	m_view.SetActivePage(nPage);

	return 0;
}

void LogWindow::UpdateStatusbar()
{
  CStringW s;
  s.Format(L"%i clients", GetLoggerRefCount());
  m_wndStatusBar.SetPaneText(ID_PANENUMCLIENTS, s);
}

void LogWindow::BringToFront()
{
  ShowWindow(SW_SHOWNOACTIVATE);
  SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
  SetWindowPos(HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
}

int LogWindow::FindTab(HWND aHWND)
{
  int m = m_view.GetPageCount();
  for (int n = 0; n < m; n++) {
    if (m_view.GetPageHWND(n) == aHWND) {
      return n;
    }
  }
  return -1;
}

ULONG LogWindow::GetLoggerRefCount()
{
  int m = m_view.GetPageCount();
  for (int n = 0; n < m; n++) {
  }
  return -1;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// ILogBucket implementation

//----------------------------------------------------------------------------
//  init
STDMETHODIMP LogWindow::init(LPCWSTR aName)
{
  ATLTRACE(L"NEW LogWindow: 0x%08x\n", this);
  mName = aName;
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
//  getHWND
STDMETHODIMP LogWindow::getHWND(HWND * aRetVal)
{
  IF_NULL_RET(aRetVal);
  (*aRetVal) = m_view;
  return S_OK;
}

//----------------------------------------------------------------------------
//  addTab
STDMETHODIMP LogWindow::addTab(HWND aHWND, LPCWSTR aName)
{
  int n = FindTab(aHWND);
  if (n >= 0) {
    return S_FALSE;
  }
	m_view.AddPage(aHWND, aName);
  return S_OK;
}

//----------------------------------------------------------------------------
//  removeTab
STDMETHODIMP LogWindow::removeTab(HWND aHWND)
{
  int n = FindTab(aHWND);
  if (n >= 0) {
    m_view.RemovePage(n);
    return S_OK;
  }
  return S_FALSE;
}
/*
//----------------------------------------------------------------------------
//  addRefLogger
STDMETHODIMP_(ULONG) LogWindow::addRefLogger(LPCWSTR aName)
{
  ++mLoggerRefcount;
  UpdateStatusbar();
  BringToFront();
  CComSafeArray<VARIANT> ar(1);
  CStringW s;
  s.Format(L"Client \"%s\" connected. Have %i clients now.", aName, mLoggerRefcount);
  ar[0] = s;
  //m_view.Log(LT_INTERNAL, aName, ar, nullptr);
  return mLoggerRefcount;
}

//----------------------------------------------------------------------------
//  removeRefLogger
STDMETHODIMP_(ULONG) LogWindow::removeRefLogger(LPCWSTR aName)
{
  --mLoggerRefcount;
  UpdateStatusbar();
  CComSafeArray<VARIANT> ar(1);
  CStringW s;
  s.Format(L"Client \"%s\" disconnected. Have %i clients now.", aName, mLoggerRefcount);
  ar[0] = s;
  //m_view.Log(LT_INTERNAL, aName, ar, nullptr);
  return mLoggerRefcount;
}

//----------------------------------------------------------------------------
//  onLoggerLog
STDMETHODIMP LogWindow::onLoggerLog(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals, LPDISPATCH pOptions)
{
  //m_view.Log(aFacility, aName, pVals, pOptions);
  return S_OK;
}
*/

} // namespace LogBucket
} // namespace PumpkinLog
