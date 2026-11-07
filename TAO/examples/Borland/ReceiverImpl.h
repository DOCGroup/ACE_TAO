//---------------------------------------------------------------------------
#ifndef ReceiverImplH
#define ReceiverImplH
//---------------------------------------------------------------------------
#include "ReceiverS.h"
//---------------------------------------------------------------------------
class TReceiverImplementation : public POA_Receiver
{
public:
  TReceiverImplementation (TReceiverImplementation const&) = delete;
  TReceiverImplementation (TReceiverImplementation&&) = delete;
  TReceiverImplementation& operator= (TReceiverImplementation const&) = delete;
  TReceiverImplementation& operator= (TReceiverImplementation&&) = delete;

  /// Constructor.
  TReceiverImplementation () = default;

  /// Destructor.
  ~TReceiverImplementation () override = default;

  void message (char const *msg) override;

  /// Called when the chat server is going away.  The client
  /// implementation should shutdown the chat client in response to
  /// this.
  void shutdown () override;

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_ {};
};
//---------------------------------------------------------------------------
#endif
