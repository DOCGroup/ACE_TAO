// $Id$
#if !defined(AFX_VIEWIORDIALOG_H__A010D782_AF86_11D3_BF23_00C0F03C565A__INCLUDED_)
#define AFX_VIEWIORDIALOG_H__A010D782_AF86_11D3_BF23_00C0F03C565A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewIORDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ViewIORDialog dialog

class ViewIORDialog : public CDialog
{
// Construction
public:
  ViewIORDialog(CORBA::ORB_ptr pORB, CORBA::Object_ptr pObject, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ViewIORDialog)
	enum { IDD = IDD_VIEWIOR_DIALOG };
	CTreeCtrl	m_Profiles;
	CString	m_TypeID;
	CString	m_IOR;
	//}}AFX_DATA
  
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ViewIORDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ViewIORDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void DecodeIOR();

  CORBA::Object_var m_Object;
  CORBA::ORB_ptr    m_pORB;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWIORDIALOG_H__A010D782_AF86_11D3_BF23_00C0F03C565A__INCLUDED_)
