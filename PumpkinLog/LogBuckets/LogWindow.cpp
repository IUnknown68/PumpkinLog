#include "StdAfx.h"
#include "LogWindow.h"

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
  // window gone, release container, it will release us
  mContainer.Release();
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

  m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL);

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
  if (0 == mLoggerRefcount) {
    DestroyWindow();
  }
  else {
    ShowWindow(SW_SHOWMINIMIZED);
  }
  return 0;
}

LRESULT LogWindow::OnCmdClearLog(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  m_view.ClearLog();
  return 0;
}

LRESULT LogWindow::OnCmdClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  PostMessage(WM_CLOSE);
  return 0;
}

LRESULT LogWindow::OnCmdExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  if (mLoggerRefcount > 0) {
    CString s;
    if (1 == mLoggerRefcount) {
      s = _T("There is still one logger connected.");
    }
    else {
      s.Format(_T("There are still %i loggers connected."), mLoggerRefcount);
    }
    s += _T("\nExiting now will disconnect ungraceful.\n\nAre you sure you want to quit?");
    if (IDYES != MessageBox(s, L"Exiting PumpkinLog", MB_YESNO|MB_ICONQUESTION)) {
      return 0;
    }
  }
  DestroyWindow();
  ::PostQuitMessage(0);
  return 0;
}

LRESULT LogWindow::OnCmdFileSaveAs(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  m_view.SaveAs();
  return 0;
}

LRESULT LogWindow::OnCmdCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  m_view.Copy();
  return 0;
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// ILogBucket implementation

//----------------------------------------------------------------------------
//  init
STDMETHODIMP LogWindow::init(LPCWSTR aUri, ILogBucket * aContainer, ILogServerInternal * aLogServer)
{
  mName = aUri;
  mServer = aLogServer;
  if (!CreateEx())
  {
    return E_FAIL;
  }
  // holding a ref to container keeps us alive, because container
  // hold a ref to us
  mContainer = aContainer;
  CStringW name;
  name.Format(_T("LogWindow - %s"), mName);
  SetWindowText(name);
  ShowWindow(SW_SHOW);
  return S_OK;
}

//----------------------------------------------------------------------------
//  addRefLogger
STDMETHODIMP_(ULONG) LogWindow::addRefLogger(LPCWSTR aName)
{
  ++mLoggerRefcount;
  CComSafeArray<VARIANT> ar(1);
  CStringW s;
  s.Format(L"Client \"%s\" connected. Have %i clients now.", aName, mLoggerRefcount);
  ar[0] = s;
  m_view.Log(LT_INTERNAL, aName, ar);
  return mLoggerRefcount;
}

//----------------------------------------------------------------------------
//  removeRefLogger
STDMETHODIMP_(ULONG) LogWindow::removeRefLogger(LPCWSTR aName)
{
  --mLoggerRefcount;
  CComSafeArray<VARIANT> ar(1);
  CStringW s;
  s.Format(L"Client \"%s\" disconnected. Have %i clients now.", aName, mLoggerRefcount);
  ar[0] = s;
  m_view.Log(LT_INTERNAL, aName, ar);
  return mLoggerRefcount;
}

//----------------------------------------------------------------------------
//  onLoggerLog
STDMETHODIMP LogWindow::onLoggerLog(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals)
{
  m_view.Log(aFacility, aName, pVals);
  return S_OK;
}

} // namespace LogBucket
} // namespace PumpkinLog
