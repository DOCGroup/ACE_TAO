//---------------------------------------------------------------------------
#ifndef ReceiverImplH
#define ReceiverImplH
//---------------------------------------------------------------------------
#include "ReceiverS.h"
//---------------------------------------------------------------------------
class TReceiverImplementation : public POA_Receiver
{
public:
  /// Constructor.
  TReceiverImplementation (;

  /// Destructor.
  ~TReceiverImplementation ();

  virtual void message (const char* msg);

  /// Called when the chat server is going away.  The client
  /// implementation should shutdown the chat client in response to
  /// this.
  virtual void shutdown ();

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;
};
//---------------------------------------------------------------------------
#endif
