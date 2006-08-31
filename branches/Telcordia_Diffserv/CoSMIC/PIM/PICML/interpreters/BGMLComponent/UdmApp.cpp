// UdmApp.cpp: implementation of the CUdmApp class.
// This file was automatically generated as UdmApp.cpp
// by UDM Interpreter Wizard on Monday, August 23, 2004 09:32:54

#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "BGML/BGML_Visitor.h"

#include "Utils/Utils.h"

#define SetUpVisitor(type, root, visitor)                               \
  do                                                                    \
    {                                                                   \
      PICML:: ## type start = PICML:: ## type ## ::Cast (root);         \
      start.Accept (visitor);                                           \
    } while (0)

/********************************************************************************/
/* Initialization function. The framework calls it before preparing the backend. */
/* Initialize here the settings in the config global object.					  */
/* Return 0 if successful.														  */
/*********************************************************************************/
int CUdmApp::Initialize()
{

	// TODO: Your initialization code comes here...
	return 0;
}

static void showUsage()
{
	static std::string message ("BGML Interpreter failed to detect BGML Paradigm");
  AfxMessageBox (message.c_str ());
  return;
}



/* 
Remarks to CUdmApp::UdmMain(...):
0.	The p_backend points to an already open backend, and the framework 
	closes it automatically. DO NOT OPEN OR CLOSE IT!
	To commit changes use p_backend->CommitEditSequence().
	To abort changes use p_backend->AbortEditSequence().
	To save changes to a different file use p_backend->SaveAs() or p_backend->CloseAs().

1.	Focus is the currently open model.

2.	The possible values for param (from GME DecoratorLib.h component_startmode_enum):
	GME_MAIN_START			=   0,
	GME_BROWSER_START		=   1,
	GME_CONTEXT_START		=   2,
	GME_EMBEDDED_START		=   3,
	GME_MENU_START			=  16,
	GME_BGCONTEXT_START		=  18,
	GME_ICON_START			=  32,
	METAMODEL_CHECK_SYNTAX	= 101

 3. The framework catches all the exceptions and reports the error in a message box,
	clean up and close the transactions aborting the changes. You can override this 
	behavior by catching udm_exception. Use udm_exception::what() to form an error 
	message.
*/

/***********************************************/
/* Main entry point for Udm-based Interpreter  */
/***********************************************/

void CUdmApp::UdmMain(
					 Udm::DataNetwork* p_backend,		// Backend pointer(already open!)
					 Udm::Object focusObject,			// Focus object
					 set<Udm::Object> selectedObjects,	// Selected objects
					 long param)						// Parameters
{	
	try
    {
		std::string outputPath;
		std::string message = "Please specify the Output Directory";
		Utils::getPath (message, outputPath);
				
		// Do not do anything if Cancel button is pressed
		if (outputPath.size () == 0)
			return;

		bool valid_interpretation = 0;

		if (focusObject != Udm::null || 
			! selectedObjects.empty())
        {
			std::set<Udm::Object> mySet (selectedObjects);
			if (focusObject != Udm::null)
				mySet.insert (focusObject);

			for (std::set<Udm::Object>::iterator iter = mySet.begin();
			iter != mySet.end();
			++iter)
            {
				// Interpretation should start from Benchmarking Aspect
				Udm::Object root = *iter;
				std::string kindName = (*iter).type().name();
				
				// Generate MPC file only for the Component
				if (kindName == "BenchmarkAnalysis")
				{
					BGML_Visitor visitor (outputPath);
					SetUpVisitor (BenchmarkAnalysis, root, visitor);
					valid_interpretation = 1;
				}
			}
		}

		if (! valid_interpretation)
		{
			Udm::Object root_obj = p_backend->GetRootObject();
			PICML::RootFolder root_folder = PICML::RootFolder::Cast (root_obj);
			std::set<PICML::ComponentAnalyses> child_benchmarks = 
				root_folder.ComponentAnalyses_children ();
			for (std::set<PICML::ComponentAnalyses>::iterator iter = child_benchmarks.begin ();
			iter != child_benchmarks.end ();
			iter ++)
			{
				std::set<PICML::BenchmarkAnalysis> benchmarks = 
					iter->BenchmarkAnalysis_children();
				for (std::set<PICML::BenchmarkAnalysis>::iterator iter2 = benchmarks.begin ();
				iter2 != benchmarks.end ();
				iter2 ++)
				{
					Udm::Object root = *iter2;
					if (root != Udm::null)
					{
						BGML_Visitor visitor (outputPath);
						SetUpVisitor (BenchmarkAnalysis, root, visitor);
						valid_interpretation = 1;
					}
				}
				
			}
		} 

		if (valid_interpretation)		
			AfxMessageBox ("Bencharking files successfully generated");
		else
			showUsage ();
	 }
      catch(udm_exception &ex)
        {
          AfxMessageBox ("Error: generation of Benchmarking files failed");
		  AfxMessageBox (ex.what());
          return;
        }
}



#ifdef _DEBUG
/*****************************************************/
/* Debug time helper function. If the object has an  */
/* attribute called "name", this function retreives  */
/* it to help you to find it in the model during the */
/* application development.	Usualy every GME Object	 */
/* has a "name" attribute. If an object hapens not	 */
/* to have it,function retreives <no name specified>.*/
/*****************************************************/
string CUdmApp::ExtractName(Udm::Object ob)
{
	Uml::Class cls= ob.type();				
	set<Uml::Attribute> attrs=cls.attributes();		
	
	// Adding parent attributes
	set<Uml::Attribute> aattrs=Uml::AncestorAttributes(cls);
	attrs.insert(aattrs.begin(),aattrs.end());

	for(set<Uml::Attribute>::iterator ai = attrs.begin();ai != attrs.end(); ai++) 
	{
		if(string(ai->type())=="String")
		{
			string str=ai->name();
			if(str=="name")
			{
				string value=ob.getStringAttr(*ai);
				if(value.empty())value="<empty string>";
				return value;
			}			
		}				
	}	
	return string("<no name specified>");
}
#endif
