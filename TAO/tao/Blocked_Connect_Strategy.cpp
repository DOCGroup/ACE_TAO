#include "Blocked_Connect_Strategy.h"
#include "Connection_Handler.h"
#include "ace/Synch_Options.h"


ACE_RCSID(tao,
          Blocked_Connect_Strategy,
          "$Id$")


TAO_Blocked_Connect_Strategy::TAO_Blocked_Connect_Strategy (
    TAO_ORB_Core *orb_core)
  : TAO_Connect_Strategy (orb_core)
{
}

TAO_Blocked_Connect_Strategy::~TAO_Blocked_Connect_Strategy (void)
{
}


void
TAO_Blocked_Connect_Strategy::synch_options (ACE_Time_Value *timeout,
                                             ACE_Synch_Options &options)
{
  if (timeout != 0)
    {
      // Blocking with a timeout
      options.set (ACE_Synch_Options::USE_TIMEOUT,
                   *timeout);
    }
  else
    {
      // Making it sure it is blocking.
      options.set (0,
                   ACE_Time_Value::zero);
    }
}


int
TAO_Blocked_Connect_Strategy::wait (TAO_Connection_Handler *,
                                    ACE_Time_Value * )
{
  // We cannot wait for connection completion
  return -1;
}

int
TAO_Blocked_Connect_Strategy::post_failed_connect (TAO_Connection_Handler *ch)
{
  // We need to do this here else we will leak memory. We cannot call
  // close_connection () for the following reasons
  // . in blocked connect the event_handlers get_handle () will return
  //   an ACE_INVALID_HANDLE
  // . We cannot relax the conditions since the conditions are right
  //   for other natural cases ie. LF
  // Hence the blocked startegy will have to take of things itself.
  ch->transport (0);
  int ref =  ch->decr_refcount ();
  return ref;
}
