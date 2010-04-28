// -*- C++ -*-
//
// $Id$

#ifndef CIAO_INTERINARGS_T_SENDER_EXEC_H_
#define CIAO_INTERINARGS_T_SENDER_EXEC_H_

#include "InterInArgsT_SenderEC.h"
#include "InterInArgsT_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_InterInArgsT_Sender_Impl
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
    asynch_foo_generator (::InterInArgsT::AMI_MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::InterInArgsT::AMI_MyFoo_var my_foo_ami_;
  };

  /// Worker thread for synchronous invocations
  class synch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    synch_foo_generator (::InterInArgsT::MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::InterInArgsT::MyFoo_var my_foo_ami_;
  };

  class MyFoo_callback_exec_i
    : public virtual ::InterInArgsT::CCM_AMI_MyFooCallback,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_callback_exec_i (void);
    virtual ~MyFoo_callback_exec_i (void);

    virtual void foo (::CORBA::Long ami_return_val, const char * answer);
    virtual void foo_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
    virtual void var_ins (const char * answer);
    virtual void var_ins_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);
    virtual void var_div_ins (const char * answer);
    virtual void var_div_ins_excep (
                 ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    virtual void var_div2_ins (const char * answer);
    virtual void var_div2_ins_excep (
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
    ::InterInArgsT::CCM_Sender_Context_var context_;
    MyFoo_callback_exec_i* global_foo_callback_;
  };

  extern "C" INTERINARGS_T_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_InterInArgsT_AMI_Sender_Impl (void);
}

#endif /* ifndef */

