// -*- C++ -*-
// $Id$

// test use multiple porttypes.
#include "UsesMulti_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_UsesMulti_Sender_Impl
{
  // Keeps the messages received back.
  Atomic_UShort nr_of_received = 0;
  // Keeps the messages on the go.
  Atomic_UShort nr_of_sent = 0;
  CORBA::Boolean asynch = false;
  //============================================================
  // Facet Executor Implementation Class: One_callback_exec_i
  //============================================================
  One_callback_exec_i::One_callback_exec_i (void)
  {
  }

  One_callback_exec_i::~One_callback_exec_i (void)
  {
  }
  //============================================================
  // Operations from ::CCM_AMI::One_callback
  //============================================================
  // FOO methods
  void
  One_callback_exec_i::foo (
    ::CORBA::Long /*ami_return_val*/,
    const char * answer)
  {
    ACE_DEBUG ((LM_DEBUG, "Sender: Get asynchroon callback from foo,"
                           " answer = <%C>\n",
                           answer));
     ++nr_of_received;
     --nr_of_sent;
  }

  void
  One_callback_exec_i::foo_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    excep_holder->raise_exception ();
  }
  //============================================================
  // Worker thread for asynchronous invocations for One
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::UsesMulti::Sender::sendc_run_my_um_oneConnections_var my_one_ami)
   : my_one_ami_ (my_one_ami)
  {
  }

  int asynch_foo_generator::svc ()
  {
    //Invoke Asynchronous calls to test
    for (CORBA::ULong i = 0; i < my_one_ami_->length (); ++i)
      {
        CORBA::String_var test;
        switch (i)
          {
            case 0:
              test = CORBA::string_dup ("Asynch. call one.");
              break;
            case 1:
              test = CORBA::string_dup ("Asynch. call two");
              break;
            case 2:
              test = CORBA::string_dup ("Asynch. call three");
              break;
            default:
              break;
          }
        ++nr_of_sent;
        my_one_ami_[i].objref->sendc_foo (new One_callback_exec_i (),
                                          test.in (),
                                          i); 
        ACE_DEBUG ((LM_DEBUG, "Sender (ASYNCH) : send <%C> !\n",
                    test.in ()));
        //there is more than 1 message sent, without receiving callbacks,
        //so it is asynchronous
        if (nr_of_sent.value() > 1)
          {
            asynch = true;
          }
      }
    return 0;
  }
  //============================================================
  // Worker thread for synchronous invocations for One
  //============================================================
  synch_foo_generator::synch_foo_generator (
   ::UsesMulti::Sender::run_my_um_oneConnections_var my_one_ami)
   : my_one_ami_ (my_one_ami)
  {
  }

  int synch_foo_generator::svc ()
  {
    for(CORBA::ULong i = 0; i < my_one_ami_->length(); ++i)
      {
        CORBA::String_var test;
        switch (i)
          {
            case 0:
              test = CORBA::string_dup ("Synch. call 0.");
              break;
            case 1:
              test = CORBA::string_dup ("Synch. call 1");
              break;
            case 2:
              test = CORBA::string_dup ("Synch. call 2");
              break;
            default:
              break;
          }

        ACE_DEBUG ((LM_DEBUG,"Sender (SYNCH) : send <%C> !\n",
                    test.in ()));

        CORBA::String_var answer;
        CORBA::ULong result = my_one_ami_[i].objref->foo( test,
                              i,
                              answer.out ());
        if (result != i)
          {
            ACE_ERROR ((LM_ERROR, 
                        "ERROR Sender (SYNCH) : CLASS One foo !\n"));
          }
        else
          { 
            ++nr_of_received;
            ACE_DEBUG ((LM_DEBUG,
                        "Sender (SYNCH) : received answer = <%C> !\n",
                        answer.in ()));
          }
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
      ::UsesMulti::CCM_Sender_Context::_narrow (ctx);
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
    ::UsesMulti::Sender::sendc_run_my_um_oneConnections_var asynch_foo =
    this->context_->get_connections_sendc_run_my_um_one();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
     asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::UsesMulti::Sender::run_my_um_oneConnections_var synch_foo =
         this->context_->get_connections_run_my_um_one ();
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
    if (asynch == false)
      {
        ACE_ERROR ((LM_ERROR,
                   "ERROR: Uses multiple test not asynchronous!\n"));
      }
    if (nr_of_received.value() != 6)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Did not get all callbacks from "
                              "multiple ports."
                              " Received = %u of 6\n",
                              nr_of_received.value()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    "OK: All messages received back by Sender\n"));
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_UsesMulti_AMI_Sender_Impl (void)
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
