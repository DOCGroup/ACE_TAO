// $Id$
// WindozeCEDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WindozeCE.h"
#include "WindozeCEDlg.h"
#include "ace/ARGV.h" 
#include "ACEApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CWnd *host_window_ = 0;

UINT worker (void * arg)
{
  ACE_ARGV cmd_line ((TCHAR *) arg);
  extern int ace_ce_main (int, ASYS_TCHAR *[]);

  int retv =  ace_ce_main (cmd_line.argc (), cmd_line.argv ());
  host_window_->SetDlgItemText (IDC_RUN, TEXT ("Stopped"));

  return retv;

  //   Sleep (500);
  //   for (size_t i = 0; i < cmd_line.argc (); i++, Sleep (500))
  //     ACE_DEBUG ((LM_DEBUG, TEXT ("The %d command arg is \"%s\""),
  //                 i, cmd_line.argv ()[i]));

  //   return 0;
  //   CString *msg = 0;
  //   while (1)
  //     {
  //       Sleep (500);
  //       msg = new CString;
  //       SYSTEMTIME curtime;
  //       GetLocalTime (&curtime);
  //       msg->Format (TEXT ("Time: %02d:%02d:%02d.%03d"), curtime.wHour,
  //                    curtime.wMinute, curtime.wSecond,
  //                    curtime.wMilliseconds);
  //       ACE_DEBUG ((LM_DEBUG, TEXT ("(%P|%t) Test logging: %s\n"), *msg));
  //       delete msg;
  //     }
  //   return 0;
}
                 
  
/////////////////////////////////////////////////////////////////////////////
// CWindozeCEDlg dialog

CWindozeCEDlg::CWindozeCEDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CWindozeCEDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CWindozeCEDlg)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindozeCEDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CWindozeCEDlg)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWindozeCEDlg, CDialog)
  //{{AFX_MSG_MAP(CWindozeCEDlg)
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_RUN, OnStart)
  //}}AFX_MSG_MAP
  END_MESSAGE_MAP()

  /////////////////////////////////////////////////////////////////////////////
  // CWindozeCEDlg message handlers

  BOOL CWindozeCEDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);			// Set big icon
  SetIcon(m_hIcon, FALSE);		// Set small icon
	
  this->SetDlgItemText (IDC_PROGRAM_OUTPUT, 
                        TEXT ("Fill in the command line arguments you want to pass to the program below:"));
  CenterWindow(GetDesktopWindow());	// center to the hpc screen

  this->ace.set_window (this, NC_ACE_NOTIFY, INC_ACE_LOGMSG);
  this->ace.set_self_default ();
  host_window_ = this;
  return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CWindozeCEDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
  static BOOL got_cmd = FALSE;
  int notify = HIBYTE (wParam);
  int id = LOBYTE (wParam);

  if (notify == NC_ACE_NOTIFY && id == INC_ACE_LOGMSG)
    {
      CString *msg = (CString *) lParam ;
      LPTSTR head = (LPTSTR) ((LPCTSTR) *msg);
      CString newbuf;

      LPTSTR newline;

      while ((newline = ACE_OS::strchr (head, '\n')) != NULL)
        {
          *newline = 0;
          newbuf += head;
          newbuf += "\r\n";
          head = newline+1;
        }

      newbuf += head;
      
      CEdit *editor = (CEdit *) GetDlgItem (IDC_PROGRAM_OUTPUT);
      editor -> ReplaceSel (newbuf);

      delete msg;
      return TRUE;
    }
  return CDialog::OnCommand(wParam, lParam);
}

void CWindozeCEDlg::OnTimer(UINT nIDEvent) 
{
  //   CWnd *text = this->GetDlgItem (IDC_TEXT_DISP);
  //   text->ModifyStyle (WS_VISIBLE, SS_CENTER, 0);
  //   this->UpdateWindow ();
	
  CDialog::OnTimer(nIDEvent);
}

void CWindozeCEDlg::OnStart() 
{
  static BOOL once = FALSE;

  if (! once)
    {
      CString cmdline;
      this->GetDlgItemText (IDC_PROGRAM_ARGUMENTS, cmdline);
      AfxBeginThread (worker, (void *) (LPCTSTR) cmdline);
      this->SetDlgItemText (IDC_RUN, TEXT ("Running..."));
      once = TRUE;
    }
}
