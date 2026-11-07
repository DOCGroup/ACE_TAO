#include "Foo_i.h"
#include "FooServantList.h"
#include "ace/Guard_T.h"
#include <string>

Foo_i::Foo_i (FooServantList *mgr)
  : mgr_ (mgr)
{
}

void
Foo_i::op1()
{
  // Sleep for 10 milliseconds (10,000 microseconds)
  //ACE_OS::sleep(ACE_Time_Value(0,10000));
}

void
Foo_i::op2(CORBA::Long value)
{
  this->value_ = value;
}

CORBA::Long
Foo_i::op3()
{
  ++this->count_op3_;

  if (this->count_op3_.load () == 30)
    {
      this->mgr_->deactivate_servant ();
    }

  return this->value_.load ();
}

void
Foo_i::op4(CORBA::Long value)
{
  ++this->count_op4_;
  this->value_ = value;

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) op4() has been called %u times now.  value == %d\n",
             this->count_op4_.load (), this->value_.load ()));

  // Sleep for 10 milliseconds (10,000 microseconds)
  //ACE_OS::sleep(ACE_Time_Value(0,10000));
}

void
Foo_i::op5()
{
  throw FooException();
}

CORBA::Boolean
Foo_i::op6 (
  char const * user_name,
  char *& message
)
{
  std::string result (message);
  result += user_name;
  CORBA::String_var replacement = CORBA::string_dup (result.c_str ());
  CORBA::string_free (message);
  message = replacement._retn ();
  return true;
}

void
Foo_i::test_unbounded_string_arg (
  char const * message
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_unbounded_string_arg ")
                       ACE_TEXT("got unbounded string %C\n"),
                       message));
}

void
Foo_i::test_bounded_string_arg (
  char const * message
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_bounded_string_arg ")
                       ACE_TEXT("got bounded string %C\n"),
                       message));
}

void
Foo_i::test_fixed_array_arg (
  ::Fixed_Array const message
)
{
  for (CORBA::ULong i = 0; i < 20; ++i)
    {
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_fixed_array_arg ")
                           ACE_TEXT("got fixed_array[i] = %d\n"),
                           i, message[i]));
    }
}

void
Foo_i::test_bounded_var_size_arg (
  ::Bounded_Var_Size const & message
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_bounded_var_size_arg ")
                       ACE_TEXT("got var array chars %C\n"),
                       message.get_buffer ()));
}

void
Foo_i::test_unbounded_var_size_arg (
  ::Unbounded_Var_Size const & message
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_unbounded_var_size_arg ")
                       ACE_TEXT("got var array chars %C\n"),
                       message.get_buffer ()));
}

void
Foo_i::test_fixed_size_arg (
  ::TimeOfDay const & t
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_fixed_size_arg ")
                       ACE_TEXT("got timeofday %d:%d:%d\n"),
                       t.hour, t.minute, t.second));
}

void
Foo_i::test_fixed_size_arg_two_way (
  ::TimeOfDay const & t
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_fixed_size_arg_two_way ")
                       ACE_TEXT("got timeofday %d:%d:%d\n"),
                       t.hour, t.minute, t.second));
}

void
Foo_i::test_var_array_arg (
  ::Var_Array const messages
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_var_array_arg ")
                       ACE_TEXT(" %C\n"), messages[0].in ()));
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_var_array_arg ")
                       ACE_TEXT(" %C\n"), messages[1].in ()));
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_var_array_arg ")
                       ACE_TEXT(" %C\n"), messages[2].in ()));
}

void
Foo_i::test_special_basic_arg (
  ::CORBA::Boolean value
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_special_basic_arg ")
                       ACE_TEXT(" got special basic arg: %d\n"), value));
}

void
Foo_i::test_objref_arg (
  ::Callback_ptr cb
)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->callback_lock_);
  this->callback_ = Callback::_duplicate (cb);
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_objref_arg ")
                       ACE_TEXT(" got callback object.\n")));
}

void
Foo_i::callback_object (
  ::Callback_ptr cb
)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->callback_lock_);
  this->callback_ = Callback::_duplicate (cb);
}

void
Foo_i::test_callback (
)
{
  Callback_var callback;
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->callback_lock_);
    callback = Callback::_duplicate (this->callback_.in ());
  }

  if (CORBA::is_nil (callback.in ()))
    {
      throw FooException ();
    }
  else
    {
      callback->test_method ();
    }
}

void
Foo_i::done()
{
  this->mgr_->client_done();
}

void
Foo_i::active (bool const flag)
{
  active_ = flag;
}

bool
Foo_i::active () const
{
  return active_.load ();
}

