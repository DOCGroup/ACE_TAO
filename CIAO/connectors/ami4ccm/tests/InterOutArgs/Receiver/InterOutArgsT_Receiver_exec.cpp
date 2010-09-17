// -*- C++ -*-
// $Id$

#include "InterOutArgsT_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_InterOutArgsT_Receiver_Impl
{
  MyFoo_exec_i::MyFoo_exec_i (void)
  {
  }

  MyFoo_exec_i::~MyFoo_exec_i (void)
  {
  }

  void
  MyFoo_exec_i::var_outs (const char * /*in_str*/,
                          ::CORBA::String_out answer,
                          ::CORBA::Double_out d_cmd,
                          ::CORBA::Long_out l_cmd)
  {
    answer = CORBA::string_dup ("This is my answer from var_outs");
    d_cmd = 3.14;
    l_cmd = 100;
  }

  void  MyFoo_exec_i::var_div_outs (::InterOutArgsT::TestTopic_out test_topic,
                                    ::InterOutArgsT::TopicString_out topic_str,
                                    ::InterOutArgsT::TopicArray_out topic_arr)
  {
    InterOutArgsT::TestTopic *new_topic = new InterOutArgsT::TestTopic();
    test_topic = new_topic;
    test_topic->key = CORBA::string_dup("aaa");
    test_topic->x = 10;

    topic_str = new InterOutArgsT::TopicString();
    topic_str->key = CORBA::string_dup("bbb");
    topic_str->x_str = CORBA::string_dup("ccc");

    topic_arr = new InterOutArgsT::TopicArray();
    topic_arr->key = CORBA::string_dup("ddd");
    for (CORBA::UShort i = 0; i < 5; i ++)
      {
        topic_arr->x_array[i] = i * 100;
      }
  }

  void  MyFoo_exec_i::var_div2_outs (::InterOutArgsT::X_Union_out x_uni,
                                     ::InterOutArgsT::test_seq_out seq)
  {
    x_uni = new InterOutArgsT::X_Union();
    x_uni->x_long(11);

    InterOutArgsT::test ttt;
    ttt.x_test = 12;
    ttt.x_teststr = CORBA::string_dup("fff") ;
    seq = new InterOutArgsT::test_seq();
    seq->length(2);
    (*seq)[0]= ttt;
  }

  void  MyFoo_exec_i::enum_out (::InterOutArgsT::test_enum_out out_test)
  {
    out_test = ::InterOutArgsT::ONE;
  }

  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::InterOutArgsT::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    return new MyFoo_exec_i ();
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::InterOutArgsT::CCM_Receiver_Context::_narrow (ctx);

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
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_InterOutArgsT_AMI_Receiver_Impl (void)
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
