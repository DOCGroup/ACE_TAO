// $Id$
#if !defined(AFX_BINDNEWCONTEXT_H__2AF5C352_B440_11D3_BF2C_00C0F03C565A__INCLUDED_)
#define AFX_BINDNEWCONTEXT_H__2AF5C352_B440_11D3_BF2C_00C0F03C565A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BindNewContext.h : header file
//
#include "orbsvcs/CosNamingC.H"
/////////////////////////////////////////////////////////////////////////////
// CBindNewContext dialog

class CBindNewContext : public CDialog
{
// Construction
public:
	CBindNewContext(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBindNewContext)
	enum { IDD = IDD_BIND_NEW_CONTEXT };
	CString	m_ID;
	CString	m_Kind;
	//}}AFX_DATA
  CosNaming::Name& GetName() {return m_Name;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBindNewContext)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBindNewContext)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  CosNaming::Name   m_Name;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BINDNEWCONTEXT_H__2AF5C352_B440_11D3_BF2C_00C0F03C565A__INCLUDED_)
