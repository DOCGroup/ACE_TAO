// $Id$
#include "Foo_C_cust_op3.h"
#include "Foo_C_i.h"
#include "CancelledExceptionC.h"


Foo_C_cust_op3::Foo_C_cust_op3(Foo_C_i* servant, long arg)
  : TAO::CSD::TP_Custom_Request_Operation(servant),
    arg_(arg),
    result_(0),
    cancelled_(false),
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


Foo_C_cust_op3::~Foo_C_cust_op3()
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


long
Foo_C_cust_op3::result(void)
{
  if (this->cancelled_)
    {
      throw CancelledException();
    }

  return this->result_;
}


void
Foo_C_cust_op3::execute_i()
{
  this->result_ = this->servant_->cust_op3(this->arg_);
}


void
Foo_C_cust_op3::cancel_i()
{
  this->cancelled_ = true;
}
