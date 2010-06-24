// -*- C++ -*-
//
// $Id$

#ifndef CIAO_HELLO_SENDER_COMP_EXEC_H_
#define CIAO_HELLO_SENDER_COMP_EXEC_H_

#include "Hello_Sender_compEC.h"
#include "Hello_Sender_comp_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_Hello_Sender_comp_Impl
{
  /// Worker thread for asynchronous invocations
  class asynch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_foo_generator (::Hello::AMI4CCM_MyFoo_obj_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::Hello::AMI4CCM_MyFoo_obj_var my_foo_ami_;
  };

  /// Worker thread for synchronous invocations
  class synch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    synch_foo_generator (::Hello::MyFoo_obj_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::Hello::MyFoo_obj_var my_foo_ami_;
  };

  class MyFoo_obj_callback_exec_i
    : public virtual ::Hello::CCM_AMI4CCM_MyFoo_objReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_obj_callback_exec_i (void);
    virtual ~MyFoo_obj_callback_exec_i (void);

    virtual void foo (::CORBA::Long ami_return_val, const char * answer);

    virtual void foo_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);

    // Hello Reply Handler
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

  class Sender_comp_exec_i
    : public virtual Sender_comp_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_comp_exec_i (void);
    virtual ~Sender_comp_exec_i (void);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Hello::CCM_Sender_comp_Context_var context_;
  };

  extern "C" HELLO_SENDER_COMP_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_AMI_Sender_comp_Impl (void);
}

#endif /* ifndef */

