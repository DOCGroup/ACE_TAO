// $Id$
// ViewIORDialog.cpp : implementation file
//

#include "stdafx.h"
#include "NamingViewer.h"
#include "ViewIORDialog.h"
#include "tao/IIOP_Profile.h"

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
    MessageBox(ex._id(), "CORBA::Exception");
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
    case 0://IOP::TAG_INTERNET_IOP: 
      {
        TAO_IIOP_Profile* pIIOPProfile = (TAO_IIOP_Profile*)pProfile;
        TAO_IIOP_Endpoint* pIIOPEndpoint =
          (TAO_IIOP_Endpoint*)pProfile->endpoint ();
        CString ProfileString;
        ProfileString.Format("IOP %d.%d@%s:%d", 
          pIIOPProfile->version().major, 
          pIIOPProfile->version().minor,
          pIIOPEndpoint->host(), 
          pIIOPEndpoint->port());
        hProfile = m_Profiles.InsertItem(ProfileString);
      }
      break;
    default:
      {
        CString ProfileString;
        ProfileString.Format("Unknown %X", pProfile->tag());
        hProfile = m_Profiles.InsertItem(ProfileString);
      }
      break;
    }

    // XXX Add in the components
    //const TAO_Tagged_Components& Compoents = pProfile->tagged_components();

  }
/*
  TAO_ObjectKey* pKey = pIIOPP->_key();
  if(pKey)
  {
    m_TextKey.resize(pKey->length() + 1);
    for(int i=0; i < pKey->length(); i++)
    {
      if(isprint((*pKey)[i]))
      {
        m_TextKey[i] = (*pKey)[i];
      }
      else
      {
        m_TextKey[i] = '.';
      }
      char Digit[4];
      sprintf(Digit, "%02X ", (*pKey)[i]);
      m_HexKey += Digit;
    }
  }  
*/
}
