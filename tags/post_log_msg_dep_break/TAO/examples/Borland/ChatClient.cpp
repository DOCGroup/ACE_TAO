// $Id$
//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include <ace/ace.h>
#include <tao/corba.h>
#include "ChatClientWnd.h"
USERES("ChatClient.res");
USEFORM("ChatClientWnd.cpp", ChatClientWindow);
USEUNIT("BroadcasterC.cpp");
USEUNIT("BroadcasterS.cpp");
USEUNIT("ReceiverC.cpp");
USEUNIT("ReceiverS.cpp");
USEUNIT("ReceiverImpl.cpp");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  ACE::init ();

  try
    {
      Application->Initialize ();
      Application->Title = "CORBA Chat Client";
      Application->CreateForm (__classid (TChatClientWindow), &ChatClientWindow);
      Application->Run ();
    }
  catch (Exception &exception)
    {
      Application->ShowException (&exception);
    }
  catch (CORBA::Exception &exception)
    {
      ShowMessage (String ("CORBA exception: ") + exception._id ());
    }
  catch (...)
    {
      ShowMessage ("Unknown exception");
    }

  // Must explicitly free the window to ensure the orb is shut down
  // before ACE::fini is called.
  delete ChatClientWindow;

  ACE::fini ();

  return 0;
}
//---------------------------------------------------------------------------
