// @file wxBindDialog.cpp
//
// @author Charlie Frasch  <cfrasch@atdesk.com>
//
// $Id$

#include "pch.h"
#include "wxBindDialog.h"

#include "wxAutoDialog.h"
#include "wxNamingViewer.h"
#include "wxViewIORDialog.h"

#include "wx/sizer.h"

namespace  // anonymous
{

void create_dialog_components(wxDialog* dialog)
{
  wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

  {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(new wxStaticText(dialog, -1, "ID:" ), 0, wxALL, 5);
    wxTextCtrl* text = new wxTextCtrl(dialog, IDC_ID);
    text->SetName("idTextCtrl");
    sizer->Add(text, 1, wxALL, 5);
    topsizer->Add(sizer, 0, wxALIGN_LEFT | wxEXPAND);
  }

  {
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(new wxStaticText(dialog, -1, "Kind:" ), 0, wxALL, 5);
    wxTextCtrl* text = new wxTextCtrl(dialog, IDC_KIND);
    text->SetName("kindTextCtrl");
    sizer->Add(text, 1, wxALL, 5);
    topsizer->Add(sizer, 0, wxALIGN_LEFT | wxEXPAND);
  }

  {
    wxBoxSizer *sizer = new wxBoxSizer( wxHORIZONTAL);
    sizer->Add(new wxStaticText( dialog, -1, "IOR:" ), 0, wxALL, 5);
    wxTextCtrl* text = new wxTextCtrl(dialog, IDC_IOR);
    text->SetName( "iorTextCtrl");
    sizer->Add(text, 1, wxALL, 5);
    topsizer->Add(sizer, 0, wxALIGN_LEFT | wxEXPAND);
  }

  wxBoxSizer* button_sizer = new wxBoxSizer( wxHORIZONTAL);
  {
    wxButton* button = new wxButton( dialog, wxID_OK, "OK" );
    button->SetName( "okButton");
    button_sizer->Add(button, 0, wxALL, 5);
  }
  {
    button_sizer->Add(new wxButton( dialog, wxID_CANCEL, "Cancel" ),
                      0,
                      wxALL,
                      5);
  }
  topsizer->Add(button_sizer, 0, wxALIGN_CENTER);

  dialog->SetSizer( topsizer);
  topsizer->SetSizeHints( dialog);
}


};  // anonymous


BEGIN_EVENT_TABLE( WxBindDialog, wxDialog)
  EVT_BUTTON( IDC_VIEWIOR, WxBindDialog::onViewIOR)
END_EVENT_TABLE()


WxBindDialog::WxBindDialog(
    bool isContext,
    CORBA::ORB_ptr orb,
    wxWindow* parent)
#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  : wxDialog()
#else
  : wxDialog(
      parent,
      IDD_BIND,
      "Bind Object/Context",
      wxDefaultPosition,
      wxSize( 300, 75),
      wxRAISED_BORDER | wxCAPTION | wxTHICK_FRAME | wxSYSTEM_MENU,
      "bindObject")
#endif  // defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  , ior()
  , id ()
  , kind ()
  , object ()
  , orb (orb)
  , name ()
  , isContext (isContext)
{
#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  LoadFromResource( parent, "bindObject");
#else
  create_dialog_components( this);
#endif  // defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  if (isContext) {

    SetTitle( "Bind Context");

  } else {

    SetTitle( "Bind Object");
  }
  wxButton* ctrl = static_cast<wxButton*>( wxFindWindowByName(
      "okButton",
      this));
  assert( ctrl);
  ctrl->SetDefault();
}


void WxBindDialog::onViewIOR( wxCommandEvent& WXUNUSED(event))
{
  TransferDataFromWindow();
  WxAutoDialog<WxViewIORDialog> dialog( new WxViewIORDialog( orb, object.in(), 0));
  dialog->ShowModal();
}


bool WxBindDialog::TransferDataFromWindow()
{
  wxTextCtrl* ctrl = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "iorTextCtrl",
      this));
  assert( ctrl);
  ior = ctrl->GetValue();

  ctrl = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "idTextCtrl",
      this));
  assert( ctrl);
  id = ctrl->GetValue();

  ctrl = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "kindTextCtrl",
      this));
  assert( ctrl);
  kind = ctrl->GetValue();

  name.length( 1);
  name[0].id = CORBA::string_dup( id);
  name[0].kind = CORBA::string_dup( kind);
  try {

    object = orb->string_to_object( ior);

  } catch(CORBA::Exception& ex) {

    wxMessageBox( ex._rep_id(), "Invalid IOR");
    object = CORBA::Object::_nil();

  }
  return true;
}

