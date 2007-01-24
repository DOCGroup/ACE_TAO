// $Id$
#include "Foo_i.h"
#include "FooServantList.h"
#include "ace/OS.h"

Foo_i::Foo_i(const char* servant_name,FooServantList* mgr)
  : value_(0),
    count_op1_(0),
    count_op2_(0),
    count_op3_(0),
    count_op4_(0),
    count_op5_(0),
    servant_name_(servant_name),
    mgr_(mgr)
{
}


Foo_i::~Foo_i()
{
}


void
Foo_i::op1(void)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ++this->count_op1_;
  // Sleep for 10 milliseconds (10,000 microseconds)
  //ACE_OS::sleep(ACE_Time_Value(0,10000));
}


void
Foo_i::op2(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ++this->count_op2_;
  this->value_ = value;
}


CORBA::Long
Foo_i::op3(void)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ++this->count_op3_;
  return this->value_;
}


void
Foo_i::op4(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ++this->count_op4_;
  this->value_ = value;

  if (this->count_op4_ % 100 == 0)
    {
      ACE_DEBUG((LM_DEBUG,
                 "(%P|%t) op4() has been called %d times now.  value == %d\n",
                 this->count_op4_, this->value_));
    }

  // Sleep for 10 milliseconds (10,000 microseconds)
  //ACE_OS::sleep(ACE_Time_Value(0,10000));
}


void
Foo_i::op5(void)
  ACE_THROW_SPEC((CORBA::SystemException, FooException))
{
  ++this->count_op5_;
  ACE_THROW_SPEC (FooException());
}


void
Foo_i::done(void)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->mgr_->client_done();
}
