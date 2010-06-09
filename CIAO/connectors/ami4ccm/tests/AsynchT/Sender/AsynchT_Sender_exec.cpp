// -*- C++ -*-
// $Id$

// test asynchronous 
#include "AsynchT_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_AsynchT_Sender_Impl
{
  // Keeps the messages on the go. 
  Atomic_UShort nr_of_sent = 0;
  // Keeps the messages received back.
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
  // FOO methods
  void
  MyFoo_callback_exec_i::foo (
    ::CORBA::Long ami_return_val,
    const char * /*answer*/)
  {
    ++nr_of_rec;
    if (ami_return_val == 1)
      {
        ACE_DEBUG ((LM_DEBUG, "OK: GET ASYNCHRONOUS CALLBACK foo.\n"));
        --nr_of_sent;
      }
    if (ami_return_val == 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: GET ASYNCHRONOUS CALLBACK "
                              "FROM SYNCHRONOUS SENT MESSAGE\n"));
      }
  }

  void
  MyFoo_callback_exec_i::foo_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::bar (
     CORBA::Long l_cmd)
  {
    ++nr_of_rec;
    if (l_cmd == 1)
      {
        --nr_of_sent;
        ACE_DEBUG ((LM_DEBUG, "OK: GET ASYNCHRONOUS CALLBACK bar.\n"));
      }
    if (l_cmd == 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: GET ASYNCHRONOUS CALLBACK "
                              "FROM SYNCHRONOUS SENT MESSAGE\n"));
      }
  }

  void
  MyFoo_callback_exec_i::bar_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::get_rw_attrib (
    ::CORBA::Short /*ami_return_val*/)
  {
    ACE_DEBUG ((LM_DEBUG, "OK: GET ASYNCHRONOUS CALLBACK get_rw_attrib.\n"));
    ++nr_of_rec;
    --nr_of_sent;
  }

  void
  MyFoo_callback_exec_i::get_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::set_rw_attrib ()
  {
    ACE_DEBUG ((LM_DEBUG, "OK: GET ASYNCHRONOUS CALLBACK set_rw_attrib.\n"));
    ++nr_of_rec;
    --nr_of_sent;
  }

  void
  MyFoo_callback_exec_i::set_rw_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::get_ro_attrib (
    ::CORBA::Short /*ami_return_val*/)
  {
    ACE_DEBUG ((LM_DEBUG, "OK: GET ASYNCHRONOUS CALLBACK get_ro_attrib.\n"));
    ++nr_of_rec;
    --nr_of_sent;
  }

  void
  MyFoo_callback_exec_i::get_ro_attrib_excep (
    ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    excep_holder->raise_exception ();
  }
  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::AsynchT::AMI4CCM_MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::AsynchT::AMI4CCM_MyFoo::_duplicate (my_foo_ami))
  {
  }

  int asynch_foo_generator::svc ()
  {
    ACE_OS::sleep(2);
    ::AsynchT::CCM_AMI4CCM_MyFooReplyHandler_var cb =
                  new MyFoo_callback_exec_i ();
    if (CORBA::is_nil (my_foo_ami_))
      {
         ACE_ERROR ((LM_ERROR, "ERROR Sender (ASYNCH) :"
                             "\tfoo_ami is NIL !\n"));  
         return 1;
      }
    //Invoke Asynchronous calls to test 
    ++nr_of_sent;
    my_foo_ami_->sendc_foo ( cb.in(),"Hi ", 1);
    ACE_DEBUG ((LM_DEBUG, "OK: SEND ASYNCHRONOUS CALL foo.\n"));

    ++nr_of_sent;
    my_foo_ami_->sendc_bar ( cb.in(), 1);
    ACE_DEBUG ((LM_DEBUG, "OK: SEND ASYNCHRONOUS CALL bar.\n"));

    ++nr_of_sent;
    my_foo_ami_->sendc_get_rw_attrib( cb.in());
    ACE_DEBUG ((LM_DEBUG, "OK: SEND ASYNCHRONOUS CALL get_rw_attrib.\n"));

    ++nr_of_sent;
    my_foo_ami_->sendc_set_rw_attrib( cb.in(),2);
    ACE_DEBUG ((LM_DEBUG, "OK: SEND ASYNCHRONOUS CALL set_rw_attrib.\n"));

    ++nr_of_sent;
    ACE_DEBUG ((LM_DEBUG, "OK: SEND ASYNCHRONOUS CALL get_ro_attrib.\n"));
    my_foo_ami_->sendc_get_ro_attrib( cb.in());

    //there is more than 1 message sent, without receiving callbacks,
    //so it is asynchronous
    if (nr_of_sent.value() > 1)
      {
        asynch = true;
      }
    return 0;
  }
  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (
    ::AsynchT::MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::AsynchT::MyFoo::_duplicate (my_foo_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep(1);
    CORBA::Boolean wait = false;
    for (int i = 0; i < 3; ++i)
      {
        // Run some synch calls, answer has to come before the next step.
         char * answer = 0;
         try
           {
             if( wait==true)
               {
                 ACE_ERROR ((LM_ERROR,
                             "ERROR: NOT RECEIVED SYNCHRONOUS answer.\n"));
               }
             wait = true;
             ACE_DEBUG ((LM_DEBUG, "OK: SEND SYNCHRONOUS CALL foo.\n"));
             CORBA::Long result = my_foo_ami_->foo ("Do something synchronous",
                                                     2 ,
                                                     answer);
             if ( result == 2)
               {
                 ACE_DEBUG ((LM_DEBUG, "OK: RECEIVED SYNCHRONOUS answer <%C>\n",
                                       answer));
                 wait = false;
               }
             if ( wait==true)
               {
                 ACE_ERROR ((LM_ERROR,
                             "ERROR: NOT RECEIVED SYNCHRONOUS answer.\n"));
               }
             wait = true;
             CORBA::Long l_cmd = 0;
             ACE_DEBUG ((LM_DEBUG, "OK: SEND SYNCHRONOUS CALL bar.\n"));
             my_foo_ami_->bar (2,l_cmd);
             if ( l_cmd == 2)
               {
                 ACE_DEBUG ((LM_DEBUG, "OK: RECEIVED SYNCHRONOUS answer <%C>\n",
                                       answer));
                 wait = false;
               }
            }
          catch (const AsynchT::InternalError&)
            {
              ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::foo: "
                                    "Unexpected exception.\n"));
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
      ::AsynchT::CCM_Sender_Context::_narrow (ctx);
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
    ::AsynchT::AMI4CCM_MyFoo_var asynch_foo =
      this->context_->get_connection_sendc_run_my_foo();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::AsynchT::MyFoo_var synch_foo =
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
    if ((asynch == false) || (nr_of_rec.value() != 5))
      {
        ACE_ERROR ((LM_ERROR, 
                   "ERROR: Test not asynchronous or not received back"
                   " all requests ( %u/5)!\n",
                   nr_of_rec.value() ));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Test asynchronous.\n"));
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_AsynchT_AMI_Sender_Impl (void)
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
