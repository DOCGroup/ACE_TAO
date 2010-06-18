// -*- C++ -*-
// $Id$

// test use multiple and simplex porttypes side by side.
#include "UsesSM_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_UsesSM_Sender_Impl
{
  // Keeps the number of messages received back.
  Atomic_UShort nr_of_received = 0;
  // Keeps the number of messages on the go.
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
  // Facet Executor Implementation Class: Two_callback_exec_i
  //============================================================
  Two_callback_exec_i::Two_callback_exec_i (void)
  {
  }

  Two_callback_exec_i::~Two_callback_exec_i (void)
  {
  }
  //============================================================
  // Operations from ::CCM_AMI::Two_callback
  //============================================================
  void
  Two_callback_exec_i::bar (const char * answer)
  {
     ACE_DEBUG ((LM_DEBUG, "Sender: Get asynchroon callback from bar,"
                           " answer = <%C>\n",
                           answer));
     ++nr_of_received;
  }

  void
  Two_callback_exec_i::bar_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
        excep_holder->raise_exception ();
  }
  //============================================================
  // Worker thread for asynchronous invocations for One (multiple)
  // and Two (simplex)
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::UsesSM::Sender::sendc_run_my_um_oneConnections_var my_one_ami,
    ::UsesSM::AMI4CCM_Two_ptr my_two_ami)
   : my_one_ami_ (my_one_ami),
     my_two_ami_ (::UsesSM::AMI4CCM_Two::_duplicate (my_two_ami))
  {
  }

  int asynch_foo_generator::svc ()
  {
    if (my_one_ami_->length () == 0)
      {
        ACE_ERROR ((LM_ERROR,
                   "ERROR Sender (ASYNCH) :my_one_ami_ is NIL !\n"));
        return 1;
      }
    if (CORBA::is_nil (my_two_ami_))
      {
        ACE_ERROR ((LM_ERROR,
                   "ERROR Sender (ASYNCH) :my_two_ami_ is NIL !\n"));
        return 1;
      }
    // Invoke Asynchronous calls to test
    for (CORBA::ULong i = 0; i < my_one_ami_->length (); ++i)
      {
        CORBA::String_var test;
        switch (i)
          {
            case 0:
              test = CORBA::string_dup ("Asynch. foo call one.");
              break;
            case 1:
              test = CORBA::string_dup ("Asynch. foo call two");
              break;
            case 2:
              test = CORBA::string_dup ("Asynch. foo call three");
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
        // There is more than 1 message sent, without receiving callbacks,
        // so it is asynchronous
        if (nr_of_sent.value() > 1)
          {
            asynch = true;
          }
        ACE_DEBUG ((LM_DEBUG,
                    "Sender (ASYNCH) : send asynch call bar <%u>!\n", i));
        my_two_ami_->sendc_bar ( new Two_callback_exec_i (), i);
    }
    return 0;
  }
  //============================================================
  // Worker thread for synchronous invocations for One
  //============================================================
  synch_foo_generator::synch_foo_generator (
   ::UsesSM::Sender::run_my_um_oneConnections_var my_one_ami)
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
      ::UsesSM::CCM_Sender_Context::_narrow (ctx);
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
    // multiple
    ::UsesSM::Sender::sendc_run_my_um_oneConnections_var asynch_foo =
    this->context_->get_connections_sendc_run_my_um_one();
    // simplex 
    ::UsesSM::AMI4CCM_Two_var asynch_two =
      this->context_->get_connection_sendc_run_my_two();

    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo, asynch_two);
     asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::UsesSM::Sender::run_my_um_oneConnections_var synch_foo =
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
    if (nr_of_received.value() != 9)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Did not get all callbacks from "
                              "multiple ports."
                              " Received = %u of 9\n",
                              nr_of_received.value()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG,
                    "OK: All messages received back by Sender\n"));
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_UsesSM_AMI_Sender_Impl (void)
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
