// $Id$
// wxNamingViewerFrame.cpp

#include "pch.h"
#include "wxNamingViewerFrame.h"

#include "wx/clipbrd.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "wxNamingViewer.h"
#include "wxSelectNSDialog.h"

#if defined(__WXGTK__) || defined(__WXMOTIF__)
#include "mondrian.xpm"
#endif


enum {
    menuQuit = 1,
    menuAbout,
    menuCopy,
    buttonSelectNS = IDC_SELECT_NS
};


BEGIN_EVENT_TABLE( WxNamingViewerFrame, wxFrame)
    EVT_MENU( menuQuit,  WxNamingViewerFrame::OnQuit)
    EVT_MENU( menuAbout, WxNamingViewerFrame::OnAbout)
    EVT_MENU( menuCopy, WxNamingViewerFrame::onMenuCopy)
    EVT_UPDATE_UI( menuCopy, WxNamingViewerFrame::onUpdateUICopy)
    EVT_BUTTON( buttonSelectNS, WxNamingViewerFrame::onSelectNS)
END_EVENT_TABLE()


WxNamingViewerFrame::WxNamingViewerFrame(
    const wxString& title,
    const wxPoint& pos,
    const wxSize& size,
    CORBA::ORB_ptr pOrb):
  wxFrame(
      (wxFrame *)NULL,
      -1, title,
      pos,
      size),
  pOrb( pOrb),
  tree( 0),
  server( "")
{
  SetIcon( wxICON( mondrian));

  wxMenu* fileMenu = new wxMenu( "", wxMENU_TEAROFF);
  fileMenu->Append( menuQuit, "E&xit", "Quit this program");

  wxMenu* editMenu = new wxMenu( "", wxMENU_TEAROFF);
  editMenu->Append( menuCopy, "&Copy\tCtrl+C");

  wxMenu* helpMenu = new wxMenu();
  helpMenu->Append( menuAbout, "&About...", "Show about dialog");

  wxMenuBar* menuBar = new wxMenuBar();
  menuBar->Append( fileMenu, "&File");
  menuBar->Append( editMenu, "&Edit");
  menuBar->Append( helpMenu, "&Help");
  SetMenuBar( menuBar);

  wxPanel* panel = new wxPanel(
      this,
      -1);
  wxBoxSizer* topSizer = new wxBoxSizer( wxVERTICAL);
  topSizer->Add( panel, 1, wxGROW);

  wxBoxSizer* nsSizer = new wxStaticBoxSizer(
      new wxStaticBox(
          panel,
          IDC_NS,
          "Name Server"),
      wxHORIZONTAL);
  serverText = new wxTextCtrl(
      panel,
      IDC_SERVER,
      "",
      wxDefaultPosition,
      wxSize( 158, 20),
      wxTE_READONLY);
  wxButton* selectNSButton = new wxButton(
      panel,
      IDC_SELECT_NS,
      "Select",
      wxDefaultPosition,
      wxSize( 50, 20));
  nsSizer->Add( serverText, 1, wxGROW);
  nsSizer->Add( selectNSButton, 0, wxALIGN_LEFT | wxLEFT, 10);

  wxBoxSizer* treeSizer = new wxBoxSizer( wxVERTICAL);
  treeSizer->Add( nsSizer, 0, wxGROW);
  tree = new WxNamingTree(
      panel,
      WxNamingTree::treeCtrl);
  treeSizer->Add( tree, 1, wxGROW);

  panel->SetAutoLayout( TRUE );
  panel->SetSizer( treeSizer );
  treeSizer->Fit( this);
  treeSizer->SetSizeHints( this);

  selectNS = new WxSelectNSDialog( this);

  tree->setOrb( pOrb);
  setDefaultNS();
  resolve();
}


WxNamingViewerFrame::~WxNamingViewerFrame()
{
  selectNS->Destroy();
}


void WxNamingViewerFrame::OnQuit( wxCommandEvent& WXUNUSED(event))
{
  Close( TRUE);
}


void WxNamingViewerFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
  wxString msg( "wxNamingViewer Version 1.0\n"
      "Copyright (C) 2000\n"
      " Author: Charlie Frasch (cfrasch@spawar.navy.mil)");

  wxMessageBox(
      msg.c_str(),
      "About wxNamingViewer",
       wxOK | wxICON_INFORMATION,
       this);
}


void WxNamingViewerFrame::onMenuCopy( wxCommandEvent& WXUNUSED( event))
{
  tree->copySelectedToClipboard();
}


void WxNamingViewerFrame::onSelectNS( wxCommandEvent& WXUNUSED( event))
{
  switch( selectNS->ShowModal()) {

    case wxID_OK:
      try {

// TODO: need hourglass
        CORBA::Object_var object = pOrb->string_to_object(
            selectNS->getIOR().c_str());
        rootContext = CosNaming::NamingContext::_narrow( object.in ());
        server = selectNS->getServerName();
        serverText->SetValue( server);
        resolve();

      } catch( CORBA::Exception& ex) {

        wxMessageBox(
            ex._id(),
            "CORBA::Exception");

      }
      break;

    case IDC_DEFAULT:
// TODO: need hourglass
      setDefaultNS();
      resolve();
      break;

  }
}


void WxNamingViewerFrame::onUpdateUICopy( wxUpdateUIEvent& event)
{
  event.Enable( tree->isNodeSelected());
}


void WxNamingViewerFrame::resolve()
{
  tree->resolve( rootContext.in ());
}


void WxNamingViewerFrame::setDefaultNS()
{
  server = "Default";
  serverText->SetValue( server);

  TAO_Naming_Client client;
  if (client.init( pOrb) == 0) {

    rootContext = client.get_context();

    // For debugging, sets up some initial contexts in the NS
/*
    CosNaming::NamingContext_var app2;
    app2 = rootContext->new_context();

    CosNaming::Name name;
    name.length( 1);
    name[0].id = CORBA::string_dup( "app2");
    rootContext->rebind_context( name, app2);

    CosNaming::NamingContext_var devices;
    devices = app2->new_context();
    name[0].id = CORBA::string_dup( "devices");
    app2->rebind_context( name, devices);

    CosNaming::NamingContext_var collections;
    collections = app2->new_context();
    name[0].id = CORBA::string_dup( "collections");
    app2->rebind_context( name, collections);

    name[0].id = CORBA::string_dup( "my app");
    CORBA::Object_var myApp = CORBA::Object::_nil();
    app2->rebind( name, myApp);

    name[0].id = CORBA::string_dup( "dev1");
    CORBA::Object_var dev1 = CORBA::Object::_nil();
    devices->rebind( name, myApp);

    name[0].id = CORBA::string_dup( "dev2");
    CORBA::Object_var dev2 = CORBA::Object::_nil();
    devices->rebind( name, myApp);
*/
  }
}
