// $Id$
// WinAdapter.h : main header file for the WinAdapter application
//

#if !defined(AFX_WinAdapter_H__5088F846_CE8B_11D1_9621_0040054CF0F0__INCLUDED_)
#define AFX_WinAdapter_H__5088F846_CE8B_11D1_9621_0040054CF0F0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinAdapterApp:
// See WinAdapter.cpp for the implementation of this class
//

class CWinAdapterApp : public CWinApp
{
public:
	CWinAdapterApp(LPCTSTR lpszAppName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinAdapterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinAdapterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WinAdapter_H__5088F846_CE8B_11D1_9621_0040054CF0F0__INCLUDED_)
