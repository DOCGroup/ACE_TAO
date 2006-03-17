// $Id$
#include "Foo_C_cust_op4.h"
#include "Foo_C_i.h"


Foo_C_cust_op4::Foo_C_cust_op4(Foo_C_i* servant, long arg)
  : TAO::CSD::TP_Custom_Request_Operation(servant),
    arg_(arg),
    servant_(servant)
{
  // This try-catch block is not really necessary, but we have to add it to  
  // satisfy the non-exception builds. Since there is actually no exception 
  // raised from _add_ref, we just ignore the exception here.
  ACE_TRY_NEW_ENV
  {
    servant_->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHALL
  {
  }
  ACE_ENDTRY;
}

Foo_C_cust_op4::~Foo_C_cust_op4()
{
  // This try-catch block is not really necessary, but we have to add it to  
  // satisfy the non-exception builds. Since there is actually no exception 
  // raised from _add_ref, we just ignore the exception here.
  ACE_TRY_NEW_ENV
  {
    servant_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHALL
  {
  }
  ACE_ENDTRY;
}


void
Foo_C_cust_op4::execute_i()
{
  this->servant_->cust_op4(this->arg_);
}


void
Foo_C_cust_op4::cancel_i()
{
}
