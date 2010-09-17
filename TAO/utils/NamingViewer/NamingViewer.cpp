// $Id$

#include "stdafx.h"
#include "NamingViewer.h"
#include "NamingViewerDlg.h"
#include "ace/ARGV.h"
#include "ace/ACE.h"
#include "ace/Argv_Type_Converter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamingViewerApp

BEGIN_MESSAGE_MAP(CNamingViewerApp, CWinApp)
  //{{AFX_MSG_MAP(CNamingViewerApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG
  ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamingViewerApp construction

CNamingViewerApp::CNamingViewerApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CNamingViewerApp object

CNamingViewerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNamingViewerApp initialization

BOOL CNamingViewerApp::InitInstance()
{
  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.
/*
   From MFC 5.0, Enable3dControls and Enable3dControlsStatic are obsolete
   because their functionality is incorporated into Microsoft's 32-bit
   operating systems. Basically no need to call with VC5.0 and above.

#if !defined (_WIN32_WCE)
#ifdef _AFXDLL
  Enable3dControls();   // Call this when using MFC in a shared DLL
#else
  Enable3dControlsStatic(); // Call this when linking to MFC statically
#endif
#endif
*/
  // Parse command line arguments so we can initialize ORB with them
  ACE_ARGV Argv (m_lpCmdLine);

  ACE::init();
  {
    int argc = Argv.argc ();
    CORBA::ORB_var ORB = CORBA::ORB_init(argc, Argv.argv());

    CNamingViewerDlg dlg(ORB);
    m_pMainWnd = &dlg;
    int const nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
      // TODO: Place code here to handle when the dialog is
      //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
      // TODO: Place code here to handle when the dialog is
      //  dismissed with Cancel
    }
  }
  ACE::fini();
  // Since the dialog has been closed, return FALSE so that we exit the
  //  application, rather than start the application's message pump.
  return FALSE;
}
