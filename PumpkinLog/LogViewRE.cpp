#include "StdAfx.h"
#include <atlfile.h>
#include "LogViewRE.h"

#define LOG_BGCOLOR         0x00ffffff
#define LOG_DEFAULTCOLOR    0
#define LOG_COLOR_INTERNAL  0x00a0a0a0
#define LOG_COLOR_LOG       0x00000000
#define LOG_COLOR_DEBUG     0x00f00000
#define LOG_COLOR_INFO      0x00008000
#define LOG_COLOR_WARN      0x000080a0
#define LOG_COLOR_ERROR     0x000000d0

namespace PumpkinLog {

LPCWSTR LogViewRE::FileDialog::sDocString = L"RichText-Document (*.rtf)\0*.rtf\0Text-Document (*.txt)\0*.txt\0\0";

DWORD CALLBACK LogViewRE::StreamFileOut(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
  CAtlFile* file = (CAtlFile*)dwCookie;
  if (!file)
    return ERROR_INVALID_FUNCTION;
  return file->Write(pbBuff, cb, (DWORD*)pcb);
}

LRESULT LogViewRE::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  DefWindowProc();

  SetReadOnly();
  SetFont(AtlGetDefaultGuiFont());
  SetBackgroundColor(LOG_BGCOLOR);

  CHARFORMAT cf = {0};
  cf.cbSize = sizeof(CHARFORMAT);
  cf.crTextColor = LOG_DEFAULTCOLOR;
  cf.yHeight = 200;
  cf.dwMask = CFM_COLOR | CFM_SIZE;
  SetDefaultCharFormat(cf);
  return 0;
}

void LogViewRE::LogIntro(LogFacility logType, LPCWSTR aName)
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
  s.Format(_T("%s %s [%s]: "), sDate, sType, aName);

  SetSel(-1, -1);
  SetSelectionCharFormat(cf);
  AppendText(s);

  cf.dwMask |= CFM_BOLD;
  cf.dwEffects = CFE_BOLD;
  SetSelectionCharFormat(cf);
  // the view has now bold font with the correct color selected
}

void LogViewRE::LogExtro()
{
  // reset color
  CHARFORMAT cf = {0};
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_COLOR;
  cf.crTextColor = LOG_DEFAULTCOLOR;
  SetSelectionCharFormat(cf);
  AppendText(_T("\r\n"));
}

void LogViewRE::LogVariant(VARIANT & value)
{
  CComVariant vt;
  vt.ChangeType(VT_BSTR, &value);
  if (vt.vt != VT_BSTR)
  {
    vt = _T("???");
  }
  AppendText(vt.bstrVal);
}

void LogViewRE::Log(LogFacility logType, LPCWSTR aName, SAFEARRAY* pVals)
{
  LogIntro(logType, aName);

  ATLASSERT(0 == pVals->rgsabound[0].lLbound);
  VARIANT* pVariants = (VARIANT*)pVals->pvData;
  for(ULONG n = 0; n < pVals->rgsabound[0].cElements; n++)
  {
    LogVariant(pVariants[pVals->rgsabound[0].cElements - n - 1]);
    AppendText(_T(" "));
  }

  LogExtro();
}

void LogViewRE::Log(LogFacility logType, LPCWSTR aName, VARIANT vtValue)
{
  LogIntro(logType, aName);
  LogVariant(vtValue);
  LogExtro();
}

void LogViewRE::ClearLog()
{
  SetSel(0, -1);
  ReplaceSel(L"", FALSE);
}

void LogViewRE::SaveAs()
{
  CString sFilename;
  FileType aType;
  if (!FileDialog::GetSaveAsFilename(GetParent(), sFilename, aType)) {
    return;
  }

  CAtlFile file;
  HRESULT hr = file.Create(sFilename, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS);
  if (FAILED(hr)) {
    CString s, sErr;
    ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), sErr.GetBuffer(4096), 4096, NULL);
    sErr.ReleaseBuffer();
    s.Format(_T("Can't open file\n%s\n\nfor saving. Windows says:\n\"%s\""), sFilename, sErr);
    MessageBox(s, L"Save logfile", MB_OK|MB_ICONERROR);
    return;
  }
  UINT format = (TXT == aType) ? SF_TEXT : SF_RTF;

  EDITSTREAM eds = {(DWORD_PTR)&file, 0, StreamFileOut};
  StreamOut(format, eds);
}

} // namespace PumpkinLog
