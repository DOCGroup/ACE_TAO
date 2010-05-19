// -*- C++ -*-
// $Id$

#include "ExceptionT_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_ExceptionT_Sender_Impl
{
  CORBA::Short nr_of_received = 0;
  CORBA::Short nr_of_syn_received = 0;
  
  void HandleException (
      long id,
      const char* error_string,
      const char* test,
      const char* func)
    {
      if (id != 42)
      {
        ACE_ERROR ((LM_ERROR, "ERROR Sender: unexpected ID received in except "
                              "handler <%u> <%C> (%C)\n",
                               id, error_string,func));
        return;
      }
      if (ACE_OS::strcmp (test, "thrown by receiver") != 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR Sender (%s): unexpected string received in"
                              " except handler <%s>\n",
                              func, test));
        return;
      }
      ACE_DEBUG ((LM_DEBUG, "Sender: Caught correct exception <%u,"
                            "%C> for %C\n",
                            id, error_string, func));
      ++nr_of_received;
    }

  void HandleException (
      ::CCM_AMI::ExceptionHolder_ptr excep_holder,
      const char* func)
    {
      try
        {
          excep_holder->raise_exception ();
        }
      catch (const ExceptionT::InternalError& ex)
        {
           HandleException (ex.id, ex.error_string.in (), ex.test.in(), func);
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("ERROR: Caught unexpected exception:");
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
    ::CORBA::Long /*ami_return_val*/,
    const char * /*answer*/)
  {
    // never should come here in this test.
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::foo: "
                           "Unexpected return.\n"));
  }

  void
  MyFoo_callback_exec_i::foo_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "FOO (asyn)");
  }

  // Hello methods
  void
  MyFoo_callback_exec_i::hello (
    ::CORBA::Long /*answer*/)
  {
    // never should come here in this test.
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::hello: "
                           "Unexpected return.\n"));
  }

  void
  MyFoo_callback_exec_i::hello_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "HELLO (asyn)");
  }

  //GET rw_attrib Reply Handler
  void
  MyFoo_callback_exec_i::get_rw_attrib (
    ::CORBA::Short /*ami_return_val*/)
  {
   // never should come here in this test.
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::get_rw_attrib : "
                           "Unexpected return.\n"));
  }

  void
  MyFoo_callback_exec_i::get_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "GET RW_ATTRIB (asyn)");
  }

  //SET rw_attrib Reply Handler
  void
  MyFoo_callback_exec_i::set_rw_attrib ()
  {
   // never should come here in this test.
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::set_rw_attrib: "
                           "Unexpected return.\n"));
  }

  void
  MyFoo_callback_exec_i::set_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "SET_RW_ATTRIB (asyn)");
  }

  //ro_attrib Reply Handler
  void
  MyFoo_callback_exec_i::get_ro_attrib (
    ::CORBA::Short /*ami_return_val*/)
  {
   // never should come here in this test.
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::get_ro_attrib: "
                           "Unexpected return.\n"));
  }

  void
  MyFoo_callback_exec_i::get_ro_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    HandleException (excep_holder, "RO_ATTRIB (asyn)");
  }

  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::ExceptionT::AMI4CCM_MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::ExceptionT::AMI4CCM_MyFoo::_duplicate (my_foo_ami))
  {
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
        //Invoke Asynchronous calls to test exception handling
        //for this test, hello , get_ro_attrib and get_rw_attrib functions of
        //receiver always sent back a exception
        my_foo_ami_->sendc_hello (new MyFoo_callback_exec_i ());
        my_foo_ami_->sendc_get_rw_attrib(new MyFoo_callback_exec_i ());
        my_foo_ami_->sendc_get_ro_attrib(new MyFoo_callback_exec_i ());
        my_foo_ami_->sendc_foo (new MyFoo_callback_exec_i (), "");
        my_foo_ami_->sendc_set_rw_attrib(new MyFoo_callback_exec_i (), 0);
      }
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (::ExceptionT::MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::ExceptionT::MyFoo::_duplicate (my_foo_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    //run synch calls
    char * out_str = 0;
        try
          {
            my_foo_ami_->foo ("Do something synchronous",
                               out_str);
          } 
        catch (const ExceptionT::InternalError&)
          {
            ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::foo: "
                                  "Unexpected exception.\n"));  
          }
        try
          {
            my_foo_ami_->foo ("", out_str);
          }
        catch (const ExceptionT::InternalError& ex)
          {
            //expected exception
            HandleException (ex.id, ex.error_string.in (), ex.test.in(),
                             "foo (syn)");
            ++nr_of_syn_received;

          }
        try
          {
            CORBA::Long answer;
            my_foo_ami_->hello (answer);
          }
        catch (const ExceptionT::InternalError& ex)
          {
            //expected exception
            HandleException (ex.id, ex.error_string.in (), ex.test.in(),
                             "hello (syn)");
            ++nr_of_syn_received;
          }
        try 
          {
            my_foo_ami_->rw_attrib ();
          }
        catch (const ExceptionT::InternalError& ex)
          {
            //expected exception
            HandleException (ex.id, ex.error_string.in (), ex.test.in(),
                             "get_rw_attrib (syn)");
            ++nr_of_syn_received;
          }
        try 
          {
            my_foo_ami_->rw_attrib (0);
          }
        catch (const ExceptionT::InternalError& ex)
          {
            //expected exception
            HandleException (ex.id, ex.error_string.in (), ex.test.in(),
                             "set_rw_attrib (syn)");
            ++nr_of_syn_received;
          }
        try 
          {
            my_foo_ami_->ro_attrib ();
          }
        catch (const ExceptionT::InternalError& ex)
          {
            HandleException (ex.id, ex.error_string.in (), ex.test.in(),
                             "ro_attrib (syn)");
            ++nr_of_syn_received;
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
      ::ExceptionT::CCM_Sender_Context::_narrow (ctx);
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
    ::ExceptionT::AMI4CCM_MyFoo_var asynch_foo =
      this->context_->get_connection_sendc_run_my_foo();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::ExceptionT::MyFoo_var synch_foo =
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
    CORBA::Short nr_of_asyn = nr_of_received - nr_of_syn_received;
    if (nr_of_asyn != 5)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: not received the expected number of"
                              " exceptions for asynchronous calls"
                              "Expected: 5, Received: %u.\n",
                              nr_of_asyn));  
      }
    if (nr_of_syn_received != 5)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: not received the expected number of"
                              " exceptions for synchronous calls"
                              "Expected: 5, Received: %u.\n",
                              nr_of_syn_received));  
      }
    if ((nr_of_asyn == 5) && (nr_of_syn_received == 5))
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Receieved the expected number of"
                              " exceptions for  asynchronous and "
                              "synchronous calls\n"));  
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_ExceptionT_AMI_Sender_Impl (void)
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

