// $Id$
// WindozeCE.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WindozeCE.h"
#include "WindozeCEDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindozeCEApp

BEGIN_MESSAGE_MAP(CWindozeCEApp, CWinApp)
	//{{AFX_MSG_MAP(CWindozeCEApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindozeCEApp construction

CWindozeCEApp::CWindozeCEApp(LPCTSTR lpszAppName)
	: CWinApp(lpszAppName)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWindozeCEApp object

// WCE MFC apps require the application name to be specified in the CWinApp 
// constructor. A help contents filename may also be specified.

CWindozeCEApp theApp(_T("WindozeCE"));

/////////////////////////////////////////////////////////////////////////////
// CWindozeCEApp initialization

BOOL CWindozeCEApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	CWindozeCEDlg dlg;
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

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
