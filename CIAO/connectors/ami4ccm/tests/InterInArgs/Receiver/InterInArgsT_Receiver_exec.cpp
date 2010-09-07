// -*- C++ -*-
// $Id$

#include "InterInArgsT_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"
 
namespace CIAO_InterInArgsT_Receiver_Impl
{
  CORBA::UShort nr_of_received = 0;
  CORBA::UShort update_val = InterInArgsT::update_val;
  MyFoo_exec_i::MyFoo_exec_i (void)
  {
  }

  MyFoo_exec_i::~MyFoo_exec_i (void)
  {
  }

  ::CORBA::Long
  MyFoo_exec_i::foo (const char * in_str,::CORBA::Long cmd,
                     ::CORBA::String_out answer)
  {
    if (ACE_OS::strlen (in_str) == 0)
      {
        InterInArgsT::InternalError ex (cmd + update_val,
                                        "foo thrown by receiver");
        throw ex;
      }
    else
      {
        answer = CORBA::string_dup ("This is my answer : Hi");
        ++nr_of_received;
        return (cmd + update_val);
      }
  }

  void
  MyFoo_exec_i::var_ins (const char * /*in_str*/,  
                         ::CORBA::String_out answer,
                         ::CORBA::Double cmd)
  {
     if ( cmd != 1.6)
       {
         ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_ins: "
                              "received the wrong double, expected 1.6,"
                              " received %2.2f\n",
                              cmd));
       }
     else
       {
         ++nr_of_received;
       }
     answer = CORBA::string_alloc(20); 
     ACE_OS::sprintf(answer,"double was %2.2f",cmd);
  }

  void
  MyFoo_exec_i::var_div_ins (const ::InterInArgsT::TestTopic &test_topic, 
                             const ::InterInArgsT::TopicString &topic_str, 
                             const ::InterInArgsT::TestArray topic_arr, 
                             ::CORBA::String_out answer)
  {
    CORBA::Boolean error = false;
    if ((ACE_OS::strcmp (test_topic.key, "aaa") != 0) || (test_topic.x != 10))
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_div_ins: "
                              "received the wrong struct, "
                              "expected key 'aaa', x = 10,"
                              " received key '%C' , x = %u\n",
                              test_topic.key.in(),test_topic.x));
        error = true;
      }
    if ((ACE_OS::strcmp (topic_str.key, "bbb") != 0) ||
        (ACE_OS::strcmp (topic_str.x_str, "ccc") != 0))
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_div_ins: "
                              "received the wrong struct, "
                              "expected key 'bbb', x = 'ccc',"
                              " received key '%C' , x = %C\n",
                              topic_str.key.in() ,topic_str.x_str.in()));
        error = true;
      }
    if ((ACE_OS::strcmp (topic_arr[0].key, "ddd") != 0) ||
        (topic_arr[1].x_array[2] != 102))
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_div_ins: "
                              "received the wrong struct, "
                              "expected key 'ddd', elem 102"
                              " received key '%C', elem %u\n",
                              topic_arr[0].key.in(),topic_arr[1].x_array[2] ));
         error = true;
      }
    answer = CORBA::string_dup ("This is my answer from var_div_ins");
    if(error == false)
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_exec_i::var_div2_ins (const ::InterInArgsT::X_Union &topic_union, 
                              const ::InterInArgsT::test_seq &seq, 
                              ::CORBA::String_out answer)
  {
    CORBA::Boolean error = false;
    if (topic_union.x_long() != 11)
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_div2_ins: "
                              "received the wrong union, "
                              "expected x = 11,"
                              " received x = %u\n",
                              topic_union.x_long()));  
        error = true;
      }
    if ((ACE_OS::strcmp (seq[0].x_teststr, "fff") != 0)||
        (seq[0].x_test != 12))
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::var_div2_ins: "
                              "received the wrong struct, "
                              "expected key 'fff', x = 10,"
                              " received key '%C' , x = %u\n",
                              seq[0].x_teststr.in(),seq[0].x_test));
        error = true;
      }
    answer = CORBA::string_dup ("This is my answer from var_div2_ins");
    if(error == false)
      {
        ++nr_of_received;
      }
  }

  void
  MyFoo_exec_i::enum_in(::CORBA::String_out answer, 
                               InterInArgsT::test_enum in_test)
  {
    if ( in_test != InterInArgsT::ONE)
      {
        ACE_ERROR ((LM_ERROR, "MyFoo_exec_i::enum_in: "
                              "received the wrong enum value, expected ONE,"
                              " received %u\n",
                              in_test));
      }
    else
      {
        ++nr_of_received;
      }
    answer = CORBA::string_dup ("This is my answer from enum_in");
  }

  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::InterInArgsT::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    return new MyFoo_exec_i ();
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::InterInArgsT::CCM_Receiver_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    if (nr_of_received == 6)
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Receiver received all expected data"
                              " from syn- and asynchronous calls\n"));  
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver didn't receive all"
                              " expected data  (%u of 6)"
                              " from syn- and asynchronous calls\n",
                              nr_of_received));  
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_InterInArgsT_AMI_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_RETURN (
      retval,
      Receiver_exec_i,
      ::Components::EnterpriseComponent::_nil ());

    return retval;
  }
}
