// -*- C++ -*-
//
// $Id$

#ifndef CIAO_EXCEPTION_T_SENDER_EXEC_H_
#define CIAO_EXCEPTION_T_SENDER_EXEC_H_

#include "ExceptionT_SenderEC.h"
#include "ExceptionT_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_ExceptionT_Sender_Impl
{
  /// Common exception handlers
  void HandleException (
      long id,
      const char* error_string,
      const char* test,
      const char* func);

  void HandleException (
      ::Messaging::ExceptionHolder * excep_holder,
      const char* func);

  /// Worker thread for asynchronous invocations
  class asynch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_foo_generator (::ExceptionT::AMI_MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::ExceptionT::AMI_MyFoo_var my_foo_ami_;
  };

  /// Worker thread for synchronous invocations
  class synch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    synch_foo_generator (::ExceptionT::MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::ExceptionT::MyFoo_var my_foo_ami_;
  };

  class MyFoo_callback_exec_i
    : public virtual ::ExceptionT::CCM_AMI_MyFooCallback,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_callback_exec_i (void);
    virtual ~MyFoo_callback_exec_i (void);

    virtual void foo (::CORBA::Long ami_return_val, const char * answer);

    virtual void foo_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);

    // ExceptionT Reply Handler
    virtual void hello (::CORBA::Long answer);

    virtual void hello_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);

    // GET rw_attrib Reply Handler
    virtual void get_rw_attrib (
      ::CORBA::Short ami_return_val);
    virtual void get_rw_attrib_excep (
      ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    // SET rw_attrib Reply Handler
    virtual void set_rw_attrib (void);
    virtual void set_rw_attrib_excep (
      ::CCM_AMI::ExceptionHolder_ptr excep_holder);

    // ro_attrib Reply Handler
    virtual void get_ro_attrib (
      ::CORBA::Short ami_return_val);
    virtual void get_ro_attrib_excep (
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
    ::ExceptionT::CCM_Sender_Context_var context_;
    MyFoo_callback_exec_i* global_foo_callback_;
  };

  extern "C" EXCEPTION_T_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ExceptionT_AMI_Sender_Impl (void);
}

#endif /* ifndef */

