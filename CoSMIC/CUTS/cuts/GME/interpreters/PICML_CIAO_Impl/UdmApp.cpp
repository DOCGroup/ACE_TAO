// $Id$

#include "StdAfx.h"
#include "UdmApp.h"
#include "Main_Dialog.h"
#include "PICML/Utils.h"

#include "cuts/be/CoWorkEr_Cache.h"
#include "cuts/be/CoWorkEr_Generator.h"
#include "cuts/be/Dependency_Graph.h"
#include "cuts/be/Dependency_Generator.h"
#include "cuts/be/CUTS_Project.h"

#include "cuts/be/BE_Workspace_Generator.h"
#include "cuts/be/BE_File_Generator_Manager.h"
#include "cuts/be/CIAO/CIAO_Project_Generator.h"

/* Dummy function for UDM meta initialization */
extern void dummy(void);

// Initialization function. The framework calls it before preparing the
// backend. Initialize here the settings in the config static object.
// Return 0 if successful.
int CUdmApp::Initialize()
{
  return 0;
}

/*
  Remarks to CUdmApp::UdmMain(...):
  0. The p_backend points to an already open backend, and the framework
  closes it automatically. DO NOT OPEN OR CLOSE IT! To commit changes
  use p_backend->CommitEditSequence(). To abort changes use
  p_backend->AbortEditSequence(). To save changes to a different file
  use p_backend->SaveAs() or p_backend->CloseAs().

  1. Focus is the currently open model.

  2. The possible values for param (from GME DecoratorLib.h
  component_startmode_enum):
  GME_MAIN_START    =   0,
  GME_BROWSER_START    =   1,
  GME_CONTEXT_START    =   2,
  GME_EMBEDDED_START    =   3,
  GME_MENU_START    =  16,
  GME_BGCONTEXT_START  =  18,
  GME_ICON_START    =  32,
  METAMODEL_CHECK_SYNTAX  = 101

  3. The framework catches all the exceptions and reports the error in a
  message box, clean up and close the transactions aborting the changes.
  You can override this behavior by catching udm_exception. Use
  udm_exception::what() to form an error message.
*/

/***********************************************/
/* Main entry point for Udm-based Interpreter  */
/***********************************************/

void CUdmApp::UdmMain(Udm::DataNetwork* p_backend,
                      Udm::Object focusObject,
                      set <Udm::Object> selectedObjects,
                      long param)
{
  PICML::RootFolder root =
    PICML::RootFolder::Cast (p_backend->GetRootObject());

  root.Accept (*CUTS_Project::instance ());

  if (!CUTS_Project::instance ()->is_valid ())
  {
    ::AfxMessageBox (CUTS_Project::instance ()->message ().c_str (),
                     MB_OK | MB_ICONEXCLAMATION);
    CUTS_Project::close ();
    return;
  }


  // Show the main dialog and let the <end-user> select which
  // <option> they want to execute. If the user selects to
  // cancel the process then we just return.
  Main_Dialog main_dialog (::AfxGetMainWnd ());

  if (main_dialog.DoModal () == IDCANCEL)
    return;

  switch (main_dialog.option ())
  {
  case Main_Dialog::OPT_GENERATE_MODELS:
    {
      CUTS_UDM_CoWorkEr_Generator coworker_generator;
      root.Accept (coworker_generator);
    }
    break;

  case Main_Dialog::OPT_GENERATE_SOURCE:
    {
      std::string output_dir;
      std::string message ("Please specify the output directory");

      if (PICML::getPath (message, output_dir))
      {
        CUTS_BE_Project_Generator * proj = new CUTS_CIAO_Project_Generator ();
        CUTS_BE_Workspace_Generator generator (output_dir, proj);
        root.Accept (generator);

        ::AfxMessageBox ("Successfully completed code generation",
                         MB_OK | MB_ICONINFORMATION);
      }
    }
    break;
  }

  CUTS_Project::close ();
}
