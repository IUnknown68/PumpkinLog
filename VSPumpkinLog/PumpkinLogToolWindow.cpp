#include "stdafx.h"
#include "..\VSPumpkinLogUI\Resource.h"
#include "PumpkinLogToolWindow.h"
#include "Guids.h"


PumpkinLogToolWindow::PumpkinLogToolWindow(const PackageVsSiteCache& rPackageVsSiteCache):
	ToolWindowBase(rPackageVsSiteCache)
{
}

// Caption of the tool window.
const wchar_t* const PumpkinLogToolWindow::GetCaption() const
{
	static CStringW strCaption;
	// Avoid to load the string from the resources more that once.
	if (0 == strCaption.GetLength())
	{
		VSL_CHECKBOOL_GLE(
			strCaption.LoadStringW(_AtlBaseModule.GetResourceInstance(), IDS_WINDOW_TITLE));
	}
	return strCaption;
}

// Creation flags for this tool window.
VSCREATETOOLWIN PumpkinLogToolWindow::GetCreationFlags() const
{
	return CTW_fInitNew|CTW_fForceCreate;
}

// Return the GUID of the persistence slot for this tool window.
const GUID& PumpkinLogToolWindow::GetToolWindowGuid() const
{
	return CLSID_guidPersistanceSlot;
}

IUnknown* PumpkinLogToolWindow::GetViewObject()
{
	// Should only be called once per-instance
	VSL_CHECKBOOLEAN_EX(m_spView == NULL, E_UNEXPECTED, IDS_E_GETVIEWOBJECT_CALLED_AGAIN);

	// Create the object that implements the window pane for this tool window.
	CComObject<VSPumpkinLogWindowPane>* pViewObject;
	VSL_CHECKHRESULT(CComObject<VSPumpkinLogWindowPane>::CreateInstance(&pViewObject));

	// Get the pointer to IUnknown for the window pane.
	HRESULT hr = pViewObject->QueryInterface(IID_IUnknown, (void**)&m_spView);
	if (FAILED(hr))
	{
		// If QueryInterface failed, then there is something wrong with the object.
		// Delete it and throw an exception for the error.
		delete pViewObject;
		VSL_CHECKHRESULT(hr);
	}

	return m_spView;
}

// This method is called by the base class after the tool window is created.
// We use it to set the icon for this window.
void PumpkinLogToolWindow::PostCreate()
{
	CComVariant srpvt;
	srpvt.vt = VT_I4;
	srpvt.intVal = IDB_IMAGES;
	// We don't want to make the window creation fail only becuase we can not set
	// the icon, so we will not throw if SetProperty fails.
	if (SUCCEEDED(GetIVsWindowFrame()->SetProperty(VSFPROPID_BitmapResource, srpvt)))
	{
		srpvt.intVal = 1;
		GetIVsWindowFrame()->SetProperty(VSFPROPID_BitmapIndex, srpvt);
	}
}
