// $Id$

#include "tao/Wait_On_Reactor.h"
#include "tao/ORB_Core.h"
#include "Transport.h"

ACE_RCSID(tao, Wait_On_Reactor, "$Id$")

TAO_Wait_On_Reactor::TAO_Wait_On_Reactor (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
{
}

TAO_Wait_On_Reactor::~TAO_Wait_On_Reactor (void)
{
}

int
TAO_Wait_On_Reactor::wait (ACE_Time_Value *max_wait_time,
                           int &reply_received)
{
  // Reactor does not change inside the loop.
  ACE_Reactor* reactor =
    this->transport_->orb_core ()->reactor ();

  // Do the event loop, till we fully receive a reply.
  int result = 0;
  while (1)
    {
      // Run the event loop.
      result = reactor->handle_events (max_wait_time);

      // If we got our reply, no need to run the event loop any
      // further.
      if (reply_received)
        break;

      // Did we timeout? If so, stop running the loop.
      if (result == 0 &&
          max_wait_time != 0 &&
          *max_wait_time == ACE_Time_Value::zero)
        break;

      // Other errors? If so, stop running the loop.
      if (result == -1)
        break;

      // Otherwise, keep going...
    }

  if (result == -1 || reply_received == -1)
    return -1;

  // Return an error if there was a problem receiving the reply.
  if (max_wait_time != 0)
    {
      if (reply_received != 1 &&
          *max_wait_time == ACE_Time_Value::zero)
        {
          result = -1;
          errno = ETIME;
        }
    }
  else
    {
      result = 0;
      if (reply_received == -1)
        result = -1;
    }

  return result;
}

// Register the handler with the Reactor.
int
TAO_Wait_On_Reactor::register_handler (void)
{
  return this->transport_->register_handler ();
}

int
TAO_Wait_On_Reactor::non_blocking (void)
{
  return 1;
}
