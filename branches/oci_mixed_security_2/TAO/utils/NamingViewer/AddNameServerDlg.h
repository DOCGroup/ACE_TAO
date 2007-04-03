// $Id$
#if !defined(AFX_ADDNAMESERVERDLG_H__2AF5C355_B440_11D3_BF2C_00C0F03C565A__INCLUDED_)
#define AFX_ADDNAMESERVERDLG_H__2AF5C355_B440_11D3_BF2C_00C0F03C565A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddNameServerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddNameServerDlg dialog

class CAddNameServerDlg : public CDialog
{
// Construction
public:
	CAddNameServerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddNameServerDlg)
	enum { IDD = IDD_ADD_NAME_SERVER };
	CString	m_IOR;
	CString	m_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddNameServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddNameServerDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDNAMESERVERDLG_H__2AF5C355_B440_11D3_BF2C_00C0F03C565A__INCLUDED_)
