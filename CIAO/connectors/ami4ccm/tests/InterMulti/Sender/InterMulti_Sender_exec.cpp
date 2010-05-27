// -*- C++ -*-
// $Id$

// test sendc methods with derived classes, multiple inheritance.
#include "InterMulti_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_InterMulti_Sender_Impl
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
  // FOO methods
  void
  Two_callback_exec_i::bar (const char * answer)
  {
     ACE_DEBUG ((LM_DEBUG, "OK: Get asynchroon callback from TWO::bar,"
                           " answer = <%C>\n",
                           answer));  
     nr_of_received++;
  }

  void
  Two_callback_exec_i::bar_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
        excep_holder->raise_exception ();
  }
   //============================================================
  // Facet Executor Implementation Class: Three_callback_exec_i
  //============================================================
  Three_callback_exec_i::Three_callback_exec_i (void)
  {
  }

  Three_callback_exec_i::~Three_callback_exec_i (void)
  {
  }

  //============================================================
  // Operations from ::CCM_AMI::Three_callback
  //============================================================
  // FOO methods
  void
  Three_callback_exec_i::plus (const char * answer)
  {
    ACE_DEBUG ((LM_DEBUG, "OK: Get asynchroon callback from THREE::plus,"
                          " answer = <%C>\n",
                          answer));  
     nr_of_received++;
  }

  void
  Three_callback_exec_i::plus_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
        excep_holder->raise_exception ();
  }

  void
  Three_callback_exec_i::foo (
    ::CORBA::Long /*ami_return_val*/,
    const char * answer)
  {
    ACE_DEBUG ((LM_DEBUG, "OK: Get asynchroon callback from THREE::foo,"
                          " answer = <%C>\n",
                          answer));  
    nr_of_received++;

  }

  void
  Three_callback_exec_i::foo_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
        excep_holder->raise_exception ();
  }

  void
  Three_callback_exec_i::bar (const char * answer)
  {
    ACE_DEBUG ((LM_DEBUG, "OK: Get asynchroon callback from THREE::bar,"
                          " answer = <%C>\n",
                          answer));  
     nr_of_received++;
  }

  void
  Three_callback_exec_i::bar_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
        excep_holder->raise_exception ();
  }
  //============================================================
  // Worker thread for asynchronous invocations for One
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::InterMulti::AMI4CCM_One_ptr my_one_ami)
  : my_one_ami_ (::InterMulti::AMI4CCM_One::_duplicate (my_one_ami))
  {
  }

  int asynch_foo_generator::svc ()
  {
    //Invoke Asynchronous calls to test 
    my_one_ami_->sendc_foo ( new One_callback_exec_i (),
      "Hi from asynch call ONE::foo", 1);
    return 0;
  }

//============================================================
  // Worker thread for asynchronous invocations for Two
  //============================================================
  asynch_two_generator::asynch_two_generator (
    ::InterMulti::AMI4CCM_Two_ptr my_two_ami)
  : my_two_ami_ (::InterMulti::AMI4CCM_Two::_duplicate (my_two_ami))
  {
  }

  int asynch_two_generator::svc ()
  {
    if (CORBA::is_nil (my_two_ami_))
      {
        ACE_ERROR ((LM_ERROR, 
                   "ERROR Sender (ASYNCH) :my_two_ami_ is NIL !\n"));  
        return 1;
      }
    else
      {
        //Invoke Asynchronous calls to test 
        my_two_ami_->sendc_bar ( new Two_callback_exec_i (), 2);
      }
    return 0;
  }

  //============================================================
  // Worker thread for asynchronous invocations for Three
  //============================================================
  asynch_three_generator::asynch_three_generator (
    ::InterMulti::AMI4CCM_Three_ptr my_three_ami)
  : my_three_ami_ (::InterMulti::AMI4CCM_Three::_duplicate (my_three_ami))
  {
  }

  int asynch_three_generator::svc ()
  {
    if (CORBA::is_nil (my_three_ami_))
      {
        ACE_ERROR ((LM_ERROR, 
                    "ERROR Sender (ASYNCH) :my_three_ami_ is NIL !\n"));  
        return 1;
      }
    else
      {
        //Invoke Asynchronous calls to test 
        my_three_ami_->sendc_plus( new Three_callback_exec_i (), 3);
        // derived from CLASS ONE 
        my_three_ami_->sendc_foo ( new Three_callback_exec_i (),"hoi", 3);
        // derived fron CLASS TWO
        my_three_ami_->sendc_bar ( new Three_callback_exec_i (), 3);
      }
    return 0;
  }
  //============================================================
  // Worker thread for synchronous invocations for One
  //============================================================
  synch_foo_generator::synch_foo_generator (
    ::InterMulti::One_ptr my_one_ami)
  : my_one_ami_ (::InterMulti::One::_duplicate (my_one_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
    char *answer = 0;
    CORBA::Long result = my_one_ami_->foo (
                            "Synchronous call foo from class One",
                            1,
                            answer);
    if (result != 1)
      {
        ACE_ERROR ((LM_ERROR, 
                    "ERROR Sender (SYNCH) : CLASS One foo !\n"));
      }
    return 0;
  }
 //============================================================
  // Worker thread for synchronous invocations for One
  //============================================================
  synch_three_generator::synch_three_generator (
    ::InterMulti::Three_ptr my_three_ami)
  : my_three_ami_ (::InterMulti::Three::_duplicate (my_three_ami))
  {
  }

  int synch_three_generator::svc ()
  {
    char *answer = 0;
    CORBA::Long result = my_three_ami_->foo (
                        "Synchronous call foo from class Three, "
                        " derived from class ONE",
                        3 ,
                        answer);
    if (result != 3)
      {
        ACE_ERROR ((LM_ERROR, 
                    "ERROR Sender (SYNCH) : CLASS Three foo !\n"));
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
      ::InterMulti::CCM_Sender_Context::_narrow (ctx);
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
    ::InterMulti::AMI4CCM_One_var asynch_foo =
      this->context_->get_connection_sendc_run_my_one();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

       ::InterMulti::AMI4CCM_Two_var asynch_two =
      this->context_->get_connection_sendc_run_my_two();
    asynch_two_generator* asynch_two_gen =
        new asynch_two_generator (asynch_two);
    asynch_two_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

       ::InterMulti::AMI4CCM_Three_var asynch_three =
      this->context_->get_connection_sendc_run_my_three();
    asynch_three_generator* asynch_three_gen =
        new asynch_three_generator (asynch_three);
    asynch_three_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);


    ::InterMulti::One_var synch_foo =
        this->context_->get_connection_run_my_one ();
    synch_foo_generator* synch_foo_gen =
      new synch_foo_generator (synch_foo);
    synch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

      ::InterMulti::Three_var synch_three  =
        this->context_->get_connection_run_my_three ();
    synch_three_generator* synch_three_gen =
      new synch_three_generator (synch_three);
    synch_three_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if (nr_of_received.value() != 5)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Did not get all callbacks for"
                              "derived classes."
                              " Received = %u of 5\n",
                              nr_of_received.value()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, 
                    "OK: All messages received back by Sender\n"));  
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_InterMulti_AMI_Sender_Impl (void)
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
