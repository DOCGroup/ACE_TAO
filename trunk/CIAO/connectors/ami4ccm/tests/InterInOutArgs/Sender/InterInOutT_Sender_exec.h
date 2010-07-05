// -*- C++ -*-
//
// $Id$

#ifndef CIAO_INTERINOUT_T_SENDER_EXEC_H_
#define CIAO_INTERINOUT_T_SENDER_EXEC_H_

#include "InterInOutT_SenderEC.h"
#include "InterInOutT_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_InterInOutT_Sender_Impl
{

  /// Common exception handlers
  void HandleException (
      long id,
      long expect_id,
      const char* error_string,
      const char* func);
  
  /// Worker thread for asynchronous invocations
  class asynch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_foo_generator (::InterInOutT::CCM_Sender_Context_ptr context);
    virtual int svc (void);

  private:
    ::InterInOutT::CCM_Sender_Context_var context_;
  };

  /// Worker thread for synchronous invocations
  class synch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    synch_foo_generator (::InterInOutT::CCM_Sender_Context_ptr context);
    virtual int svc (void);

  private:
    ::InterInOutT::CCM_Sender_Context_var context_;

  };

  class MyFoo_callback_exec_i
    : public virtual ::InterInOutT::CCM_AMI4CCM_MyFooReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_callback_exec_i (void);
    virtual ~MyFoo_callback_exec_i (void);

    virtual void foo (::CORBA::Long ami_return_val, CORBA::Long l_cmd, const char * answer);
    virtual void foo_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
    virtual void var_ins ( const char * answer, CORBA::Double);
    virtual void var_ins_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);
    virtual void var_div_ins (const InterInOutT::TestTopic &test_topic,
                              const InterInOutT::TopicString &topic_str,
                              const InterInOutT::TestArray topic_arr, const char * answer);

    virtual void var_div_ins_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void var_div2_ins (const InterInOutT::X_Union &x_uni,
                                 const InterInOutT::test_seq &seq, const char * answer);
    virtual void var_div2_ins_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void enum_in(const char * answer, InterInOutT::test_enum test_enum);
    virtual void enum_in_excep (
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
    ::InterInOutT::CCM_Sender_Context_var context_;
  };

  extern "C" INTERINOUT_T_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_InterInOutT_AMI_Sender_Impl (void);
}

#endif /* ifndef */

