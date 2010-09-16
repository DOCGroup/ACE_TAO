// -*- C++ -*-
// $Id$

// test 3 components: Master <-> Sender <-> Receiver
// Sender uses and provides ports.
#include "ThreeComp_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_ThreeComp_Sender_Impl
{
  // Keeps the messages on the go. 
  Atomic_UShort nr_of_sent = 0;
  // Keeps the messages received back from Receiver.
  Atomic_UShort nr_of_rec = 0;
  CORBA::Boolean asynch = false;
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
  // for Receiver-Sender callback's
  void
  MyFoo_callback_exec_i::foo (
      CORBA::Long ami_ret_val,
      const char * answer)
  {
    ++nr_of_rec;
    if (ami_ret_val == 1 )
      {
        --nr_of_sent;
        ACE_DEBUG ((LM_DEBUG, "OK: Sender get ASYNCHRONOUS callback from Receiver: <%C>.\n",
                    answer));
      }
   }

  void
  MyFoo_callback_exec_i::foo_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    excep_holder->raise_exception ();
  }

  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
   ::ThreeComp::CCM_Sender_Context_ptr context)
  : context_(::ThreeComp::CCM_Sender_Context::_duplicate (context))
  {
  }

  int asynch_foo_generator::svc ()
  {
    ::ThreeComp::Sender::sendc_run_my_fooConnections_var  my_foo_ami_ =
       context_->get_connections_sendc_run_my_foo();
    ::ThreeComp::CCM_AMI4CCM_MyFooReplyHandler_var cb =
                   new MyFoo_callback_exec_i ();

     // Invoke Asynchronous calls to test
     for (CORBA::ULong i = 0; i < my_foo_ami_->length (); ++i)
       {
         ++nr_of_sent;
         my_foo_ami_[i].objref->sendc_foo (cb.in(),
                                           10 );
         ACE_DEBUG ((LM_DEBUG, "OK Sender send ASYNCHRONOUS call to Receiver.\n"));
        // There is more than 1 message sent, without receiving callbacks,
        // so it is asynchronous
        if (nr_of_sent.value() > 1)
          {
            asynch = true;
          }
       }
    return 0;
  }
  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (
   ::ThreeComp::CCM_Sender_Context_ptr context)
  : context_(::ThreeComp::CCM_Sender_Context::_duplicate (context))
  {
  }

  int synch_foo_generator::svc ()
  {

    ::ThreeComp::Sender::run_my_fooConnections_var my_foo_ami_ =
          context_->get_connections_run_my_foo ();

    CORBA::Boolean wait = false;

    for(CORBA::ULong i = 0; i < my_foo_ami_->length(); ++i)
       {
         CORBA::String_var answer;
         try
           {
             if ( wait==true)
               {
                  ACE_ERROR ((LM_ERROR,
                               "ERROR: Sender didn't receive SYNCHRONOUS answer"
                               " from Receiver.\n"));
                }
             wait = true;
             ACE_DEBUG ((LM_DEBUG, "OK Sender send SYNCHRONOUS CALL to Receiver.\n"));

             CORBA::ULong result = my_foo_ami_[i].objref->foo( 20,
                                       answer.out ());
             if (result == 2)
               {
                 ACE_DEBUG ((LM_DEBUG, "OK Sender received SYNCHRONOUS answer "
                                       "from Receiver <%C>\n",
                                       answer.in ()));
                 ++nr_of_rec;
                 wait = false;
               }
             }
         catch (const ThreeComp::InternalError&)
           {
             ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::foo: "
                                "Unexpected exception.\n"));
           }
       }
    return 0;
  }
  //============================================================
  // Receptacle Executor Implementation Class: State_exec_i
  //============================================================
  // for Master-Sender interactions
  State_exec_i::State_exec_i (void)
  {
  }

  State_exec_i::~State_exec_i (void)
  {
  }

  ::CORBA::Long
  State_exec_i::bar (::CORBA::Long cmd,
                       ::CORBA::String_out answer)
  {
    ACE_OS::sleep(2);
    answer = CORBA::string_dup ("Hi from sender.");
    return cmd;
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

  //for connection with Master component
  ::ThreeComp::CCM_State_ptr
  Sender_exec_i::get_do_my_state (void)
    {
      return new State_exec_i ();
    }

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::ThreeComp::CCM_Sender_Context::_narrow (ctx);
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
    //for connection with Receiver component
    asynch_foo_generator* asynch_foo_gen =
      new asynch_foo_generator (this->context_.in ());
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    synch_foo_generator* synch_foo_gen =
       new synch_foo_generator (this->context_.in());
    synch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    // each Receiver (total 3, see deployment plan) send back 2 requests.
    if ((asynch == false) || (nr_of_rec.value() != 6) ||
        (nr_of_sent.value() != 0))
      {
        ACE_ERROR ((LM_ERROR, 
                   "ERROR: Test not asynchronous or not received back"
                   " all requests ( %u/6)!\n",
                   nr_of_rec.value() ));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Sender works as expected.\n"));
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_ThreeComp_AMI_Sender_Impl (void)
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
