//
// $Id$
//
#include "Hello.h"
#include "tao/ORB_Core.h"
#include "tao/ORB_Table.h"
#include "tao/ORB_Core_Auto_Ptr.h"
// Force static build to load the DII_Arguments_Converter service.
#include "tao/DynamicInterface/DII_Arguments_Converter_Impl.h"

extern CORBA::Boolean debug;
extern char TEST_STR[];
extern CORBA::Long TEST_BASIC_VALUE;
extern CORBA::Char TEST_SPECIAL_VALUE;
extern CORBA::Short TEST_HOUR;
extern CORBA::Short TEST_MINUTE;
extern CORBA::Short TEST_SECOND;

#define ERROR_COUNT(message)                                                 \
{ error_count_ ++;                                                           \
  ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)%s - failed error_count = %u\n"),    \
                       message, error_count_));                              \
}

// Implementation skeleton constructor
Test_Simple_Test_i::Test_Simple_Test_i (void)
{
}

// Implementation skeleton destructor
Test_Simple_Test_i::~Test_Simple_Test_i (void)
{
}

void Test_Simple_Test_i::test_method (
    ::CORBA::ULong & error_count
  )
{
  error_count --;
}

Hello::Hello (CORBA::ORB_ptr orb,
              ACE_thread_t thrid)
  : orb_ (CORBA::ORB::_duplicate (orb))
    , thr_id_ (thrid)
    , error_count_ (0)
{
}

void
Hello::test_basic_arg (
    ::CORBA::Long basic,
    ::CORBA::Long_out x
  )
{
  if (basic != TEST_BASIC_VALUE)
    ERROR_COUNT ("Hello::test_basic_arg")

  x = 10;
}

void
Hello::test_unbounded_string_arg(const char* message)
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_unbounded_string_arg ")
                        ACE_TEXT("got unbounded string: %s\n"),
                        message));
  if (ACE_OS::strncmp (message, TEST_STR, ACE_OS::strlen (TEST_STR) != 0))
  {
    ERROR_COUNT ("Hello::test_unbounded_string_arg")
  }
}

void
Hello::test_unbounded_string_arg_out(const char* message, ::CORBA::String_out out_message)
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_unbounded_string_arg_out ")
                        ACE_TEXT("got unbounded string: %s\n"),
                        message));
  if (ACE_OS::strncmp (message, TEST_STR, ACE_OS::strlen (TEST_STR) != 0))
  {
    ERROR_COUNT ("Hello::test_unbounded_string_arg_out")
  }
  out_message = CORBA::string_dup (message);
}


void
Hello::test_bounded_string_arg(const char* message)
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_bounded_string_arg ")
                        ACE_TEXT("got bounded string: %s\n"),
                        message));
  if (ACE_OS::strncmp (message, TEST_STR, ::Test::BOUNDED_STRING_SIZE) != 0)
  {
    ERROR_COUNT ("Hello::test_bounded_string_arg")
  }
}


void
Hello::test_fixed_array_arg(const ::Test::Fixed_Array message)
{
  for (unsigned i = 0; i < ::Test::FIX_ARRAY_SIZE; i++)
  {
    if (debug)
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_fixed_array_arg ")
        ACE_TEXT("got fixed array: %d\n"),
        message[i]));
    if (message[i] != static_cast<CORBA::Long> (i + 999))
      ERROR_COUNT ("Hello::test_fixed_array_arg")
  }
}


void
Hello::test_bounded_var_size_arg(const ::Test::Bounded_Var_Size_Arg& message)
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_bounded_var_size_arg ")
      ACE_TEXT("got bounded var size arg\n")));
  if (ACE_OS::strncmp (message.get_buffer (), TEST_STR, ::Test::BOUNDED_VAR_SIZE) != 0)
    ERROR_COUNT ("Hello::test_bounded_var_size_arg")
}


void
Hello::test_unbounded_var_size_arg(const ::Test::Unbounded_Var_Size_Arg& message)
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_unbounded_var_size_arg ")
                        ACE_TEXT("got unbounded var size arg.\n")));
  if (ACE_OS::strncmp (message.get_buffer (), TEST_STR, ACE_OS::strlen (TEST_STR)) != 0)
    ERROR_COUNT ("Hello::test_unbounded_var_size_arg")
}


void
Hello::test_fixed_size_arg(const ::Test::TimeOfDay& t)
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_fixed_size_arg ")
                        ACE_TEXT("got timeofday: %d:%d:%d\n"),
                        t.hour, t.minute, t.second));

  if (t.hour != TEST_HOUR
      || t.minute != TEST_MINUTE
      || t.second != TEST_SECOND)
    ERROR_COUNT ("Hello::test_fixed_size_arg")
}


void
Hello::test_var_array_arg(const Test::Var_Array messages)
{
  for (CORBA::ULong i = 0; i != ::Test::VAR_ARRAY_SIZE; i ++)
  {
    if (debug)
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_var_array_arg ")
                        ACE_TEXT("messages[%d] = %s\n"), i, messages[i].in ()));
    if (ACE_OS::strncmp (messages[i].in (), TEST_STR, ACE_OS::strlen (TEST_STR)) != 0)
      ERROR_COUNT ("Hello::test_var_array_arg")
  }
}


void
Hello::test_special_basic_arg(CORBA::Char value)
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_special_basic_arg ")
                          ACE_TEXT(" got special basic arg: %c\n"), value));
  if (value != TEST_SPECIAL_VALUE)
    ERROR_COUNT ("Hello::test_special_basic_arg")
}


void Hello::test_objref_arg (
    ::Test::Simple_Test_ptr test
  )
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_objref_arg\n")));

  if (CORBA::is_nil (test))
    ERROR_COUNT ("Hello::test_objref_arg")
  else
  {
    // Increment the error_count before calling test_method() and decrement
    // the error_count when the server handling the test_method() call.
    CORBA::ULong error_count = 0;
    error_count ++;
    test->test_method (error_count);
    if (error_count != 0)
    {
      ERROR_COUNT ("Hello::test_objref_arg")
    }
  }
}

void Hello::test_object_arg (
    ::CORBA::Object_ptr o
  )
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_object_arg\n")));
  ::Test::Simple_Test_var simple_test = ::Test::Simple_Test::_narrow (o);

  CORBA::ULong error_count = 0;
  error_count ++;
  simple_test->test_method (error_count);
  if (error_count != 0)
  {
    ERROR_COUNT ("Hello::test_object_arg")
  }
}

void Hello::test_args_1 (
    ::CORBA::Object_ptr arg1,
    ::CORBA::Char arg2,
    ::Test::Simple_Test_ptr arg3
  )
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_args_1\n")));
  ::Test::Simple_Test_var simple_test = ::Test::Simple_Test::_narrow (arg1);

  CORBA::ULong error_count = 0;
  error_count ++;
  simple_test->test_method (error_count);
  error_count ++;
  arg3->test_method (error_count);

  if (error_count != 0)
  {
    ERROR_COUNT ("Hello::test_args_1")
  }

  if (arg2 != TEST_SPECIAL_VALUE)
    ERROR_COUNT ("Hello::test_args_1")
}

void Hello::test_args_2 (
    const char * arg1,
    const ::Test::Fixed_Array arg2,
    const ::Test::TimeOfDay & arg3
  )
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_args_2\n")));
  if (ACE_OS::strncmp (arg1, TEST_STR, ACE_OS::strlen (TEST_STR)) != 0 )
    ERROR_COUNT ("Hello::test_args_2")

  for (unsigned i = 0; i < ::Test::FIX_ARRAY_SIZE; i++)
  {
    if (arg2[i] != static_cast<CORBA::Long> (i + 999))
      ERROR_COUNT ("Hello::test_args_2")
  }

  if (arg3.hour != TEST_HOUR
      || arg3.minute != TEST_MINUTE
      || arg3.second != TEST_SECOND)
    ERROR_COUNT ("Hello::test_args_2")
}

void Hello::test_args_3 (
    const char * arg1,
    const ::Test::Bounded_Var_Size_Arg & arg2,
    const ::Test::Unbounded_Var_Size_Arg & arg3,
    const ::Test::Var_Array arg4
  )
{
  if (debug)
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("(%P|%t)Hello::test_args_3\n")));
  if (ACE_OS::strncmp (arg1, TEST_STR, ACE_OS::strlen (arg1)) != 0)
    ERROR_COUNT ("Hello::test_args_3")
  if (ACE_OS::strncmp (arg2.get_buffer (), TEST_STR, ::Test::BOUNDED_VAR_SIZE) != 0)
    ERROR_COUNT ("Hello::test_args_3")
  if (ACE_OS::strncmp (arg3.get_buffer (), TEST_STR, ACE_OS::strlen (TEST_STR)) != 0)
    ERROR_COUNT ("Hello::test_args_3")
  for (CORBA::ULong i = 0; i != ::Test::VAR_ARRAY_SIZE; i ++)
  {
    if (ACE_OS::strncmp (arg4[i].in (), TEST_STR, ACE_OS::strlen (TEST_STR)) != 0)
      ERROR_COUNT ("Hello::test_args_3")
  }
}

char *
Hello::get_string (void)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) Upcall in process ..\n"));

  if (ACE_Thread::self () == this->thr_id_)
    {
      if (this->orb_->orb_core ()->optimize_collocation_objects () &&
          this->orb_->orb_core ()->use_global_collocation ())
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR: A remote call has been made "
                      " exiting ..\n"));
          ACE_OS::abort ();
        }
      else if (this->orb_->orb_core ()->optimize_collocation_objects () &&
               this->orb_->orb_core ()->use_global_collocation () == 0)
        {
          TAO::ORB_Table * const orb_table =
            TAO::ORB_Table::instance ();

          TAO_ORB_Core_Auto_Ptr tmp (orb_table->find ("server_orb"));
          if (tmp.get () == 0)
            {
              // We are running on a single ORB and this is an error.
              ACE_ERROR ((LM_ERROR,
                          "(%P|%t) ERROR: A remote call has been made "
                          " with a single ORB "
                          " exiting ..\n"));
              ACE_OS::abort ();
            }
        }
    }

  return CORBA::string_dup ("Hello there!");
}

char *
Hello::test_unbounded_string (
    const char * s1,
    char *& s2,
    CORBA::String_out s3
  )
{
  char *retstr = CORBA::string_dup (s1);
  s3 = CORBA::string_dup (s1);
  char *tmp = CORBA::string_alloc (2*ACE_OS::strlen (s2));
  ACE_OS::sprintf (tmp, "%s%s", s2, s2);
  CORBA::string_free (s2);
  s2 = tmp;
  return retstr;
}

void
Hello::shutdown (void)
{
  // Give the client thread time to return from the collocated
  // call to this method before shutting down the ORB.  We sleep
  // to avoid BAD_INV_ORDER exceptions on fast dual processor machines.
  ACE_OS::sleep (1);
  this->orb_->shutdown (0);
}

CORBA::ULong Hello::error_count () const
{
  return error_count_;
}



