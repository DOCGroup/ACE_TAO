// $Id$

#include "tao/LF_Strategy_Complete.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

  int const result = lf.elect_new_leader ();

  if (result == -1)
    TAOLIB_ERROR ((LM_ERROR,
                ACE_TEXT ("TAO (%P|%t) - Failed to wake up ")
                ACE_TEXT ("a follower thread\n")));
}

TAO_END_VERSIONED_NAMESPACE_DECL
