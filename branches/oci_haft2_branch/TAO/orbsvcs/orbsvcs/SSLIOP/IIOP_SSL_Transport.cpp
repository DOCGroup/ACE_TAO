// -*- C++ -*-

#include "IIOP_SSL_Transport.h"
#include "IIOP_SSL_Connection_Handler.h"

#include "tao/Timeprobe.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/debug.h"

ACE_RCSID (TAO_SSLIOP,
           IIOP_SSL_Transport,
           "$Id$")

TAO_IIOP_SSL_Transport::TAO_IIOP_SSL_Transport (
    TAO_IIOP_SSL_Connection_Handler *handler,
    TAO_ORB_Core *orb_core,
    TAO_SSLIOP_Current_ptr current,
    CORBA::Boolean flag)
  : TAO_IIOP_Transport (handler,
                        orb_core,
                        flag)
{
  this->current_ = TAO_SSLIOP_Current::_duplicate (current);
}

TAO_IIOP_SSL_Transport::~TAO_IIOP_SSL_Transport (void)
{
}

int
TAO_IIOP_SSL_Transport::handle_input (TAO_Resume_Handle &rh,
                                      ACE_Time_Value *max_wait_time,
                                      int block)
{
  int result = 0;

  // Invalidate the TSS SSL session state to make sure that SSL state
  // from a previous SSL connection is not confused with this non-SSL
  // connection.
  TAO_Null_SSL_State_Guard guard (this->current_.in (), result);

  if (result != 0)
    return -1;

  return
    this->TAO_IIOP_Transport::handle_input (rh,
                                            max_wait_time,
                                            block);
}
