// $Id$
#if !defined(AFX_BINDDIALOG_H__1F34ADC5_B0D8_11D3_BF27_00C0F03C565A__INCLUDED_)
#define AFX_BINDDIALOG_H__1F34ADC5_B0D8_11D3_BF27_00C0F03C565A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BindDialog.h : header file
//
#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
/////////////////////////////////////////////////////////////////////////////
// CBindDialog dialog

class CBindDialog : public CDialog
{
// Construction
public:
  CBindDialog(bool Context, CORBA::ORB_ptr pORB, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBindDialog)
	enum { IDD = IDD_BIND };
	CString	m_IOR;
	CString	m_ID;
	CString	m_Kind;
	//}}AFX_DATA
  CORBA::Object_ptr GetObject() {return m_Object;};

  CosNaming::Name& GetName() {return m_Name;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBindDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBindDialog)
	afx_msg void OnViewior();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
  CORBA::Object_var m_Object;
  CORBA::ORB_ptr    m_pORB;
  CosNaming::Name   m_Name;
  bool              m_Context;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BINDDIALOG_H__1F34ADC5_B0D8_11D3_BF27_00C0F03C565A__INCLUDED_)
