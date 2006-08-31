//###############################################################################################################################################
//
//  Meta and Builder Object Network V2.0 for GME
//  BON2Component.cpp
//
//###############################################################################################################################################

/*
  Copyright (c) Vanderbilt University, 2000-2004
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

#include "BON2Component.h"
#include "IDMLBonExtension.h"
#include "DependencyVisitor.h"
#include "IDLEmitVisitor.h"
#include <fstream>

namespace
{
  // This method prompts a dialog to allow the user to specify a folder
  bool getPath (const std::string& description, std::string& path)
  {
    // Initalize the com library
    HRESULT hr = ::CoInitialize(NULL);
    if (FAILED(hr))
      return false;

    // Dialog instruction
    char display_buffer[MAX_PATH];
    BROWSEINFO folder_browsinfo;
    memset (&folder_browsinfo, 0, sizeof (folder_browsinfo));

    // Set GME as the owner of the dialog
    folder_browsinfo.hwndOwner = GetForegroundWindow();
    // Start the browse from desktop
    folder_browsinfo.pidlRoot = NULL;
    // Pointer to the folder name display buffer
    folder_browsinfo.pszDisplayName = &display_buffer[0];
    // Dialog instruction string
    folder_browsinfo.lpszTitle = description.c_str();
    // Use new GUI style and allow edit plus file view
    folder_browsinfo.ulFlags = BIF_RETURNONLYFSDIRS|BIF_USENEWUI;
    // No callback function
    folder_browsinfo.lpfn = NULL;
    // No parameter passing into the dialog
    folder_browsinfo.lParam = 0;

    LPITEMIDLIST folder_pidl = SHBrowseForFolder(&folder_browsinfo);

    if(folder_pidl == NULL)
      return false;
    else
      {
        TCHAR FolderNameBuffer[MAX_PATH];

        // Convert the selection into a path
        if (SHGetPathFromIDList (folder_pidl, FolderNameBuffer))
          path = FolderNameBuffer;

        // Free the ItemIDList object returned from the call to
        // SHBrowseForFolder using Gawp utility function
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
          {
            imalloc->Free ( folder_pidl );
            imalloc->Release ( );
          }
      }
    ::CoUninitialize();
    return true;
  }
}

namespace BON
{

using namespace IDML;

//###############################################################################################################################################
//
//   C L A S S : BON::Component
//
//###############################################################################################################################################

Component::Component()
  : m_bIsInteractive( false )
{
}

Component::~Component()
{
  if ( m_project ) {
    m_project->finalizeObjects();
    finalize( m_project );
    m_project = NULL;
  }
}

// ====================================================
// This method is called after all the generic initialization is done
// This should be empty unless application-specific initialization is needed

void Component::initialize( Project& project )
{
  // ======================
  // Insert application specific code here
}

// ====================================================
// This method is called before the whole BON2 project released and disposed
// This should be empty unless application-specific finalization is needed

void Component::finalize( Project& project )
{
  // ======================
  // Insert application specific code here
}

// ====================================================
// This is the obsolete component interface
// This present implementation either tries to call InvokeEx, or does nothing except of a notification

void Component::invoke( Project& project, const std::set<FCO>& setModels, long lParam )
{
  #ifdef SUPPORT_OLD_INVOKE
    Object focus;
    invokeEx( project, focus, setModels, lParam );
  #else
    if ( m_bIsInteractive )
      AfxMessageBox("This BON2 Component does not support the obsolete invoke mechanism!");
  #endif
}

// ====================================================
// This is the main component method for Interpereters and Plugins.
// May also be used in case of invokeable Add-Ons

void Component::invokeEx( Project& project,
                          FCO& currentFCO,
                          const std::set<FCO>& setSelectedFCOs,
                          long lParam )
{
  std::string outputPath;
  std::string message = "Please specify the Output Directory";

  if (!getPath (message, outputPath))
    {
      return;
    }

  std::set<Object> selected = project->findByKind ("File");

  for (std::set<Object>::const_iterator it = selected.begin ();
       it != selected.end ();
       ++it)
    {
      File root (*it);

      if (!root)
        {
          AfxMessageBox ("Interpretation must start from a File model!");
          continue;
        }
      if (root->isInLibrary ())
        {
          continue;
        }

      DependencyVisitor root_visitor;
      root_visitor.visitOrderableImpl (root);

      // A strstream may also be passed to the visitor here, to
      // redirect the output.
      std::string raw_filename = root->getName ();
      std::string filename = outputPath + "\\" + raw_filename + ".idl";

      std::ofstream strm (filename.c_str ());

      IDLEmitVisitor emit_visitor (strm);
      emit_visitor.visitOrderableImpl (root);
    }

  AfxMessageBox ("IDL generation completed.",
                 MB_OK | MB_ICONINFORMATION);
}

// ====================================================
// GME currently does not use this function
// You only need to implement it if other invokation mechanisms are used

void Component::objectInvokeEx( Project& project, Object& currentObject, const std::set<Object>& setSelectedObjects, long lParam )
  {
    if ( m_bIsInteractive )
      AfxMessageBox("This BON2 Component does not support objectInvokeEx method!");
  }

// ====================================================
// Implement application specific parameter-mechanism in these functions

Util::Variant Component::getParameter( const std::string& strName )
{
  // ======================
  // Insert application specific code here

  return Util::Variant();
}

void Component::setParameter( const std::string& strName, const Util::Variant& varValue )
{
  // ======================
  // Insert application specific code here
}

#ifdef GME_ADDON

// ====================================================
// If the component is an Add-On, then this method is called for every Global Event

void Component::globalEventPerformed( globalevent_enum event )
{
  // ======================
  // Insert application specific code here
}

// ====================================================
// If the component is an Add-On, then this method is called for every Object Event

void Component::objectEventPerformed( Object& object, unsigned long event, VARIANT v )
{
  // ======================
  // Insert application specific code here
}

#endif // GME_ADDON

}; // namespace BON
