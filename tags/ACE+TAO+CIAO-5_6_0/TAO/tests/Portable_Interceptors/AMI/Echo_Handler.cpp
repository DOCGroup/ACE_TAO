/**
 * @file Echo_Handler.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "Echo_Handler.h"


ACE_RCSID (AMI,
           Echo_Handler,
           "$Id$")


Echo_Handler::Echo_Handler(void)
  : replies_ (0)
{
}

void
Echo_Handler::echo_operation (char const *)
{
  ++this->replies_;
}

void
Echo_Handler::echo_operation_excep (::Messaging::ExceptionHolder *)
{
}

void
Echo_Handler::shutdown (void)
{
}

void
Echo_Handler::shutdown_excep (::Messaging::ExceptionHolder *)
{
}

CORBA::ULong
Echo_Handler::replies (void) const
{
  return this->replies_;
}
