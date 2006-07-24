// $Id$
#include "Foo_C_cust_op1.h"
#include "Foo_C_i.h"
#include "CancelledExceptionC.h"


Foo_C_cust_op1::Foo_C_cust_op1(Foo_C_i* servant)
  : TAO::CSD::TP_Custom_Request_Operation(servant),
    cancelled_(false),
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


Foo_C_cust_op1::~Foo_C_cust_op1()
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
Foo_C_cust_op1::result(ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->cancelled_)
    {
      ACE_THROW (CancelledException());
    }
}


void
Foo_C_cust_op1::execute_i()
{
  this->servant_->cust_op1();
}


void
Foo_C_cust_op1::cancel_i()
{
  this->cancelled_ = true;
}
