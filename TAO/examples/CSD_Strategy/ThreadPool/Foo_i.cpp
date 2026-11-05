#include "Foo_i.h"
#include "OrbShutdownTask.h"

Foo_i::Foo_i (unsigned num_clients)
  : num_clients_ (num_clients)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Number of clients for Foo_i servant: %u\n",
             num_clients));
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

  if (count_op4 % 500 == 0)
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
  unsigned const num_left = this->num_clients_.fetch_sub (1, std::memory_order_acq_rel) - 1;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) A client has reported that it is done(). "
              "There are %u clients left.\n", num_left));

  if (num_left == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Stats:\n"
                 "op1() count: %u\n"
                 "op2() count: %u\n"
                 "op3() count: %u\n"
                 "op4() count: %u\n"
                 "op5() count: %u\n",
                 this->count_op1_.load (std::memory_order_relaxed),
                 this->count_op2_.load (std::memory_order_relaxed),
                 this->count_op3_.load (std::memory_order_relaxed),
                 this->count_op4_.load (std::memory_order_relaxed),
                 this->count_op5_.load (std::memory_order_relaxed)));

      if (TheOrbShutdownTask::instance ()->open (nullptr) != 0)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) Foo_i::done: failed to create orb "
            "shutdown thread.\n"));
        }
    }
}
