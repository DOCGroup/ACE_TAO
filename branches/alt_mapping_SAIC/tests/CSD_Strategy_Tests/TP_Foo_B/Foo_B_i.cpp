// $Id$

#include "Foo_B_i.h"
#include "AppShutdown.h"
#include "TestAppExceptionC.h"
#include "ace/SString.h"

Foo_B_i::Foo_B_i()
: error_count_ (0)
{
   for (unsigned i = 0; i < 16; i++)
    {
      op_count_[i] = 0;
    }
}


Foo_B_i::~Foo_B_i()
{
}


void
Foo_B_i::op1(void)
{
  this->op_count_[0] ++;
}


void
Foo_B_i::op2(CORBA::Long value)
{
  this->op_count_[1] ++;
  this->in_long_[1].push_back (value);
}


CORBA::Long
Foo_B_i::op3(CORBA::Long value)
{
  this->op_count_[2] ++;
  this->in_long_[2].push_back (value);
  return value;
}


void
Foo_B_i::op4(CORBA::Long value)
{
  this->op_count_[3] ++;
  this->in_long_[3].push_back (value);
}


void
Foo_B_i::op5(void)
{
  this->op_count_[4] ++;
  throw FooException();
}


CORBA::Boolean
Foo_B_i::op6(const TimeOfDay& t,
             char*& message)
{
  this->op_count_[5] ++;
  char buf [20];
  ACE_OS::sprintf (buf, "%s %d:%d:%d", message, t.hour, t.minute, t.second);
  CORBA::string_free (message);
  message = CORBA::string_dup (buf);

  return 1;
}


void
Foo_B_i::op7(Callback_ptr cb)
{
  this->op_count_[6] ++;

  if (CORBA::is_nil (cb))
    {
      error_count_ ++;
      ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_i::op7  nil callback error_count %u\n",
        error_count_));
      throw FooException ();
    }
  else
    {
      cb->test_method ();
    }
}


void
Foo_B_i::test_unbounded_string_arg(const char* message)
{
  this->op_count_[7] ++;
  //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_unbounded_string_arg ")
  //                     ACE_TEXT("got unbounded string %s\n"),
  //                     message));
  this->in_string_[7].push_back (message);
}


void
Foo_B_i::test_bounded_string_arg(const char* message)
{
  this->op_count_[8] ++;
  //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_bounded_string_arg ")
  //                     ACE_TEXT("got bounded string %s\n"),
  //                     message));
  this->in_string_[8].push_back (message);
}


void
Foo_B_i::test_fixed_array_arg(const Fixed_Array message)
{
  this->op_count_[9] ++;

  for (unsigned i = 0; i < 19; i++)
    {
      //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_fixed_array_arg ")
      //                     ACE_TEXT("got fixed array[i] = %d\n"),
      //                     i, message[i]));

      if (message[i] != message[i + 1] -1)
        {
          error_count_ ++;
          ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_i::test_fixed_array_arg:  value checking failed "
            "- message[%u]=%d message[%u]=%d error_count=%u\n",
            i, message[i], i+1, message[i + 1], error_count_));
          break;
        }
    }


  this->in_long_[9].push_back (message[0]);
}


void
Foo_B_i::test_bounded_var_size_arg(const Bounded_Var_Size& message)
{
  this->op_count_[10] ++;
  //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_bounded_var_size_arg ")
  //                    ACE_TEXT("got var array chars %s\n"),
  //                    message.get_buffer ()));
  this->in_string_[10].push_back (message.get_buffer ());
}


void
Foo_B_i::test_unbounded_var_size_arg(const Unbounded_Var_Size& message)
{
  this->op_count_[11] ++;
  //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_unbounded_var_size_arg ")
  //                     ACE_TEXT("got var array chars %s\n"),
  //                     message.get_buffer ()));
  this->in_string_[11].push_back (message.get_buffer ());
}


void
Foo_B_i::test_fixed_size_arg(const TimeOfDay& t)
{
  ACE_UNUSED_ARG(t);

  this->op_count_[12] ++;
  //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_fixed_size_arg ")
  //                     ACE_TEXT("got timeofday %d:%d:%d\n"),
  //                     t.hour, t.minute, t.second));
}


void
Foo_B_i::test_var_array_arg(const Var_Array messages)
{
  this->op_count_[13] ++;
  //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_var_array_arg ")
  //                     ACE_TEXT(" %s\n"), messages[0].in ()));
  //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_var_array_arg ")
  //                     ACE_TEXT(" %s\n"), messages[1].in ()));
  //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_var_array_arg ")
  //                     ACE_TEXT(" %s\n"), messages[2].in ()));
  unsigned cur_client_id = 0;

  for (unsigned i = 0 ; i < 3; i ++)
  {
    unsigned client_id = 0;
    unsigned str_id = 0;
    char test_str[20];
    sscanf (messages[i].in (), "%u %s %u", &client_id, test_str, &str_id);

    if (str_id != i)
    {
      error_count_ ++;
      ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_i::test_var_array_arg:  str_id checking failed "
        "- str_id=%u i=%u error_count_=%u\n", str_id, i, error_count_));
    }

    if (i == 0)
    {
       cur_client_id = client_id;
    }
    else if (client_id != cur_client_id)
    {
       error_count_ ++;
       ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_i::test_var_array_arg:  client_id checking failed "
        "- client_id=%u cur_client_id=%u error_count_=%u\n",
        client_id, cur_client_id, error_count_));
    }
  }
  this->in_string_[13].push_back (messages[0].in ());
}


void
Foo_B_i::test_special_basic_arg(CORBA::Boolean value,
                                CORBA::Long client_id)
{
  this->op_count_[14] ++;
  //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_special_basic_arg ")
  //                     ACE_TEXT(" got special basic arg: %d\n"), value));

  CORBA::Boolean expected_special_value = client_id % 2;
  if (expected_special_value != value)
    {
      error_count_ ++;
      ACE_ERROR((LM_ERROR, "(%P|%t)Foo_B_i::test_special_basic_arg  "
        "value checking failed - client_id %d got value %d error_count=%u\n",
        client_id, value, error_count_));
    }
}


void
Foo_B_i::test_objref_arg(Callback_ptr cb)
{
  ACE_UNUSED_ARG(cb);
  this->op_count_[15] ++;
  //ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Foo_B_i::test_objref_arg ")
  //                     ACE_TEXT(" got callback object.\n")));
}


void
Foo_B_i::done(void)
{
  TheAppShutdown->client_done();
}


void
Foo_B_i::gather_stats (Foo_B_Statistics& stats)
{
  for (unsigned i = 0; i < 16; i++)
    {
      stats.actual (i+1, this->op_count_[i]);
      stats.actual (i+1, this->in_string_[i]);
      stats.actual (i+1, this->in_long_[i]);
      stats.servant_error_count (this->error_count_);
    }
}
