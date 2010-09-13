// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    ClientTask.cpp
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#include "ClientTask.h"
#include "ace/SString.h"

ClientTask::ClientTask(CORBA::ORB_ptr orb,
                       Foo_ptr foo,
                       Callback_ptr callback,
                       bool         collocated)
: orb_ (CORBA::ORB::_duplicate (orb)),
  foo_(Foo::_duplicate(foo)),
  callback_(Callback::_duplicate(callback)),
  collocated_ (collocated)
{
}


ClientTask::~ClientTask()
{
}


int
ClientTask::open(void*)
{
  if (this->activate(THR_NEW_LWP | THR_JOINABLE, 1) != 0)
    {
      // Assumes that when activate returns non-zero return code that
      // no threads were activated.
      ACE_ERROR_RETURN((LM_ERROR,
                        "(%P|%t) ClientTask failed to activate "
                        "the client thread.\n"),
                       -1);
    }

  return 0;
}


int
ClientTask::svc()
{
  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ClientTask::svc start\n"));

  try
  {
    // Make sure the connection is established before making
    // remote invocations.
    if (this->validate_connection () == false)
      {
        ACE_ERROR((LM_ERROR, "(%P|%t)ClientTask::svc  " \
                            "client connect failed.\n"));
        return -1;
      }

    for (CORBA::Long i = 1; i <= 100; i++)
    {
      // Simple Two-way calls.
      this->foo_->op1();

      this->foo_->op2(i);

      CORBA::Long value = this->foo_->op3();

      ACE_DEBUG((LM_DEBUG,
                 "(%P|%t) ===> Value retrieved from op3() == %d\n",
                 value));

      for (CORBA::ULong j = 1; j <= 5; j++)
        {
          this->foo_->op4(495 + (i * 5) + j);
        }

      try
      {
        ACE_DEBUG((LM_DEBUG, "(%P|%t) ===> Invoke op5()\n"));
        this->foo_->op5();
        ACE_DEBUG((LM_DEBUG, "(%P|%t) ===> No exception raised from op5().  :-(\n"));
      }
      catch (const FooException& )
      {
        ACE_DEBUG((LM_DEBUG,
                   "(%P|%t) ===> Caught FooException - as expected.\n"));

      }

      // Two-Way calls with inout parameters.
      CORBA::String_var message = CORBA::string_dup( "Hello! " );
      CORBA::Boolean result
        = this->foo_->op6( "TAO User", message.inout());

      if (result)
        {
          ACE_DEBUG((LM_DEBUG,
                     "(%P|%t) ===> Message from op6() == %s\n",
                     message.in ()));
        }
      else
        {
          ACE_DEBUG((LM_DEBUG,
                     "(%P|%t) ===> op6() returned false.\n"));
        }

      // Callback test.
      this->foo_->callback_object (this->callback_.in ());

      this->foo_->test_callback ();

      // One-Way calls with various arguments.
      CORBA::String_var ub_string = CORBA::string_dup( "UNBOUNDED STRING" );
      this->foo_->test_unbounded_string_arg (ub_string.in ());

      CORBA::String_var bd_string = CORBA::string_dup( "BOUNDED STRING" );
      this->foo_->test_bounded_string_arg (bd_string.in ());

      Fixed_Array fixed_array;

      for (CORBA::ULong m = 0; m < 20; m ++)
        {
          fixed_array[m] = i + m;
        }

      this->foo_->test_fixed_array_arg (fixed_array);

      Var_Array var_array;
      var_array[0] = CORBA::string_dup( "STRING 1" );
      var_array[1] = CORBA::string_dup( "STRING 2" );
      var_array[2] = CORBA::string_dup( "STRING 3" );
      this->foo_->test_var_array_arg (var_array);

      Bounded_Var_Size_var bd_var_size_string = new Bounded_Var_Size();
      char*  buffer1 = CORBA::string_dup ("BOUNDED VAR SIZE CHAR");
      bd_var_size_string->replace (ACE_OS::strlen (buffer1) + 1,
                                   buffer1,
                                   true);
      this->foo_->test_bounded_var_size_arg (bd_var_size_string.in ());

      char* buffer2 = CORBA::string_dup ("UNBOUNDED VAR SIZE CHAR");
      Unbounded_Var_Size_var ub_var_size_string = new Unbounded_Var_Size(100);
      ub_var_size_string->replace (ub_var_size_string->maximum (),
                                   ACE_OS::strlen (buffer2) + 1,
                                   buffer2,
                                   true);
      this->foo_->test_unbounded_var_size_arg (ub_var_size_string.in ());

      TimeOfDay t;
      t.hour = 12;
      t.minute = 30;
      t.second = 10;
      this->foo_->test_fixed_size_arg (t);

      this->foo_->test_fixed_size_arg_two_way (t);

      CORBA::Boolean special_value = 1;
      this->foo_->test_special_basic_arg (special_value);

      this->foo_->test_objref_arg (this->callback_.in ());
    }

    ACE_DEBUG((LM_DEBUG,
               "(%P|%t) ClientTask::svc - Invoke foo->done()\n"));

    this->foo_->done ();

    ACE_DEBUG((LM_DEBUG,
               "(%P|%t) ClientTask::svc - Back from foo->done()\n"));
  }
  catch (const CORBA::Exception& ex)
  {
     ex._tao_print_exception ("Caught exception in ClientTask::svc:");
  }
  catch (...)
  {
    ACE_ERROR((LM_ERROR,
               "(%P|%t) Unknown (...) exception caught in ClientTask::svc()\n"));
  }

  if (! collocated_)
    {
      ACE_OS::sleep (2);
      this->orb_->shutdown(0);
    }

  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ClientTask::svc end\n"));

  return 0;
}


int
ClientTask::close(u_long)
{
  ACE_DEBUG((LM_DEBUG,
             "(%P|%t) ClientTask::close() - enter/exit\n"));
  return 0;
}


bool
ClientTask::validate_connection ()
{
  for (CORBA::ULong j = 0; j != 1000; ++j)
    {
      try
        {
#if (TAO_HAS_CORBA_MESSAGING == 1)
          CORBA::PolicyList_var unused;
          this->foo_->_validate_connection (unused);
#else
          this->foo_->_is_a ("Not_An_IDL_Type");
#endif /* TAO_HAS_MESSAGING == 1 */
          return true;
        }
      catch (const CORBA::Exception&)
        {
        }
    }

  return false;
}
