//
// $Id$
//
#include "Oneway_Receiver.h"

ACE_RCSID(Crash_On_Write, Oneway_Receiver, "$Id$")

Oneway_Receiver::Oneway_Receiver (void)
{
}

void
Oneway_Receiver::receive_oneway (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
