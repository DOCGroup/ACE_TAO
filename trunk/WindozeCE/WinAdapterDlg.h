// $Id$
// WinAdapterDlg.h : header file
//

#include "ace/OS.h"

#if !defined(AFX_WinAdapterDLG_H__5088F848_CE8B_11D1_9621_0040054CF0F0__INCLUDED_)
#define AFX_WinAdapterDLG_H__5088F848_CE8B_11D1_9621_0040054CF0F0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CWinAdapterDlg dialog

class CWinAdapterDlg : public CDialog
{
  // Construction
public:
  CWinAdapterDlg(CWnd* pParent = NULL);	// standard constructor

  // Dialog Data
  //{{AFX_DATA(CWinAdapterDlg)
  enum { IDD = IDD_WinAdapter_DIALOG };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA
  
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CWinAdapterDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  HICON m_hIcon;

  ACE_CE_Bridge ace;

  // Generated message map functions
  //{{AFX_MSG(CWinAdapterDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStart();
	//}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WinAdapterDLG_H__5088F848_CE8B_11D1_9621_0040054CF0F0__INCLUDED_)
