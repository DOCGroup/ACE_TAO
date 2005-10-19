// $Id$
#include "Foo_i.h"
#include "TestInf/AppShutdown.h"
#include "ace/OS.h"
#include "ace/Time_Value.h"




Foo_i::Foo_i()
{
  for (unsigned i = 0; i < 5; i++)
    {
      this->op_count_[i] = 0;
    }
}


Foo_i::~Foo_i()
{
}


void
Foo_i::op1(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->op_count_[0]++;
}


void
Foo_i::op2(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ACE_UNUSED_ARG(value);
  this->op_count_[1]++;
}


CORBA::Long
Foo_i::op3(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->op_count_[2]++;
  return value;
}


void
Foo_i::op4(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ACE_UNUSED_ARG(value);
  this->op_count_[3]++;
}


void
Foo_i::op5(ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC((CORBA::SystemException, FooException))
{
  this->op_count_[4]++;
  ACE_THROW (FooException());
}


void
Foo_i::done(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  TheAppShutdown->client_done();
}


void
Foo_i::gather_stats(Foo_Statistics& stats)
{
  for (unsigned i = 0; i < 5; i++)
    {
      stats.actual(i + 1, this->op_count_[i]);
    }
}
