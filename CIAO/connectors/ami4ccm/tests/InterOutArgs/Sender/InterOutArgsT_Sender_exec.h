// -*- C++ -*-
//
// $Id$

#ifndef CIAO_INTEROUTARGS_T_SENDER_EXEC_H_
#define CIAO_INTEROUTARGS_T_SENDER_EXEC_H_

#include "InterOutArgsT_SenderEC.h"
#include "InterOutArgsT_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_InterOutArgsT_Sender_Impl
{
  /// Worker thread for asynchronous invocations
  class asynch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_foo_generator (::InterOutArgsT::AMI4CCM_MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::InterOutArgsT::AMI4CCM_MyFoo_var my_foo_ami_;
  };

  /// Worker thread for synchronous invocations
  class synch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    synch_foo_generator (::InterOutArgsT::MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::InterOutArgsT::MyFoo_var my_foo_ami_;
  };

  class MyFoo_callback_exec_i
    : public virtual ::InterOutArgsT::CCM_AMI4CCM_MyFooReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_callback_exec_i (void);
    virtual ~MyFoo_callback_exec_i (void);

    virtual void var_outs (const char * answer, CORBA::Double d_cmd,
                           CORBA::Long l_cmd);
    virtual void var_outs_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void var_div_outs (const InterOutArgsT::TestTopic &test_topic,
                               const InterOutArgsT::TopicString &topic_str,
                               const InterOutArgsT::TopicArray &topic_arr);
    virtual void var_div_outs_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void var_div2_outs ( const InterOutArgsT::X_Union &x_uni,
                                 const InterOutArgsT::test_seq &seq);
    virtual void var_div2_outs_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);
  };

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::InterOutArgsT::CCM_Sender_Context_var context_;
    MyFoo_callback_exec_i* global_foo_callback_;
  };

  extern "C" INTEROUTARGS_T_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_InterOutArgsT_AMI_Sender_Impl (void);
}

#endif /* ifndef */

