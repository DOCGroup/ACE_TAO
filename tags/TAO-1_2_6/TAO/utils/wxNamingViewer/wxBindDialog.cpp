// $Id$
// wxBindDialog.cpp

#include "pch.h"
#include "wxBindDialog.h"

#include "wxAutoDialog.h"
#include "wxNamingViewer.h"
#include "wxViewIORDialog.h"


BEGIN_EVENT_TABLE( WxBindDialog, wxDialog)
  EVT_BUTTON( IDC_VIEWIOR, WxBindDialog::onViewIOR)
END_EVENT_TABLE()


WxBindDialog::WxBindDialog(
    bool isContext,
    CORBA::ORB_ptr orb,
    wxWindow* parent):
  wxDialog(),
  isContext( isContext),
  orb( orb)
{
  LoadFromResource( parent, "bindObject");
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
  WxAutoDialog<WxViewIORDialog> dialog( new WxViewIORDialog( orb, object, 0));
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

