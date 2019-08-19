//---------------------------------------------------------------------------
#ifndef ReceiverImplH
#define ReceiverImplH
//---------------------------------------------------------------------------
#include "ReceiverS.h"
//---------------------------------------------------------------------------
class TReceiverImplementation : public POA_Receiver
{
public:
  // Constructor.
  TReceiverImplementation (void);

  // Destructor.
  ~TReceiverImplementation (void);

  virtual void message (const char* msg);

  virtual void shutdown (void);
  // Called when the chat server is going away.  The client
  // implementation should shutdown the chat client in response to
  // this.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.
};
//---------------------------------------------------------------------------
#endif
