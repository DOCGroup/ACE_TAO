// $Id$
// wxBindDialog.h

#ifndef i_wxBindDialog_h
#define i_wxBindDialog_h

#include "orbsvcs/CosNamingC.h"


class WxBindDialog:
  public wxDialog
{
public:
  WxBindDialog( bool isContext, CORBA::ORB_ptr orb, wxWindow* parent);

  CORBA::Object_ptr getObject() {return object;};
  CosNaming::Name& getName() {return name;};

private:
  wxString ior;
  wxString id;
  wxString kind;

  virtual bool TransferDataFromWindow();

  void onViewIOR( wxCommandEvent& event);

  CORBA::Object_var object;
  CORBA::ORB_ptr    orb;
  CosNaming::Name   name;
  bool              isContext;

  DECLARE_EVENT_TABLE()
};

#endif

