// @file wxSelectNSDialog.cpp
//
// @author Charlie Frasch  <cfrasch@atdesk.com>
//
// $Id$

#include "pch.h"
#include "wxSelectNSDialog.h"

#include "ace/Configuration.h"
#include "wx/sizer.h"
#include "wxAddNameServerDlg.h"
#include "wxAutoDialog.h"
#include "wxNamingViewer.h"

namespace  // anonymous
{
  void create_dialog_components( wxDialog* dialog)
    {
      wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL);

      topsizer->Add(
                    new wxListBox(
                                  dialog,
                                  IDC_SERVERS,
                                  wxDefaultPosition,
                                  wxDefaultSize,
                                  0,
                                  0,
                                  wxLB_SINGLE,
                                  wxDefaultValidator,
                                  "serversList"
                                  ),
                    1,
                    wxEXPAND | wxALL,
                    5);

      wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );
      {
        wxButton* okButton = new wxButton( dialog, wxID_OK, "OK" );
        okButton->SetName( "okButton");
        button_sizer->Add(
                          okButton,
                          0,
                          wxALL,
                          5);
      }
      button_sizer->Add(
                        new wxButton( dialog, IDC_DEFAULT, "Default" ),
                        0,
                        wxALL,
                        5);
      button_sizer->Add(
                        new wxButton( dialog, IDC_ADD, "Add" ),
                        0,
                        wxALL,
                        5);
      button_sizer->Add(
                        new wxButton( dialog, IDC_REMOVE, "Remove" ),
                        0,
                        wxALL,
                        5);
      button_sizer->Add(
                        new wxButton( dialog, wxID_CANCEL, "Cancel" ),
                        0,
                        wxALL,
                        5);

      topsizer->Add(
                    button_sizer,
                    0,
                    wxALIGN_CENTER);

      dialog->SetSizer( topsizer);
      topsizer->SetSizeHints( dialog);
    }
};  // anonymous

BEGIN_EVENT_TABLE( WxSelectNSDialog, wxDialog)
  EVT_BUTTON( IDC_ADD, WxSelectNSDialog::onAdd)
  EVT_BUTTON( IDC_DEFAULT, WxSelectNSDialog::onDefault)
  EVT_BUTTON( wxID_OK, WxSelectNSDialog::onOK)
  EVT_BUTTON( IDC_REMOVE, WxSelectNSDialog::onRemove)
  EVT_INIT_DIALOG( WxSelectNSDialog::onInitDialog)
  EVT_LISTBOX_DCLICK( IDC_SERVERS, WxSelectNSDialog::onLeftDClick)
END_EVENT_TABLE()


WxSelectNSDialog::WxSelectNSDialog( wxWindow* parent)
#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  : wxDialog()
#else
  : wxDialog(
      parent,
      IDD_SELECT_NS,
      "Select Naming Service",
      wxDefaultPosition,
      wxSize(181,94),
      wxRAISED_BORDER | wxCAPTION | wxTHICK_FRAME | wxSYSTEM_MENU,
      "selectNS")
#endif  // defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  , config( 0)
{
#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  LoadFromResource( parent, "selectNS");
#else
  create_dialog_components( this);
#endif  // defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  servers = static_cast<wxListBox*>( wxFindWindowByName(
      "serversList",
      this));
  assert( servers);

#if defined (ACE_WIN32)
  HKEY hKey = ACE_Configuration_Win32Registry::resolve_key(
      HKEY_LOCAL_MACHINE,
      "Software\\TAO\\NamingViewer\\Servers");
  config = new ACE_Configuration_Win32Registry( hKey);
#else
  // TODO: non-windoz
  ACE_Configuration_Heap *heap = new ACE_Configuration_Heap;
  heap->open ();
  config = heap;
#endif
  ACE_Configuration_Section_Key section = config->root_section();
  int index = 0;
  ACE_TString name;
  ACE_Configuration::VALUETYPE type;
  while(config->enumerate_values( section, index, name, type) == 0) {

    ACE_TString value;
    if(config->get_string_value( section, name.c_str(), value) == 0) {

      servers->Append(
          name.c_str(),
          new wxString( value.c_str()));

    }
    index++;

  }
}


WxSelectNSDialog::~WxSelectNSDialog()
{
  int count = servers->Number();
  for (int i = 0; i < count; i++) {

    delete static_cast<wxString*>( servers->GetClientData( i));

  }
}


void WxSelectNSDialog::onAdd( wxCommandEvent& WXUNUSED(event))
{
  WxAutoDialog<WxAddNameServerDlg> dialog( new WxAddNameServerDlg( this));
  if (dialog->ShowModal() == wxID_OK) {

    servers->Append(
        dialog->getServerName(),
        new wxString( dialog->getIor()));
    ACE_Configuration_Section_Key section = config->root_section();
    ACE_TString value = dialog->getIor().c_str();
    config->set_string_value(
        section,
        dialog->getServerName().c_str(),
        value);

  }
}


void WxSelectNSDialog::onDefault( wxCommandEvent& WXUNUSED(event))
{
  EndModal( IDC_DEFAULT);
}


void WxSelectNSDialog::onInitDialog( wxInitDialogEvent& event)
{
  ACE_UNUSED_ARG( event);

  wxButton* ctrl = static_cast<wxButton*>( wxFindWindowByName(
      "okButton",
      this));
  assert( ctrl);
  ctrl->SetDefault();
  servers->SetFocus();
}


void WxSelectNSDialog::onLeftDClick( wxMouseEvent& event)
{
  ACE_UNUSED_ARG( event);

  int index = servers->GetSelection();
  // I don't think index will ever be -1!
  assert( index != -1);
  ior = *static_cast<wxString*>( servers->GetClientData( index));
  serverName = servers->GetString( index);
  EndModal( wxID_OK);
}


void WxSelectNSDialog::onOK( wxCommandEvent& WXUNUSED(event))
{
  int index = servers->GetSelection();
  if (index == -1) {

    wxMessageBox(
        "You must select a server or cancel",
        "Error",
        wxOK | wxICON_EXCLAMATION);
    return;
  }
  ior = *static_cast<wxString*>( servers->GetClientData( index));
  serverName = servers->GetString( index);
  EndModal( wxID_OK);
}


void WxSelectNSDialog::onRemove( wxCommandEvent& WXUNUSED(event))
{
  int index = servers->GetSelection();
  if (index != -1) {

    wxString name = servers->GetString( index);
    delete static_cast<wxString*>( servers->GetClientData( index));
    servers->Delete( index);

    ACE_Configuration_Section_Key section = config->root_section();;
    config->remove_value( section, name);

  }
}
