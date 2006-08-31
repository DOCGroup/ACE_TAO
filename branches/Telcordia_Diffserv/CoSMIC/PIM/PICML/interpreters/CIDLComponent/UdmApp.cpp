// UdmApp.cpp: implementation of the CUdmApp class.
// This file was automatically generated as UdmApp.cpp
// by UDM Interpreter Wizard on Wednesday, July 21, 2004 16:52:24

#include "stdafx.h"
#include "UdmApp.h"
#include "UdmConfig.h"
#include "CIDL_Interpreter/Cidlc_Visitor.h"
#include "Utils/Utils.h"

//
// Initialize
//
int CUdmApp::Initialize()
{
  return 0;
}


/*
  Remarks to CUdmApp::UdmMain(...):
  0.  The p_backend points to an already open backend, and the framework
  closes it automatically. DO NOT OPEN OR CLOSE IT!
  To commit changes use p_backend->CommitEditSequence().
  To abort changes use p_backend->AbortEditSequence().
  To save changes to a different file use p_backend->SaveAs() or p_backend->CloseAs().

  1.  Focus is the currently open model.

  2.  The possible values for param (from GME DecoratorLib.h component_startmode_enum):
  GME_MAIN_START      =   0,
  GME_BROWSER_START    =   1,
  GME_CONTEXT_START    =   2,
  GME_EMBEDDED_START    =   3,
  GME_MENU_START      =  16,
  GME_BGCONTEXT_START    =  18,
  GME_ICON_START      =  32,
  METAMODEL_CHECK_SYNTAX  = 101

  3. The framework catches all the exceptions and reports the error in a message box,
  clean up and close the transactions aborting the changes. You can override this
  behavior by catching udm_exception. Use udm_exception::what() to form an error
  message.
*/

/***********************************************/
/* Main entry point for Udm-based Interpreter  */
/***********************************************/

void CUdmApp::UdmMain(Udm::DataNetwork* p_backend,
                      Udm::Object focusObject,
                      std::set <Udm::Object> selectedObjects,
                      long param)
{
  std::string output;
  std::string message = "Please specify the output directory";

  // If there is no output path specified
  if (!Utils::getPath (message, output))
    return;

  // Get the root object and visit it.
  Udm::Object root_obj = p_backend->GetRootObject();
  PICML::RootFolder root = PICML::RootFolder::Cast (root_obj);

  PICML::Cidlc_Visitor visitor (output);
  root.Accept (visitor);

  AfxMessageBox ("Successfully generated CIDL files",
                 MB_OK | MB_ICONINFORMATION);
}
