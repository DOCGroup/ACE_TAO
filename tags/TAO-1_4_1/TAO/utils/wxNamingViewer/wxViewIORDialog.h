// $Id$
// wxViewIORDialog.h

#ifndef i_wxViewIORDialog_h
#define i_wxViewIORDialog_h


class WxViewIORDialog:
  public wxDialog
{
public:
  WxViewIORDialog(
      CORBA::ORB_ptr orb,
      CORBA::Object_ptr object,
      wxWindow* parent);

private:
  void OnApply( wxCommandEvent& event);
  void onIORText( wxCommandEvent& event);
  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();
  void decodeIOR();

  wxString typeID;
  wxString ior;
  class wxTextCtrl* typeIDText;
  class wxTextCtrl* iorText;
  class wxTreeCtrl* profiles;
  class wxButton* applyButton;

  CORBA::Object_var object;
  CORBA::ORB_ptr    orb;

  DECLARE_EVENT_TABLE()
};

#endif

