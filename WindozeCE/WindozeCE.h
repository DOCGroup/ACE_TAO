// $Id$
// WindozeCE.h : main header file for the WINDOZECE application
//

#if !defined(AFX_WINDOZECE_H__37544C69_E3BC_11D1_968E_0040054CF0F0__INCLUDED_)
#define AFX_WINDOZECE_H__37544C69_E3BC_11D1_968E_0040054CF0F0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ace/OS.h"

/////////////////////////////////////////////////////////////////////////////
// CWindozeCEApp:
// See WindozeCE.cpp for the implementation of this class
//

class CWindozeCEApp : public CWinApp
{
public:
	CWindozeCEApp(LPCTSTR lpszAppName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindozeCEApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWindozeCEApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
          private:
            ACE_Object_Manager obj_mgr_;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOZECE_H__37544C69_E3BC_11D1_968E_0040054CF0F0__INCLUDED_)
