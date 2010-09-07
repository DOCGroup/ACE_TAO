// $Id$

#include "tao/Leader_Follower_Flushing_Strategy.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "tao/Transport.h"
#include "tao/Queued_Message.h"
#include "tao/ORB_Core.h"

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

      while (!transport->queue_is_empty ())
        {
          // In case max_wait_time==0 we cannot simply run the orb because
          // in multi-threaded applications it can easily happen that
          // the other thread will run the orb and drain the queue in the
          // transport we're coping with here and this thread will block.
          // Instead we do run for a small amount of time and then recheck
          // the queue.
          if (max_wait_time == 0)
            {
              ACE_Errno_Guard eguard (errno);

              // Poll the reactor's queue.
              ACE_Time_Value tv = ACE_Time_Value::zero;
              orb_core->orb ()->perform_work (&tv);
            }
          else
            {
              orb_core->orb ()->perform_work (max_wait_time);
            }

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
