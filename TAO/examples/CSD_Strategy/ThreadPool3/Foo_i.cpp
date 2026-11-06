#include "Foo_i.h"
#include "OrbShutdownTask.h"

Foo_i::Foo_i (unsigned num_clients)
  : num_clients_ (num_clients)
{
}

void
Foo_i::op1 ()
{
}

void
Foo_i::op2 (CORBA::Long value)
{
  this->value_.store (value, std::memory_order_relaxed);
}

CORBA::Long
Foo_i::op3 ()
{
  return this->value_.load (std::memory_order_relaxed);
}

void
Foo_i::op4 (CORBA::Long value)
{
  this->value_.store (value, std::memory_order_relaxed);
}

void
Foo_i::op5 ()
{
  throw FooException ();
}

void
Foo_i::done ()
{
  unsigned const num_left = this->num_clients_.fetch_sub (1, std::memory_order_acq_rel) - 1;

  if (num_left == 0)
    {
      if (TheOrbShutdownTask::instance ()->open (nullptr) != 0)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) Foo_i::done: "
            "failed to create orb shutdown thread.\n"));
        }
    }
}
