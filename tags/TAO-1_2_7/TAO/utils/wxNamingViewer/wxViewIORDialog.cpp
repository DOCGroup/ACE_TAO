// $Id$
// wxViewIORDialog.cpp

#include "pch.h"
#include "wxViewIORDialog.h"

#include "tao/Profile.h"
#include "wx/treectrl.h"
#include "wxNamingViewer.h"


BEGIN_EVENT_TABLE( WxViewIORDialog, wxDialog)
  EVT_BUTTON( wxID_APPLY, WxViewIORDialog::OnApply)
  EVT_TEXT( IDC_IOR, WxViewIORDialog::onIORText)
END_EVENT_TABLE()


WxViewIORDialog::WxViewIORDialog(
    CORBA::ORB_ptr orb,
    CORBA::Object_ptr object,
    wxWindow* parent):
  wxDialog(),
  orb( orb)
{
  LoadFromResource( parent, "viewIOR");
  iorText = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "iorText",
      this));
  assert( iorText);
  typeIDText = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "typeIDText",
      this));
  assert( typeIDText);
  profiles = new wxTreeCtrl(
      this,
      IDC_PROFILES,
      wxPoint( 7, 99),
      wxSize( 675, 140));
  wxButton* ctrl = static_cast<wxButton*>( wxFindWindowByName(
      "okButton",
      this));
  assert( ctrl);
  ctrl->SetDefault();
  applyButton = static_cast<wxButton*>( wxFindWindowByName(
      "applyButton",
      this));
  assert( applyButton);
  applyButton->Enable( false);

  if (object != CORBA::Object::_nil()) {

    WxViewIORDialog::object = CORBA::Object::_duplicate( object);

  }
  CORBA::String_var ior = orb->object_to_string( object);
  WxViewIORDialog::ior = ior;
  decodeIOR();
}


void WxViewIORDialog::decodeIOR()
{
  profiles->DeleteAllItems();

  // if object is nil, return out
  if(CORBA::is_nil( object)) {

    typeID = "";
    TransferDataToWindow();
    return;

  }

  // Get the stub
  TAO_Stub* stub = object->_stubobj();
  const char* type = stub->type_id;
  typeID = type ? type : ""; // special case for INS objects, tao doesn't get the type id
  TransferDataToWindow();

  // Iterate through each profile and add an entry to the tree control
  const TAO_MProfile& baseProfiles = stub->base_profiles();
  CORBA::ULong count = baseProfiles.profile_count();
  wxTreeItemId rootItem = profiles->AddRoot( "Profiles");
  for( CORBA::ULong slot = 0; slot < count; slot++) {

    const TAO_Profile* profile = baseProfiles.get_profile( slot);
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY {

      // The need to const_cast should disappear in TAO 1.1.2
      char* profileString =
          const_cast<TAO_Profile*>(profile)->to_string(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      profiles->AppendItem( rootItem, profileString);
      delete [] profileString;

    } ACE_CATCH( CORBA::Exception, ex) {

      wxMessageBox( ex._id(), "CORBA::Exception");

    } ACE_ENDTRY;

  }
  profiles->Expand( rootItem);
}


void WxViewIORDialog::OnApply( wxCommandEvent& event)
{
  wxDialog::OnApply( event);
  try {

    object = orb->string_to_object( ior);
    decodeIOR();

  } catch( CORBA::Exception& ex) {

    wxMessageBox( ex._id(), "CORBA::Exception");
  }
}


void WxViewIORDialog::onIORText( wxCommandEvent& event)
{
  // Enable the Apply button if the IOR has changed
  if (event.GetString() != ior) {

    applyButton->Enable( true);

  }
}


bool WxViewIORDialog::TransferDataFromWindow()
{
  ior = iorText->GetValue();
  return true;
}


bool WxViewIORDialog::TransferDataToWindow()
{
  iorText->SetValue( ior);
  typeIDText->SetValue( typeID);
  return true;
}

