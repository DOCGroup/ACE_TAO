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
  ACE_ASSERT(ch != 0);

  // @@todo We need to use a auto_ptr<>-like object here!
  // TAO_Transport * transport = ch->get_transport_locked();
  TAO_Transport *transport = ch->transport ();

  // Basically the connection was EINPROGRESS, but before we could
  // wait for it some other thread detected a failure and cleaned up
  // the connection handler.
  if(transport == 0)
    {
      return -1;
    }

  TAO_Leader_Follower &leader_follower =
    this->orb_core_->leader_follower ();

  int result =
    leader_follower.wait_for_event (ch,
                                    transport,
                                    max_wait_time);

  // Set the result.
  if (!ch->successful () && result != -1)
    result = -1;

  return result;
}
