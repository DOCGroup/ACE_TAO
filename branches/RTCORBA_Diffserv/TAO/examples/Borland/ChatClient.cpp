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
USEUNIT("ORBThread.cpp");
//---------------------------------------------------------------------------
#pragma package(smart_init)

void ace_init(void)
{
  ACE::init();
}
#pragma startup ace_init

void ace_fini(void)
{
  ACE::fini();
}
#pragma exit ace_fini
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
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

  return 0;
}
//---------------------------------------------------------------------------
