// Log_Msg_MFCDlg.h : header file
// $Id$

#if !defined(AFX_LOG_MSG_MFCDLG_H__5BB9CACE_CBCE_40DD_B57F_ED05B79DB033__INCLUDED_)
#define AFX_LOG_MSG_MFCDLG_H__5BB9CACE_CBCE_40DD_B57F_ED05B79DB033__INCLUDED_

#include "ace/config-all.h"

#if defined (ACE_WIN32)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



/////////////////////////////////////////////////////////////////////////////
// CLog_Msg_MFCDlg dialog

class CLog_Msg_MFCDlg : public CDialog
{
// Construction
public:
	CLog_Msg_MFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLog_Msg_MFCDlg)
	enum { IDD = IDD_LOG_MSG_MFC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLog_Msg_MFCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLog_Msg_MFCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDebugButtonClicked();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif /* ACE_WIN32 */

#endif // !defined(AFX_LOG_MSG_MFCDLG_H__5BB9CACE_CBCE_40DD_B57F_ED05B79DB033__INCLUDED_)
