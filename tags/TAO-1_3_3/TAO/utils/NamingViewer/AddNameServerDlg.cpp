// $Id$

#include "stdafx.h"
#include "NamingViewer.h"
#include "AddNameServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddNameServerDlg dialog


CAddNameServerDlg::CAddNameServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddNameServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddNameServerDlg)
	m_IOR = _T("corbaloc:iiop:1.1@myserver:nsport/NameService");
	m_Name = _T("");
	//}}AFX_DATA_INIT
}


void CAddNameServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddNameServerDlg)
	DDX_Text(pDX, IDC_IOR, m_IOR);
	DDX_Text(pDX, IDC_NAME, m_Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddNameServerDlg, CDialog)
	//{{AFX_MSG_MAP(CAddNameServerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddNameServerDlg message handlers

void CAddNameServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
