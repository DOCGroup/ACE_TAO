// @file wxBindNewContext.cpp
//
// @author Charlie Frasch  <cfrasch@atdesk.com>
//
// $Id$

#include "pch.h"
#include "wxBindNewContext.h"

#include "wxNamingViewer.h"

namespace  // anonymous
{
  void create_dialog_components( wxDialog* dialog)
  {
    wxBoxSizer *topsizer = new wxBoxSizer( wxVERTICAL);

    {
      wxBoxSizer *sizer = new wxBoxSizer( wxHORIZONTAL);
      sizer->Add(
                 new wxStaticText( dialog, -1, "ID:" ),
                 0,
                 wxALL,
                 5);
      wxTextCtrl* text = new wxTextCtrl(
                                        dialog,
                                        IDC_ID
                                        );
      text->SetName( "idText");
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
                 new wxStaticText( dialog, -1, "Kind:" ),
                 0,
                 wxALL,
                 5);
      wxTextCtrl* text = new wxTextCtrl(
                                        dialog,
                                        IDC_KIND
                                        );
      text->SetName( "kindText");
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

}; // anonymous

WxBindNewContext::WxBindNewContext( wxWindow* parent)
#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  : wxDialog()
#else
   : wxDialog(
       parent,
       IDD_BIND_NEW_CONTEXT,
       "Bind New Context",
       wxDefaultPosition,
       wxSize( 300, 75),
       wxRAISED_BORDER | wxCAPTION | wxTHICK_FRAME | wxSYSTEM_MENU,
       "bindNewContext")
#endif  // defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
 {
#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
    LoadFromResource( parent, "bindNewContext");
#else
   create_dialog_components( this);
#endif  // defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
 }

bool WxBindNewContext::TransferDataFromWindow()
{
  name.length(1);
  wxTextCtrl* ctrl = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "idText",
      this));
  assert( ctrl);
  name[0].id = CORBA::string_dup( ctrl->GetValue());

  ctrl = static_cast<wxTextCtrl*>( wxFindWindowByName(
      "kindText",
      this));
  assert( ctrl);
  name[0].kind = CORBA::string_dup( ctrl->GetValue());

  return true;
}

