// $Id$

#include "stdafx.h"
#include "NamingViewer.h"
#include "ViewIORDialog.h"
#include "tao/IIOP_Profile.h"
#include "tao/Stub.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ViewIORDialog dialog


ViewIORDialog::ViewIORDialog(CORBA::ORB_ptr pORB, CORBA::Object_ptr pObject, CWnd* pParent /*=NULL*/)
	: CDialog(ViewIORDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(ViewIORDialog)
	m_TypeID = _T("");
	m_IOR = _T("");
	//}}AFX_DATA_INIT
  m_pORB = pORB;
  if(pObject)
  {
    m_Object = CORBA::Object::_duplicate(pObject);
  }
  CORBA::String_var IOR = pORB->object_to_string(pObject);
  m_IOR = IOR;
}


void ViewIORDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ViewIORDialog)
	DDX_Control(pDX, IDC_PROFILES, m_Profiles);
	DDX_Text(pDX, IDC_TYPE_ID, m_TypeID);
	DDX_Text(pDX, IDC_IOR, m_IOR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ViewIORDialog, CDialog)
	//{{AFX_MSG_MAP(ViewIORDialog)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ViewIORDialog message handlers

BOOL ViewIORDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  DecodeIOR();
  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ViewIORDialog::OnApply() 
{
	// TODO: Add your control notification handler code here
  UpdateData();
  try
  {
    m_Object = m_pORB->string_to_object(m_IOR);
    DecodeIOR();
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._rep_id(), "CORBA::Exception");
  }
}

void ViewIORDialog::DecodeIOR()
{
  // Reset the fields
  m_Profiles.DeleteAllItems();
  m_TypeID = "";
  UpdateData(FALSE);
  
  // if object is nil, return out
  if(CORBA::is_nil(m_Object))
  {
    return;
  }
  
  // Get the stub
  TAO_Stub* pStub = m_Object->_stubobj();
  const char* pType = pStub->type_id;
  m_TypeID = pType ? pType : ""; // special case for INS objects, tao doesn't get the type id
  UpdateData(FALSE);
  
  // Iterate through each profile and add an entry to the tree control
  const TAO_MProfile& BaseProfiles= pStub->base_profiles();
  CORBA::ULong Count = BaseProfiles.profile_count();
  for(CORBA::ULong Slot = 0; Slot < Count; Slot++)
  {
    const TAO_Profile* pProfile = BaseProfiles.get_profile(Slot);
    HTREEITEM hProfile;
    switch(pProfile->tag())
    {
    case 0: //IOP::TAG_INTERNET_IOP: 
      {
        TAO_IIOP_Profile* pIIOPProfile = (TAO_IIOP_Profile*)pProfile;
        CString ProfileString;
        ProfileString.Format("IIOP %d.%d", 
          pIIOPProfile->version().major, 
          pIIOPProfile->version().minor);
        hProfile = m_Profiles.InsertItem(ProfileString);
        TAO_IIOP_Endpoint* pIIOPEndpoint =
          (TAO_IIOP_Endpoint*)pIIOPProfile->endpoint ();
        while(pIIOPEndpoint)
        {
          CString EndpointString;
          EndpointString.Format("%s:%d",
            pIIOPEndpoint->host(), 
            pIIOPEndpoint->port());
          HTREEITEM hItem = m_Profiles.InsertItem(EndpointString, hProfile);
          m_Profiles.EnsureVisible(hItem);
          pIIOPEndpoint = (TAO_IIOP_Endpoint*)pIIOPEndpoint->next();
        }
      
      }
      break;
    default:
      {
        CString ProfileString;
        char* pToString = ((TAO_Profile*)pProfile)->to_string();
        ProfileString.Format("Unknown Profile (Tag=%d) %s", pProfile->tag(), pToString);
        delete pToString;
        hProfile = m_Profiles.InsertItem(ProfileString);
      }
      break;
    }
  }
}
