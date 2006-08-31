///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"


#include "UdmCorba.h"

UDM_USE_MGA

#include "DialogCompletionStatus.h"
#include "DialogBackplaneMonitor.h"
#include "resource.h"
#include "afxmt.h"


#include "ComHelp.h"
#include "GMECOM.h"
#include <ComponentConfig.h>
#include "RawComponent.h"

#include "Utils.h"

RawComponent::ReceiverThreadsInfo RawComponent::s_rti;


// this method is called after all the generic initialization is done
// this should be empty, unless application-specific initialization is needed
STDMETHODIMP RawComponent::Initialize(struct IMgaProject *) {
	return S_OK;
}

// this is the obsolete component interface
// this present implementation either tries to call InvokeEx, or returns an error;
STDMETHODIMP RawComponent::Invoke(IMgaProject* gme, IMgaFCOs *models, long param) {
#ifdef SUPPORT_OLD_INVOKE
	CComPtr<IMgaFCO> focus;
	CComVariant parval = param;
	return InvokeEx(gme, focus, selected, parvar);
#else
	if(interactive) {
		AfxMessageBox("This component does not support the obsolete invoke mechanism");
	}
	return E_MGA_NOT_SUPPORTED;
#endif
}


UINT EventSinkThreadFunction(LPVOID pParam)
{
	CoInitialize(NULL);

	RawComponent::ReceiverThreadsInfo &rti = *(RawComponent::ReceiverThreadsInfo*) pParam;

	rti.receiverThreadIsRunning=true;

	string paradigmname;
	string projectfullpath;

	{
		CSingleLock lock(&rti.alloc_sync);
		lock.Lock();

		paradigmname = rti.paradigmName;
		projectfullpath = rti.projectFullPath;
	}


	//Publish it
	try 
	{   
		CDialogBackplaneMonitor(paradigmname, projectfullpath, rti.OTIF_ReceiverDialogBox).DoModal();
	} 
	catch (udmcorba_exception &e) 
	{
		if ( e.code()<11 ) 
		{
			AfxMessageBox((string()+"CORBA exception: "+ e.what()).c_str());
		} 
		else if (e.code()<21) 
		{
			AfxMessageBox((string()+"Paradigm exception: "+e.what()).c_str());
		} 
		else if (e.code()<41) 
		{
			AfxMessageBox((string()+"UdmCORBA exception: "+e.what()).c_str());
		} 
		else if (e.code()<91) 
		{
			AfxMessageBox((string()+"ToolAdaptor exception: "+e.what()).c_str());
		}
	}
	catch (exception &e) //it was udm_exception but...
	{
		AfxMessageBox(e.what());
	} 


	rti.receiverThreadIsRunning=false;

	return 1;
}


		/*
	{
		MSG msg;
		for (;;) {
			GetMessage(&msg,NULL,0,0);
			if (msg.message==WM_QUIT) goto skipout; //Devo uscire dalla mia number_crunching function.
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}
	skipout:
	*/



// This is the main component method for interpereters and plugins. 
// May als be used in case of invokeable addons
STDMETHODIMP RawComponent::InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  
									IMgaFCOs *selectedobjs,  long param) {
	COMTRY {
	  if(interactive) {
		CComBSTR projname;
		CComBSTR focusname = "<nothing>";
		CComPtr<IMgaTerritory> terr;
		COMTHROW(project->CreateTerritory(NULL, &terr));
		COMTHROW(project->BeginTransaction(terr));
		try {

			CComBSTR bstr_project;
			COMTHROW(project->get_ProjectConnStr(&bstr_project));
			std::string projconnstr = Util::Copy(bstr_project);

			CComBSTR bstr_paradigmname;
			CComPtr<IMgaMetaProject> meta; COMTHROW(project->get_RootMeta(&meta));
			COMTHROW(meta->get_Name(&bstr_paradigmname));

			string paradigmname = Util::Copy(bstr_paradigmname);
			
				//Save it
				CComPtr<IMgaTerritory> terr;
				COMTHROW( project->get_ActiveTerritory(&terr));
				ASSERT(terr);
				COMTHROW( project->CommitTransaction() );
				COMTHROW( project->Save() );
				COMTHROW( project->BeginTransaction(terr) );

			//AfxMessageBox((projconnstr+" | "+ paradigmname).c_str());

			//Not running? Start it.
			if (s_rti.receiverThreadIsRunning==false)
			{
				{
					CSingleLock lock(&s_rti.alloc_sync);
					lock.Lock();

					s_rti.paradigmName = paradigmname;
					s_rti.projectFullPath = projconnstr.c_str()+4;
				}

				//We start the threads here
				AfxBeginThread(EventSinkThreadFunction, (LPVOID) &s_rti);
			}
			else 
			{
				SetForegroundWindow(s_rti.OTIF_ReceiverDialogBox);
			}
			

			COMTHROW(project->CommitTransaction());
		}	catch(...) { project->AbortTransaction(); throw; }
		
	  } 
	} COMCATCH(;);
}

// GME currently does not use this function
// you only need to implement it if other invokation mechanisms are used
STDMETHODIMP RawComponent::ObjectsInvokeEx( IMgaProject *project,  IMgaObject *currentobj,  IMgaObjects *selectedobjs,  long param) {
	if(interactive) {
		AfxMessageBox("Tho ObjectsInvoke method is not implemented");
	}
	return E_MGA_NOT_SUPPORTED;
}


// implement application specific parameter-mechanism in these functions:
STDMETHODIMP RawComponent::get_ComponentParameter(BSTR name, VARIANT *pVal) {
	return S_OK;
}

STDMETHODIMP RawComponent::put_ComponentParameter(BSTR name, VARIANT newVal) {
	return S_OK;
}


#ifdef GME_ADDON

// these two functions are the main 
STDMETHODIMP RawComponent::GlobalEvent(globalevent_enum event) { 
	if(event == GLOBALEVENT_UNDO) {
		AfxMessageBox("UNDO!!");
	}
	return S_OK; 
}

STDMETHODIMP RawComponent::ObjectEvent(IMgaObject * obj, unsigned long eventmask, VARIANT v) {
	if(eventmask & OBJEVENT_CREATED) {
		CComBSTR objID;
		COMTHROW(obj->get_ID(&objID));
		AfxMessageBox( "Object created! ObjID: " + CString(objID)); 
	}		
	return S_OK;
}

#endif
