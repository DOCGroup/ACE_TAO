// -*- C++ -*-
// $Id$

#include "tao/Leader_Follower_Flushing_Strategy.h"
#include "tao/Leader_Follower.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/Queued_Message.h"
#include "tao/debug.h"

ACE_RCSID(tao, Leader_Follower_Flushing_Strategy, "$Id$")

int
TAO_Leader_Follower_Flushing_Strategy::schedule_output (TAO_Transport *transport)
{
  return transport->schedule_output_i ();
}

int
TAO_Leader_Follower_Flushing_Strategy::cancel_output (TAO_Transport *transport)
{
  return transport->cancel_output_i ();
}

int
TAO_Leader_Follower_Flushing_Strategy::flush_message (TAO_Transport *transport,
                                                      TAO_Queued_Message *msg,
                                                      ACE_Time_Value *max_wait_time)
{
  TAO_Leader_Follower &leader_follower =
    transport->orb_core ()->leader_follower ();
  return leader_follower.wait_for_event (msg, transport, max_wait_time);
}

int
TAO_Leader_Follower_Flushing_Strategy::flush_transport (TAO_Transport *transport)
{
  // @todo This is not the right way to do this....

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_ORB_Core *orb_core = transport->orb_core ();

      while (!transport->queue_is_empty ())
        {
          int result = orb_core->run (0, 1 ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (result == -1)
            return -1;
        }
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
