// $Id$

#include "IIOP_SSL_Transport.h"
#include "tao/Timeprobe.h"
#include "tao/ORB_Core.h"
#include "tao/ORB.h"
#include "tao/debug.h"

ACE_RCSID(TAO_SSLIOP, IIOP_SSL_Connect, "$Id$")

TAO_IIOP_SSL_Transport::TAO_IIOP_SSL_Transport (
    TAO_IIOP_SSL_Connection_Handler *handler,
    TAO_ORB_Core *orb_core,
    CORBA::Boolean flag)
  : TAO_IIOP_Transport (handler,
                        orb_core,
                        flag)
{

}

TAO_IIOP_SSL_Transport::~TAO_IIOP_SSL_Transport (void)
{
}

int
TAO_IIOP_SSL_Transport::handle_input_i (TAO_Resume_Handle &rh,
                                        ACE_Time_Value *max_wait_time
                                        int block)
{
  int result = 0;

  // Invalidate the TSS SSL session state to make sure that SSL state
  // from a previous SSL connection is not confused with this non-SSL
  // connection.
  TAO_Null_SSL_State_Guard guard (this->orb_core (), result);

  if (result != 0)
    return -1;

  return
    this->TAO_IIOP_Transport::handle_input_i (rh,
                                              max_wait_time,
                                              block);
}
