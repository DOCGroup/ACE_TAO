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
  try
  {
    servant_->_add_ref ();
  }
  catch (...)
  {
  }
}

Foo_C_cust_op4::~Foo_C_cust_op4()
{
  // This try-catch block is not really necessary, but we have to add it to
  // satisfy the non-exception builds. Since there is actually no exception
  // raised from _add_ref, we just ignore the exception here.
  try
  {
    servant_->_remove_ref ();
  }
  catch (...)
  {
  }
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
