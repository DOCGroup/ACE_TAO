// $Id$
// wxBindNewContext.cpp


#include "pch.h"
#include "wxBindNewContext.h"


WxBindNewContext::WxBindNewContext( wxWindow* parent):
  wxDialog()
{
  LoadFromResource( parent, "bindNewContext");
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

