// $Id$
#include "Foo_C_cust_op5.h"
#include "Foo_C_i.h"
#include "CancelledExceptionC.h"
#include "CustomExceptionC.h"


Foo_C_cust_op5::Foo_C_cust_op5(Foo_C_i* servant)
  : TAO::CSD::TP_Custom_Request_Operation(servant),
    exception_(false),
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


Foo_C_cust_op5::~Foo_C_cust_op5()
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
Foo_C_cust_op5::result(void)
{
  if (this->cancelled_)
    {
      throw CancelledException();
    }

  if (this->exception_)
    {
      throw CustomException();
    }
}


void
Foo_C_cust_op5::execute_i()
{
  try
  {
    this->servant_->cust_op5();
  }
  catch (const CustomException& )
  {
    this->exception_ = true;
  }
}


void
Foo_C_cust_op5::cancel_i()
{
  this->cancelled_ = true;
}
