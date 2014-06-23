// $Id$
#include "Foo_i.h"
#include "FooServantList.h"

Foo_i::Foo_i(const char* servant_name,FooServantList* mgr)
  : value_(0),
    count_op1_(0),
    count_op2_(0),
    count_op3_(0),
    count_op4_(0),
    count_op5_(0),
    servant_name_(servant_name),
    mgr_(mgr),
    active_ (true)
{
}


Foo_i::~Foo_i()
{
}


void
Foo_i::op1(void)
{
  ++this->count_op1_;
  // Sleep for 10 milliseconds (10,000 microseconds)
  //ACE_OS::sleep(ACE_Time_Value(0,10000));
}


void
Foo_i::op2(CORBA::Long value)
{
  ++this->count_op2_;
  this->value_ = value;
}


CORBA::Long
Foo_i::op3(void)
{
  ++this->count_op3_;

  if (this->count_op3_ == 30)
    {
      this->mgr_->deactivate_servant ();
    }

  return this->value_.value ();
}


void
Foo_i::op4(CORBA::Long value)
{
  ++this->count_op4_;
  this->value_ = value;

  if (this->count_op4_.value () % 1 == 0)
    {
      ACE_DEBUG((LM_DEBUG,
                 "(%P|%t) op4() has been called %d times now.  value == %d\n",
                 this->count_op4_.value (), this->value_.value ()));
    }

  // Sleep for 10 milliseconds (10,000 microseconds)
  //ACE_OS::sleep(ACE_Time_Value(0,10000));
}


void
Foo_i::op5(void)
{
  ++this->count_op5_;
  throw FooException();
}


CORBA::Boolean
Foo_i::op6 (
  const char * user_name,
  char *& message
)
{
  ACE_CString str(message);
  str += user_name;
  CORBA::string_free (message);
  message = CORBA::string_dup (str.c_str ());
  return 1;
}


void
Foo_i::test_unbounded_string_arg (
  const char * message
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_unbounded_string_arg ")
                       ACE_TEXT("got unbounded string %s\n"),
                       message));
}


void
Foo_i::test_bounded_string_arg (
  const char * message
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_bounded_string_arg ")
                       ACE_TEXT("got bounded string %s\n"),
                       message));
}


void
Foo_i::test_fixed_array_arg (
  const ::Fixed_Array message
)
{
  for (CORBA::ULong i = 0; i < 20; i++)
    {
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_fixed_array_arg ")
                           ACE_TEXT("got fixed_array[i] = %d\n"),
                           i, message[i]));
    }
}


void
Foo_i::test_bounded_var_size_arg (
  const ::Bounded_Var_Size & message
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_bounded_var_size_arg ")
                       ACE_TEXT("got var array chars %s\n"),
                       message.get_buffer ()));
}


void
Foo_i::test_unbounded_var_size_arg (
  const ::Unbounded_Var_Size & message
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_unbounded_var_size_arg ")
                       ACE_TEXT("got var array chars %s\n"),
                       message.get_buffer ()));
}


void
Foo_i::test_fixed_size_arg (
  const ::TimeOfDay & t
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_fixed_size_arg ")
                       ACE_TEXT("got timeofday %d:%d:%d\n"),
                       t.hour, t.minute, t.second));
}

void
Foo_i::test_fixed_size_arg_two_way (
  const ::TimeOfDay & t
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_fixed_size_arg_two_way ")
                       ACE_TEXT("got timeofday %d:%d:%d\n"),
                       t.hour, t.minute, t.second));
}

void
Foo_i::test_var_array_arg (
  const ::Var_Array messages
)
{
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_var_array_arg ")
                       ACE_TEXT(" %s\n"), messages[0].in ()));
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_var_array_arg ")
                       ACE_TEXT(" %s\n"), messages[1].in ()));
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_var_array_arg ")
                       ACE_TEXT(" %s\n"), messages[2].in ()));
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
  this->callback_ = Callback::_duplicate (cb);
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_i::test_objref_arg ")
                       ACE_TEXT(" got callback object.\n")));
}


void
Foo_i::callback_object (
  ::Callback_ptr cb
)
{
  this->callback_ = Callback::_duplicate (cb);
}

void
Foo_i::test_callback (
)
{
  if (CORBA::is_nil (this->callback_.in ()))
    {
      throw FooException ();
    }
  else
    {
      this->callback_->test_method ();
    }
}


void
Foo_i::done(void)
{
  this->mgr_->client_done();
}


void
Foo_i::active (bool flag)
{
  active_ = flag;
}


bool
Foo_i::active ()
{
  return active_.value ();
}


