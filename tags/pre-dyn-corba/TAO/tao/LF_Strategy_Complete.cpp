// -*- C++ -*-
// $Id$

#include "tao/LF_Strategy_Complete.h"
#include "tao/Leader_Follower.h"

#if !defined (__ACE_INLINE__)
# include "tao/LF_Strategy_Complete.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, LF_Strategy_Complete, "$Id$")

TAO_LF_Strategy_Complete::~TAO_LF_Strategy_Complete (void)
{
}

void
TAO_LF_Strategy_Complete::set_upcall_thread (TAO_Leader_Follower &lf)
{
  lf.set_upcall_thread ();
}

int
TAO_LF_Strategy_Complete::set_event_loop_thread (ACE_Time_Value *tv,
                                                 TAO_Leader_Follower &lf)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, lf.lock (), -1);

  return lf.set_event_loop_thread (tv);
}

void
TAO_LF_Strategy_Complete::reset_event_loop_thread (int call_reset,
                                                   TAO_Leader_Follower &lf)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, lf.lock ());

  if (call_reset)
    lf.reset_event_loop_thread ();

  int result = lf.elect_new_leader ();

  if (result == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("TAO (%P|%t) Failed to wake up ")
                ACE_TEXT ("a follower thread\n")));
}
