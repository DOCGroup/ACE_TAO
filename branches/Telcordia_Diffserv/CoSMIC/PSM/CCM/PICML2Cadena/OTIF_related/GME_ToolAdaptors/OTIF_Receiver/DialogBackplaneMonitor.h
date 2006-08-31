#if !defined(AFX_DIALOGBACKPLANEMONITOR_H__A3F8BDDC_F785_46B1_9578_C4C108FE1750__INCLUDED_)
#define AFX_DIALOGBACKPLANEMONITOR_H__A3F8BDDC_F785_46B1_9578_C4C108FE1750__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogBackplaneMonitor.h : header file
//

#include "resource.h"
#include "ReceiverToolAdaptor.h"
#include <afxcmn.h>
#include <string>


/////////////////////////////////////////////////////////////////////////////
// CDialogBackplaneMonitor dialog


using std::string;


// Note: Workflow MUST have a tooladaptor with name {paradigmname}_Receiver like PICML_Receiver!!
class CDialogBackplaneMonitor : public CDialog
{
// Construction
public:
	CDialogBackplaneMonitor(const string &paradigmname, 
							const string &projectfullpath,
							HWND & pushOutDialogHWND,
							CWnd* pParent = NULL);   // standard constructor

	~CDialogBackplaneMonitor();

// Dialog Data
	//{{AFX_DATA(CDialogBackplaneMonitor)
	enum { IDD = IDD_DIALOG_BACKPLANE_MONITOR };
	CListCtrl	m_ListDataNetworks;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogBackplaneMonitor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	string _paradigm_version;
	string _paradigmname;
	string _projectfullpath;
	ReceiverToolAdaptor _ta;
	HWND & _pushOutDialogHWND;
	Uml::Diagram _myUmlDiagram;
	Udm::UdmDiagram _myUdmDiagram;
	UdmCORBA::CORBADataNetwork *_dn;
	enum 
	{
		TIMERID_POLL_NOTIFIED
	};

	// Generated message map functions
	//{{AFX_MSG(CDialogBackplaneMonitor)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonGetSelectedDocument();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBACKPLANEMONITOR_H__A3F8BDDC_F785_46B1_9578_C4C108FE1750__INCLUDED_)
