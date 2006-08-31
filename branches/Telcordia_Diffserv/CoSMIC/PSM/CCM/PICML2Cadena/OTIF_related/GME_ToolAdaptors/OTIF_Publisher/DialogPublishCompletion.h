#if !defined(AFX_DIALOGPUBLISHCOMPLETION_H__87B688EA_5DAA_4F65_A135_B6668D53DFB1__INCLUDED_)
#define AFX_DIALOGPUBLISHCOMPLETION_H__87B688EA_5DAA_4F65_A135_B6668D53DFB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogPublishCompletion.h : header file
//

#include "resource.h"
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogPublishCompletion dialog

class CDialogPublishCompletion : public CDialog
{
// Construction
public:
	CDialogPublishCompletion(CWnd* pParent = NULL, int progressbar_minrange=0, int progressbar_maxrange=100);   // standard constructor
	void UpdateState(const char *newstring, short progress_offset);

// Dialog Data
	//{{AFX_DATA(CDialogPublishCompletion)
	enum { IDD = IDD_DIALOG_PUBLISH_COMPLETION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogPublishCompletion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int _progressbar_minrange;
	int _progressbar_maxrange;

	// Generated message map functions
	//{{AFX_MSG(CDialogPublishCompletion)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGPUBLISHCOMPLETION_H__87B688EA_5DAA_4F65_A135_B6668D53DFB1__INCLUDED_)
