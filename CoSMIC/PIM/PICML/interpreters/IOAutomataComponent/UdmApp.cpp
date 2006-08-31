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

#include "PICML/Utils.h"
#include "PICML/PICML.h"
#include "IOAutomata/IOAutomataVisitor.h"

using xercesc::XMLPlatformUtils;
using xercesc::XMLException;
using xercesc::DOMException;
using xercesc::XMLString;
using PICML::XStr;

extern PICML_Export bool PICML::getPath (const std::string& description,
                                         std::string& path);

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

/***********************************************/
/* Main entry point for Udm-based Interpreter  */
/***********************************************/

void CUdmApp::UdmMain(Udm::DataNetwork* p_backend,      // Backend pointer
                                                        // (already open!)
                      Udm::Object focusObject,          // Focus object
                      set<Udm::Object> selectedObjects,	// Selected objects
                      long param)			// Parameters
{
  try
    {
      XMLPlatformUtils::Initialize();
      try
        {
          std::string outputPath;
          std::string message = "Please specify the Output Directory";
          if (! ::PICML::getPath (message, outputPath))       return;
	  //	  outputPath = "C:\\Documents and Settings\\sumant\\Desktop\\backup";
          PICML::IOAutomataVisitor visitor (outputPath);
          PICML::RootFolder
            start = PICML::RootFolder::Cast (p_backend->GetRootObject());
          start.Accept (visitor);
        }
      catch(udm_exception &e)
        {
          AfxMessageBox ("IOAutomata Interpretation Failed. Caught UDM Exception: "
                         + CString (e.what()));
          return;
        }
      catch (const DOMException& e)
        {
          const unsigned int maxChars = 2047;
          XMLCh errText[maxChars + 1];

          std::stringstream estream;
          estream << "DOMException code: " << e.code << std::endl;
          if (xercesc::DOMImplementation::loadDOMExceptionMsg(e.code, errText, maxChars))
            {
              std::string message (XMLString::transcode (errText));
              estream << "Message is: " << message << std::endl;
            }
          AfxMessageBox (estream.str().c_str());
          return;
		}
      XMLPlatformUtils::Terminate();
    }
  catch (const XMLException& e)
    {
      std::string message (XMLString::transcode (e.getMessage()));
      AfxMessageBox (message.c_str());
      return;
    }
  AfxMessageBox ("Descriptor files were successfully generated!",
                 MB_OK| MB_ICONINFORMATION);
  return;
}
