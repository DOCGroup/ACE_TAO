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
Echo_Handler::echo_operation (char const *
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ++this->replies_;
}

void
Echo_Handler::echo_operation_excep (::Messaging::ExceptionHolder *
                                    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
}

void
Echo_Handler::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
}

void
Echo_Handler::shutdown_excep (::Messaging::ExceptionHolder *
                              ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
}

CORBA::ULong
Echo_Handler::replies (void) const
{
  return this->replies_;
}
