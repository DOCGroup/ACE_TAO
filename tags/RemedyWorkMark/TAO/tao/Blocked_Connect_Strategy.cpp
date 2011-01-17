// $Id$

#include "tao/Blocked_Connect_Strategy.h"
#include "tao/Connection_Handler.h"

#include "ace/Synch_Options.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Blocked_Connect_Strategy::TAO_Blocked_Connect_Strategy (
    TAO_ORB_Core *orb_core)
  : TAO_Connect_Strategy (orb_core)
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
TAO_Blocked_Connect_Strategy::wait_i (TAO_LF_Event *,
                                      TAO_Transport *,
                                      ACE_Time_Value * )
{
  // We cannot wait for connection completion
  return -1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
