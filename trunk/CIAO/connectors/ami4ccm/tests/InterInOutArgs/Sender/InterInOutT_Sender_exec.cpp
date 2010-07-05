// -*- C++ -*-
// $Id$

// test of different datatypes as in-argumenten
#include "InterInOutT_Sender_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_InterInOutT_Sender_Impl
{
  CORBA::Short nr_of_excep_received = 0;
  CORBA::Short nr_of_received = 0;
  CORBA::UShort update_val = InterInOutT::update_val;
  const CORBA::Short cmd_synch_ok = 1;
  const CORBA::Short cmd_synch_nok = 2;
  const CORBA::Short cmd_asynch_ok = 3;
  const CORBA::Short cmd_asynch_nok =  4;

  void HandleException (
      long id,
      long expect_id,
      const char* error_string,
      const char* func)
  {
    //expected exception
    if ( id == expect_id)
      {
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
    ::CORBA::Long l_cmd,
    const char * answer)
  {
    if (( ami_return_val == ( cmd_asynch_ok + update_val )) &&
        ( l_cmd == ami_return_val) &&
        ( ACE_OS::strcmp (answer, "Hi from receiver") == 0))
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
    catch (const InterInOutT::InternalError& ex)
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
                const char * /*answer*/,
                CORBA::Double d_cmd)
  {
    if (d_cmd == 3.2)
    {
      ++nr_of_received;
    }
  }

  void
  MyFoo_callback_exec_i::var_ins_excep (
                    ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void
  MyFoo_callback_exec_i::var_div_ins (
               const InterInOutT::TestTopic &test_topic,
               const InterInOutT::TopicString &topic_str,
               const InterInOutT::TestArray topic_arr, 
               const char * /*answer*/)
  {
    CORBA::Boolean error = false;
    if ((ACE_OS::strcmp (test_topic.key, "111") != 0) || (test_topic.x != 111))
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_div_ins: "
                              "received the wrong struct, "
                              "expected key '111', x = 111,"
                              " received key '%C' , x = %u\n",
                              test_topic.key.in(),test_topic.x));
        error = true;
      }

    if ((ACE_OS::strcmp (topic_str.key, "222") != 0) ||
        (ACE_OS::strcmp (topic_str.x_str, "333") != 0))
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_div_ins: "
                              "received the wrong struct, "
                              "expected key '222', x = '333',"
                              " received key '%C' , x = %C\n",
                              topic_str.key.in() ,topic_str.x_str.in()));
        error = true;
      }
 
    if ((ACE_OS::strcmp (topic_arr[0].key, "444") != 0) ||
        (topic_arr[1].x_array[2] != 444))
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_div_ins: "
                              "received the wrong struct, "
                              "expected key '444', elem 444"
                              " received key '%C', elem %u\n",
                              topic_arr[0].key.in(),topic_arr[1].x_array[2] ));
         error = true;
      }

    if(error == false)
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::var_div_ins_excep (
                   ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }
  
  void
  MyFoo_callback_exec_i::var_div2_ins (const InterInOutT::X_Union &x_uni,
                                       const InterInOutT::test_seq &seq,
                                       const char * /*answer*/)
  {
    CORBA::Boolean error = false;
    if (x_uni.x_long() != 555)
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_div2_ins: "
                              "received the wrong union, "
                              "expected x = 555,"
                              " received x = %u\n",
                              x_uni.x_long()));  
        error = true;
      }
     if ((ACE_OS::strcmp (seq[0].x_teststr, "666") != 0)||
        (seq[0].x_test != 666))
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_div2_ins: "
                              "received the wrong struct, "
                              "expected key '666', x = 666,"
                              " received key '%C' , x = %u\n",
                              seq[0].x_teststr.in(),seq[0].x_test));
        error = true;
      }
    if(error == false)
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::var_div2_ins_excep (
                     ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }

  void 
  MyFoo_callback_exec_i::enum_in( const char * /*answer*/,
                                  InterInOutT::test_enum test_enum)
  {
      if ( test_enum != InterInOutT::TWO)
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::enum_in: "
                              "received the wrong enum value, expected TWO,"
                              " received %u\n",
                              test_enum));
      }
    else
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_callback_exec_i::enum_in_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder)
  {
    excep_holder->raise_exception ();
  }
  
  //============================================================
  // Worker thread for asynchronous invocations for MyFoo
  //============================================================
  asynch_foo_generator::asynch_foo_generator (
    ::InterInOutT::CCM_Sender_Context_ptr context)
    : context_(::InterInOutT::CCM_Sender_Context::_duplicate (context))
  {
  }

  int asynch_foo_generator::svc ()
  {
    ACE_OS::sleep (3); 
    ::InterInOutT::AMI4CCM_MyFoo_var my_foo_ami_  =
       context_->get_connection_sendc_run_my_foo();
    if (CORBA::is_nil (my_foo_ami_))
      {
        ACE_ERROR ((LM_ERROR, "ERROR Sender (ASYNCH) :\tfoo_ami is NIL !\n"));  
       return 1;
      }
    else
      {
        CORBA::Long l_cmd = 3;
        CORBA::String_var answer = CORBA::string_dup("Hi from sender");
        //Invoke Asynchronous calls to test 
        my_foo_ami_->sendc_foo ( new MyFoo_callback_exec_i (),
          "Do something synchronous", l_cmd, answer.inout());

        //this should invoke a exception
        l_cmd = 4;
        my_foo_ami_->sendc_foo ( new MyFoo_callback_exec_i (),
          "", l_cmd, answer.inout());
        my_foo_ami_->sendc_var_ins( new MyFoo_callback_exec_i (),
                                   "Here a double for you.", 1.6);
         
        InterInOutT::TestTopic test_topic;
        test_topic.key = "aaa";
        test_topic.x = 10;
        InterInOutT::TopicString topic_str;
        topic_str.key = "bbb";
        topic_str.x_str = "ccc";
        InterInOutT::TestArray topic_arr;
        for ( CORBA::UShort i = 0; i < 5; i ++)
          {
            topic_arr[i].key = CORBA::string_dup("ddd");
            for (CORBA::UShort y = 0; y < 5; y ++)
              {
                topic_arr[i].x_array[y] = i * 100 + y ;
              } 
          }
        my_foo_ami_->sendc_var_div_ins (new MyFoo_callback_exec_i (),
                                        test_topic,topic_str,topic_arr, answer);

        InterInOutT::X_Union topic_union;
        topic_union.x_long(11);
        InterInOutT::test ttt;
        ttt.x_test = 12;
        ttt.x_teststr = "fff" ;
        InterInOutT::test_seq seq;
        seq.length(2);
        seq[0] = ttt; 

        my_foo_ami_->sendc_var_div2_ins (new MyFoo_callback_exec_i (),
                                         topic_union, seq);
        InterInOutT::test_enum in_test;
        in_test = ::InterInOutT::ONE;
        my_foo_ami_->sendc_enum_in(new MyFoo_callback_exec_i (), 
                                          in_test);
      }
    return 0;
  }

  //============================================================
  // Worker thread for synchronous invocations for MyFoo
  //============================================================
  synch_foo_generator::synch_foo_generator (
   ::InterInOutT::CCM_Sender_Context_ptr context)
  : context_(::InterInOutT::CCM_Sender_Context::_duplicate (context))
  {
  }

  int synch_foo_generator::svc ()
  {
    ACE_OS::sleep (3);

    //run some synch calls
    ::InterInOutT::MyFoo_var my_foo_ami_ =
         context_->get_connection_run_my_foo ();
    CORBA::String_var answer = CORBA::string_dup("Hi from sender");
    CORBA::Long l_cmd = 1;
    try
      {
        CORBA::Long result = my_foo_ami_->foo ("Do something synchronous",
                                                l_cmd ,
                                                answer.inout ());
        if ((result == (update_val + cmd_synch_ok)) &&
            (ACE_OS::strcmp (answer.in(), "Hi from receiver") == 0))
          {
            ++nr_of_received;
          }
      } 
    catch (const InterInOutT::InternalError&)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: synch_foo_generator::foo: "
                              "Unexpected exception.\n"));  
      }
    try
      {
        l_cmd = 2;
        answer = CORBA::string_dup("Hi from sender");
        my_foo_ami_->foo ("",l_cmd, answer.inout());
      }
    catch (const InterInOutT::InternalError& ex)
      {
          HandleException (ex.id, (update_val + cmd_synch_nok),
                           ex.error_string.in(),
                           "synch foo");
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
      ::InterInOutT::CCM_Sender_Context::_narrow (ctx);
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
    asynch_foo_generator* asynch_foo_gen =
      new asynch_foo_generator (this->context_);
    asynch_foo_gen->activate (THR_NEW_LWP | THR_JOINABLE, 1);

    synch_foo_generator* synch_foo_gen =
       new synch_foo_generator (this->context_);
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
    if (nr_of_received != 6)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Sender not received the expected number"
                              " of callbacks and returns  for syn- and "
                              "asynchronous calls. Expected: 6,"
                              " Received: %u.\n",
                              nr_of_excep_received));  
      }
    if ((nr_of_received == 6) && (nr_of_excep_received == 2))
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Sender received the expected number of"
                              " callbacks and exceptions for syn- and "
                              "asynchronous calls\n"));  
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_InterInOutT_AMI_Sender_Impl (void)
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
