//---------------------------------------------------------------------------
#ifndef ChatClientWndH
#define ChatClientWndH
//---------------------------------------------------------------------------
#include "ReceiverImpl.h"
#include "BroadcasterC.h"
#include "ace/Auto_Ptr.h"
#include "ORBThread.h"
#include <Classes.hpp>
#include <Dialogs.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>

//---------------------------------------------------------------------------
// Message used to notify window of incoming data
#define WM_MESSAGE_RECEIVED (WM_APP + 0x123)
//---------------------------------------------------------------------------
class TChatClientWindow : public TForm
{
__published:  // IDE-managed Components
  TMemo *OutputMemo;
  TMemo *InputMemo;
  TOpenDialog *OpenDialog;
  void __fastcall FormClose (TObject *Sender, TCloseAction &Action);
  void __fastcall InputMemoKeyPress (TObject *Sender, char &Key);

private:
  // Function to read the server ior from a file.
  void __fastcall ReadIOR (String filename);

  // IOR of the obj ref of the server.
  String ior_;

  // The filename that stores the ior of the server
  String ior_file_name_;

  // Nickname of the user chatting.
  String nickname_;

  // We run the orb's main loop in a separate thread.
  auto_ptr<TORBThread> orb_thread_;

  // Our orb. Order is important! The orb must have a longer lifetime than
  // any of the servants or stub vars/ptrs. Therefore we declare the orb var
  // first. There is probably a better way to do this, maybe by using a
  // singleton.
  CORBA::ORB_var orb_;

  // Server object ptr.
  Broadcaster_var server_;

  // The receiver object.
  TReceiverImplementation receiver_i_;

  // Pointer to the receiver object registered with the ORB.
  Receiver_var receiver_var_;

public:    // User declarations
  __fastcall TChatClientWindow (TComponent* Owner);

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

