// Server.h : Declaration of the Server

#pragma once
#include "resource.h"       // main symbols
#include "PumpkinLog.h"

#include "WindowManager.h"


namespace PumpkinLog {
namespace LogBucket {

class LogViewRE :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CWindowImpl<LogViewRE, CRichEditCtrl>,
  public ILogBucket
{
public:
  typedef CComObject<LogViewRE>  _ComObject;

public:
  LogViewRE() : mLoggerRefcount(0)
  {
  }

  virtual void OnFinalMessage(HWND aHWND);
  void Log(LogFacility logType, LPCWSTR aName, SAFEARRAY* pVals, LPDISPATCH pOptions);
  void Log(LogFacility logType, LPCWSTR aName, VARIANT vtValue, LPDISPATCH pOptions);
  void ClearLog();
  void SaveAs();

  HRESULT FinalConstruct();
	void PenultimateRelease();
  void FinalRelease();

public:
  DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())

  BEGIN_MSG_MAP(LogViewRE)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_EDIT_CLEAR_ALL, OnCmdClearLog)
  END_MSG_MAP()

  LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCmdClearLog(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  BEGIN_COM_MAP(LogViewRE)
    COM_INTERFACE_ENTRY(ILogBucket)
  END_COM_MAP()

  STDMETHOD(init)(LPCWSTR aUri, ILogBucket * aContainer, ILogServerInternal * aLogServer);
  STDMETHOD_(ULONG, addRefLogger)(LPCWSTR aName);
  STDMETHOD_(ULONG, removeRefLogger)(LPCWSTR aName);
  STDMETHOD(onLoggerLog)(LogFacility aFacility, LPCWSTR aName, SAFEARRAY * pVals, LPDISPATCH pOptions);

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
  ULONG mLoggerRefcount;
  CComPtr<ILogServerInternal> mServer;
  CComPtr<ILogBucket> mContainer;
  CStringW  mName;
  CComPtr<ILogWindow> mFrameWindow;

};

} // namespace LogBucket
} // namespace PumpkinLog
