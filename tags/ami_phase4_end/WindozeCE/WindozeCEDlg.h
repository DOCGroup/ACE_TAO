// $Id$
// WindozeCEDlg.h : header file
//

#include "ace/OS.h"

#if !defined(AFX_WINDOZECEDLG_H__37544C6B_E3BC_11D1_968E_0040054CF0F0__INCLUDED_)
#define AFX_WINDOZECEDLG_H__37544C6B_E3BC_11D1_968E_0040054CF0F0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CWindozeCEDlg dialog

class CWindozeCEDlg : public CDialog
{
  // Construction
public:
  CWindozeCEDlg(CWnd* pParent = NULL);	// standard constructor

  // Dialog Data
  //{{AFX_DATA(CWindozeCEDlg)
  enum { IDD = IDD_WINDOZECE_DIALOG };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindozeCEDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
  //}}AFX_VIRTUAL

// Implementation
protected:
  HICON m_hIcon;

  ACE_CE_Bridge ace;

  // Generated message map functions
  //{{AFX_MSG(CWindozeCEDlg)
  virtual BOOL OnInitDialog();
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnStart();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
    };

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDOZECEDLG_H__37544C6B_E3BC_11D1_968E_0040054CF0F0__INCLUDED_)
