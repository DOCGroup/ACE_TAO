// Log_Msg_MFC.h : main header file for the LOG_MSG_MFC application
// $Id$

#if !defined(AFX_LOG_MSG_MFC_H__03FA2C60_71EA_44F9_AC4C_2F19168B4755__INCLUDED_)
#define AFX_LOG_MSG_MFC_H__03FA2C60_71EA_44F9_AC4C_2F19168B4755__INCLUDED_

#include "ace/config-all.h"

#if defined (ACE_WIN32)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// Add some includes here for our Logger
#include <ace/Log_Msg.h>
#include "MFC_Log.h"

/////////////////////////////////////////////////////////////////////////////
// CLog_Msg_MFCApp:
// See Log_Msg_MFC.cpp for the implementation of this class
//

class CLog_Msg_MFCApp : public CWinApp
{
public:
	CLog_Msg_MFCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLog_Msg_MFCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLog_Msg_MFCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    // MFC Log class for ACE_Log_Msg.  To be used by ACE_DEBUG
    MFC_Log m_mfc_logger;

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* ACE_WIN32 */

#endif // !defined(AFX_LOG_MSG_MFC_H__03FA2C60_71EA_44F9_AC4C_2F19168B4755__INCLUDED_)
