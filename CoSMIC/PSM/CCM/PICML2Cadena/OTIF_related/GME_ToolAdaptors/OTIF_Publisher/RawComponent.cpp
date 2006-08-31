///////////////////////////////////////////////////////////////////////////
// RawComponent.cpp, the main RAW COM component implementation file
// This is the file (along with its header RawComponent.h)
// that the component implementor is expected to modify in the first place
//
///////////////////////////////////////////////////////////////////////////
#include "stdafx.h"


#define OTIF_KEEPALIVE_MILLISECONDS 300   //Keepalive 5 minutes should be enough

#include "UdmCorba.h"

UDM_USE_MGA

#include "DialogPublishCompletion.h"
#include "resource.h"
#include "afxmt.h"


#include "ComHelp.h"
#include "GMECOM.h"
#include <ComponentConfig.h>
#include "RawComponent.h"

#include "Utils.h"


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


class myToolAdaptor : public UdmCORBA::ToolAdaptor 
{
public:
	myToolAdaptor(int argc, char **argv) : ToolAdaptor(argc, argv) {}; //Creates the UML tooladaptor
    myToolAdaptor(const char *type, int argc, char **argv) : ToolAdaptor(type, argc, argv) {}; //Creates a specific tooladaptor
    void notify(const char *par_name, const char *par_version, const char *name,
                const char *version, const char *remarks, long docId, long keepAlive) {};
};


namespace
{
	void dummy() {}
}


// This is the main component method for interpereters and plugins. 
// May als be used in case of invokeable addons
STDMETHODIMP RawComponent::InvokeEx( IMgaProject *project,  IMgaFCO *currentobj,  
									IMgaFCOs *selectedobjs,  long param) {
	COMTRY {
	  if(interactive) {
		CComPtr<IMgaTerritory> terr;
		COMTHROW(project->CreateTerritory(NULL, &terr));
		COMTHROW(project->BeginTransaction(terr));
		try {


			//My application code starts here
			try
			{
				static CMutex singleAccessMutex;
				CSingleLock lock(&singleAccessMutex);
				if (FALSE==lock.Lock(0)) throw exception("OTIF_Publisher already running. Wait for completion of previous operation.");


				CDialogPublishCompletion dpc(NULL,0,150);

				dpc.Create(CDialogPublishCompletion::IDD);
				dpc.ShowWindow(SW_SHOW);
				dpc.UpdateState("Getting GME project connection string", 4);
			

				CComBSTR bstr_project;
				COMTHROW(project->get_ProjectConnStr(&bstr_project));
				std::string projconnstr = Util::Copy(bstr_project);

				CComBSTR bstr_paradigmname;

				dpc.UpdateState("Getting GME paradigm name", 4);

				CComPtr<IMgaMetaProject> meta; COMTHROW(project->get_RootMeta(&meta));
				COMTHROW(meta->get_Name(&bstr_paradigmname));

				string paradigmname = Util::Copy(bstr_paradigmname);

				//AfxMessageBox((projconnstr+" | "+ paradigmname).c_str());

				dpc.UpdateState("Getting GME territory", 2);

				//Save it
				CComPtr<IMgaTerritory> terr;
				COMTHROW( project->get_ActiveTerritory(&terr));
				ASSERT(terr);

				dpc.UpdateState("Committing transaction", 2);
				COMTHROW( project->CommitTransaction() );

				dpc.UpdateState("Saving GME project", 10);
				COMTHROW( project->Save() );

				dpc.UpdateState("Beginning new transaction", 2);
				COMTHROW( project->BeginTransaction(terr) );



				//Publish it
				try 
				{   
					dpc.UpdateState("Creating ToolAdaptor", 4);

					// Instantiate the tooladaptor and login to the Backplane
					// Note: Workflow MUST have a tooladaptor with name {paradigmname}_Publisher like PICML_Publisher!!
					myToolAdaptor ta((paradigmname+"_Publisher").c_str(), __argc, __argv);  


					dpc.UpdateState("Getting Working Paradigms from the backplane", 10);

					//Let's fetch the Diagram from the OTIF backplane. 
					map<long, UdmCORBA::PARADIGM_STRUCT> workingparadigms; ta.getWorkingParadigms(workingparadigms);


					dpc.UpdateState("Filtering names matching this GME project's paradigm name", 2);



					{for (std::map<long, UdmCORBA::PARADIGM_STRUCT>::iterator iter = workingparadigms.begin(); iter != workingparadigms.end(); )
					{
						map<long, UdmCORBA::PARADIGM_STRUCT>::iterator olditer = iter++;
						if (olditer->second.name != paradigmname) workingparadigms.erase(olditer);
					}}
					if (workingparadigms.empty()) throw exception("Current paradigm not found on OTIF backplane! Check the workflow please.");

					dpc.UpdateState("Searching for highest version number", 2);

					//We are looking for the latest version of our paradigm from the backplane.
					string maxversion_str; double maxversion_number = 0;
					{for (std::map<long, UdmCORBA::PARADIGM_STRUCT>::iterator iter = workingparadigms.begin(); iter != workingparadigms.end(); iter++)
					{
						if (maxversion_number < atof(iter->second.version.c_str()))
						{					
							maxversion_number = atof(iter->second.version.c_str());
							maxversion_str = iter->second.version;
						}
					}}



					dpc.UpdateState("Creating UML ToolAdaptor", 4);
					myToolAdaptor uml_ta(__argc, __argv);

					dpc.UpdateState("Getting Uml::diagram datanetwork", 4);
					uml_ta.getDatanetwork(Uml::diagram);

					dpc.UpdateState((string()+"Fetching remote paradigm \""+paradigmname+"\" version \""+maxversion_str+"\" with the UML ToolAdaptor").c_str(), 10);
					Uml::Diagram &myUmlDiagram = uml_ta.remoteFetchMyParadigm(paradigmname.c_str(), maxversion_str.c_str());
					Udm::UdmDiagram myUdmDiagram={&myUmlDiagram, dummy};


					dpc.UpdateState("Getting CORBADataNetwork from OTIF backplane", 2);
					UdmCORBA::CORBADataNetwork *dn = ta.getDatanetwork(myUdmDiagram);


					dpc.UpdateState("Opening DataNetwork of this project from disk", 30);
					dn->OpenExisting(projconnstr.c_str()+4, "PICML"); //We skip the "MGA=" prefix

					string comment = "Datetime: " + CTime::GetCurrentTime().Format("%A, %B %d, %H:%M:%S");  //Monday, March 27, 15:41:12
					string rel_filename = projconnstr.c_str() + projconnstr.find_last_of('\\') + 1;


					// publish this datanetwork to the OTIF backplane
					dpc.UpdateState("Publishing DataNetwork", 30);
					dn->publishDocument(rel_filename.c_str(), "1.0", comment.c_str(), OTIF_KEEPALIVE_MILLISECONDS);

					dpc.UpdateState("DataNetwork published!", 80);
					AfxMessageBox("DataNetwork published!");
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
			}    
			catch (BON::Exception &e)
			{
				AfxMessageBox(e.getErrorMessage().c_str());
			}
			catch (exception &e)
			{
				AfxMessageBox(e.what());
			};
			//My application code ends here



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
