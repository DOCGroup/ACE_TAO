#include "LF_Connect_Strategy.h"
#include "Connection_Handler.h"
#include "Transport.h"
#include "ORB_Core.h"
#include "LF_Event.h"
#include "Leader_Follower.h"
#include "debug.h"
#include "ace/Synch_Options.h"


ACE_RCSID(tao,
          LF_Connect_Strategy,
          "$Id$")


TAO_LF_Connect_Strategy::TAO_LF_Connect_Strategy (
    TAO_ORB_Core *orb_core)
  : TAO_Connect_Strategy (orb_core)
{
}

TAO_LF_Connect_Strategy::~TAO_LF_Connect_Strategy (void)
{
}

void
TAO_LF_Connect_Strategy::synch_options (ACE_Time_Value *timeout,
                                        ACE_Synch_Options &options)
{
  if (timeout != 0)
    {
      options.set (ACE_Synch_Options::USE_REACTOR,
                   *timeout);
    }
  else
    {
      // Making it sure it is blocking.
      options.set (ACE_Synch_Options::USE_REACTOR,
                   ACE_Time_Value::zero);
    }
}


int
TAO_LF_Connect_Strategy::wait (TAO_Connection_Handler *ch,
                               ACE_Time_Value *max_wait_time)
{
  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_LIB_TEXT ("(%P|%t) Waiting on the LF for")
                ACE_LIB_TEXT (" connection completion - wait () \n")));

  TAO_Transport *transport =
    ch->transport ();

  TAO_Leader_Follower &leader_follower =
    transport->orb_core ()->leader_follower ();

  int result =
    leader_follower.wait_for_event (ch,
                                    transport,
                                    max_wait_time);

  // Set the result.
  if (!ch->successful () && result != -1)
    result = -1;

  return result;
}
