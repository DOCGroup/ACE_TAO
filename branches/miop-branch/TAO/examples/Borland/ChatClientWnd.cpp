// $Id$
//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include <cctype>
#include <dos.h>
#include "ChatClientWnd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChatClientWindow *ChatClientWindow;
//---------------------------------------------------------------------------
__fastcall TChatClientWindow::TChatClientWindow (TComponent* Owner)
   : TForm (Owner),
     orb_thread_ (0)
{
  try
    {
      nickname_ = InputBox ("Enter Nickname",
                            "Enter the nickname you would like to use:",
                            "noname");

      if (!OpenDialog->Execute ())
        throw Exception ("IOR file not selected - unable to continue");
      ior_file_name_ = OpenDialog->FileName;

      // Retrieve the ORB.
      orb_ = CORBA::ORB_init (_argc, _argv, 0);

      // Get reference to the Root POA
      CORBA::Object_var obj =
        orb_->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa = PortableServer::POA::_narrow (obj);

      // Activate the POA manager
      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      mgr->activate ();

      // Run the orb in a separate thread
      orb_thread_.reset (new TORBThread (orb_));

      // set the orb in the receiver_i_ object.
      receiver_i_.orb (orb_);

      // read the ior from file
      ReadIOR (ior_file_name_);

      CORBA::Object_var server_object =
        orb_->string_to_object (ior_.c_str ());

      if (CORBA::is_nil (server_object.in ()))
        throw Exception ("Invalid IOR " + ior_);

      server_ = Broadcaster::_narrow (server_object);

      receiver_var_ = receiver_i_._this ();

      // Register ourselves with the server.
      server_->add (receiver_var_, nickname_.c_str ());
    }
  catch (CORBA::Exception &e)
    {
      ShowMessage ("CORBA Exception in TChatClientWindow constructor: " 
                   + String (e._id ()));
      throw;
    }
}
//---------------------------------------------------------------------------
void __fastcall TChatClientWindow::ReadIOR (String filename)
{
  std::auto_ptr<TStringList> ior (new TStringList);
  ior->LoadFromFile (filename);
  ior_ = ior->Text;
}
//---------------------------------------------------------------------------
void __fastcall TChatClientWindow::WMMessageReceived (TMessage& Message)
{
  String* str = (String*)Message.WParam;
  for (int i = 1; i <= str->Length (); i++)
    if (isspace ((*str)[i]))
      (*str)[i] = ' ';
  OutputMemo->Lines->Append (str->Trim ());
  delete str;
}
//---------------------------------------------------------------------------
void __fastcall TChatClientWindow::FormClose (TObject *Sender,
      TCloseAction &Action)
{
  try
    {
      // Remove ourselves from the server.
      server_->remove (receiver_var_);
      receiver_i_.shutdown ();
    }
  catch (CORBA::Exception &e)
    {
      ShowMessage ("CORBA Exception in FormClose: " + String (e._id ()));
    }
}
//---------------------------------------------------------------------------
void __fastcall TChatClientWindow::InputMemoKeyPress (TObject *Sender, char &Key)
{
  if (Key == '\n' || Key == '\r')
    {
      try
        {
          // Call the server function <say> to pass the string typed by
          // the server.
          server_->say (receiver_var_, InputMemo->Text.c_str ());
        }
      catch (CORBA::Exception &e)
        {
          ShowMessage ("CORBA Exception in InputMemoKeyPress: " + String (e._id ()));
        }

      Key = 0;
      InputMemo->Lines->Clear ();
    }
}
//---------------------------------------------------------------------------
