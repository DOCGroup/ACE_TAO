// $Id$

#include "stdafx.h"
#include "NamingViewer.h"
#include "BindDialog.h"
#include "ViewIORDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBindDialog dialog


CBindDialog::CBindDialog(bool Context, CORBA::ORB_ptr pORB, CWnd* pParent /*=NULL*/)
	: CDialog(CBindDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBindDialog)
	m_IOR = _T("");
	m_ID = _T("");
	m_Kind = _T("");
	//}}AFX_DATA_INIT
  m_pORB = pORB;
  m_Context = Context;
}


void CBindDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBindDialog)
	DDX_Text(pDX, IDC_IOR, m_IOR);
	DDX_Text(pDX, IDC_ID, m_ID);
	DDX_Text(pDX, IDC_KIND, m_Kind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBindDialog, CDialog)
	//{{AFX_MSG_MAP(CBindDialog)
	ON_BN_CLICKED(IDC_VIEWIOR, OnViewior)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBindDialog message handlers

void CBindDialog::OnViewior() 
{
	// TODO: Add your control notification handler code here
  try
  {
    UpdateData();
    m_Object = m_pORB->string_to_object(m_IOR);
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._rep_id(), "CORBA::Exception");
    return;
  }
  ViewIORDialog Dialog(m_pORB, m_Object);
  Dialog.DoModal();
}

BOOL CBindDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(m_Context)
  {
    SetWindowText("Bind Context");
  }
  else
  {
    SetWindowText("Bind Object");
  }
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBindDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
  m_Name.length(1);
  m_Name[0].id = CORBA::string_dup(m_ID);
  m_Name[0].kind = CORBA::string_dup(m_Kind);
  try
  {
    m_Object = m_pORB->string_to_object(m_IOR);
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._rep_id(), "Invalid IOR");
    return;
  }
  CDialog::OnOK();
}
