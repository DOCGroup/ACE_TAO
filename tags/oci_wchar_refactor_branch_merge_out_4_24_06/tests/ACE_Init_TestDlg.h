// $Id$
// ACE_Init_TestDlg.h : header file

#if !defined(AFX_ACE_INIT_TESTDLG_H__64FDCA00_F7F9_11D2_89B6_00A024CC68DB__INCLUDED_)
#define AFX_ACE_INIT_TESTDLG_H__64FDCA00_F7F9_11D2_89B6_00A024CC68DB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// CACE_Init_TestDlg dialog

class CACE_Init_TestDlg : public CDialog
{
// Construction
public:
	CACE_Init_TestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CACE_Init_TestDlg)
	enum { IDD = IDD_ACE_INIT_TEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACE_Init_TestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CACE_Init_TestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif /* !defined(AFX_ACE_INIT_TESTDLG_H__64FDCA00_F7F9_11D2_89B6_00A024CC68DB__INCLUDED_) */
