// -*- C++ -*-
// $Id$

// test use multiple porttypes.
#include "UsesMulti_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_UsesMulti_Sender_Impl
{
  Atomic_UShort nr_of_received = 0;
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
     ACE_DEBUG ((LM_DEBUG, "OK: Get asynchroon callback from ONE::foo,"
                           " answer = <%C>\n",
                           answer));  
     nr_of_received++;
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
              test = CORBA::string_dup ("Asynchronous call een.");
              break;
            case 1:
              test = CORBA::string_dup ("Asynchronous call twee");
              break;
            case 2:
              test = CORBA::string_dup ("Asynchronous call drie");
              break;
            default:
              break;
          }

        /// Don't we have to get hold of a POA and create an object
        /// reference for the reply handler? Or is that handled 
        /// under the hood in AMI4CCM? If so, we may not need to
        /// create the reply handler servant on the heap.
        my_one_ami_[i].objref->sendc_foo (new One_callback_exec_i (),
                                          test.in (),
                                          i);                                     
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
        char *test = 0;
        if ( i == 0)
          test = CORBA::string_dup("Synchronous call een.");
        if ( i == 1)
          test = CORBA::string_dup("Synchronous call twee");
        if ( i == 2)
          test = CORBA::string_dup("Synchronous call drie");

        char *answer = 0;
        CORBA::ULong result = my_one_ami_[i].objref->foo( test,
                              i,
                              answer);
        if (result != i)
          {
            ACE_ERROR ((LM_ERROR, 
                        "ERROR Sender (SYNCH) : CLASS One foo !\n"));
          }
        else
          { 
            nr_of_received++;
            ACE_DEBUG ((LM_DEBUG, 
                        "Sender (SYNCH) : received answer = <%C> !\n", answer));
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
    //Expected somehing like this with use multiple asynchroon:
    ::UsesMulti::Sender::sendc_run_my_um_oneConnections_var asynch_foo =
    this->context_->get_connections_sendc_run_my_um_one();
 
    //this isn't use multiple asynchroon
    //::UsesMulti::AMI4CCM_One_var asynch_foo =
      //this->context_->get_connection_sendc_run_my_um_one();
     asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
     asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    //this is 'use multiple' synchroon.
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
