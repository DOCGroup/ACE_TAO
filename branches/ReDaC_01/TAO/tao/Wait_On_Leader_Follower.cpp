// $Id$

#include "tao/Wait_On_Leader_Follower.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "tao/Transport.h"
#include "tao/Synch_Reply_Dispatcher.h"
#include "tao/ORB_Core.h"

ACE_RCSID (tao,
           Wait_On_Leader_Follower,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Wait_On_Leader_Follower::TAO_Wait_On_Leader_Follower (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
{
}

TAO_Wait_On_Leader_Follower::~TAO_Wait_On_Leader_Follower (void)
{
}

int
TAO_Wait_On_Leader_Follower::register_handler (void)
{
  if (!this->is_registered_)
    {
      return this->transport_->register_handler ();
    }

  return 1;
}

bool
TAO_Wait_On_Leader_Follower::non_blocking (void) const
{
  return true;
}

int
TAO_Wait_On_Leader_Follower::sending_request (TAO_ORB_Core *orb_core,
                                              int two_way)
{
  // Register the handler.
  if (!this->is_registered_)
    {
      this->transport_->register_handler ();
    }

  // Send the request.
  return this->TAO_Wait_Strategy::sending_request (orb_core,
                                                   two_way);
}

int
TAO_Wait_On_Leader_Follower::wait (ACE_Time_Value *max_wait_time,
                                   TAO_Synch_Reply_Dispatcher &rd)
{
  TAO_Leader_Follower& leader_follower =
    this->transport_->orb_core ()->leader_follower ();
  return leader_follower.wait_for_event (&rd,
                                         this->transport_,
                                         max_wait_time);
}

bool
TAO_Wait_On_Leader_Follower::can_process_upcalls (void) const
{
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
