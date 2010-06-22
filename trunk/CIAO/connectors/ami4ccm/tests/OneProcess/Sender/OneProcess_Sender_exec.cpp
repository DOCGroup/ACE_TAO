// -*- C++ -*-
// $Id$

// sender, receiver and connector in one node: asynchronous callbacks
// should become received synchronous.
#include "OneProcess_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_OneProcess_Sender_Impl
{
  Atomic_UShort nr_of_sent = 0;
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
    if (ami_return_val == 1)
      {
        ACE_DEBUG ((LM_DEBUG, "OK: GET ASYNCHRONOUS CALLBACK, \n"));
        asynch = false;
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
  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::OneProcess::AMI4CCM_MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::OneProcess::AMI4CCM_MyFoo::_duplicate (my_foo_ami))
  {
  }

  int asynch_foo_generator::svc ()
  {
    ACE_OS::sleep(3);
    ::OneProcess::CCM_AMI4CCM_MyFooReplyHandler_var cb0 =
                  new MyFoo_callback_exec_i ();
    ::OneProcess::CCM_AMI4CCM_MyFooReplyHandler_var cb1 =
                  new MyFoo_callback_exec_i ();
    ::OneProcess::CCM_AMI4CCM_MyFooReplyHandler_var cb2 =
                  new MyFoo_callback_exec_i ();
    ::OneProcess::CCM_AMI4CCM_MyFooReplyHandler_var cb3 =
                  new MyFoo_callback_exec_i ();
    ::OneProcess::CCM_AMI4CCM_MyFooReplyHandler_var cb4 =
                  new MyFoo_callback_exec_i ();

    for (int i = 0; i < 5; ++i)
      {
        if (CORBA::is_nil (my_foo_ami_))
          {
             ACE_ERROR ((LM_ERROR, "ERROR Sender (ASYNCH) :"
                                   "\tfoo_ami is NIL !\n"));  
             return 1;
          }
        if (asynch == true)
          {
            //expect to have receive an answer before you come here again.
            //if asynch == true, error because no answer received in 
            //MyFoo_callback_exec_i::foo
            ACE_ERROR ((LM_ERROR, "ERROR: not received synchronus answer "
                                   "for asynchronous call\n"));
          }
        else
          {
            ++nr_of_sent;
            //Invoke Asynchronous calls to test 
            if (i == 0)
              {  
                asynch = true;
                my_foo_ami_->sendc_foo ( cb0.in(),"Hi 1", 1);
              }   
            else if (i == 1)
              {
                asynch = true;
                my_foo_ami_->sendc_foo ( cb1.in(),"Hi 2", 1);
              }
            else if (i == 2)
              {
                asynch = true;
                my_foo_ami_->sendc_foo ( cb2.in(),"Hi 3", 1);
              }
            else if (i == 3)
              {
                asynch = true;
                my_foo_ami_->sendc_foo ( cb3.in(),"Hi 4", 1);
              }
            else if (i == 4)
              {
                asynch = true;
                my_foo_ami_->sendc_foo ( cb4.in(),"Hi 5", 1);
              }
          }
      }
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (
    ::OneProcess::MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::OneProcess::MyFoo::_duplicate (my_foo_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep(3);
    CORBA::Boolean wait = false;
    for (int i = 0; i < 3; ++i)
      {
         //run some synch calls
         try
           {
             CORBA::String_var answer;
             if( wait==true)
               {
                 ACE_ERROR ((LM_ERROR,
                             "ERROR: NOT RECEIVED SYNCHRONOUS answer.\n"));
               }
             wait = true;
             CORBA::Long result = my_foo_ami_->foo ("Do something synchronous",
                                                     2 ,
                                                     answer.out ());
             if ( result == 2)
               {
                 ACE_DEBUG ((LM_DEBUG, "OK: RECEIVED SYNCHRONOUS answer <%C>\n",
                                       answer.in ()));
                 wait = false;
               }
            }
          catch (const OneProcess::InternalError&)
            {
              ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::foo: "
                                    "Unexpected exception.\n"));
            }
          ACE_OS::sleep(1);
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
  // Operations from Components::SessionComponent.
  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::OneProcess::CCM_Sender_Context::_narrow (ctx);
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
    ::OneProcess::AMI4CCM_MyFoo_var asynch_foo =
      this->context_->get_connection_sendc_run_my_foo();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::OneProcess::MyFoo_var synch_foo =
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
    if ((asynch == true) || (nr_of_sent.value() != 5))
      {
        ACE_ERROR ((LM_ERROR, 
                   "ERROR: All in one proces worked asynchronous, "
                   "this was not expected!\n"));  
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, 
                   "OK: All in one proces worked synchronous as expected\n"));  
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_OneProcess_AMI_Sender_Impl (void)
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
