#include "stdafx.h"
#include "PumpkinLogPane.h"

#include "../PumpkinLog/PumpkinLog_i.c"

#define LOG_BGCOLOR         0x00ffffff
#define LOG_DEFAULTCOLOR    0
#define LOG_COLOR_INTERNAL  0x00a0a0a0
#define LOG_COLOR_LOG       0x00000000
#define LOG_COLOR_DEBUG     0x00f00000
#define LOG_COLOR_INFO      0x00008000
#define LOG_COLOR_WARN      0x000080a0
#define LOG_COLOR_ERROR     0x000000d0

VSPumpkinLogWindowPane::VSPumpkinLogWindowPane() :
	m_BroadcastCookie(VSCOOKIE_NIL)
{}

VSPumpkinLogWindowPane::~VSPumpkinLogWindowPane() {}
	
LRESULT VSPumpkinLogWindowPane::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  DefWindowProc();

  SetReadOnly();
  mFont.CreateFont(
    20,                  // nHeight
    0,                         // nWidth
    0,                         // nEscapement
    0,                         // nOrientation
    FW_NORMAL,                 // nWeight
    FALSE,                     // bItalic
    FALSE,                     // bUnderline
    0,                         // cStrikeOut
    ANSI_CHARSET,              // nCharSet
    OUT_DEFAULT_PRECIS,        // nOutPrecision
    CLIP_DEFAULT_PRECIS,       // nClipPrecision
    DEFAULT_QUALITY,           // nQuality
    DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
    _T("Consolas")                // font face
    );
  SetFont(mFont);
//  SetFont(AtlGetDefaultGuiFont());
  SetBackgroundColor(LOG_BGCOLOR);

  CHARFORMAT cf = {0};
  cf.cbSize = sizeof(CHARFORMAT);
  cf.crTextColor = LOG_DEFAULTCOLOR;
  cf.yHeight = 200;
  cf.dwMask = CFM_COLOR | CFM_SIZE;
  SetDefaultCharFormat(cf);

  PARAFORMAT2 pf;
  memset(&pf, 0, sizeof(PARAFORMAT2));
  pf.cbSize = sizeof(PARAFORMAT2);
  pf.cTabCount = 2;
  pf.rgxTabs[0] = cf.yHeight * 5;
  pf.rgxTabs[1] = cf.yHeight * 10;
  pf.dwMask = PFM_TABSTOPS;
  SetParaFormat(pf);

//  HRESULT hr = mPumpkinLog.CoCreateInstance(CLSID_Server, NULL, CLSCTX_ALL);
//  ATLASSERT(SUCCEEDED(hr));

  return 0;
}

STDMETHODIMP VSPumpkinLogWindowPane::CreatePaneWindow(HWND hwndParent, int x, int y, int cx, int cy, HWND *phwnd)
{
  HWND h = Create(hwndParent, CRect(x, y, cx, cy), NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL);
  if (!h) {
    DWORD dw = ::GetLastError();
    return E_FAIL;
  }
  (*phwnd) = h;
  Log(LT_INTERNAL, L"Test", CComVariant(L"Message of type INTERNAL 1"));
  Log(LT_DEBUG, L"Test", CComVariant(L"Message of type DEBUG 1"));
  Log(LT_INFO, L"Test", CComVariant(L"Message of type INFO 1"));
  Log(LT_WARN, L"Test", CComVariant(L"Message of type WARN"));
  Log(LT_ERROR, L"Test", CComVariant(L"Message of type ERROR"));
  Log(LT_LOG, L"Test", CComVariant(L"Message of type LOG"));
	return S_OK;
}

STDMETHODIMP VSPumpkinLogWindowPane::TranslateAccelerator(LPMSG lpmsg)
{
	VSL_STDMETHODTRY{

	VSL_CHECKBOOLEAN(NULL != m_hWnd, E_UNEXPECTED);

	// Check if the shell can process this message.
	CComPtr<IVsUIShell> spUiShell = m_VsSiteCache.GetCachedService<IVsUIShell, SID_SVsUIShell>();
	VSL_CHECKBOOLEAN(spUiShell != NULL, E_UNEXPECTED);
	HRESULT hr = spUiShell->TranslateAcceleratorAsACmd(lpmsg);
	VSL_CHECKHRESULT(hr);

	// The shell will return S_OK if it has proccessed the message, S_FALSE otherwise.
	if(S_OK != hr)
	{
    return S_FALSE;
		// If this is a dialog message, then we can handle it.
		VSL_SET_STDMETHOD_HRESULT(IsDialogMessage(lpmsg) ? S_OK : S_FALSE);
	}

	}VSL_STDMETHODCATCH()

	return VSL_GET_STDMETHOD_HRESULT();
}

STDMETHODIMP VSPumpkinLogWindowPane::OnBroadcastMessage(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	switch (uMsg)
	{
	case WM_SYSCOLORCHANGE:
	case WM_PALETTECHANGED:
		// Re-initialize VS colors when the theme changes.
		//InitVSColors();
		break;
	}

	return S_OK;
}




//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
// ILogBucket implementation

//----------------------------------------------------------------------------
//  init
STDMETHODIMP VSPumpkinLogWindowPane::init(LPCWSTR aUri, ILogBucket * aContainer, ILogServerInternal * aLogServer)
{
#ifdef ALKJASKLJ
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
#endif // def ALKJASKLJ
  return S_OK;
}

//----------------------------------------------------------------------------
//  addRefLogger
STDMETHODIMP_(ULONG) VSPumpkinLogWindowPane::addRefLogger(LPCWSTR aName)
{
#ifdef ALKJASKLJ
  ++mLoggerRefcount;
  CComSafeArray<VARIANT> ar(1);
  CStringW s;
  s.Format(L"Client \"%s\" connected. Have %i clients now.", aName, mLoggerRefcount);
  ar[0] = s;
  m_view.Log(LT_INTERNAL, aName, ar);
  return mLoggerRefcount;
#endif // def ALKJASKLJ
  return 0;
}

//----------------------------------------------------------------------------
//  removeRefLogger
STDMETHODIMP_(ULONG) VSPumpkinLogWindowPane::removeRefLogger(LPCWSTR aName)
{
#ifdef ALKJASKLJ
  --mLoggerRefcount;
  CComSafeArray<VARIANT> ar(1);
  CStringW s;
  s.Format(L"Client \"%s\" disconnected. Have %i clients now.", aName, mLoggerRefcount);
  ar[0] = s;
  m_view.Log(LT_INTERNAL, aName, ar);
  return mLoggerRefcount;
#endif // def ALKJASKLJ
  return 0;
}

//----------------------------------------------------------------------------
//  onLoggerLog
STDMETHODIMP VSPumpkinLogWindowPane::onLoggerLog(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals)
{
  Log(aFacility, aName, pVals);
  return S_OK;
}






void VSPumpkinLogWindowPane::LogIntro(LogFacility logType, LPCWSTR aName)
{
  // set color, set bold, print date, logtype, source and module
  CHARFORMAT cf = {0};
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_COLOR;
  CString sType;
  switch(logType)
  {
    case LT_INTERNAL:
      sType = _T("internal");
      cf.crTextColor = LOG_COLOR_INTERNAL;
      break;
    case LT_DEBUG:
      sType = _T("debug");
      cf.crTextColor = LOG_COLOR_DEBUG;
      break;
    case LT_INFO:
      sType = _T("info");
      cf.crTextColor = LOG_COLOR_INFO;
      break;
    case LT_WARN:
      sType = _T("warning");
      cf.crTextColor = LOG_COLOR_WARN;
      break;
    case LT_ERROR:
      sType = _T("error");
      cf.crTextColor = LOG_COLOR_ERROR;
      break;
    default:
      sType = _T("log");
      cf.crTextColor = LOG_DEFAULTCOLOR;
      break;
  }

  CTime ts = CTime::GetCurrentTime();
  CString sDate(ts.Format(_T("%H:%M:%S")));

  CString s;
  s.Format(_T("%s\t%s\t[%s]: "), sDate, sType, aName);

  SetSel(-1, -1);
  SetSelectionCharFormat(cf);
  AppendText(s);

  cf.dwMask |= CFM_BOLD;
  cf.dwEffects = CFE_BOLD;
  SetSelectionCharFormat(cf);
  // the view has now bold font with the correct color selected
}

void VSPumpkinLogWindowPane::LogExtro()
{
  // reset color
  CHARFORMAT cf = {0};
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_COLOR;
  cf.crTextColor = LOG_DEFAULTCOLOR;
  SetSelectionCharFormat(cf);
  AppendText(_T("\r\n"));
}

void VSPumpkinLogWindowPane::LogVariant(VARIANT & value)
{
  CComVariant vt;
  vt.ChangeType(VT_BSTR, &value);
  if (vt.vt != VT_BSTR)
  {
    vt = _T("???");
  }
  AppendText(vt.bstrVal);
}

void VSPumpkinLogWindowPane::Log(LogFacility logType, LPCWSTR aName, SAFEARRAY* pVals)
{
  LogIntro(logType, aName);

  ATLASSERT(0 == pVals->rgsabound[0].lLbound);
  VARIANT* pVariants = (VARIANT*)pVals->pvData;

  // see if we can do a printf
  ULONG valueCount = pVals->rgsabound[0].cElements;
  ULONG currentValueIndex = 0;

  if ((valueCount > 1) && (VT_BSTR == pVariants[0].vt)) {
    ++currentValueIndex;
    CStringW sIn(pVariants[0].bstrVal), sOut;
    int pos = sIn.Find(L'%');
    int posLast = 0;
    int sInLength = sIn.GetLength();
    while((-1 != pos) && (pos < sInLength-1) && (currentValueIndex < valueCount)) {
      // add everything before
      sOut += sIn.Mid(posLast, (pos - posLast));
      ++pos;
      wchar_t format = sIn[pos];
      CComVariant vt(pVariants[currentValueIndex]);
      HRESULT hr = E_FAIL;
      switch(format) {
        case L's':
          // a string: change to string type and log as it is
          hr = vt.ChangeType(VT_BSTR);
          if (SUCCEEDED(hr)) {
            sOut += vt.bstrVal;
          }
          ++currentValueIndex;
          ++pos;
          break;
        case L'd':
        case L'i':
          // an integer number
          // check for type, set "undefined" if empty, "NaN" if can't be converted to a number
          {
            CStringW s;
            switch (vt.vt) {
              case VT_I1:
              case VT_I2:
              case VT_I4:
              case VT_INT:
                s.Format(L"%i", vt.lVal);
                break;
              case VT_UI1:
              case VT_UI2:
              case VT_UI4:
              case VT_UINT:
                s.Format(L"%u", vt.ulVal);
                break;
              case VT_EMPTY:
                s = L"-undefined-";
                break;
              default:
                if (SUCCEEDED(vt.ChangeType(VT_UI4))) {
                  s.Format(L"%u", vt.ulVal);
                }
                else if (SUCCEEDED(vt.ChangeType(VT_I4))) {
                  s.Format(L"%u", vt.lVal);
                }
                else {
                  s = L"NaN";
                }
                break;
            }
            ++currentValueIndex;
            sOut += s;
          }
          ++pos;
          break;
        case L'f':
          // a float number
          // check for type, set "undefined" if empty, "NaN" if can't be converted to a number
          {
            CStringW s;
            switch (vt.vt) {
              case VT_R4:
                s.Format(L"%f", vt.fltVal);
                break;
              case VT_R8:
                s.Format(L"%f", vt.dblVal);
                break;
              case VT_EMPTY:
                s = L"-undefined-";
                break;
              default:
                if (SUCCEEDED(vt.ChangeType(VT_R8))) {
                  s.Format(L"%f", vt.dblVal);
                }
                else {
                  s = L"NaN";
                }
                break;
            }
            ++currentValueIndex;
            sOut += s;
          }
          ++pos;
          break;
        case L'o':
          // an object
          // check for type, set "undefined" if empty, "NaO" if can't be converted to string representation
          if (VT_DISPATCH == vt.vt) {
            CStringW s, s2;
            s2.Format(L"[Object at 0x%%0%ix]", sizeof(vt.pdispVal) * 2);
            s.Format(s2, (IUnknown*)(vt.pdispVal));
            sOut += s;
          }
          else if (VT_EMPTY == vt.vt) {
            sOut += L"-undefined-";
          }
          else if (SUCCEEDED(vt.ChangeType(VT_BSTR))) {
            sOut += vt.bstrVal;
          }
          else {
            sOut += L"NaO";
          }
          ++currentValueIndex;
          ++pos;
          break;
        default:
          // all other cases: not a format spec, take it as it is
          sOut += L'%';
          sOut += format;
          ++pos;
          break;
      }
      posLast = pos;
      pos = sIn.Find(L'%', pos);
    }
    // append the rest
    sOut += sIn.Mid(posLast);
    LogVariant(CComVariant(sOut));
    AppendText(_T(" "));
  }

  // log all the rest
  for(ULONG n = currentValueIndex; n < pVals->rgsabound[0].cElements; n++)
  {
    LogVariant(pVariants[n]);
    AppendText(_T(" "));
  }

  LogExtro();
}

void VSPumpkinLogWindowPane::Log(LogFacility logType, LPCWSTR aName, VARIANT vtValue)
{
  LogIntro(logType, aName);
  LogVariant(vtValue);
  LogExtro();
}

void VSPumpkinLogWindowPane::ClearLog()
{
  SetSel(0, -1);
  ReplaceSel(L"", FALSE);
}
