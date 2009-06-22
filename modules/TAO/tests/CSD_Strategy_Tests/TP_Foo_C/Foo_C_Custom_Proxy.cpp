// $Id$
#include "Foo_C_Custom_Proxy.h"
#include "Foo_C_cust_op1.h"
#include "Foo_C_cust_op2.h"
#include "Foo_C_cust_op3.h"
#include "Foo_C_cust_op4.h"
#include "Foo_C_cust_op5.h"
#include "AppHelper.h"


Foo_C_Custom_Proxy::Foo_C_Custom_Proxy(Foo_C_i* servant,
                                       Foo_C_ptr objref,
                                       TAO::CSD::TP_Strategy* strategy)
  : servant_(servant),
    objref_(Foo_C::_duplicate(objref)),
    strategy_(strategy, false)
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


Foo_C_Custom_Proxy::~Foo_C_Custom_Proxy()
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


bool
Foo_C_Custom_Proxy::validate_connection ()
{
  return AppHelper::validate_connection (this->objref_.in ());
}


void
Foo_C_Custom_Proxy::op1(void)
{
  this->objref_->op1();
}


void
Foo_C_Custom_Proxy::op2(CORBA::Long x)
{
  this->objref_->op2(x);
}


CORBA::Long
Foo_C_Custom_Proxy::op3(CORBA::Long x)
{
  CORBA::Long result = this->objref_->op3(x);
  return result;
}


void
Foo_C_Custom_Proxy::op4(CORBA::Long x)
{
  this->objref_->op4(x);
}


void
Foo_C_Custom_Proxy::op5(void)
{
  this->objref_->op5();
}


void
Foo_C_Custom_Proxy::done(void)
{
  this->objref_->done();
}


void
Foo_C_Custom_Proxy::cust_op1(void)
{
  Foo_C_cust_op1_Handle op = new Foo_C_cust_op1(this->servant_);
  this->strategy_->custom_synch_request(op.in());
}


void
Foo_C_Custom_Proxy::cust_op2(long x)
{
  Foo_C_cust_op2_Handle op = new Foo_C_cust_op2(this->servant_, x);
  this->strategy_->custom_synch_request(op.in());
}


long
Foo_C_Custom_Proxy::cust_op3(long x)
{
  Foo_C_cust_op3_Handle op = new Foo_C_cust_op3(this->servant_, x);
  this->strategy_->custom_synch_request(op.in());
  int ret = op->result();
  return ret;
}


void
Foo_C_Custom_Proxy::cust_op4(long x)
{
  Foo_C_cust_op4_Handle op = new Foo_C_cust_op4(this->servant_,x);
  this->strategy_->custom_asynch_request(op.in());
}


void
Foo_C_Custom_Proxy::cust_op5(void)
{
  Foo_C_cust_op5_Handle op = new Foo_C_cust_op5(this->servant_);
  this->strategy_->custom_synch_request(op.in());
  op->result();
}
