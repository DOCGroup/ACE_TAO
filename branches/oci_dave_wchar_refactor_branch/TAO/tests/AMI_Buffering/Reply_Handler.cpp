//
// $Id$
//
#include "Reply_Handler.h"

ACE_RCSID(AMI_Buffering, Reply_Handler, "$Id$")

Reply_Handler::Reply_Handler (void)
{
}

void
Reply_Handler::receive_data (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Reply_Handler::receive_data_excep (Test::AMI_AMI_BufferingExceptionHolder *holder
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      holder->raise_receive_data (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "receive_data: ");
    }
  ACE_ENDTRY;
}

void
Reply_Handler::sync (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Reply_Handler::sync_excep (Test::AMI_AMI_BufferingExceptionHolder *holder
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      holder->raise_sync (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "sync: ");
    }
  ACE_ENDTRY;
}

void
Reply_Handler::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Reply_Handler::shutdown_excep (Test::AMI_AMI_BufferingExceptionHolder *holder
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      holder->raise_shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "shutdown: ");
    }
  ACE_ENDTRY;
}
