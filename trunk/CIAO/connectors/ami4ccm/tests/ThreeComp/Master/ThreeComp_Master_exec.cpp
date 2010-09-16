// -*- C++ -*-
// $Id$

// test 3 components: Master <-> Sender <-> Receiver
#include "ThreeComp_Master_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_ThreeComp_Master_Impl
{
  // Keeps the messages on the go. 
  Atomic_UShort nr_of_sent = 0;
  // Keeps the messages received back from the Sender.
  Atomic_UShort nr_of_rec = 0;
  CORBA::Boolean asynch = false;
  //============================================================
  // Facet Executor Implementation Class: State_callback_exec_i
  //============================================================
  State_callback_exec_i::State_callback_exec_i (void)
  {
  }

  State_callback_exec_i::~State_callback_exec_i (void)
  {
  }
  //============================================================
  // Operations from ::CCM_AMI::State_callback
  //============================================================
  void
  State_callback_exec_i::bar (::CORBA::Long ami_return_val,
      const char * answer)
  {
    if (ami_return_val == 1)
      {
        ++nr_of_rec;
        --nr_of_sent;
        ACE_DEBUG ((LM_DEBUG, "OK: Master get ASYNCHRONOUS callback "
                   "from Sender: <%C>.\n",
                   answer));
      }
  }

  void
  State_callback_exec_i::bar_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    excep_holder->raise_exception ();
  }

  //============================================================
  // Worker thread for asynchronous invocations for State
  //============================================================
  asynch_state_generator::asynch_state_generator (
   ::ThreeComp::CCM_Master_Context_ptr context)
  : context_(::ThreeComp::CCM_Master_Context::_duplicate (context))
  {
  }

  int asynch_state_generator::svc ()
  {
    ::ThreeComp::AMI4CCM_State_var my_state_ami_  =
       context_->get_connection_sendc_run_my_state();

    ACE_OS::sleep(2);
    ::ThreeComp::CCM_AMI4CCM_StateReplyHandler_var cb =
                  new State_callback_exec_i ();
    if (CORBA::is_nil (my_state_ami_))
      {
         ACE_ERROR ((LM_ERROR, "ERROR Master (ASYNCH) :"
                             "\tstate_ami is NIL !\n"));
         return 1;
      }
    //Invoke Asynchronous calls to test 

    for (int i = 0; i < 3; i ++)
      {
        ++nr_of_sent;
        my_state_ami_->sendc_bar ( cb.in(), 1);
        ACE_DEBUG ((LM_DEBUG, "OK: Master sends  ASYNCHRONOUS call to Sender.\n"));
      }
    //there is more than 1 message sent, without receiving callbacks,
    //so it is asynchronous
    if (nr_of_sent.value() > 1)
      {
        asynch = true;
      }
    return 0;
  }
  //============================================================
  // Worker thread for synchronous invocations for State
  //============================================================
  synch_state_generator::synch_state_generator (
   ::ThreeComp::CCM_Master_Context_ptr context)
  : context_(::ThreeComp::CCM_Master_Context::_duplicate (context))
  {
  }

  int synch_state_generator::svc ()
  {
    ::ThreeComp::State_var my_state_ami_ =
         context_->get_connection_run_my_state ();

    ACE_OS::sleep(1);
    CORBA::Boolean wait = false;
    for (int i = 0; i < 3; ++i)
      {
        // Run some synch calls, answer has to come before the next step.
         CORBA::String_var answer;
         ::CORBA::Long ret = 0;
         try
           {
             if ( wait==true)
               {
                 ACE_ERROR ((LM_ERROR,
                             "ERROR: NOT RECEIVED SYNCHRONOUS answer.\n"));
               }
             wait = true;
             ACE_DEBUG ((LM_DEBUG, "OK: Master send SYNCHRONOUS call to Sender.\n"));
             ret = my_state_ami_->bar (2,answer.out());
             if (ret == 2)
               {
                 ACE_DEBUG ((LM_DEBUG, "OK: Master received SYNCHRONOUS answer from Sender <%C>\n",
                                       answer.in ()));
                 wait = false;
               }
            }
          catch (const ThreeComp::InternalError&)
            {
              ACE_ERROR ((LM_ERROR, "ERROR: synch_state_generator::state: "
                                    "Unexpected exception.\n"));
            }
       }
    return 0;
  }
  //============================================================
  // Component Executor Implementation Class: Master_exec_i
  //============================================================
  Master_exec_i::Master_exec_i (void)
  {
  }

  Master_exec_i::~Master_exec_i (void)
  {
  }

  // Supported operations and attributes.
  // Component attributes.
  // Operations from Components::SessionComponent.
  void
  Master_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::ThreeComp::CCM_Master_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Master_exec_i::configuration_complete (void)
  {
  }

  void
  Master_exec_i::ccm_activate (void)
  {
    asynch_state_generator* asynch_state_gen =
      new asynch_state_generator (this->context_.in ());
    asynch_state_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    synch_state_generator* synch_state_gen =
       new synch_state_generator (this->context_.in());
    synch_state_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);
  }

  void
  Master_exec_i::ccm_passivate (void)
  {
  }

  void
  Master_exec_i::ccm_remove (void)
  {
    if ((asynch == false) || (nr_of_rec.value() != 3))
      {
        ACE_ERROR ((LM_ERROR, 
                   "ERROR: Test not asynchronous or not received back"
                   " all requests ( %u/3)!\n",
                   nr_of_rec.value() ));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Test Master.\n"));
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_ThreeComp_AMI_Master_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_RETURN (
      retval,
      Master_exec_i,
      ::Components::EnterpriseComponent::_nil ());

    return retval;
  }
}
