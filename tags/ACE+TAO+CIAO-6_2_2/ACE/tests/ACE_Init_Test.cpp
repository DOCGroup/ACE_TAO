// $Id$

// This is a Win32-only test for verifying that the ACE dll
// initializes itself without having a nonstatic object manager
// present by way of a console app's main function.  It's a MFC dialog
// app - it pops up a dialog and spawns a thread - the thread will
// wait 2 seconds and programatically dismiss the dialog box.  The
// main thread waits for the other one to exit, and that's the test.
// If the ACE DLL doesn't initialize correctly, it will go boom!
//
// This test program was initially generated from MSVC AppWizard, then
// some files were renamed and moved around to fit in with the ACE
// test directory structure.
//
// ACE_Init_Test.cpp : Defines the class behaviors for the application.

#if !defined(ACE_HAS_MFC)

#include "test_config.h"

// If this is not a WIN32 platform do not even try to compile the
// test, many of the #includes make little sense.
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("ACE_Init_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("This is not a Win32 platform with MFC support, test skipped\n")));
  ACE_END_TEST;
  return 0;
}

#else

#include "ACE_Init_Test_StdAfx.h"
#include "ACE_Init_Test.h"
#include "ACE_Init_TestDlg.h"
#include "test_config.h"
#include "ace/ACE.h"
#include "ace/Init_ACE.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Thread_Manager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static ACE_THR_FUNC_RETURN wait_and_kill_dialog (void *pBox);

/////////////////////////////////////////////////////////////////////////////
// CACE_Init_TestApp

BEGIN_MESSAGE_MAP(CACE_Init_TestApp, CWinApp)
        //{{AFX_MSG_MAP(CACE_Init_TestApp)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG
        ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACE_Init_TestApp construction

CACE_Init_TestApp::CACE_Init_TestApp()
{
        // TODO: add construction code here,
        // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CACE_Init_TestApp object

CACE_Init_TestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CACE_Init_TestApp initialization

BOOL CACE_Init_TestApp::InitInstance()
{
  // This is needed because there's no overridden main(int, char *[])
  // which would normally handle the initialization.  Also see the
  // corresponding ACE::fini, below.
  ACE::init();

  ACE_START_TEST (ACE_TEXT ("ACE_Init_Test"));

  CACE_Init_TestDlg dlg;
  m_pMainWnd = &dlg;
  ACE_Thread_Manager::instance()->spawn (wait_and_kill_dialog,
                                         m_pMainWnd);
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

  ACE_Thread_Manager::instance()->wait();

  ACE_END_TEST;

  // Since the dialog has been closed, return FALSE so that we exit the
  //  application, rather than start the application's message pump.
  ACE::fini();

  return FALSE;
}

// This function runs in a separate thread - it will wait a couple of
// seconds and then programatically dismiss the dialog box.  If ACE is
// not properly initialized, we will have crashed before getting here.
static ACE_THR_FUNC_RETURN
wait_and_kill_dialog (void *pBox)
{
  CACE_Init_TestDlg *pDialog = reinterpret_cast<CACE_Init_TestDlg *> (pBox);
  ACE_OS::sleep(2);
  pDialog->EndModalLoop (IDOK);
  return 0;

}

#endif /* ACE_HAS_MFC */
