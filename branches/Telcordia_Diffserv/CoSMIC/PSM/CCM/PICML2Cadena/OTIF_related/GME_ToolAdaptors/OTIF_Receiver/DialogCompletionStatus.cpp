// DialogCompletionStatus.cpp : implementation file
//

#include "stdafx.h"
#include "DialogCompletionStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogCompletionStatus dialog


CDialogCompletionStatus::CDialogCompletionStatus(CWnd* pParent /*=NULL*/, 
												   const char * caption, int progressbar_minrange, int progressbar_maxrange)
	: CDialog(CDialogCompletionStatus::IDD, pParent), 
	  _caption(caption),
	  _progressbar_minrange(progressbar_minrange), 
	  _progressbar_maxrange(progressbar_maxrange)
{
	//{{AFX_DATA_INIT(CDialogCompletionStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogCompletionStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogCompletionStatus)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogCompletionStatus, CDialog)
	//{{AFX_MSG_MAP(CDialogCompletionStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogCompletionStatus message handlers

void CDialogCompletionStatus::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//We do NOTHING. This operation cannot be stopped.

	//CDialog::OnCancel();
}

void CDialogCompletionStatus::UpdateState(const char *newstring, short progress_offset)
{
	((CStatic*) GetDlgItem(IDC_STATIC_STATE))->SetWindowText(newstring);
	((CProgressCtrl*) GetDlgItem(IDC_PROGRESS_COMPLETION))->OffsetPos(progress_offset);
}

BOOL CDialogCompletionStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	SetWindowText(_caption.c_str());

	((CProgressCtrl*) GetDlgItem(IDC_PROGRESS_COMPLETION))->SetRange32(_progressbar_minrange,_progressbar_maxrange);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
