#include "Foo_i.h"
#include "FooServantList.h"

Foo_i::Foo_i (ACE_TCHAR const *servant_name, FooServantList *mgr)
  : servant_name_ (servant_name),
    mgr_ (mgr)
{
}

void
Foo_i::op1 ()
{
  this->count_op1_.fetch_add (1, std::memory_order_relaxed);
}

void
Foo_i::op2 (CORBA::Long value)
{
  this->count_op2_.fetch_add (1, std::memory_order_relaxed);
  this->value_.store (value, std::memory_order_relaxed);
}

CORBA::Long
Foo_i::op3 ()
{
  this->count_op3_.fetch_add (1, std::memory_order_relaxed);
  return this->value_.load (std::memory_order_relaxed);
}

void
Foo_i::op4 (CORBA::Long value)
{
  unsigned const count_op4 = this->count_op4_.fetch_add (1, std::memory_order_relaxed) + 1;
  this->value_.store (value, std::memory_order_relaxed);

  if (count_op4 % 100 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                 "(%P|%t) op4() has been called %u times now.  value == %d\n",
                 count_op4, this->value_.load (std::memory_order_relaxed)));
    }
}

void
Foo_i::op5 ()
{
  this->count_op5_.fetch_add (1, std::memory_order_relaxed);
}

void
Foo_i::done ()
{
  this->mgr_->client_done ();
}
