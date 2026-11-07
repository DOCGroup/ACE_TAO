#include "Foo_i.h"
#include "OrbShutdownTask.h"
#include "ace/Log_Msg.h"
#include <string>

void
Foo_i::op1 ()
{}

void
Foo_i::op2 (CORBA::Long value)
{
  this->value_ = value;
}

CORBA::Long
Foo_i::op3 ()
{
  return this->value_.load ();
}

void
Foo_i::op4 (CORBA::Long value)
{
  this->value_ = value;
  unsigned const count = ++this->count_op4_;
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) op4() has been called %u times now.  value == %d\n",
              count,
              this->value_.load ()));

  // Sleep for 10 milliseconds (10,000 microseconds)
  // ACE_OS::sleep (ACE_Time_Value (0, 10000));
}

void
Foo_i::op5 ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Foo_i::op5() - ENTER\n"));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Foo_i::op5() - Increment counter.\n"));
  unsigned const count = ++this->count_op5_;
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Foo_i::op5() - Counter is now %u.\n", count));
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Foo_i::op5() - EXIT - Raise a FooException.\n"));
  throw FooException ();
}

CORBA::Boolean
Foo_i::op6 (
  char const *user_name,
  char *& message
)
{
  std::string combined (message);
  combined += user_name;
  CORBA::String_var replacement = CORBA::string_dup (combined.c_str ());
  CORBA::string_free (message);
  message = replacement._retn ();
  return true;
}

void
Foo_i::test_unbounded_string_arg (char const *message)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_unbounded_string_arg ")
                         ACE_TEXT ("got unbounded string %s\n"),
                         message));
}

void
Foo_i::test_bounded_string_arg (char const *message)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_bounded_string_arg ")
                         ACE_TEXT ("got bounded string %s\n"),
                         message));
}

void
Foo_i::test_fixed_array_arg (::Fixed_Array const message)
{
  for (CORBA::ULong i = 0; i < 20; ++i)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_fixed_array_arg ")
                           ACE_TEXT ("got fixed_array[%u] = %d\n"),
                           i, message[i]));
    }
}

void
Foo_i::test_bounded_var_size_arg (::Bounded_Var_Size const &message)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_bounded_var_size_arg ")
                         ACE_TEXT ("got var array chars %s\n"),
                         message.get_buffer ()));
}

void
Foo_i::test_unbounded_var_size_arg (::Unbounded_Var_Size const &message)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_unbounded_var_size_arg ")
                         ACE_TEXT ("got var array chars %s\n"),
                         message.get_buffer ()));
}

void
Foo_i::test_fixed_size_arg (::TimeOfDay const &t)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_fixed_size_arg ")
                         ACE_TEXT ("got timeofday %d:%d:%d\n"),
                         t.hour, t.minute, t.second));
}

void
Foo_i::test_fixed_size_arg_two_way (::TimeOfDay const &t)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_fixed_size_arg_two_way ")
                         ACE_TEXT ("got timeofday %d:%d:%d\n"),
                         t.hour, t.minute, t.second));
}

void
Foo_i::test_var_array_arg (::Var_Array const messages)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_var_array_arg %s\n"),
              messages[0].in ()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_var_array_arg %s\n"),
              messages[1].in ()));
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_var_array_arg %s\n"),
              messages[2].in ()));
}

void
Foo_i::test_special_basic_arg (::CORBA::Boolean value)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_special_basic_arg ")
                         ACE_TEXT ("got special basic arg: %d\n"), value));
}

void
Foo_i::test_objref_arg (::Callback_ptr cb)
{
  this->set_callback (cb);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Foo_i::test_objref_arg ")
                         ACE_TEXT ("got callback object.\n")));
}

void
Foo_i::callback_object (::Callback_ptr cb)
{
  this->set_callback (cb);
}

void
Foo_i::test_callback ()
{
  Callback_var callback;
  {
    std::lock_guard<std::mutex> guard (this->callback_mutex_);
    callback = Callback::_duplicate (this->callback_.in ());
  }

  if (CORBA::is_nil (callback.in ()))
    {
      throw FooException ();
    }

  callback->test_method ();
}

void
Foo_i::set_callback (Callback_ptr callback)
{
  std::lock_guard<std::mutex> guard (this->callback_mutex_);
  this->callback_ = Callback::_duplicate (callback);
}

void
Foo_i::done ()
{
  static std::atomic<unsigned> num_clients { 2 };

  unsigned const num_clients_left = --num_clients;

  if (num_clients_left == 0)
    {
      if (TheOrbShutdownTask::instance ()->open (nullptr) != 0)
        {
          ACE_ERROR ((LM_ERROR, "(%P|%t) Foo_i::done: "
                      "failed to create orb shutdown thread.\n"));
        }
    }
}
