// $Id$
// NamingViewer.h : main header file for the NAMINGVIEWER application
//

#if !defined(AFX_NAMINGVIEWER_H__B9C0EB96_AD28_11D3_BF1F_00C0F03C565A__INCLUDED_)
#define AFX_NAMINGVIEWER_H__B9C0EB96_AD28_11D3_BF1F_00C0F03C565A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNamingViewerApp:
// See NamingViewer.cpp for the implementation of this class
//

class CNamingViewerApp : public CWinApp
{
public:
	CNamingViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamingViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNamingViewerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMINGVIEWER_H__B9C0EB96_AD28_11D3_BF1F_00C0F03C565A__INCLUDED_)
