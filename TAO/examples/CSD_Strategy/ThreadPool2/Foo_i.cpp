// $Id$
#include "Foo_i.h"
#include "FooServantList.h"

Foo_i::Foo_i(const ACE_TCHAR* servant_name,FooServantList* mgr)
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
{
  ++this->count_op1_;
  // Sleep for 10 milliseconds (10,000 microseconds)
  //ACE_OS::sleep(ACE_Time_Value(0,10000));
}


void
Foo_i::op2(CORBA::Long value)
{
  ++this->count_op2_;
  this->value_ = value;
}


CORBA::Long
Foo_i::op3(void)
{
  ++this->count_op3_;
  return this->value_;
}


void
Foo_i::op4(CORBA::Long value)
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
{
  ++this->count_op5_;
}


void
Foo_i::done(void)
{
  this->mgr_->client_done();
}
