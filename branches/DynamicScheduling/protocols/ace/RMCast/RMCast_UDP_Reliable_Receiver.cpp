// $Id$

#include "RMCast_UDP_Reliable_Receiver.h"
#include "RMCast_UDP_Event_Handler.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_UDP_Reliable_Receiver.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_UDP_Reliable_Receiver, "$Id$")

ACE_RMCast_UDP_Reliable_Receiver::ACE_RMCast_UDP_Reliable_Receiver (ACE_RMCast_Module *user_module)
  : user_factory_ (user_module)
  , factory_ (&user_factory_)
  , io_udp_ (&factory_)
{
}

ACE_RMCast_UDP_Reliable_Receiver::~ACE_RMCast_UDP_Reliable_Receiver (void)
{
}

void
ACE_RMCast_UDP_Reliable_Receiver::reactive_incoming_messages (ACE_Reactor *reactor)
{
  ACE_RMCast_UDP_Event_Handler *eh;
  ACE_NEW (eh, ACE_RMCast_UDP_Event_Handler (&this->io_udp_));

  /// @@ TODO Make sure it is removed from the Reactor at some point
  (void) reactor->register_handler (eh, ACE_Event_Handler::READ_MASK);
}
