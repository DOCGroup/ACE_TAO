// -*- C++ -*-
// $Id$

#include "tao/Follower.h"
#include "tao/Leader_Follower.h"

#if !defined (__ACE_INLINE__)
# include "tao/Follower.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Follower, "$Id$")

TAO_Follower::TAO_Follower (TAO_Leader_Follower &leader_follower)
  : leader_follower_ (leader_follower)
  , condition_ (leader_follower.lock ())
{
}

TAO_Follower::~TAO_Follower (void)
{
}

int
TAO_Follower::signal (void)
{
  // We *must* remove ourselves from the list of followers, otherwise
  // we could get signalled twice: to wake up as a follower and as the
  // next leader.
  // The follower may not be there if the reply is received while
  // the consumer is not yet waiting for it (i.e. it send the
  // request but has not blocked to receive the reply yet).
  // Ignore errors.
  (void) this->leader_follower_.remove_follower (this);

  return this->condition_.signal ();
}
