// $Id$
// wxNamingViewerFrame.h

#ifndef i_wxNamingViewerFrame_h
#define i_wxNamingViewerFrame_h

#include "orbsvcs/CosNamingC.h"
#include "wxNamingTree.h"

class WxNamingViewerFrame:
  public wxFrame
{
public:
  WxNamingViewerFrame(
      const wxString& title,
      const wxPoint& pos,
      const wxSize& size,
      CORBA::ORB_ptr pOrb);
  virtual ~WxNamingViewerFrame();

  void OnQuit( wxCommandEvent& event);
  void OnAbout( wxCommandEvent& event);
  void onSelectNS( wxCommandEvent& event);

private:
  CORBA::ORB_ptr pOrb;
  CosNaming::NamingContext_var rootContext;
  WxNamingTree* tree;
  wxTextCtrl* serverText;
  wxString server;
  class WxSelectNSDialog* selectNS;
  void resolve();
  void setDefaultNS();

  void onMenuCopy( wxCommandEvent& event);
  void onUpdateUICopy( wxUpdateUIEvent& event);

  DECLARE_EVENT_TABLE()
};

#endif
