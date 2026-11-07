#include "Foo_i.h"
#include "TestInf/AppShutdown.h"
#include "ace/Time_Value.h"

Foo_i::Foo_i () = default;

Foo_i::~Foo_i () = default;

void
Foo_i::op1 ()
{
  this->op_count_[0]++;
}

void
Foo_i::op2 (CORBA::Long value)
{
  ACE_UNUSED_ARG(value);
  this->op_count_[1]++;
}

CORBA::Long
Foo_i::op3 (CORBA::Long value)
{
  this->op_count_[2]++;
  return value;
}

void
Foo_i::op4 (CORBA::Long value)
{
  ACE_UNUSED_ARG(value);
  this->op_count_[3]++;
}

void
Foo_i::op5 ()
{
  this->op_count_[4]++;
  throw FooException();
}

void
Foo_i::done ()
{
  TheAppShutdown->client_done();
}

void
Foo_i::gather_stats (Foo_Statistics& stats)
{
  for (unsigned i = 0; i < this->op_count_.size (); i++)
    {
      stats.actual (i + 1, this->op_count_[i]);
    }
}
