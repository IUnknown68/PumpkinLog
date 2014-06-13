#pragma once

#include "PumpkinLogPane.h"

class PumpkinLogToolWindow :
	public VSL::ToolWindowBase<PumpkinLogToolWindow>
{
public:
	// Constructor of the tool window object.
	// The goal of this constructor is to initialize the base class with the site cache
	// of the owner package.
	PumpkinLogToolWindow(const PackageVsSiteCache& rPackageVsSiteCache);

	// Caption of the tool window.
	const wchar_t* const GetCaption() const;

	// Creation flags for this tool window.
	VSCREATETOOLWIN GetCreationFlags() const;

	// Return the GUID of the persistence slot for this tool window.
	const GUID& GetToolWindowGuid() const;

	IUnknown* GetViewObject();

	// This method is called by the base class after the tool window is created.
	// We use it to set the icon for this window.
	void PostCreate();

private:
	CComPtr<IUnknown> m_spView;
};
