// $Id$
#include "Foo_B_ClientEngine.h"
#include "Callback_i.h"
#include "Foo_B_Statistics.h"
#include "TestAppExceptionC.h"
#include "AppHelper.h"
#include "ace/Log_Msg.h"

const char* ONEWAY_ARG_TEST_STR = "TEST";

Foo_B_ClientEngine::Foo_B_ClientEngine(Foo_B_ptr obj,
                                       Callback_ptr callback,
                                       unsigned client_id,
                                       bool collocated)
  : obj_(Foo_B::_duplicate(obj)),
    callback_(Callback::_duplicate(callback)),
    client_id_(client_id),
    collocated_(collocated)
{
}


Foo_B_ClientEngine::~Foo_B_ClientEngine()
{
}


bool
Foo_B_ClientEngine::execute(void)
{
  // Make sure the connection is established before making
  // remote invocations.
  if (AppHelper::validate_connection (this->obj_.in ()) == false)
    {
      ACE_ERROR((LM_ERROR, "(%P|%t)Foo_A_ClientEngine::execute  " \
                          "client %d connect failed.\n", this->client_id_));
      return false;
    }

  // Verify the return values and return the results.
  bool check_validity = true;

  this->obj_->op1();

  this->obj_->op2(this->client_id_);

  CORBA::Long value = this->obj_->op3(this->client_id_);

  if (value != static_cast<CORBA::Long>(this->client_id_))
    {
      check_validity = false;
    }

  for (CORBA::ULong j = 1; j <= 5; j++)
    {
      this->obj_->op4(495 + (this->client_id_ * 5) + j);
    }

  bool caught_exception = false;

  try
  {
    this->obj_->op5();
  }
  catch (const FooException& )
  {
    // Expected
    caught_exception = true;
  }

  if (! caught_exception)
    {
      check_validity = false;
    }

  TimeOfDay t;
  t.hour = 12;
  t.minute = 30;
  t.second = 10;

  char test_str [20];
  ACE_OS::sprintf (test_str, "%d %s", this->client_id_, ONEWAY_ARG_TEST_STR);

  char buffer [20];

  // Two-Way calls with "inout" and fixed size "in" arguments.
  CORBA::String_var message = CORBA::string_dup(test_str);

  CORBA::Boolean result = this->obj_->op6( t, message.inout());

  ACE_UNUSED_ARG(result);

  ACE_OS::sprintf (buffer, "%d %s %d:%d:%d", this->client_id_, ONEWAY_ARG_TEST_STR,
    t.hour, t.minute, t.second);

  if (ACE_OS::strncmp (message.in (), buffer, ACE_OS::strlen (buffer)) != 0)
    {
      check_validity = false;
    }

  // Callback test.
  this->obj_->op7 (this->callback_.in ());

  // One-Way calls with various arguments.
  CORBA::String_var ub_string = CORBA::string_dup( test_str );
  this->obj_->test_unbounded_string_arg (ub_string.in ());

  CORBA::String_var bd_string = CORBA::string_dup( test_str );
  this->obj_->test_bounded_string_arg (bd_string.in ());

  Fixed_Array fixed_array;

  for (CORBA::ULong m = 0 ; m < 20; m ++)
  {
     fixed_array[m] = this->client_id_ + m;
  }

  this->obj_->test_fixed_array_arg (fixed_array);

  Var_Array var_array;

  for (CORBA::ULong k = 0; k < 3; k++)
    {
      ACE_OS::sprintf (buffer, "%d %s %d",
                       this->client_id_, ONEWAY_ARG_TEST_STR, k);
      var_array[k] = CORBA::string_dup(buffer);
    }

  this->obj_->test_var_array_arg (var_array);

  Bounded_Var_Size_var bd_var_size_string = new Bounded_Var_Size();

  bd_var_size_string->replace (ACE_OS::strlen (test_str) + 1,
                               test_str);
  this->obj_->test_bounded_var_size_arg (bd_var_size_string.in ());

  Unbounded_Var_Size_var ub_var_size_string = new Unbounded_Var_Size(100);
  ub_var_size_string->replace (ub_var_size_string->maximum (),
                               ACE_OS::strlen (test_str) + 1,
                               test_str);
  this->obj_->test_unbounded_var_size_arg (ub_var_size_string.in ());

  this->obj_->test_fixed_size_arg (t);

  this->obj_->test_special_basic_arg (this->client_id_ % 2,
                                      this->client_id_);

  this->obj_->test_objref_arg (this->callback_.in ());

  // Sleep for 5 seconds before invoking done().
  // This is a workaround with the problem that some oneway requests lost
  // when the server has multiple orb threads.
  ACE_OS::sleep (5);
  this->obj_->done();

  return check_validity;
}


void
Foo_B_ClientEngine::expected_results(Foo_B_Statistics& stats)
{
  stats.expected(1, 1, Foo_B_Statistics::FOO_B_STAT_NONE);
  stats.expected(2, 1, Foo_B_Statistics::FOO_B_STAT_LONG);
  stats.expected(3, 1, Foo_B_Statistics::FOO_B_STAT_LONG);
  stats.expected(4, 5, Foo_B_Statistics::FOO_B_STAT_LONG);
  stats.expected(5, 1, Foo_B_Statistics::FOO_B_STAT_NONE);
  stats.expected(6, 1, Foo_B_Statistics::FOO_B_STAT_NONE);
  stats.expected(7, 1, Foo_B_Statistics::FOO_B_STAT_NONE);
  stats.expected(8, 1, Foo_B_Statistics::FOO_B_STAT_STRING);
  stats.expected(9, 1, Foo_B_Statistics::FOO_B_STAT_STRING);
  stats.expected(10, 1, Foo_B_Statistics::FOO_B_STAT_LONG);
  stats.expected(11, 1, Foo_B_Statistics::FOO_B_STAT_STRING);
  stats.expected(12, 1, Foo_B_Statistics::FOO_B_STAT_STRING);
  stats.expected(13, 1, Foo_B_Statistics::FOO_B_STAT_NONE);
  stats.expected(14, 1, Foo_B_Statistics::FOO_B_STAT_STRING);
  stats.expected(15, 1, Foo_B_Statistics::FOO_B_STAT_NONE);
  stats.expected(16, 1, Foo_B_Statistics::FOO_B_STAT_NONE);
  stats.expected_callbacks (1);
}


unsigned
Foo_B_ClientEngine::expected_callbacks ()
{
  return 1;
}
