// -*- C++ -*-
// $Id$

// test the use of one replyhandler with more then one request
#include "OneReplyH_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_OneReplyH_Sender_Impl
{
  CORBA::Short nr_of_excep_received = 0;
  CORBA::Short nr_of_received = 0;
  const CORBA::Short cmd_synch_foo_ok = 1;
  const CORBA::Short cmd_synch_foo_nok = 2;
  const CORBA::Short cmd_asynch_foo_ok = 3;
  const CORBA::Short cmd_asynch_foo_nok = 4;
  const CORBA::Short cmd_synch_bar_ok = 5;
  const CORBA::Short cmd_synch_bar_nok = 6;
  const CORBA::Short cmd_asynch_bar_ok = 7;
  const CORBA::Short cmd_asynch_bar_nok = 8;

  void HandleException (
      long id,
      long expect_id,
      const char* /*error_string*/,
      const char* func)
  {
    //expected exception
    if ( id == expect_id)
      {
        ++nr_of_excep_received;
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR Sender: wrong exception received"
                              " for %C\n", func));
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
    const char * /*answer*/)
  {
    if ( ami_return_val == cmd_asynch_foo_ok)
     {
       ++nr_of_received;
     }
  }

  void
  MyFoo_callback_exec_i::foo_excep (::CCM_AMI::ExceptionHolder * excep_holder)
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const OneReplyH::InternalError& ex)
      {
        HandleException (ex.id, cmd_asynch_foo_nok,
                         ex.error_string.in(), "asynch foo");
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Caught unexpected exception:");
      }
  }

  void
  MyFoo_callback_exec_i::bar (
     const char * /*answer*/, CORBA::Long l_cmd)
  {
    if ( l_cmd == cmd_asynch_bar_ok )
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::bar_excep (::CCM_AMI::ExceptionHolder * excep_holder)
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const OneReplyH::InternalError& ex)
      {
        HandleException (ex.id, cmd_asynch_bar_nok,
                         ex.error_string.in(), "asynch bar");
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Caught unexpected exception:");
      }
  }
  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::OneReplyH::AMI4CCM_MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::OneReplyH::AMI4CCM_MyFoo::_duplicate (my_foo_ami))
  {
    cb_ =  new MyFoo_callback_exec_i ();
  }

  int asynch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    if (CORBA::is_nil (my_foo_ami_))
      {
        ACE_ERROR ((LM_ERROR, "ERROR Sender (ASYNCH) :\tfoo_ami is NIL !\n"));
        return 1;
      }
    else
      {
        //Invoke Asynchronous calls ,test with  one replyhandler.
        my_foo_ami_->sendc_foo ( this->cb_.in(), "Hi from foo 1",
                                cmd_asynch_foo_ok);
        //this should invoke a exception
        my_foo_ami_->sendc_foo (  this->cb_.in(), "", cmd_asynch_foo_nok);

        my_foo_ami_->sendc_foo ( this->cb_.in(), "Hi from foo 2",
                                cmd_asynch_foo_ok);
        my_foo_ami_->sendc_bar ( this->cb_.in(), "Hi from bar",
                                cmd_asynch_bar_ok);
        my_foo_ami_->sendc_foo ( this->cb_.in(), "Hi from foo 3",
                                cmd_asynch_foo_ok);

         //this should invoke a exception
        my_foo_ami_->sendc_bar (  this->cb_.in(), "", cmd_asynch_bar_nok);
    }
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (
    ::OneReplyH::MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::OneReplyH::MyFoo::_duplicate (my_foo_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    //run some synch calls
    CORBA::String_var out_str;
    CORBA::Long l_cmd = 0;
    try
      {
        CORBA::Long result = my_foo_ami_->foo ("Do something synchronous",
                                                cmd_synch_foo_ok ,
                                                out_str.out ());
        if ( result == cmd_synch_foo_ok)
          {
            ++nr_of_received;
          }
      }
    catch (const OneReplyH::InternalError&)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::foo: "
                              "Unexpected exception.\n"));
      }
    try
      {
        my_foo_ami_->foo ("",cmd_synch_foo_nok, out_str);
      }
    catch (const OneReplyH::InternalError& ex)
      {
          HandleException (ex.id, cmd_synch_foo_nok, ex.error_string.in(),
                           "synch foo");
      }
    try
      {
        my_foo_ami_->bar ("Do something synchronous",
                           cmd_synch_bar_ok,
                           out_str, l_cmd);
        if ( l_cmd == cmd_synch_bar_ok)
          {
            ++nr_of_received;
          }
      }
    catch (const OneReplyH::InternalError&)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::bar: "
                              "Unexpected exception.\n"));
      }
    try
      {
        my_foo_ami_->bar ( "", cmd_synch_bar_nok, out_str, l_cmd);
      }
    catch (const OneReplyH::InternalError& ex)
      {
          HandleException (ex.id, cmd_synch_bar_nok, ex.error_string.in(),
                           "synch bar");
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
  // Operations from Components::SessionComponent.
  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::OneReplyH::CCM_Sender_Context::_narrow (ctx);
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
    ::OneReplyH::AMI4CCM_MyFoo_var asynch_foo =
      this->context_->get_connection_sendc_run_my_foo();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::OneReplyH::MyFoo_var synch_foo =
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
    if (nr_of_excep_received != 4)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: not received the expected number of"
                              " exceptions"
                              "Expected: 4, Received: %u.\n",
                              nr_of_excep_received));
      }
    if (nr_of_received != 6)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Sender not received the expected number"
                              " of callbacks and returns  for syn- and "
                              "asynchronous calls. Expected: 6,"
                              " Received: %u.\n",
                              nr_of_excep_received));
      }
    if ((nr_of_received == 6) && (nr_of_excep_received == 4))
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Sender received the expected number of"
                              " callbacks and exceptions for syn- and "
                              "asynchronous calls\n"));
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_OneReplyH_AMI_Sender_Impl (void)
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
