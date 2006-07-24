// $Id$
#include "Foo_A_i.h"
#include "AppShutdown.h"


Foo_A_i::Foo_A_i()
{
  for (unsigned i = 0; i < 5; i++)
    {
      this->op_count_[i] = 0;
    }
}


Foo_A_i::~Foo_A_i()
{
}


void
Foo_A_i::op1(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->op_count_[0] ++;
}


void
Foo_A_i::op2(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->in_values_[1].push_back (value);
  this->op_count_[1] ++;
}


CORBA::Long
Foo_A_i::op3(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->in_values_[2].push_back (value);
  this->op_count_[2] ++;
  return value;
}


void
Foo_A_i::op4(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->in_values_[3].push_back (value);
  this->op_count_[3] ++;
}


void
Foo_A_i::op5(ACE_ENV_SINGLE_ARG_DECL)  
  ACE_THROW_SPEC((CORBA::SystemException, FooException))
{
  this->op_count_[4] ++;
  ACE_THROW (FooException());
}


void
Foo_A_i::done(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  TheAppShutdown->client_done();
}


void
Foo_A_i::gather_stats(Foo_A_Statistics& stats)
{
  for (unsigned i = 0; i < 5; i++)
    {
      stats.actual (i + 1, this->op_count_[i]);
      stats.actual_in_values (i + 1, this->in_values_[i]);
    }
}

