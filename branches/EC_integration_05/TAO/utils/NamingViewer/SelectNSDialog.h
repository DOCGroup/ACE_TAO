// $Id$
#if !defined(AFX_SELECTNSDIALOG_H__2AF5C353_B440_11D3_BF2C_00C0F03C565A__INCLUDED_)
#define AFX_SELECTNSDIALOG_H__2AF5C353_B440_11D3_BF2C_00C0F03C565A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectNSDialog.h : header file
//
#include "ace/Configuration.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectNSDialog dialog

class CSelectNSDialog : public CDialog
{
// Construction
public:
	CSelectNSDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectNSDialog)
	enum { IDD = IDD_SELECT_NS };
	CListBox	m_Servers;
	//}}AFX_DATA
  
  const CString& GetIOR() const {return m_IOR;};
  const CString& GetName() const {return m_Name;};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectNSDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectNSDialog)
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


  ACE_Configuration_Win32Registry* m_pConfig;
  CString   m_IOR;
  CString   m_Name;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTNSDIALOG_H__2AF5C353_B440_11D3_BF2C_00C0F03C565A__INCLUDED_)
