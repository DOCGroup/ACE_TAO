// $Id$

#include "tao/Wait_Strategy.h"

ACE_RCSID(tao, Wait_Strategy, "$Id$")

// Constructor.
TAO_Wait_Strategy::TAO_Wait_Strategy (TAO_Transport *transport)
  : transport_ (transport)
{
}

// Destructor.
TAO_Wait_Strategy::~TAO_Wait_Strategy (void)
{
}

int
TAO_Wait_Strategy::sending_request (TAO_ORB_Core * /* orb_core */,
                                    int            /* two_way */)
{
  return 0;
}

TAO_SYNCH_CONDITION *
TAO_Wait_Strategy::leader_follower_condition_variable (void)
{
  return 0;
}

int
TAO_Wait_Strategy::reply_dispatched (int &reply_received,
                                     TAO_SYNCH_CONDITION *)
{
  // In most implementations of this strategy there is no need to
  // acquire any mutex to set the reply_received flag.
  reply_received = 1;
  return 0;
}

void
TAO_Wait_Strategy::connection_closed (int &reply_received,
                                      TAO_SYNCH_CONDITION*)
{
  // In most implementations of this strategy there is no need to
  // acquire any mutex to set the reply_received flag.
  reply_received = -1;
}

