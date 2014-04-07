// $Id$

#include "tao/LF_Connect_Strategy.h"
#include "tao/Nested_Upcall_Guard.h"
#include "tao/LF_Multi_Event.h"
#include "tao/Connection_Handler.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "ace/Synch_Options.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LF_Connect_Strategy::TAO_LF_Connect_Strategy (TAO_ORB_Core *orb_core,
                                                  bool no_upcall)
  : TAO_Connect_Strategy (orb_core),
    no_upcall_ (no_upcall)
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
      options.set (ACE_Synch_Options::USE_REACTOR, *timeout);
    }
  else
    {
      // Making it sure it is blocking.
      options.set (ACE_Synch_Options::USE_REACTOR, ACE_Time_Value::zero);
    }
}

int
TAO_LF_Connect_Strategy::wait_i (TAO_LF_Event *ev,
                                 TAO_Transport *transport,
                                 ACE_Time_Value *max_wait_time)
{
  if (transport == 0)
    return -1;

  TAO::Nested_Upcall_Guard guard(transport, this->no_upcall_);

  TAO_Leader_Follower &leader_follower =
    this->orb_core_->leader_follower ();

  int result =
    leader_follower.wait_for_event (ev, transport, max_wait_time);

  if (ev->error_detected () && result != -1)
    result = -1;

  return result;
}


TAO_END_VERSIONED_NAMESPACE_DECL
