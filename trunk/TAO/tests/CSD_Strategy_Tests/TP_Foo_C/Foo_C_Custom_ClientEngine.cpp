// $Id$
#include "Foo_C_Custom_ClientEngine.h"
#include "CustomExceptionC.h"
#include "ace/Log_Msg.h"


Foo_C_Custom_ClientEngine::Foo_C_Custom_ClientEngine
                                        (Foo_C_i*               servant,
                                         Foo_C_ptr              objref,
                                         TAO::CSD::TP_Strategy* strategy,
                                         unsigned               client_id)
  : proxy_(servant,objref,strategy),
    client_id_(client_id)
{
}


Foo_C_Custom_ClientEngine::~Foo_C_Custom_ClientEngine()
{
}


bool
Foo_C_Custom_ClientEngine::execute(ACE_ENV_SINGLE_ARG_DECL)
{
  // Make sure the connection is established before making  
  // remote invocations.
  if (this->proxy_.validate_connection () == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t)Foo_A_ClientEngine::execute  " \
                          "client %d connect failed.\n", this->client_id_));
      return false;
    }

  bool check_validity = true;
  CORBA::Long i = this->client_id_;

  this->proxy_.op1(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);
  this->proxy_.op2(i ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);
  CORBA::Long value = this->proxy_.op3(i ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  if (value != i)
    {
      check_validity = false;
    }


  for (CORBA::Long j = 1; j <= 5; j++)
    {
      this->proxy_.op4(495 + (i * 5) + j ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (false);
    }

  bool caught_exception = false;
  ACE_TRY_EX (op5)
    {
      this->proxy_.op5(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (op5);
    }
  ACE_CATCH (FooException, ex)
    {
      // Expected
      caught_exception = true;
    }
  ACE_ENDTRY;

  if (! caught_exception)
    {
      check_validity = false;
    }

  long cust_i = this->client_id_;

  this->proxy_.cust_op1(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);
  this->proxy_.cust_op2(cust_i ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);
  long cust_value = this->proxy_.cust_op3(cust_i ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);

  if (cust_value != i)
    {
      check_validity = false;
    }

  for (long cust_j = 1; cust_j <= 5; cust_j++)
    {
      this->proxy_.cust_op4(495 + (cust_i * 5) + cust_j ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (false);
    }

  caught_exception = false;

  ACE_TRY_EX (cust_op5)
    {
      this->proxy_.cust_op5(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (cust_op5)
    }
  ACE_CATCHANY //(CustomException, ex)
    {
      // Expected
      caught_exception = true;
    }
  ACE_ENDTRY;

  if (! caught_exception)
    {
      check_validity = false;
    }

  this->proxy_.done(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (false);
  
  return check_validity;
}


void
Foo_C_Custom_ClientEngine::expected_results(Foo_C_Statistics& stats)
{
  // This ClientEngine is used by collocated clients.
  stats.expected(1, 1, false);
  stats.expected(2, 1, false);
  stats.expected(3, 1, false);
  stats.expected(4, 5, false);
  stats.expected(5, 1, false);
  stats.expected(6, 1, false);
  stats.expected(7, 1, false);
  stats.expected(8, 1, false);
  stats.expected(9, 5, false);
  stats.expected(10, 1, false);
}

