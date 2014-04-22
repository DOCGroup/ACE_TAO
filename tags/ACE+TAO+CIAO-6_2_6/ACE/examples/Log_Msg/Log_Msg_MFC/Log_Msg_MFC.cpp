// Log_Msg_MFC.cpp : Defines the class behaviors for the application.
// $Id$

#include "ace/Log_Msg.h"
#include "Log_Msg_MFC.h"
#include "ace/Init_ACE.h"

#if defined (ACE_WIN32)

#include "stdafx.h"
#include "Log_Msg_MFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <ace/ACE.h>

/////////////////////////////////////////////////////////////////////////////
// CLog_Msg_MFCApp

BEGIN_MESSAGE_MAP(CLog_Msg_MFCApp, CWinApp)
    //{{AFX_MSG_MAP(CLog_Msg_MFCApp)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLog_Msg_MFCApp construction

CLog_Msg_MFCApp::CLog_Msg_MFCApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLog_Msg_MFCApp object

CLog_Msg_MFCApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLog_Msg_MFCApp initialization

BOOL CLog_Msg_MFCApp::InitInstance()
{
  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

  // Need to call ACE::init() before calling any functions in ACE.
  // If we were writing a console based application, the ACE macros which
  // re-define main() would automatically call this.  However, since
  // we are running inside an MFC application, we need to explicitly call this.
  ACE::init();

  //  Call to set the ACE Log_Msg class here.
  //  From now on, whenever we use ACE_DEBUG macros, the output
  //  will be sent to the log() method of our MFC_Log class.
  ACE_Log_Msg::instance()->msg_callback( &m_mfc_logger );
  ACE_Log_Msg::instance()->set_flags( ACE_Log_Msg::MSG_CALLBACK );

  CLog_Msg_MFCDlg dlg;
  m_pMainWnd = &dlg;
  int nResponse = dlg.DoModal();
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

  // Since we called ACE::init(), we need to call ACE:fini(), otherwise
  // the debugger will complain about memory leaks.
  ACE::fini();

  // Since the dialog has been closed, return FALSE so that we exit the
  //  application, rather than start the application's message pump.
  return FALSE;
}

#endif /* ACE_WIN32 */
