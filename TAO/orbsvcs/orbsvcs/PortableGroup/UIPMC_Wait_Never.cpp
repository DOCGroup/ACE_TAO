// $Id$

#include "UIPMC_Wait_Never.h"
#include "tao/ORB_Core.h"
#include "tao/Transport.h"
#include "tao/debug.h"

ACE_RCSID(tao, Wait_On_Leader_Follower, "$Id$")

TAO_UIPMC_Wait_Never::TAO_UIPMC_Wait_Never (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
{
}

TAO_UIPMC_Wait_Never::~TAO_UIPMC_Wait_Never (void)
{
}

int
TAO_UIPMC_Wait_Never::register_handler (void)
{
  return -1;
}

int
TAO_UIPMC_Wait_Never::non_blocking (void)
{
  return 1;
}

int
TAO_UIPMC_Wait_Never::sending_request (TAO_ORB_Core *,
                                       int)
{
  return 0;
}

int
TAO_UIPMC_Wait_Never::wait (ACE_Time_Value *,
                            TAO_Synch_Reply_Dispatcher &)
{
  return -1;
}
