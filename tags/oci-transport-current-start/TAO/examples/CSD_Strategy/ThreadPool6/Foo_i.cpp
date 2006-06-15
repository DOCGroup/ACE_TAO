// $Id$
#include "Foo_i.h"
#include "OrbShutdownTask.h"
#include "ace/OS.h"

Foo_i::Foo_i(unsigned num_clients)
  : value_(0),
    num_clients_(num_clients),
    count_op1_(0),
    count_op2_(0),
    count_op3_(0),
    count_op4_(0),
    count_op5_(0)

{
  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) Number of clients for Foo_i servant: %d\n",
             num_clients));
}


Foo_i::~Foo_i()
{
}


void
Foo_i::op1(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)  
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
Foo_i::op3(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)  
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

  if (this->count_op4_ % 500 == 0)
    {
      ACE_DEBUG((LM_DEBUG,
                 "(%P|%t) op4() has been called %d times now.  value == %d\n",
                 this->count_op4_, this->value_));
    }

  // Sleep for 10 milliseconds (10,000 microseconds)
  //ACE_OS::sleep(ACE_Time_Value(0,10000));
}


void
Foo_i::op5(ACE_ENV_SINGLE_ARG_DECL)  
  ACE_THROW_SPEC((CORBA::SystemException, FooException))
{
  ++this->count_op5_;
  ACE_THROW (FooException());
}


void
Foo_i::done(ACE_ENV_SINGLE_ARG_DECL)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  unsigned num_left = --this->num_clients_;

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) A client has reported that it is done(). "
             "There are %d clients left.\n", num_left));

  if (num_left == 0)
    {
      ACE_DEBUG((LM_DEBUG, "(%P|%t) Stats:\n"
                 "op1() count: %d\n"
                 "op2() count: %d\n"
                 "op3() count: %d\n"
                 "op4() count: %d\n"
                 "op5() count: %d\n",
                 this->count_op1_,
                 this->count_op2_,
                 this->count_op3_,
                 this->count_op4_,
                 this->count_op5_));

      if (TheOrbShutdownTask::instance()->open(0) != 0)
        {
          ACE_ERROR((LM_ERROR, "(%P|%t)Foo_i::done: "
            "failed to create orb shutdown thread.\n"));
        }        
    }
}
