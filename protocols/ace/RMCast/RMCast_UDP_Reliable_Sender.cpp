// $Id$

#include "RMCast_UDP_Reliable_Sender.h"
#include "RMCast_UDP_Event_Handler.h"
#include "RMCast_Resend_Handler.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_UDP_Reliable_Sender.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_UDP_Reliable_Sender, "$Id$")

ACE_RMCast_UDP_Reliable_Sender::ACE_RMCast_UDP_Reliable_Sender (ACE_RMCast_Module *user_control)
  : user_control_ (user_control)

  // We use a singleton factory, all proxys send their messages to the
  // retransmission module.  There should be only control messages
  // coming back, so this is OK.
  , factory_ (&retransmission_)
  , io_udp_ (&factory_)
{
  // Messages are passed down to the retransmission module.
  this->next (&this->retransmission_);

  // Then to the splitter, at this point control messages are sent
  // back to the user, other messages continue down to the
  // fragmentation layer.
  this->retransmission_.next (&this->splitter_);
  this->splitter_.next (&this->fragment_);
  this->splitter_.control_module (this->user_control_);

  // The fragmentation layer delegates all messages to the UDP I/O
  // module, that sends every message back to the application.
  this->fragment_.next (&this->io_udp_);
}

ACE_RMCast_UDP_Reliable_Sender::~ACE_RMCast_UDP_Reliable_Sender (void)
{
}

void
ACE_RMCast_UDP_Reliable_Sender::reactive_incoming_messages (ACE_Reactor *reactor)
{
  ACE_RMCast_UDP_Event_Handler *eh;
  ACE_NEW (eh, ACE_RMCast_UDP_Event_Handler (&this->io_udp_));

  /// @@ TODO Make sure it is removed from the Reactor at some point
  (void) reactor->register_handler (eh, ACE_Event_Handler::READ_MASK);
}

void
ACE_RMCast_UDP_Reliable_Sender::reactive_resends (ACE_Reactor *reactor,
                                         const ACE_Time_Value &period)
{
  ACE_RMCast_Resend_Handler *eh;
  ACE_NEW (eh, ACE_RMCast_Resend_Handler (&this->retransmission_));

  /// @@ TODO make sure it is removed from the Reactor at some point
  (void) reactor->schedule_timer (eh, 0, period, period);
}

int
ACE_RMCast_UDP_Reliable_Sender::has_data (void)
{
  return this->retransmission_.has_data ();
}
