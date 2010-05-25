// -*- C++ -*-
// $Id$

// test of different datatypes as in-argumenten
#include "InterInArgsT_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_InterInArgsT_Sender_Impl
{
  CORBA::Short nr_of_excep_received = 0;
  CORBA::Short nr_of_received = 0;
  CORBA::UShort update_val = InterInArgsT::update_val;
  const CORBA::Short cmd_synch_ok = 1;
  const CORBA::Short cmd_synch_nok = 2;
  const CORBA::Short cmd_asynch_ok = 3;
  const CORBA::Short cmd_asynch_nok = 4;

  void HandleException (
      long id,
      long expect_id,
      const char* error_string,
      const char* func)
  {
    //expected exception
    if ( id == expect_id)
      {
        ACE_DEBUG ((LM_DEBUG, "Sender: Caught correct exception <%u,"
                              "%C> for %C\n",
                              id, error_string, func));
        ++nr_of_excep_received;
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR Sender: wrong exception received"
                              " for %C\n", func));
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
    ::CORBA::Long ami_return_val,
    const char * /*answer*/)
  {
    if ( ami_return_val == ( cmd_asynch_ok + update_val ))
     {
       ++nr_of_received;
     }
  }

  void
  MyFoo_callback_exec_i::foo_excep (
      ::CCM_AMI::ExceptionHolder * excep_holder)
  {
    try
      {
        excep_holder->raise_exception ();
      }
    catch (const InterInArgsT::InternalError& ex)
      {
        HandleException (ex.id, (cmd_asynch_nok + update_val),
                         ex.error_string.in(), "asynch foo");
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Caught unexpected exception:");
      }
  }

  void
  MyFoo_callback_exec_i::var_ins (
                const char * /*answer*/)
  {
    ++nr_of_received;
  }

  void
  MyFoo_callback_exec_i::var_ins_excep (
                    ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::var_div_ins (
                    const char * /*answer*/)
  {
    ++nr_of_received;
  }

  void
  MyFoo_callback_exec_i::var_div_ins_excep (
                   ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }
  
  void
  MyFoo_callback_exec_i::var_div2_ins (
                    const char * /*answer*/)
  {
    ++nr_of_received;
  }

  void
  MyFoo_callback_exec_i::var_div2_ins_excep (
                     ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::InterInArgsT::AMI4CCM_MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::InterInArgsT::AMI4CCM_MyFoo::_duplicate (my_foo_ami))
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
        //Invoke Asynchronous calls to test 
        my_foo_ami_->sendc_foo ( new MyFoo_callback_exec_i (),
                                "Hi", cmd_asynch_ok);
        //this shoukd invoke a exception
        my_foo_ami_->sendc_foo ( new MyFoo_callback_exec_i (),
                                "", cmd_asynch_nok);
        my_foo_ami_->sendc_var_ins( new MyFoo_callback_exec_i (),
                                   "Here a double for you.", 1.6);
         
        InterInArgsT::TestTopic test_topic;
        test_topic.key = "aaa";
        test_topic.x = 10;
        InterInArgsT::TopicString topic_str;
        topic_str.key = "bbb";
        topic_str.x_str = "ccc";
        InterInArgsT::TopicArray topic_arr;
        topic_arr.key = "ddd";
        topic_arr.x_array[0] = 11;
  
        my_foo_ami_->sendc_var_div_ins (new MyFoo_callback_exec_i (),
                                        test_topic,topic_str,topic_arr);

        InterInArgsT::TopicUnion topic_union;
        topic_union.key = "eee";
        topic_union.x_uni.x_long(11);
        InterInArgsT::test ttt;
        ttt.x_test = 12;
        ttt.x_teststr = "fff" ;
        InterInArgsT::test_seq seq;
        seq.length(2);
        seq[0] = ttt; 

        my_foo_ami_->sendc_var_div2_ins (new MyFoo_callback_exec_i (),
                                         topic_union, seq);
      }
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (
    ::InterInArgsT::MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::InterInArgsT::MyFoo::_duplicate (my_foo_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    //run some synch calls
    char * out_str = 0;
    try
      {
        CORBA::Long result = my_foo_ami_->foo ("Do something synchronous",
                                                cmd_synch_ok ,
                                                out_str);
        if ( result == (update_val + cmd_synch_ok))
          {
            ++nr_of_received;
          }
      } 
    catch (const InterInArgsT::InternalError&)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::foo: "
                              "Unexpected exception.\n"));  
      }
    try
      {
        my_foo_ami_->foo ("",cmd_synch_nok, out_str);
      }
    catch (const InterInArgsT::InternalError& ex)
      {
          HandleException (ex.id, (update_val + cmd_synch_nok),ex.error_string.in(),
                           "synch foo");
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
      ::InterInArgsT::CCM_Sender_Context::_narrow (ctx);
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
    ::InterInArgsT::AMI4CCM_MyFoo_var asynch_foo =
      this->context_->get_connection_sendc_run_my_foo();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::InterInArgsT::MyFoo_var synch_foo =
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
    if (nr_of_excep_received != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: not received the expected number of"
                              " exceptions"
                              "Expected: 2, Received: %u.\n",
                              nr_of_excep_received));  
      }
    if (nr_of_received != 5)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Sender not received the expected number"
                              " of callbacks and returns  for syn- and "
                              "asynchronous calls. Expected: 5,"
                              " Received: %u.\n",
                              nr_of_excep_received));  
      }
    if ((nr_of_received == 5) && (nr_of_excep_received == 2))
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Sender received the expected number of"
                              " callbacks and exceptions for syn- and "
                              "asynchronous calls\n"));  
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_InterInArgsT_AMI_Sender_Impl (void)
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
