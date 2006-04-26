#include "tao/LF_Connect_Strategy.h"
#include "tao/Connection_Handler.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "ace/Synch_Options.h"

ACE_RCSID(tao,
          LF_Connect_Strategy,
          "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  ACE_ASSERT (ch != 0);

  return this->wait (ch->transport (),
                     max_wait_time);
}

int
TAO_LF_Connect_Strategy::wait (TAO_Transport *transport,
                               ACE_Time_Value *max_wait_time)
{
  // Basically the connection was EINPROGRESS, but before we could
  // wait for it some other thread detected a failure and cleaned up
  // the connection handler.
  if (transport == 0)
    return -1;

  TAO_Connection_Handler *ch =
    transport->connection_handler ();

  TAO_Leader_Follower &leader_follower =
    this->orb_core_->leader_follower ();

  int result =
    leader_follower.wait_for_event (ch,
                                    transport,
                                    max_wait_time);

  // Set the result.
  if (ch->error_detected () && result != -1)
    result = -1;

  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
