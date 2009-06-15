// $Id$

#include "tao/Leader_Follower_Flushing_Strategy.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "tao/Transport.h"
#include "tao/Queued_Message.h"
#include "tao/ORB_Core.h"

ACE_RCSID (tao,
           Leader_Follower_Flushing_Strategy,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Leader_Follower_Flushing_Strategy::schedule_output (TAO_Transport *transport)
{
  return transport->schedule_output_i ();
}

int
TAO_Leader_Follower_Flushing_Strategy::cancel_output (
    TAO_Transport *transport)
{
  return transport->cancel_output_i ();
}

int
TAO_Leader_Follower_Flushing_Strategy::flush_message (
    TAO_Transport *transport,
    TAO_Queued_Message *msg,
    ACE_Time_Value *max_wait_time)
{
  TAO_Leader_Follower &leader_follower =
    transport->orb_core ()->leader_follower ();
  return leader_follower.wait_for_event (msg, transport, max_wait_time);
}

int
TAO_Leader_Follower_Flushing_Strategy::flush_transport (
    TAO_Transport *transport,
    ACE_Time_Value *max_wait_time)
{
  try
    {
      TAO_ORB_Core * const orb_core = transport->orb_core ();

      if (max_wait_time == 0)
        {
          // In case max_wait_time==0 it doesn't make sense to run
          // while loop depending on transport->queue_is_empty ()
          // since in multi-threaded application it can easily happen
          // that the other thread will run the orb and drain the
          // queue in the transport we're coping with here. So, that
          // transport->queue_is_empty () will return false but before
          // we get a chance to run the orb the queue in the transport
          // will become empty and we will wait forever. Instead while
          // loop depending on reactor->work_pending () is much safer
          // since transport will return 0 (letting the reactor know
          // about more pending work) when handling output/timeout as
          // long as its queue is not empty.
          while (orb_core->reactor ()->work_pending ())
            {
              ACE_Time_Value tv (0, 100);
              if (orb_core->run (&tv, 1) == -1)
                return -1;
            }
        }
      else
        {
          if (orb_core->run (max_wait_time, 1) == -1)
            return -1;

          if (max_wait_time != 0) {
            if (*max_wait_time <= ACE_Time_Value::zero) {
              errno = ETIME;
              return -1;
            }
          }
        }
    }
  catch (const ::CORBA::Exception&)
    {
      return -1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
