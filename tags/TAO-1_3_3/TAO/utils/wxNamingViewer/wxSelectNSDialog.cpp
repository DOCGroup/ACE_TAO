// $Id$
// wxSelectNSDialog.cpp

#include "pch.h"
#include "wxSelectNSDialog.h"

#include "ace/Configuration.h"
#include "wxAddNameServerDlg.h"
#include "wxAutoDialog.h"
#include "wxNamingViewer.h"



BEGIN_EVENT_TABLE( WxSelectNSDialog, wxDialog)
  EVT_BUTTON( IDC_ADD, WxSelectNSDialog::onAdd)
  EVT_BUTTON( IDC_DEFAULT, WxSelectNSDialog::onDefault)
  EVT_BUTTON( wxID_OK, WxSelectNSDialog::onOK)
  EVT_BUTTON( IDC_REMOVE, WxSelectNSDialog::onRemove)
  EVT_INIT_DIALOG( WxSelectNSDialog::onInitDialog)
  EVT_LISTBOX_DCLICK( IDC_SERVERS, WxSelectNSDialog::onLeftDClick)
END_EVENT_TABLE()


WxSelectNSDialog::WxSelectNSDialog( wxWindow* parent):
  wxDialog(),
  config( 0)
{
  LoadFromResource( parent, "selectNS");
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
  wxButton* ctrl = static_cast<wxButton*>( wxFindWindowByName(
      "okButton",
      this));
  assert( ctrl);
  ctrl->SetDefault();
  servers->SetFocus();
}


void WxSelectNSDialog::onLeftDClick( wxMouseEvent& event)
{
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
