// -*- C++ -*-
// $Id$

#include "InterReturnT_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_InterReturnT_Receiver_Impl
{
  MyFoo_exec_i::MyFoo_exec_i (void)
  {
  }

  MyFoo_exec_i::~MyFoo_exec_i (void)
  {
  }

  void MyFoo_exec_i::ret_void (const char * /*in_str*/,
                               ::CORBA::String_out answer,
                               ::CORBA::Long_out l_cmd )
  {
    answer = CORBA::string_dup ("This is my answer from ret_void");
    l_cmd = 1;
  }

  CORBA::Long MyFoo_exec_i::ret_long (const char * /*in_str*/,
                                  ::CORBA::String_out answer,
                                  ::CORBA::Long_out l_cmd)
  {
    answer = CORBA::string_dup ("This is my answer from ret_long");
    l_cmd = 2;
    return 200;
  }

  CORBA::Double MyFoo_exec_i::ret_double (const char * /*in_str*/,
                                      ::CORBA::String_out answer,
                                      ::CORBA::Long_out l_cmd)
  {
    answer = CORBA::string_dup ("This is my answer from ret_double");
    l_cmd = 3;
    return 3.14;
  }

  InterReturnT::TestStruct * MyFoo_exec_i::ret_struct (const char * /*in_str*/,
                                    ::CORBA::String_out answer,
                                    ::CORBA::Long_out l_cmd)
  {
    answer = CORBA::string_dup ("This is my answer from ret_struct");
    l_cmd = 4;
    InterReturnT::TestStruct *test_struct = new InterReturnT::TestStruct();
    test_struct->key = CORBA::string_dup("aaa");
    test_struct->x = 10;
    return test_struct;
  }

  InterReturnT::TestArray_slice * MyFoo_exec_i::ret_array (
                                 const char * /*in_str*/,
                                 ::CORBA::String_out answer,
                                 ::CORBA::Long_out l_cmd)
  {
    answer = CORBA::string_dup ("This is my answer from ret_array");
    l_cmd = 5;
    InterReturnT::TestArray test_array;
    for ( CORBA::UShort i = 0; i < 5; i ++)
      {
        test_array[i].key = CORBA::string_dup("bbb");
        for (CORBA::UShort y = 0; y < 5; y ++)
        {
          test_array[i].x_array[y] = i * 100 + y ;
        } 
      }
    return InterReturnT::TestArray_dup(test_array);
  }

  InterReturnT::TestSeq * MyFoo_exec_i::ret_seq (const char * /*in_str*/,
                             ::CORBA::String_out,
                             ::CORBA::Long_out l_cmd)
  {
    l_cmd = 6;
    InterReturnT::test ttt;
    ttt.x_test = 12;
    ttt.x_teststr = CORBA::string_dup("ccc");
    InterReturnT::TestSeq *seq = new InterReturnT::TestSeq();
    seq->length(2);
    (*seq)[0]= ttt;
    ttt.x_teststr = CORBA::string_dup("ddd");
    (*seq)[1]= ttt;
    return seq;
  }

  InterReturnT::X_Union *MyFoo_exec_i::ret_union (const char * /*in_str*/,
                               ::CORBA::String_out answer,
                               ::CORBA::Long_out l_cmd)
  {
    answer = CORBA::string_dup ("This is my answer from ret_union");
    l_cmd = 7;
    InterReturnT::X_Union *x_uni = new InterReturnT::X_Union();
    x_uni->x_long(11);
    return x_uni;
  }

  InterReturnT::test_enum MyFoo_exec_i::ret_enum (const char * /*in_str*/,
                               ::CORBA::String_out answer,
                               ::CORBA::Long_out l_cmd)
  {
    answer = CORBA::string_dup ("This is my answer from ret_enum");
    l_cmd = 8;
    InterReturnT::test_enum test_enum;
    test_enum = InterReturnT::TWO;
    return test_enum;
  };

  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::InterReturnT::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    return new MyFoo_exec_i ();
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::InterReturnT::CCM_Receiver_Context::_narrow (ctx);

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
  create_InterReturnT_AMI_Receiver_Impl (void)
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
