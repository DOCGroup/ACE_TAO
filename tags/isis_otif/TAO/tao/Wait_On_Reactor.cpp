// $Id$

#include "ace/Reactor.h"
#include "tao/Wait_On_Reactor.h"
#include "tao/ORB_Core.h"
#include "tao/Transport.h"
#include "tao/Synch_Reply_Dispatcher.h"

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
                           TAO_Synch_Reply_Dispatcher &rd)
{
  // Start the count down timer to account for the time spent in this
  // method.
  ACE_Countdown_Time countdown (max_wait_time);

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
      if (!rd.keep_waiting ())
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

  if (result == -1 || rd.error_detected ())
    return -1;

  // Return an error if there was a problem receiving the reply.
  if (max_wait_time != 0)
    {
      if (rd.successful () &&
          *max_wait_time == ACE_Time_Value::zero)
        {
          result = -1;
          errno = ETIME;
        }
    }
  else
    {
      result = 0;
      if (rd.error_detected ())
        result = -1;
    }

  return result;
}

// Register the handler with the Reactor.
int
TAO_Wait_On_Reactor::register_handler (void)
{
  if (this->is_registered_ == 0)
     return this->transport_->register_handler ();

  return 1;
}

int
TAO_Wait_On_Reactor::non_blocking (void)
{
  return 1;
}
