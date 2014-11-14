// $Id$

// server.cpp : Defines the class behaviors for the application.

#include "stdafx.h"
#include "server.h"

// Demospecific includes
#include "w32_test_impl.h"
// Demospecific includes end!

#include "MainFrm.h"
#include "serverDoc.h"
#include "serverView.h"

#include "ace/ACE.h"
#include "ace/Init_ACE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CServerApp

BEGIN_MESSAGE_MAP(CServerApp, CWinApp)
        //{{AFX_MSG_MAP(CServerApp)
        ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
        // Standard file based document commands
        ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
        ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
        // Standard print setup command
        ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// Thread-Function!
static unsigned long
spawn_my_orb_thread (void *)
{
  try
    {
      CORBA::ORB_var the_orb = CORBA::ORB_init (__argc, __argv);

      CORBA::Object_var orb_obj =
        the_orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var the_root_poa =
        PortableServer::POA::_narrow (orb_obj.in ());

      PortableServer::POAManager_var the_poa_manager =
        the_root_poa->the_POAManager ();

      the_poa_manager->activate ();

      // Initializing the NamingService
      W32_Test_Impl myservant;
      W32_Test_Interface_var orb_servant =
        myservant._this ();

      CORBA::String_var ior =
        the_orb->object_to_string (orb_servant.in ());

      FILE *output_file = ACE_OS::fopen ("ior.txt", "w");
      ACE_OS::fprintf (output_file,
                       "%s",
                       ior.in ());
      ACE_OS::fclose (output_file);

      the_orb->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 0;
    }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CServerApp construction

CServerApp::CServerApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}


CServerApp::~CServerApp()
{
  try
    {
      CORBA::ORB_var the_shutdown_orb;

      int argc = 0;
      ACE_TCHAR **argv = 0;
      const ACE_TCHAR *orb_name = ACE_TEXT("");

      // Retrieving a reference to the ORB used inside the thread
      the_shutdown_orb =
        CORBA::ORB_init (argc,
                         argv,
                         orb_name);

      the_shutdown_orb->shutdown (0);

      ACE_Thread_Manager::instance ()->wait ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }


  ACE::fini ();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerApp object

CServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CServerApp initialization

BOOL CServerApp::InitInstance()
{
  ACE::init();

  AfxEnableControlContainer();

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
  Enable3dControls(); // Call this when using MFC in a shared DLL
#else
  Enable3dControlsStatic(); // Call this when linking to MFC statically
#endif
#endif
*/

  // Change the registry key under which our settings are stored.
  // TODO: You should modify this string to be something appropriate
  // such as the name of your company or organization.
  SetRegistryKey(_T("Local AppWizard-Generated Applications"));

  LoadStdProfileSettings();  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.

  CSingleDocTemplate* pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(
                                        IDR_MAINFRAME,
                                        RUNTIME_CLASS(CServerDoc),
                                        RUNTIME_CLASS(CMainFrame),       // main SDI frame window
                                        RUNTIME_CLASS(CServerView));
  AddDocTemplate(pDocTemplate);

  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  // Dispatch commands specified on the command line
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  // The one and only window has been initialized, so show and update it.
  m_pMainWnd->ShowWindow(SW_SHOW);
  m_pMainWnd->UpdateWindow();

  ACE_Thread_Manager::instance()->spawn (spawn_my_orb_thread);

  return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
        CAboutDlg();

// Dialog Data
        //{{AFX_DATA(CAboutDlg)
        enum { IDD = IDD_ABOUTBOX };
        //}}AFX_DATA

        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CAboutDlg)
        protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

// Implementation
protected:
        //{{AFX_MSG(CAboutDlg)
                // No message handlers
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
        //{{AFX_DATA_INIT(CAboutDlg)
        //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CAboutDlg)
        //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
        //{{AFX_MSG_MAP(CAboutDlg)
                // No message handlers
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CServerApp::OnAppAbout()
{
        CAboutDlg aboutDlg;
        aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CServerApp message handlers

