// $Id$
// wxSelectNSDialog.h

#ifndef i_WxSelectNSDialog_h
#define i_WxSelectNSDialog_h

class WxSelectNSDialog : public
  wxDialog
{
public:
  WxSelectNSDialog( wxWindow* parent);
  virtual ~WxSelectNSDialog();

  const wxString& getIOR() const {return ior;};
  const wxString& getServerName() const {return serverName;};

private:
  void onAdd( wxCommandEvent& event);
  void onDefault( wxCommandEvent& event);
  void onInitDialog( wxInitDialogEvent& event);
  void onOK( wxCommandEvent& event);
  void onRemove( wxCommandEvent& event);
  void onLeftDClick( wxMouseEvent& event);

  class ACE_Configuration* config;

  wxListBox* servers;
  wxString ior;
  wxString serverName;

  DECLARE_EVENT_TABLE()
};

#endif

