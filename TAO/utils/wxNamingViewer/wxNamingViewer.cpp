// @file wxNamingViewer.cpp
// A wxWindows-based CORBA naming viewer.
// Based on NamingViewer Version 1.0 by Chris Hafey (chris@stentorsoft.com)
//
// @author:  Charlie Frasch (cfrasch@atdesk.com)
//
// $Id$

#include "ace/Init_ACE.h"
#include "pch.h"
#include "wx/resource.h"
#include "wxNamingViewerFrame.h"
#include "wxNamingViewer.wxr"

#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
#include "wxNamingViewer.wxr"
#endif // defined(wxUSE_RESOURCES)

// Stuff for the C++Builder IDE
#define USEUNIT(x)
#define USERC(x)
#define USEFILE(x)
USEFILE("Readme");
USEUNIT("wxNamingViewerFrame.cpp");
USEUNIT("wxNamingTree.cpp");
USEUNIT("wxNamingObject.cpp");
USEFILE("wxNamingViewer.wxr");
USEUNIT("wxSelectNSDialog.cpp");
USEUNIT("wxBindNewContext.cpp");
USEUNIT("wxAddNameServerDlg.cpp");
USEUNIT("wxBindDialog.cpp");
USEUNIT("wxViewIORDialog.cpp");
USERC("wxNamingViewer.rc");

//---------------------------------------------------------------------------

class WxNamingViewer:
    public wxApp
{
public:
  virtual bool OnInit();
  virtual int OnExit();
};


IMPLEMENT_APP(WxNamingViewer)

// Need this to keep C++Builder 4 happy
#ifdef __BORLANDC__
extern WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int);
#endif


int WxNamingViewer::OnExit()
{
  ACE::fini();
  return 0;
}


bool WxNamingViewer::OnInit()
{
  ACE::init();
  CORBA::ORB_var orb = CORBA::ORB_init(
      argc,
      argv);

#if defined(wxUSE_RESOURCES) && (wxUSE_RESOURCES == 1)
  wxResourceParseData( bindObject);
  wxResourceParseData( selectNS);
  wxResourceParseData( addNameServer);
  wxResourceParseData( bindNewContext);
  wxResourceParseData( viewIOR);
#endif // defined(wxUSE_WX_RESOURCES)

  WxNamingViewerFrame* frame = new WxNamingViewerFrame(
      "wxNamingViewer",
      wxDefaultPosition,
      wxSize( 248, 198),
      orb.in ());
  frame->Show( TRUE);
  return TRUE;
}
