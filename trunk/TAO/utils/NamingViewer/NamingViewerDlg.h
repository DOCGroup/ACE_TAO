// $Id$
// NamingViewerDlg.h : header file
//

#if !defined(AFX_NAMINGVIEWERDLG_H__B9C0EB98_AD28_11D3_BF1F_00C0F03C565A__INCLUDED_)
#define AFX_NAMINGVIEWERDLG_H__B9C0EB98_AD28_11D3_BF1F_00C0F03C565A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "orbsvcs/CosNamingC.h"
#include "NamingTreeCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CNamingViewerDlg dialog

class CNamingViewerDlg : public CDialog
{
// Construction
public:
  CNamingViewerDlg(CORBA::ORB_ptr pORB, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNamingViewerDlg)
	enum { IDD = IDD_NAMINGVIEWER_DIALOG };
	CNamingTreeCtrl	m_Tree;
	CString	m_Server;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamingViewerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNamingViewerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelectNs();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
  afx_msg void OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI );
	DECLARE_MESSAGE_MAP()

  void Resolve();

  CORBA::ORB_ptr                  m_pORB;
  CosNaming::NamingContext_var    m_RootContext;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMINGVIEWERDLG_H__B9C0EB98_AD28_11D3_BF1F_00C0F03C565A__INCLUDED_)
