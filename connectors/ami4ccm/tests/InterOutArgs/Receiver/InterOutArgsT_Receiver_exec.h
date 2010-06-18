// -*- C++ -*-
// $Id$

#ifndef CIAO_INTEROUTARGS_T_RECEIVER_EXEC_H_
#define CIAO_INTEROUTARGS_T_RECEIVER_EXEC_H_

#include "InterOutArgsT_ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "InterOutArgsT_Receiver_exec_export.h"

namespace CIAO_InterOutArgsT_Receiver_Impl
{
  class MyFoo_exec_i
    : public virtual ::InterOutArgsT::CCM_MyFoo,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_exec_i (void);
    virtual ~MyFoo_exec_i (void);
    virtual void var_outs (const char * in_str,  ::CORBA::String_out answer,
      ::CORBA::Double_out d_cmd, ::CORBA::Long_out l_cmd);
    virtual void var_div_outs (InterOutArgsT::TestTopic_out test_topic,
                               InterOutArgsT::TopicString_out topic_str,
                               InterOutArgsT::TopicArray_out topic_arr);
    virtual void var_div2_outs (InterOutArgsT::X_Union_out x_uni,
                                InterOutArgsT::test_seq_out seq);
    virtual void enum_out (InterOutArgsT::test_enum_out out_test);
  };

  class  Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    virtual ::InterOutArgsT::CCM_MyFoo_ptr get_do_my_foo (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::InterOutArgsT::CCM_Receiver_Context_var context_;
  };

  extern "C" INTEROUTARGS_T_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_InterOutArgsT_AMI_Receiver_Impl (void);
}

#endif /* ifndef */
