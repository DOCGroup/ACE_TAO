// $Id$
// SelectNSDialog.cpp : implementation file
//

#include "stdafx.h"
#include "NamingViewer.h"
#include "SelectNSDialog.h"
#include "AddNameServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectNSDialog dialog


CSelectNSDialog::CSelectNSDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectNSDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectNSDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  m_pConfig = 0;
}


void CSelectNSDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectNSDialog)
	DDX_Control(pDX, IDC_SERVERS, m_Servers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectNSDialog, CDialog)
	//{{AFX_MSG_MAP(CSelectNSDialog)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectNSDialog message handlers

void CSelectNSDialog::OnOK() 
{
	// TODO: Add extra validation here
	int index = m_Servers.GetCurSel();
  if(index == LB_ERR)
  {
    AfxMessageBox("You must select a server or cancel");
    return;
  }
  char* pIOR = (char*)m_Servers.GetItemData(index);
  m_IOR = pIOR;
  m_Servers.GetText(index, m_Name);
	CDialog::OnOK();
}

void CSelectNSDialog::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CAddNameServerDlg Dialog;
  if(Dialog.DoModal() != IDOK)
  {
    return;
  }
  ACE_Configuration_Section_Key Section = m_pConfig->root_section();
  ACE_TString Value = Dialog.m_IOR;
  m_pConfig->set_string_value(Section, Dialog.m_Name, Value);
  int pos = m_Servers.AddString(Dialog.m_Name);
  char* pIOR = new char[Value.length() + 1];
  strcpy(pIOR, Value.c_str());
  m_Servers.SetItemData(pos, (DWORD)pIOR);

}

void CSelectNSDialog::OnRemove() 
{
	// TODO: Add your control notification handler code here
	int index = m_Servers.GetCurSel();
  if(index == LB_ERR)
  {
    return;
  }
  delete (char*)m_Servers.GetItemData(index);
  CString Name;
  m_Servers.GetText(index, Name);
  ACE_Configuration_Section_Key Section = m_pConfig->root_section();;
  m_pConfig->remove_value(Section, Name);
  m_Servers.DeleteString(index);
}

BOOL CSelectNSDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  HKEY hKey = ACE_Configuration_Win32Registry::resolve_key(HKEY_LOCAL_MACHINE, "Software\\TAO\\NamingViewer\\Servers");
  m_pConfig = new ACE_Configuration_Win32Registry(hKey);
  ACE_Configuration_Section_Key Section = m_pConfig->root_section();;
  int index = 0; 
  ACE_TString name; 
  ACE_Configuration::VALUETYPE type;
  while(m_pConfig->enumerate_values(Section, index, name, type) == 0)
  {
    ACE_TString value;
    if(m_pConfig->get_string_value(Section, name.c_str(), value) == 0)
    {
      int pos = m_Servers.AddString(name.c_str());
      char* pIOR = new char[value.length() + 1];
      strcpy(pIOR, value.c_str());
      m_Servers.SetItemData(pos, (DWORD)pIOR);
    }
    index++;
  }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectNSDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	int count = m_Servers.GetCount();
  for(int i=0; i < count; i++)
  {
    delete (char*)m_Servers.GetItemData(i);
  }
}
