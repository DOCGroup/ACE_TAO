// -*- C++ -*-
// $Id$

// test of different datatypes as in-argumenten
#include "InterOutArgsT_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_InterOutArgsT_Sender_Impl
{
  CORBA::Short nr_of_received = 0;
 
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
  MyFoo_callback_exec_i::var_outs (
  const char * /*answer*/, CORBA::Double d_cmd, CORBA::Long l_cmd )
  {
    if (d_cmd != 3.14)
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::var_outs: "
                              "received the wrong double, expected 3.14,"
                              " received %2.2f\n",
                              d_cmd));
      }
    else if (l_cmd != 100)
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::var_outs: "
                              "received the wrong double, expected 3.14,"
                              " received %lu\n",
                              l_cmd));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::var_outs_excep (
                    ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::var_div_outs (
                    const InterOutArgsT::TestTopic &test_topic,
                    const InterOutArgsT::TopicString &topic_str,
                    const InterOutArgsT::TopicArray &topic_arr)
  {
    CORBA::Boolean error = false;
    if ((ACE_OS::strcmp (test_topic.key, "aaa") != 0) || (test_topic.x != 10))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_exec_i::var_div_outs: "
                              "received the wrong struct, "
                              "expected key 'aaa', x = 10,"
                              " received key '%C' , x = %u\n",
                              test_topic.key.in(),test_topic.x));
        error = true;
      }
    if (( ACE_OS::strcmp (topic_str.key,"bbb") != 0) ||
        (ACE_OS::strcmp (topic_str.x_str, "ccc") != 0))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_exec_i::var_div_outs: "
                              "received the wrong struct, "
                              "expected key 'bbb', x = 'ccc',"
                              " received key '%C' , x = %C\n",
                              topic_str.key.in() ,topic_str.x_str.in()));
        error = true;
      }
    if (( ACE_OS::strcmp (topic_arr.key,"ddd") != 0) ||
        (topic_arr.x_array[1] != 100))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_exec_i::var_div_outs: "
                              "received the wrong struct, "
                              "expected key 'ddd', second elem 100 ,"
                              " received key '%C', second elem %u\n",
                              topic_arr.key.in(), topic_arr.x_array[1]));
        error = true;
      }
    if (error == false)
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::var_div_outs_excep (
                   ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }
  
  void
  MyFoo_callback_exec_i::var_div2_outs (
                     const InterOutArgsT::X_Union &x_uni,
                     const InterOutArgsT::test_seq &seq)
  {
    CORBA::Boolean error = false;

    if (x_uni.x_long() != 11)
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_exec_i::var_div2_outs: "
                              "received the wrong union, "
                              "expected x_long = 11,"
                              " received x_long = %u\n",
                               x_uni.x_long()));
        error = true;
      }
    if (( ACE_OS::strcmp (seq[0].x_teststr, "fff") != 0) ||
        (seq[0].x_test != 12))
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_exec_i::var_div2_outs: "
                              "received the wrong sequence, "
                              "expected key 'fff', x = 12,"
                              " received key '%C' , x = %u\n",
                              seq[0].x_teststr.in(),seq[0].x_test));
        error = true;
      }
    if (error == false)
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::var_div2_outs_excep (
                     ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::enum_out ( InterOutArgsT::test_enum test_enum)
  {
    if (test_enum != InterOutArgsT::ONE)
      {
        ACE_ERROR ((LM_ERROR, "ERROR MyFoo_callback_exec_i::enum_out: "
                              "received the wrong value, expected ONE,"
                              " received %u\n",
                              test_enum));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::enum_out_excep (
                     ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }
  
  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::InterOutArgsT::AMI4CCM_MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::InterOutArgsT::AMI4CCM_MyFoo::_duplicate (my_foo_ami))
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
        my_foo_ami_->sendc_var_outs ( new MyFoo_callback_exec_i (),
                                      "Send me a double and a long.");
        my_foo_ami_->sendc_var_div_outs (new MyFoo_callback_exec_i ());
        my_foo_ami_->sendc_var_div2_outs (new MyFoo_callback_exec_i ());
        my_foo_ami_->sendc_enum_out (new MyFoo_callback_exec_i ());
      }
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (
    ::InterOutArgsT::MyFoo_ptr my_foo_ami)
  : my_foo_ami_ (::InterOutArgsT::MyFoo::_duplicate (my_foo_ami))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);
    //run some synch calls
    InterOutArgsT::TestTopic_var test_topic = 0;
    InterOutArgsT::TopicString_var topic_str = 0;
    InterOutArgsT::TopicArray_var topic_arr = 0;
    CORBA::Double d_cmd = 0;
    CORBA::Long l_cmd = 0;
    char * out_str = 0;
    try
      {
        my_foo_ami_->var_outs ("Do something synchronous",
                                                out_str,
                                                d_cmd ,
                                                l_cmd);
        if ( (d_cmd == 3.14 ) && (l_cmd == 100))
          {
            ++nr_of_received;
          }
      } 
    catch (const InterOutArgsT::InternalError&)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::var_outs: "
                              "Unexpected exception.\n"));  
      }
    try
      {
        my_foo_ami_->var_div_outs(test_topic,topic_str,topic_arr);
        if ((ACE_OS::strcmp (test_topic->key, "aaa") == 0) &&
            (test_topic->x == 10))
          {
            ++nr_of_received;
          }
        else
         {
           ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::var_div_outs: "
                                "Unexpected data returned.\n"));  
         }
      }
    catch (const InterOutArgsT::InternalError&)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::var_div_outs: "
                              "Unexpected exception.\n"));  
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
      ::InterOutArgsT::CCM_Sender_Context::_narrow (ctx);
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
    ::InterOutArgsT::AMI4CCM_MyFoo_var asynch_foo =
      this->context_->get_connection_sendc_run_my_foo();
    asynch_foo_generator* asynch_foo_gen =
        new asynch_foo_generator (asynch_foo);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    ::InterOutArgsT::MyFoo_var synch_foo =
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
    if (nr_of_received == 6)
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Sender received all expected data"
                              " for syn- and asynchronous calls\n"));  
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Sender didn't receive all expected data"
                              " for syn- and asynchronous calls\n"));  
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_InterOutArgsT_AMI_Sender_Impl (void)
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
