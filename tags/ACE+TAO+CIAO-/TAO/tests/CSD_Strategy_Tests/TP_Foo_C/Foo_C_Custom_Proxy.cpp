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


Foo_C_Custom_Proxy::~Foo_C_Custom_Proxy()
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


bool
Foo_C_Custom_Proxy::validate_connection ()
{
  return AppHelper::validate_connection (this->objref_.in ());
}


void
Foo_C_Custom_Proxy::op1(ACE_ENV_SINGLE_ARG_DECL)
{
  this->objref_->op1(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


void
Foo_C_Custom_Proxy::op2(CORBA::Long x ACE_ENV_ARG_DECL)
{
  this->objref_->op2(x ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


CORBA::Long
Foo_C_Custom_Proxy::op3(CORBA::Long x ACE_ENV_ARG_DECL)
{
  CORBA::Long result = this->objref_->op3(x ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  return result;
}


void
Foo_C_Custom_Proxy::op4(CORBA::Long x ACE_ENV_ARG_DECL)
{
  this->objref_->op4(x ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


void
Foo_C_Custom_Proxy::op5(ACE_ENV_SINGLE_ARG_DECL)
{
  this->objref_->op5(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


void
Foo_C_Custom_Proxy::done(ACE_ENV_SINGLE_ARG_DECL)
{
  this->objref_->done(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


void
Foo_C_Custom_Proxy::cust_op1(ACE_ENV_SINGLE_ARG_DECL)
{
  Foo_C_cust_op1_Handle op = new Foo_C_cust_op1(this->servant_);
  this->strategy_->custom_synch_request(op.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


void
Foo_C_Custom_Proxy::cust_op2(long x ACE_ENV_ARG_DECL)
{
  Foo_C_cust_op2_Handle op = new Foo_C_cust_op2(this->servant_, x);
  this->strategy_->custom_synch_request(op.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


long
Foo_C_Custom_Proxy::cust_op3(long x ACE_ENV_ARG_DECL)
{
  Foo_C_cust_op3_Handle op = new Foo_C_cust_op3(this->servant_, x);
  this->strategy_->custom_synch_request(op.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  int ret = op->result(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  return ret;
}


void
Foo_C_Custom_Proxy::cust_op4(long x ACE_ENV_ARG_DECL)
{
  Foo_C_cust_op4_Handle op = new Foo_C_cust_op4(this->servant_,x);
  this->strategy_->custom_asynch_request(op.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


void
Foo_C_Custom_Proxy::cust_op5(ACE_ENV_SINGLE_ARG_DECL)
{
  Foo_C_cust_op5_Handle op = new Foo_C_cust_op5(this->servant_);
  this->strategy_->custom_synch_request(op.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  op->result(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}
