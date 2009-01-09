// @file wxViewIORDialog.cpp
//
// @author Charlie Frasch  <cfrasch@atdesk.com>
//
// $Id$

#include "pch.h"
#include "wxViewIORDialog.h"

#include "tao/Profile.h"
#include "tao/Stub.h"
#include "wx/sizer.h"
#include "wx/textctrl.h"
#include "wx/treectrl.h"
#include "wxNamingViewer.h"
#include "ace/SString.h"

namespace  // anonymous
{
  void create_dialog_components( wxDialog* dialog)
    {
      wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL);

      {
        wxBoxSizer *sizer = new wxBoxSizer( wxHORIZONTAL);
        sizer->Add(
                   new wxStaticText( dialog, -1, "IOR:" ),
                   0,
                   wxALL,
                   5);
        wxTextCtrl* iorText = new wxTextCtrl(
                                             dialog,
                                             IDC_IOR
                                             );
        iorText->SetName( "iorText");
        sizer->Add(
                   iorText,
                   1,
                   wxALL,
                   5);
        topsizer->Add(
                      sizer,
                      0,
                      wxALIGN_LEFT | wxEXPAND);
      }

      {
        wxBoxSizer *sizer = new wxBoxSizer( wxHORIZONTAL);
        sizer->Add(
                   new wxStaticText( dialog, -1, "Repository ID:" ),
                   0,
                   wxALL,
                   5);
        wxTextCtrl* typeID = new wxTextCtrl(
                                            dialog,
                                            IDC_TYPE_ID
                                            );
        typeID->SetName( "typeIDText");
        sizer->Add(
                   typeID,
                   1,
                   wxALL,
                   5);
        topsizer->Add(
                      sizer,
                      0,
                      wxALIGN_LEFT | wxEXPAND);
      }

      {
        wxTreeCtrl* profiles = new wxTreeCtrl(
                                              dialog,
                                              IDC_PROFILES,
                                              wxDefaultPosition,
                                              wxSize( 675, 140)
                                              );
        profiles->SetName( "profilesTree");
        topsizer->Add(
                      profiles,
                      1,
                      wxALL | wxEXPAND,
                      5);
      }

      wxBoxSizer* button_sizer = new wxBoxSizer( wxHORIZONTAL);
      {
        wxButton* okButton = new wxButton( dialog, wxID_OK, "OK" );
        okButton->SetName( "okButton");
        button_sizer->Add(
                          okButton,
                          0,
                          wxALL,
                          5);
      }
      {
        wxButton* applyButton = new wxButton( dialog, wxID_APPLY, "Apply" );
        applyButton->SetName( "applyButton");
        button_sizer->Add(
                          applyButton,
                          0,
                          wxALL,
                          5);
      }
      {
        button_sizer->Add(
                          new wxButton( dialog, wxID_CANCEL, "Cancel" ),
                          0,
                          wxALL,
                          5);
      }
      topsizer->Add(
                    button_sizer,
                    0,
                    wxALIGN_CENTER);

      dialog->SetSizer( topsizer);
      topsizer->SetSizeHints( dialog);
    }

};  // anonymous

BEGIN_EVENT_TABLE( WxViewIORDialog, wxDialog)
  EVT_BUTTON( wxID_APPLY, WxViewIORDialog::OnApply)
  EVT_TEXT( IDC_IOR, WxViewIORDialog::onIORText)
END_EVENT_TABLE()


WxViewIORDialog::WxViewIORDialog(
    CORBA::ORB_ptr orb,
    CORBA::Object_ptr object,
    wxWindow* parent)
#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  : wxDialog()
#else
  : wxDialog(
      parent,
      IDD_NAMINGVIEWER_DIALOG,
      "View IOR",
      wxDefaultPosition,
      wxSize(394,127),
      wxRAISED_BORDER | wxCAPTION | wxTHICK_FRAME | wxSYSTEM_MENU,
      "viewIOR")
#endif  // defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  , orb( orb)
{
#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  LoadFromResource( parent, "viewIOR");
#else
  create_dialog_components( this);
#endif  // defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  iorText = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "iorText",
      this));
  assert( iorText);
  typeIDText = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "typeIDText",
      this));
  assert( typeIDText);
  profiles = static_cast<wxTreeCtrl*>( wxFindWindowByName(
      "profilesTree",
      this));
  assert( typeIDText);
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
  if(CORBA::is_nil( object.in())) {

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
    try{

      // The need to const_cast should disappear in TAO 1.1.2 BUT IT DIDN'T
      char* profileString =
          const_cast<TAO_Profile*>(profile)->to_string();
      profiles->AppendItem( rootItem, profileString);
      delete [] profileString;

    } catch (const CORBA::Exception& ex) {

      wxMessageBox( ex._info().c_str(), "CORBA::Exception");

    }

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

    wxMessageBox( ex._info().c_str(), "CORBA::Exception");
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

