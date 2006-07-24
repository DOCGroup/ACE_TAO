// $Id$
#include "Foo_C_i.h"
#include "AppShutdown.h"
#include "CustomExceptionC.h"


Foo_C_i::Foo_C_i()
{
  for (unsigned i = 0; i < 10; i++)
    {
      this->count_[i] = 0;
    }
}


Foo_C_i::~Foo_C_i()
{
}


void
Foo_C_i::op1(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ++this->count_[0];
}


void
Foo_C_i::op2(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->in_values_[1].push_back (value);
  ++this->count_[1];
}


CORBA::Long
Foo_C_i::op3(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->in_values_[2].push_back (value);
  ++this->count_[2];
  return value;
}


void
Foo_C_i::op4(CORBA::Long value ACE_ENV_ARG_DECL_NOT_USED)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  this->in_values_[3].push_back (value);
  ++this->count_[3];
}


void
Foo_C_i::op5(ACE_ENV_SINGLE_ARG_DECL)  
  ACE_THROW_SPEC((CORBA::SystemException, FooException))
{
  ++this->count_[4];
  ACE_THROW (FooException());
}


void
Foo_C_i::done(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)  
  ACE_THROW_SPEC((CORBA::SystemException))
{
  TheAppShutdown->client_done();
}


void
Foo_C_i::cust_op1(ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  ++this->count_[5];
}


void
Foo_C_i::cust_op2(long value ACE_ENV_ARG_DECL_NOT_USED)
{
  this->in_values_[6].push_back (value);
  ++this->count_[6];
}


long
Foo_C_i::cust_op3(long value ACE_ENV_ARG_DECL_NOT_USED)
{
  this->in_values_[7].push_back (value);
  ++this->count_[7];
  return value;
}


void
Foo_C_i::cust_op4(long value ACE_ENV_ARG_DECL_NOT_USED)
{
  this->in_values_[8].push_back (value);
  ++this->count_[8];
}


void
Foo_C_i::cust_op5(ACE_ENV_SINGLE_ARG_DECL)
{
  ++this->count_[9];
  ACE_THROW (CustomException());
}


void
Foo_C_i::gather_stats(Foo_C_Statistics& stats)
{
  for (unsigned i = 0; i < 10; i++)
    {
      stats.actual (i + 1, this->count_[i]);
      stats.actual_in_values (i + 1, this->in_values_[i]);
    }
}


void
Foo_C_i::dump()
{
  static unsigned id = 0;

  ++id;

  ACE_DEBUG((LM_DEBUG, "Servant %d Stats:\n", id));
  ACE_DEBUG((LM_DEBUG, "------------------\n"));

  unsigned i;

  for (i = 0; i < 5; i++)
    {
      ACE_DEBUG((LM_DEBUG, "op%d     : %d\n", i+1, this->count_[i]));
    }

  for (i = 5; i < 10; i++)
    {
      ACE_DEBUG((LM_DEBUG, "cust_op%d: %d\n", i+1, this->count_[i]));
    }

  ACE_DEBUG((LM_DEBUG, "------------------\n"));
}
