/**
 * @file Echo_Handler.cpp
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "Echo_Handler.h"

Echo_Handler::Echo_Handler()
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
Echo_Handler::shutdown ()
{
}

void
Echo_Handler::shutdown_excep (::Messaging::ExceptionHolder *)
{
}

CORBA::ULong
Echo_Handler::replies () const
{
  return this->replies_;
}
