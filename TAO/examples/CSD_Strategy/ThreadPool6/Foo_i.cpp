#include "Foo_i.h"
#include "OrbShutdownTask.h"

Foo_i::Foo_i (unsigned num_clients)
  : num_clients_ (num_clients)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Number of clients for Foo_i servant: %u\n"),
             num_clients));
}

void
Foo_i::op1 ()
{
  ++this->count_op1_;
  // Sleep for 10 milliseconds (10,000 microseconds)
  //ACE_OS::sleep(ACE_Time_Value(0,10000));
}

void
Foo_i::op2 (CORBA::Long value)
{
  ++this->count_op2_;
  this->value_.store (value);
}

CORBA::Long
Foo_i::op3 ()
{
  ++this->count_op3_;
  return this->value_.load ();
}

void
Foo_i::op4 (CORBA::Long value)
{
  unsigned const count = ++this->count_op4_;
  this->value_.store (value);

  if (count % 500 == 0)
    {
      CORBA::Long const current_value = this->value_.load ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) op4() has been called %u times now.  value == %d\n"),
                 count, current_value));
    }

  // Sleep for 10 milliseconds (10,000 microseconds)
  //ACE_OS::sleep(ACE_Time_Value(0,10000));
}

void
Foo_i::op5 ()
{
  ++this->count_op5_;
  throw FooException ();
}

void
Foo_i::done ()
{
  unsigned const num_left = this->num_clients_.fetch_sub (1) - 1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) A client has reported that it is done(). ")
              ACE_TEXT ("There are %u clients left.\n"), num_left));

  if (num_left == 0)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Stats:\n")
                 ACE_TEXT ("op1() count: %u\n")
                 ACE_TEXT ("op2() count: %u\n")
                 ACE_TEXT ("op3() count: %u\n")
                 ACE_TEXT ("op4() count: %u\n")
                 ACE_TEXT ("op5() count: %u\n"),
                 this->count_op1_.load (),
                 this->count_op2_.load (),
                 this->count_op3_.load (),
                 this->count_op4_.load (),
                 this->count_op5_.load ()));

      if (TheOrbShutdownTask::instance ()->open (nullptr) != 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Foo_i::done: ")
                      ACE_TEXT ("failed to create ORB shutdown thread.\n")));
        }
    }
}
