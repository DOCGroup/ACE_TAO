// $Id$
#include "Reply_Handler.h"

Reply_Handler::Reply_Handler (CORBA::ORB_ptr o)
  : orb_ (CORBA::ORB::_duplicate (o))
{
}

void
Reply_Handler::next_prime (CORBA::ULong
                           ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return;
}

void
Reply_Handler::next_prime_excep (
    Test::AMI_HelloExceptionHolder *ex
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  ACE_TRY
    {
      ex->raise_next_prime (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::COMM_FAILURE, t)
    {
      /*ACE_DEBUG ((LM_DEBUG,
                  "Callback method <next_prime_excep> called: \n"));
                  */

      ACE_Time_Value tv (10, 0);
      this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}
