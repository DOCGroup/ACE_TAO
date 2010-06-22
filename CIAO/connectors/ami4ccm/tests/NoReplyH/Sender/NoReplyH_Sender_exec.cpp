// -*- C++ -*-
// $Id$

#include "NoReplyH_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_NoReplyH_Sender_Impl
{
  CORBA::UShort nr_received_back = 0;

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
    const char * answer)
  {
    ++nr_received_back;
    ACE_DEBUG ((LM_DEBUG, "OK: ASYNCH foo() returns answer: %C.\n",
                           answer));
  }

  void
  MyFoo_callback_exec_i::foo_excep (
      ::CCM_AMI::ExceptionHolder * /*excep_holder*/)
  {
    // never should come here in this test.
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::foo_excep: "
                           "Unexpected return.\n"));
  }

  // HELLO methods
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
      ::CCM_AMI::ExceptionHolder * /*excep_holder*/)
  {
    // never should come here in this test.
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::hello_excep: "
                           "Unexpected return.\n"));
  }

  //GET rw_attrib Reply Handler
  void
  MyFoo_callback_exec_i::get_rw_attrib (
    ::CORBA::Short /*ami_return_val*/)
  {
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::get_rw_attrib: "
                           "Unexpected return.\n"));
  }

  void
  MyFoo_callback_exec_i::get_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * /*excep_holder*/)
  {
    // never should come here in this test.
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::get_rw_attrib_excep: "
                           "Unexpected return.\n"));
  }

  //SET rw_attrib Reply Handler
  void
  MyFoo_callback_exec_i::set_rw_attrib ()
  {
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::set_rw_attrib: "
                           "Unexpected return.\n"));
  }

  void
  MyFoo_callback_exec_i::set_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * /*excep_holder*/)
  {
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::set_rw_attrib_excep: "
                           "Unexpected return.\n"));
  }

  //ro_attrib Reply Handler
  void
  MyFoo_callback_exec_i::get_ro_attrib (
    ::CORBA::Short /*ami_return_val*/)
  {
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::get_ro_attrib: "
                           "Unexpected return.\n"));
  }

  void
  MyFoo_callback_exec_i::get_ro_attrib_excep (
    ::CCM_AMI::ExceptionHolder * /*excep_holder*/)
  {
    ACE_ERROR ((LM_ERROR, "ERROR: MyFoo_callback_exec_i::get_ro_attrib_excep: "
                           "Unexpected return.\n"));
  }

  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::NoReplyH::AMI4CCM_MyFoo_ptr my_foo_ami)
    : my_foo_ami_ (::NoReplyH::AMI4CCM_MyFoo::_duplicate (my_foo_ami))
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
        //Invoke Asynchronous calls
        //test with  replyhandler.
        my_foo_ami_->sendc_foo ( new MyFoo_callback_exec_i (),
                                 "Do something asynchronous");
        
        //test without replyhandlers.
        my_foo_ami_->sendc_foo ( NoReplyH::AMI4CCM_MyFooReplyHandler::_nil (),
                          "Do something asynchronous with no replyhandler");
        my_foo_ami_->sendc_hello ( NoReplyH::AMI4CCM_MyFooReplyHandler::_nil ());
        my_foo_ami_->sendc_get_rw_attrib (NoReplyH::AMI4CCM_MyFooReplyHandler::_nil ());
        my_foo_ami_->sendc_set_rw_attrib (NoReplyH::AMI4CCM_MyFooReplyHandler::_nil (),
                                          15);
        my_foo_ami_->sendc_get_ro_attrib (NoReplyH::AMI4CCM_MyFooReplyHandler::_nil ());
      }
      //Invoke Asynchronous calls to test exception handling 
      //without replyhandlers.
      my_foo_ami_->sendc_foo ( NoReplyH::AMI4CCM_MyFooReplyHandler::_nil (), "");
      my_foo_ami_->sendc_set_rw_attrib (NoReplyH::AMI4CCM_MyFooReplyHandler::_nil (),
                                        0);
 
      return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (::NoReplyH::MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::NoReplyH::MyFoo::_duplicate (my_foo_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
     ACE_OS::sleep (3);
    //run some synch calls
    try 
      {
        CORBA::Short rw_attrib = my_foo_ami_->rw_attrib ();
        ACE_DEBUG ((LM_DEBUG, "OK: SYNCH rw_attrib() returns %u.\n",
                              rw_attrib));
      }
    catch (const NoReplyH::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, "ERROR: SYNCH rw_attrib(): "
                              "Unexpected return.\n"));
      }
    try 
      {
        CORBA::Short ro_attrib = my_foo_ami_->ro_attrib ();
        ACE_DEBUG ((LM_DEBUG, "OK: SYNCH ro_attrib() returns %u.\n",
                              ro_attrib));
      }
    catch (const NoReplyH::InternalError& )
      {
        ACE_ERROR ((LM_ERROR, "ERROR: SYNCH ro_attrib(): "
                              "Unexpected return.\n"));
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
      ::NoReplyH::CCM_Sender_Context::_narrow (ctx);
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
    ::NoReplyH::AMI4CCM_MyFoo_var asynch_foo =
      this->context_->get_connection_sendc_run_my_foo();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::NoReplyH::MyFoo_var synch_foo =
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
    if(nr_received_back != 1)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Sender doesn't received the expected "
                              "number of callbacks"
                              "Expected: 1, Received: %u.\n",
                              nr_received_back));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Sender received the expected number of "
                              "callbacks: %u.\n",
                              nr_received_back));
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_NoReplyH_AMI_Sender_Impl (void)
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
