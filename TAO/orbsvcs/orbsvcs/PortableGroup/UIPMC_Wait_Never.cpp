#include "orbsvcs/PortableGroup/UIPMC_Wait_Never.h"
#include "tao/ORB_Core.h"
#include "tao/Transport.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIPMC_Wait_Never::TAO_UIPMC_Wait_Never (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
{
}

int
TAO_UIPMC_Wait_Never::register_handler ()
{
  return -1;
}

bool
TAO_UIPMC_Wait_Never::non_blocking () const
{
  return true;
}

bool
TAO_UIPMC_Wait_Never::can_process_upcalls () const
{
  return true;
}

int
TAO_UIPMC_Wait_Never::sending_request (TAO_ORB_Core *,
                                       TAO_Message_Semantics )
{
  return 0;
}

int
TAO_UIPMC_Wait_Never::wait (ACE_Time_Value *, TAO_Synch_Reply_Dispatcher &)
{
  return -1;
}

TAO_END_VERSIONED_NAMESPACE_DECL
