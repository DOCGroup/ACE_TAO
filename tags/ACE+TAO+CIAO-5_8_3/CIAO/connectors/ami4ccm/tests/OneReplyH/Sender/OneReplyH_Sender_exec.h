// -*- C++ -*-
//
// $Id$

#ifndef CIAO_ONEREPLYH_T_SENDER_EXEC_H_
#define CIAO_ONEREPLYH_T_SENDER_EXEC_H_

#include "OneReplyH_SenderEC.h"
#include "OneReplyH_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_OneReplyH_Sender_Impl
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
    asynch_foo_generator (::OneReplyH::AMI4CCM_MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::OneReplyH::AMI4CCM_MyFoo_var my_foo_ami_;
    ::OneReplyH::CCM_AMI4CCM_MyFooReplyHandler_var cb_;
  };

  /// Worker thread for synchronous invocations
  class synch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    synch_foo_generator (::OneReplyH::MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::OneReplyH::MyFoo_var my_foo_ami_;
  };

  class MyFoo_callback_exec_i
    : public virtual ::OneReplyH::CCM_AMI4CCM_MyFooReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_callback_exec_i (void);
    virtual ~MyFoo_callback_exec_i (void);

    virtual void foo (::CORBA::Long ami_return_val, const char * answer);
    virtual void foo_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
    virtual void bar (const char * answer, CORBA::Long l_cmd);
    virtual void bar_excep (
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
    ::OneReplyH::CCM_Sender_Context_var context_;
 };

  extern "C" ONEREPLYH_T_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_OneReplyH_AMI_Sender_Impl (void);
}

#endif /* ifndef */

