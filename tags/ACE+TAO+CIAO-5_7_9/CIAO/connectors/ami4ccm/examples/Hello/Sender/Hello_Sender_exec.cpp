// -*- C++ -*-
// $Id$

#include "Hello_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_Hello_Sender_Impl
{
  void HandleException (
      long id,
      const char* error_string,
      const char* func)
    {
      printf ("Sender (%s) :\tCallback except from AMI\n", func);
      if (id != 42)
      {
        printf ("ERROR (%s) :\tReceived unexpected ID received in except handler\n",
              func);
        return;
      }
      if (ACE_OS::strcmp (error_string, "Hello world") != 0)
      {
        printf ("ERROR (%s) :\tReceived unexpected error string received in except handler\n",
              func);
        return;
      }
      printf ("OK (%s) :\tReceived the correct except parameters.\n", func);
    }

  void HandleException (
      ::CCM_AMI::ExceptionHolder_ptr excep_holder,
      const char* func)
    {
      printf ("Sender (%s) :\tHandle except AMI\n", func);
      try
        {
          excep_holder->raise_exception ();
        }
      catch (const Hello::InternalError& ex)
        {
          ACE_DEBUG ((LM_DEBUG, "Sender: Caught the correct except type <%d> <%C>\n",
                  ex.id, ex.error_string.in ()));
          HandleException (ex.id, ex.error_string.in (), func);
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("ERROR: Caught unexpected except:");
        }
    }

  //============================================================
  // Facet Executor Implementation Class: MyFoo_callback_exec_i
  //============================================================
  MyFoo_callback_exec_i::MyFoo_callback_exec_i (void)
  {
  }

  MyFoo_callback_exec_i::~MyFoo_callback_exec_i (void)
  {
  }

  //============================================================
  // Operations from ::CCM_AMI::MyFoo_callback
  //============================================================

  // FOO methods
  void
  MyFoo_callback_exec_i::foo (
    ::CORBA::Long ami_return_val,
    const char * answer)
  {
    printf ("Sender (FOO) :\tCallback from AMI : result <%d> answer <%s>\n", ami_return_val, answer);
  }

  void
  MyFoo_callback_exec_i::foo_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "FOO");
  }

  // HELLO methods
  void
  MyFoo_callback_exec_i::hello (
    ::CORBA::Long answer)
  {
    printf ("Sender (FOO) :\tCallback from AMI (HELLO) : answer <%d>\n", answer);
  }

  void
  MyFoo_callback_exec_i::hello_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "HELLO");
  }

  //GET rw_attrib Reply Handler
  void
  MyFoo_callback_exec_i::get_rw_attrib (
    ::CORBA::Short ami_return_val)
  {
    printf ("Sender (RW_ATTRIB) :\tCallback from AMI (RW_ATTRIB) : answer <%d>\n", ami_return_val);
  }

  void
  MyFoo_callback_exec_i::get_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "GET RW_ATTRIB");
  }

  //SET rw_attrib Reply Handler
  void
  MyFoo_callback_exec_i::set_rw_attrib ()
  {
    printf ("Sender (RW_ATTRIB) :\tCallback from AMI (RW_ATTRIB)\n");
  }

  void
  MyFoo_callback_exec_i::set_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "SET_RW_ATTRIB");
  }

  //ro_attrib Reply Handler
  void
  MyFoo_callback_exec_i::get_ro_attrib (
    ::CORBA::Short ami_return_val)
  {
    printf ("Sender (RO_ATTRIB) :\tCallback from AMI (RO_ATTRIB) : answer <%d>\n", ami_return_val);
  }

  void
  MyFoo_callback_exec_i::get_ro_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "RO_ATTRIB");
  }

  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (::Hello::AMI4CCM_MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::Hello::AMI4CCM_MyFoo::_duplicate (my_foo_ami))
  {
  }

  int asynch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    for (int i = 0; i < 5; ++i)
      {
        if (CORBA::is_nil (my_foo_ami_))
          {
            printf ("ERROR Sender (ASYNCH) :\tfoo_ami is NIL !!!\n");
            return 1;
          }
        else
          {
            printf ("Sender (ASYNCH) :\tInvoke Asynchronous calls\n");
            my_foo_ami_->sendc_foo (new MyFoo_callback_exec_i (), "Do something asynchronous");
            my_foo_ami_->sendc_hello (new MyFoo_callback_exec_i ());
            my_foo_ami_->sendc_get_rw_attrib(new MyFoo_callback_exec_i ());
            my_foo_ami_->sendc_get_rw_attrib(new MyFoo_callback_exec_i ());
            my_foo_ami_->sendc_set_rw_attrib(new MyFoo_callback_exec_i (), 15);
            my_foo_ami_->sendc_get_ro_attrib(new MyFoo_callback_exec_i ());
            my_foo_ami_->sendc_get_ro_attrib(new MyFoo_callback_exec_i ());
            printf ("Sender (ASYNCH) :\tInvoked Asynchronous calls\n");
          }
      }
    printf ("Sender (ASYNCH) :\tInvoke Asynchronous calls to test except handling\n");
    my_foo_ami_->sendc_foo (new MyFoo_callback_exec_i (), "");
    my_foo_ami_->sendc_set_rw_attrib(new MyFoo_callback_exec_i (), 0);
    printf ("Sender (ASYNCH) :\tInvoked Asynchronous call.\n");
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (::Hello::MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::Hello::MyFoo::_duplicate (my_foo_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    //run synch calls
    char * out_str = 0;
    for (int i = 0; i < 5; ++i)
      {
        CORBA::Long result = my_foo_ami_->foo ("Do something synchronous", out_str);
        printf ("Sender (SYNCH):\tInvoked synchronous call (FOO) result <%d> answer <%s>\n", result, out_str);

        CORBA::Long answer;
        my_foo_ami_->hello (answer);
        printf ("Sender (SYNCH):\tInvoked synchronous call (HELLO) answer <%d>\n", answer);

        try 
          {
            CORBA::Short rw_attrib = my_foo_ami_->rw_attrib ();
            printf ("Sender (SYNCH):\tInvoked synchronous call (GET_RW_ATTRIB) answer <%d>\n", rw_attrib);
          }
        catch (const Hello::InternalError& ex)
          {
            printf ("Sender (SYNCH FOO) :\tExpected Except caught : <%d> <%s>\n", ex.id, ex.error_string.in ());
          }
          
        try 
          {
            my_foo_ami_->rw_attrib (15);
            printf ("Sender (SYNCH):\tInvoked synchronous call (SET_RW_ATTRIB) to <15>\n");
          }
        catch (const Hello::InternalError& ex)
          {
            printf ("Sender (SYNCH FOO) :\tExpected Except caught : <%d> <%s>\n", ex.id, ex.error_string.in ());
          }

        try 
          {
            CORBA::Short ro_attrib = my_foo_ami_->ro_attrib ();
            printf ("Sender (SYNCH):\tInvoked synchronous call (GET_RO_ATTRIB) answer <%d>\n", ro_attrib);
          }
        catch (const Hello::InternalError& ex)
          {
            printf ("Sender (SYNCH FOO) :\tExpected Except caught : <%d> <%s>\n", ex.id, ex.error_string.in ());
          }
      }
    try
      {
        CORBA::Long result = my_foo_ami_->foo ("", out_str);
        printf ("Sender (SYNCH) :\tInvoked synchronous call result <%d> answer <%s>\n", result, out_str);
      }
    catch (const Hello::InternalError& ex)
      {
        printf ("Sender (SYNCH FOO) :\tExpected Except caught : <%d> <%s>\n", ex.id, ex.error_string.in ());
      }

    try
      {
        my_foo_ami_->rw_attrib (0);
        printf ("Sender (SYNCH) :\tInvoked synchronous call rw_attrib\n");
      }
    catch (const Hello::InternalError& ex)
      {
        printf ("Sender (SYNCH RW_ATTRIB) :\tExpected Except caught : <%d> <%s>\n", ex.id, ex.error_string.in ());
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Caught unexpected except:");
      }
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
  : global_foo_callback_ (0)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.

  // Component attributes.

  // Port operations.
  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Hello::CCM_Sender_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    ::Hello::AMI4CCM_MyFoo_var asynch_foo =
      this->context_->get_connection_sendc_run_my_foo();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::Hello::MyFoo_var synch_foo =
        this->context_->get_connection_run_my_foo ();
    synch_foo_generator* synch_foo_gen =
      new synch_foo_generator (synch_foo);
    synch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_Hello_AMI_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_RETURN (
      retval,
      Sender_exec_i,
      ::Components::EnterpriseComponent::_nil ());

    return retval;
  }
}

