// $Id$
// BindNewContext.cpp : implementation file

#include "stdafx.h"
#include "NamingViewer.h"
#include "BindNewContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBindNewContext dialog


CBindNewContext::CBindNewContext(CWnd* pParent /*=NULL*/)
	: CDialog(CBindNewContext::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBindNewContext)
	m_ID = _T("");
	m_Kind = _T("");
	//}}AFX_DATA_INIT
}


void CBindNewContext::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBindNewContext)
	DDX_Text(pDX, IDC_ID, m_ID);
	DDX_Text(pDX, IDC_KIND, m_Kind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBindNewContext, CDialog)
	//{{AFX_MSG_MAP(CBindNewContext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBindNewContext message handlers

void CBindNewContext::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
  m_Name.length(1);
  m_Name[0].id = CORBA::string_dup(m_ID);
  m_Name[0].kind = CORBA::string_dup(m_Kind);
	CDialog::OnOK();
}
