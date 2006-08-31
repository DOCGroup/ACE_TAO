/*
  Copyright (c) Vanderbilt University, 2000-2001
  ALL RIGHTS RESERVED
  Vanderbilt University disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall Vanderbilt University be liable for
  any special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/


// UdmApp.cpp: implementation of the CUdmApp class.
// This file was automatically generated as UdmApp.cpp
// by UDM Interpreter Wizard on Monday, May 13, 2002 13:45:42

// Tihamér Levendovszky 05-13-02

#include "stdafx.h"
#pragma warning( disable : 4290 )

#include <windows.h>
#include <shlobj.h>
#include "resource.h"
#include <stdlib.h>
#include <sstream>

#include "UdmStatic.h"
#include "UmlExt.h"
#include "UdmUtil.h"

#include "UdmApp.h"
#include "UdmConfig.h"

#include "PICML/PICML.h"

#include <map>
#include <sstream>
#include <fstream>
#include <cstdlib>

#include <ui/configurator_ui.hpp>

#define SetUpVisitor(type, root, visitor)       \
do                                              \
  {                                             \
    type start = type ## ::Cast (root);         \
    start.Accept (visitor);                     \
  } while (0)

extern void dummy(void); // Dummy function for UDM meta initialization

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
  GME_MAIN_START		=   0,
  GME_BROWSER_START		=   1,
  GME_CONTEXT_START		=   2,
  GME_EMBEDDED_START		=   3,
  GME_MENU_START		=  16,
  GME_BGCONTEXT_START	=  18,
  GME_ICON_START		=  32,
  METAMODEL_CHECK_SYNTAX	= 101

  3. The framework catches all the exceptions and reports the error in a
  message box, clean up and close the transactions aborting the changes.
  You can override this behavior by catching udm_exception. Use
  udm_exception::what() to form an error message.
*/

struct addon_info
{
  std::string schema;
  std::string rules;
  std::string attr;
};

void launchViaCreateProcess(LPCTSTR program, LPCTSTR args)
{
  HANDLE hProcess = NULL;
  PROCESS_INFORMATION processInfo;
  STARTUPINFO startupInfo;
  ::ZeroMemory(&startupInfo, sizeof(startupInfo));
  startupInfo.cb = sizeof(startupInfo);
  ::ZeroMemory(&processInfo, sizeof(processInfo));
  if(!::CreateProcess(program, (LPTSTR)args,
                    NULL,  // process security
                    NULL,  // thread security
                    FALSE, // no inheritance
                    0,     // no startup flags
                    NULL,  // no special environment
                    NULL,  // default startup directory
                    &startupInfo,
                    &processInfo))
    {
      std::string errorMsg ("Unable to launch process using");
      errorMsg += program;
      errorMsg += " to create configurator GUI.";
      throw udm_exception (errorMsg);
    }

  WaitForSingleObject (processInfo.hProcess, INFINITE);
  CloseHandle (processInfo.hProcess);
  CloseHandle (processInfo.hThread);

}

/***********************************************/
/* Main entry point for Udm-based Interpreter  */
/***********************************************/

void CUdmApp::UdmMain(Udm::DataNetwork* p_backend,      // Backend pointer
                                                        // (already open!)
                      Udm::Object focusObject,          // Focus object
                      set<Udm::Object> selectedObjects,	// Selected objects
                      long param)			// Parameters
{
  typedef std::map<std::string, addon_info> addons_type;
  typedef addons_type::const_iterator addon_iterator;
  addons_type add_ons;

  char *cosmic_root = getenv("COSMIC_ROOT");
  std::string bin_dir =
    std::string(cosmic_root) + std::string("\\bin\\");

  std::ostringstream error_msg;

  error_msg << "Selected object should be one of the following" << std::endl;

  // Read the ini file TODO: This is going to be read from registry.
  {
    std::string ini_file_name = bin_dir + std::string("ocml_configurator.ini");

    std::ifstream ini_file (ini_file_name.c_str());
    while (!ini_file.eof())
      {
        std::string name;
//        std::string rule_file_name;
        addon_info info;
        ini_file >> name >> info.schema >> info.rules >> info.attr;
//        ini_file >> name >> info.schema >> rule_file_name >> info.attr;
//        rule_file_name = bin_dir + rule_file_name;
//
//        std::ifstream rule_file(rule_file_name.c_str());
//        std::copy(std::istream_iterator<char>(rule_file),
//                  std::istream_iterator<char>(),
//                  std::back_inserter(info.rules));

        add_ons[name] = info;

        error_msg << name << std::endl;
      }
  }

  try
    {
      // Select *one* object from the selected list, which has registered with
      // OCML in the windows registry (read in the previous step). The result
      // will be stored in the active_object_ptr, if non found it will be
      // selectedObjects.end().
      set<Udm::Object>::iterator active_object_ptr = selectedObjects.end();
      addon_iterator info_iter;

      for (set<Udm::Object>::iterator iter = selectedObjects.begin();
           iter != selectedObjects.end(); ++iter)
        {
          std::string kind_name = iter->type().name();
          info_iter = add_ons.find(kind_name);

          if (info_iter !=add_ons.end())
            {
              active_object_ptr = iter;
              break;
            }
        }

      if (active_object_ptr == selectedObjects.end())
        {
          AfxMessageBox (error_msg.str().c_str());
          return;
        }

      // display the gui.
      std::string old_value;
      active_object_ptr->GetStrValue(info_iter->second.attr, old_value);

//    Configurator_Dialog dlg;
//    std::string new_values =
//      dlg.show(info_iter->second.schema, old_value, info_iter->second.rules);

      /////////////////////////////////////////////////////////////////

      char *tmp_path    = getenv("TEMP");
      std::string temp_file_name =
        std::string(tmp_path) + std::string("\\ocml.value");
      {
        std::ofstream value_file(temp_file_name.c_str());
        value_file << old_value;
      }
      std::ostringstream application;
      std::ostringstream arguments;
      application << bin_dir << "config_exe.exe";
      arguments << info_iter->second.schema
                << " -v " << '"' << temp_file_name << '"'
         //       << " -r " << '"' << info_iter->second.rules << '"'
                << " -o " << '"' << temp_file_name <<'"';
      std::string new_values;
      std::string application_s = application.str();
      std::string arguments_s = arguments.str();
      launchViaCreateProcess(application_s.c_str(), arguments_s.c_str());
      {
        std::ifstream value_file(temp_file_name.c_str());
        std::copy(std::istream_iterator<char>(value_file),
                  std::istream_iterator<char>(),
                  std::back_inserter(new_values));
      }

      /////////////////////////////////////////////////////////////////

      active_object_ptr->SetStrValue(info_iter->second.attr, new_values);
    }
  catch(udm_exception &e)
    {
      AfxMessageBox ("Caught UDM Exception: " + CString (e.what()));
      return;
    }

  return;
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
