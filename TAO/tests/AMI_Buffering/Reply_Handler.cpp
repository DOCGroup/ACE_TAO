//
// $Id$
//
#include "Reply_Handler.h"

ACE_RCSID(AMI_Buffering, Reply_Handler, "$Id$")

Reply_Handler::Reply_Handler (void)
{
}

void
Reply_Handler::receive_data (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Reply_Handler::receive_data_excep (::Messaging::ExceptionHolder *holder
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      holder->raise_exception ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "receive_data: ");
    }
  ACE_ENDTRY;
}

void
Reply_Handler::sync (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Reply_Handler::sync_excep (::Messaging::ExceptionHolder *holder
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      holder->raise_exception ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "sync: ");
    }
  ACE_ENDTRY;
}

void
Reply_Handler::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Reply_Handler::shutdown_excep (::Messaging::ExceptionHolder *holder
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      holder->raise_exception ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "shutdown: ");
    }
  ACE_ENDTRY;
}
