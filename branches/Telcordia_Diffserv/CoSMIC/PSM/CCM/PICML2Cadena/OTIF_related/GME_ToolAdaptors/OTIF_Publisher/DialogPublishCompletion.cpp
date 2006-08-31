// DialogPublishCompletion.cpp : implementation file
//

#include "stdafx.h"
#include "DialogPublishCompletion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPublishCompletion dialog


CDialogPublishCompletion::CDialogPublishCompletion(CWnd* pParent /*=NULL*/, 
												   int progressbar_minrange, int progressbar_maxrange)
	: CDialog(CDialogPublishCompletion::IDD, pParent), 
	  _progressbar_minrange(progressbar_minrange), 
	  _progressbar_maxrange(progressbar_maxrange)
{
	//{{AFX_DATA_INIT(CDialogPublishCompletion)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogPublishCompletion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPublishCompletion)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPublishCompletion, CDialog)
	//{{AFX_MSG_MAP(CDialogPublishCompletion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPublishCompletion message handlers

void CDialogPublishCompletion::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//We do NOTHING. This operation cannot be stopped.

	//CDialog::OnCancel();
}

void CDialogPublishCompletion::UpdateState(const char *newstring, short progress_offset)
{
	((CStatic*) GetDlgItem(IDC_STATIC_STATE))->SetWindowText(newstring);
	((CProgressCtrl*) GetDlgItem(IDC_PROGRESS_COMPLETION))->OffsetPos(progress_offset);
}

BOOL CDialogPublishCompletion::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	((CProgressCtrl*) GetDlgItem(IDC_PROGRESS_COMPLETION))->SetRange32(_progressbar_minrange,_progressbar_maxrange);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
