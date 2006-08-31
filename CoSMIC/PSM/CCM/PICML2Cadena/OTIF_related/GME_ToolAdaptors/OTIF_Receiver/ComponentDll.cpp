// Component.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
//#include <afxctl.h>

#include "RawComponent.h"

#include "ComponentDll.h"
#include "ComponentObj.h"
#include <ComponentConfig.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CComponentApp

BEGIN_MESSAGE_MAP(CComponentApp, CWinApp)
	//{{AFX_MSG_MAP(CComponentApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComponentApp construction

CComponentApp::CComponentApp() : CWinApp(COMPONENT_NAME)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CComponentApp object

CComponentApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CComponentApp initialization

BOOL CComponentApp::InitInstance()
{
	// Register all OLE server (factories) as running.  This enables the
	//  OLE libraries to create objects from other applications.
	COleObjectFactory::RegisterAll();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// Special entry points required for inproc servers

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}


//Defined in 
extern struct ReceiverThreadsInfo g_rti;

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//We cannot unload the DLL if the receiver thread is running or it will crash!
	if (RawComponent::s_rti.receiverThreadIsRunning) return S_FALSE;
	else return AfxDllCanUnloadNow();
}


#define COMRETURN(hr) { HRESULT res; if((res = (hr)) != S_OK) return res; }

// this flag supresses running Dll(Un)RegisterServer if DllInstall is also used
// bool called = false; 

// by exporting DllRegisterServer, you can use regsvr.exe
STDAPI DllRegisterServer(void)
{
/*	
	if(called) return S_OK;
	called = true;
*/
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!COleObjectFactory::UpdateRegistryAll(TRUE))
		return SELFREG_E_CLASS;

	// Note:
	// We can register the typelib either from the .tlb file
	// or from the resources. But the resource ID of the 
	// TYPELIB must be 1 !!!
/*
	if( !AfxOleRegisterTypeLib(AfxGetInstanceHandle(),
		LIBID_MgaComponentLib, NULL) )
		return E_FAIL;
*/
	CComponentReg reg;
#ifdef REGISTER_SYSTEMWIDE
	COMRETURN( reg.RegisterParadigms(REGACCESS_SYSTEM));
#else
	COMRETURN( reg.RegisterParadigms());
#endif
	

	return S_OK;
}

STDAPI DllUnregisterServer(void)
{
/*	
	if(called) return S_OK;
	called = true;
*/
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!COleObjectFactory::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	CComponentReg reg;
#ifdef REGISTER_SYSTEMWIDE
	COMRETURN( reg.UnregisterParadigms(REGACCESS_SYSTEM));
#else
	COMRETURN( reg.UnregisterParadigms());
#endif
	
	return S_OK;
}

#define DIM(x) (sizeof(x)/sizeof((x)[0]))

STDAPI DllInstall(BOOL bInstall, LPCWSTR cmdl) {

	regaccessmode_enum code = REGACCESS_NONE;
	if(iswdigit(cmdl[0]) && cmdl[1] == 0) {
		code = regaccessmode_enum(cmdl[0] - L'0');
	}
	else {
		struct {
			LPCWSTR name;
			regaccessmode_enum cc;
		} mnemonics[] = { { L"USER" , REGACCESS_USER},
						  { L"REGACCESS_USER" , REGACCESS_USER},	
						  { L"SYSTEM" , REGACCESS_SYSTEM},	
						  { L"REGACCESS_SYSTEM" , REGACCESS_SYSTEM},	
						  { L"BOTH" , REGACCESS_BOTH},	
						  { L"REGACCESS_BOTH" , REGACCESS_BOTH} };
		for(int i = 0; i < DIM(mnemonics); i++) {
			if(_wcsicmp(cmdl, mnemonics[i].name) == 0) {
				code = mnemonics[i].cc;
				break;
			}
		}
	}
	
//	if(called || code == REGACCESS_NONE) return E_INVALIDARG;
//	called = true;

	CComponentReg reg;
	if(bInstall) { COMRETURN(reg.RegisterParadigms(code)); }
	else		 { COMRETURN(reg.UnregisterParadigms(code)); }
	return S_OK;
};



