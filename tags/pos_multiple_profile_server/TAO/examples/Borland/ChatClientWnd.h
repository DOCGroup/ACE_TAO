// $Id$
//---------------------------------------------------------------------------
#ifndef ChatClientWndH
#define ChatClientWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ReceiverImpl.h"
#include "BroadcasterC.h"
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
// Message used to notify window of incoming data
#define WM_MESSAGE_RECEIVED (WM_APP + 0x123)
//---------------------------------------------------------------------------
class TChatClientWindow : public TForm
{
  __published:	// IDE-managed Components
  TMemo *OutputMemo;
  TMemo *InputMemo;
  TOpenDialog *OpenDialog;
  TTimer *TimerToEnsureRegularMessages;
  void __fastcall FormClose (TObject *Sender, TCloseAction &Action);
  void __fastcall InputMemoKeyPress (TObject *Sender, char &Key);
  void __fastcall TimerToEnsureRegularMessagesTimer (TObject *Sender);
   
private:
  void __fastcall ReadIOR (String filename);
  // Function to read the server ior from a file.

  String ior_;
  // IOR of the obj ref of the server.

  String ior_file_name_;
  // The filename that stores the ior of the server

  String nickname_;
  // Nickname of the user chatting.

  CORBA::ORB_var orb_;
  // Our orb. Order is important! The orb must have a longer lifetime than
  // any of the servants or stub vars/ptrs. Therefore we declare the orb var
  // first. There is probably a better way to do this, maybe by using a
  // singleton.

  Broadcaster_var server_;
  // Server object ptr.

  TReceiverImplementation receiver_i_;
  // The receiver object.

  Receiver_var receiver_var_;
  // Pointer to the receiver object registered with the ORB.

public:		// User declarations
  __fastcall TChatClientWindow (TComponent* Owner);
  void __fastcall ApplicationMessage (MSG& msg, bool& Handled);

protected: // Message handlers
  void __fastcall WMMessageReceived (TMessage& Message);
  BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER (WM_MESSAGE_RECEIVED, TMessage, WMMessageReceived)
    END_MESSAGE_MAP (TForm)
    };
//---------------------------------------------------------------------------
extern PACKAGE TChatClientWindow *ChatClientWindow;
//---------------------------------------------------------------------------
#endif

