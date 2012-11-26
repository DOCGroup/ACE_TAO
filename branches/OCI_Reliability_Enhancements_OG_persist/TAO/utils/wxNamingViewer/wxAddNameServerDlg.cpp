// @file wxAddNameServerDlg.cpp
//
// @author Charlie Frasch  <cfrasch@atdesk.com>
//
// $Id$

#include "pch.h"
#include "wxAddNameServerDlg.h"

#include "wxNamingViewer.h"

#include "wx/sizer.h"
#include "wx/textctrl.h"

namespace  // anonymous
{
  void create_dialog_components( wxDialog* dialog)
    {
      wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL);

      {
        wxBoxSizer *sizer = new wxBoxSizer( wxHORIZONTAL);
        sizer->Add(
                   new wxStaticText( dialog, -1, "Name:" ),
                   0,
                   wxALL,
                   5);
        wxTextCtrl* text = new wxTextCtrl(
                                          dialog,
                                          IDC_NAME
                                          );
        text->SetName( "nameText");
        sizer->Add(
                   text,
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
                   new wxStaticText( dialog, -1, "IOR:" ),
                   0,
                   wxALL,
                   5);
        wxTextCtrl* text = new wxTextCtrl(
                                          dialog,
                                          IDC_IOR
                                          );
        text->SetName( "iorText");
        sizer->Add(
                   text,
                   1,
                   wxALL,
                   5);
        topsizer->Add(
                      sizer,
                      0,
                      wxALIGN_LEFT | wxEXPAND);
      }

      wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL);
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

WxAddNameServerDlg::WxAddNameServerDlg( wxWindow* parent)
#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  : wxDialog()
#else
  : wxDialog(
      parent,
      IDD_ADD_NAME_SERVER,
      "Add Name Server",
      wxDefaultPosition,
      wxSize(186,69),
      wxRAISED_BORDER | wxCAPTION | wxTHICK_FRAME | wxSYSTEM_MENU,
      "addNameServer")
#endif  // defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  , ior("")
  , serverName("")
{
#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  LoadFromResource( parent, "addNameServer");
#else
  create_dialog_components( this);
#endif  // defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
   wxButton* ctrl = static_cast<wxButton*>( wxFindWindowByName(
       "okButton",
       this));

   assert( ctrl);
 ctrl->SetDefault();
}

bool WxAddNameServerDlg::TransferDataFromWindow()
{
  wxTextCtrl* ctrl = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "iorText",
      this));
  assert( ctrl);
  ior = ctrl->GetValue();

  ctrl = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "nameText",
      this));
  assert( ctrl);
  serverName = ctrl->GetValue();
  return true;
}

