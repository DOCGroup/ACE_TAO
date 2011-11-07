// $Id$

#include "orbsvcs/SSLIOP/IIOP_SSL_Transport.h"
#include "orbsvcs/SSLIOP/IIOP_SSL_Connection_Handler.h"
#include "orbsvcs/SSLIOP/SSLIOP_Util.h"

#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/Timeprobe.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::IIOP_SSL_Transport::IIOP_SSL_Transport (
    IIOP_SSL_Connection_Handler *handler,
    TAO_ORB_Core *orb_core)
  : TAO_IIOP_Transport (handler, orb_core)
{
  this->current_ =
    TAO::SSLIOP::Util::current (orb_core);
}

TAO::IIOP_SSL_Transport::~IIOP_SSL_Transport (void)
{
}

int
TAO::IIOP_SSL_Transport::handle_input (TAO_Resume_Handle &rh,
                                       ACE_Time_Value *max_wait_time)
{
  int result = 0;

  // Invalidate the TSS SSL session state to make sure that SSL state
  // from a previous SSL connection is not confused with this non-SSL
  // connection.
  TAO::Null_SSL_State_Guard guard (this->current_.in (), result);

  if (result != 0)
    return -1;

  return this->TAO_IIOP_Transport::handle_input (rh, max_wait_time);
}

TAO_END_VERSIONED_NAMESPACE_DECL
