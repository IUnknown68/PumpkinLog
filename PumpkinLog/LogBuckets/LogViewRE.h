// Server.h : Declaration of the Server

#pragma once
#include "resource.h"       // main symbols
#include "PumpkinLog.h"

namespace PumpkinLog {
namespace LogBucket {

class LogViewRE : public CWindowImpl<LogViewRE, CRichEditCtrl>
{
public:
  void Log(LogFacility logType, LPCWSTR aName, SAFEARRAY* pVals, LPDISPATCH pOptions);
  void Log(LogFacility logType, LPCWSTR aName, VARIANT vtValue, LPDISPATCH pOptions);
  void ClearLog();
  void SaveAs();

public:
  DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())

  BEGIN_MSG_MAP(LogViewRE)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
  END_MSG_MAP()

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

private:
  enum FileType {
    RTF = 1,
    TXT = 2
  };

  class FileDialog : public CFileDialogImpl<FileDialog>
  {
  public:
    static LPCWSTR sDocString;
    // Construction
    FileDialog(HWND hWndParent)
      : CFileDialogImpl<FileDialog>(FALSE, _T("rtf"), 0, OFN_DONTADDTORECENT|OFN_ENABLESIZING|OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT, sDocString, hWndParent)
    {
      m_ofn.lpstrTitle = _T("Save Log as:");
    }

    static BOOL GetSaveAsFilename(HWND hWndParent, CStringW & aName, FileType & aType)
    {
      FileDialog dlg(hWndParent);

      INT_PTR nRes = dlg.DoModal();
      if (IDOK != nRes) {
        return FALSE;
      }
      aName = dlg.m_szFileName;
      aType = RTF;
      if (L"txt" == aName.Mid(dlg.m_ofn.nFileExtension)) {
        aType = TXT;
      }
      return TRUE;
    }

    // Maps
    BEGIN_MSG_MAP(FileDialog)
      CHAIN_MSG_MAP(CFileDialogImpl<FileDialog>)
    END_MSG_MAP()

    // Overrides
    void OnInitDone ( LPOFNOTIFY lpon )
    {
      GetFileDialogWindow().CenterWindow(lpon->lpOFN->hwndOwner);
    }
  };

private:
  static DWORD CALLBACK StreamFileOut(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
  void LogIntro(LogFacility logType, LPCWSTR aName);
  void LogVariant(VARIANT & value);
  void LogExtro();

  CFont mFont;

};

} // namespace LogBucket
} // namespace PumpkinLog
