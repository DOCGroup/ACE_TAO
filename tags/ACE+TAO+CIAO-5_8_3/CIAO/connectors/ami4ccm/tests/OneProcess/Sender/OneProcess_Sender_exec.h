// -*- C++ -*-
//
// $Id$

#ifndef CIAO_ONEPROCESS_SENDER_EXEC_H_
#define CIAO_ONEPROCESS_SENDER_EXEC_H_

#include "OneProcess_SenderEC.h"
#include "OneProcess_Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"

namespace CIAO_OneProcess_Sender_Impl
{
  typedef ACE_Atomic_Op <TAO_SYNCH_MUTEX, CORBA::UShort > Atomic_UShort;

  /// Worker thread for asynchronous invocations
  class asynch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    asynch_foo_generator (::OneProcess::AMI4CCM_MyFoo_ptr my_foo_ami);

    virtual int svc (void);

  private:
    ::OneProcess::AMI4CCM_MyFoo_var my_foo_ami_;
  };

  /// Worker thread for synchronous invocations
  class synch_foo_generator : public virtual ACE_Task_Base
  {
  public:
    synch_foo_generator (::OneProcess::MyFoo_ptr my_foo_ami);
    virtual int svc (void);

  private:
    ::OneProcess::MyFoo_var my_foo_ami_;
  };

  class MyFoo_callback_exec_i
    : public virtual ::OneProcess::CCM_AMI4CCM_MyFooReplyHandler,
      public virtual ::CORBA::LocalObject
  {
  public:
    MyFoo_callback_exec_i (void);
    virtual ~MyFoo_callback_exec_i (void);

    virtual void foo (::CORBA::Long ami_return_val, const char * answer);
    virtual void foo_excep (::CCM_AMI::ExceptionHolder_ptr excep_holder);
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
    ::OneProcess::CCM_Sender_Context_var context_;
  };

  extern "C" ONEPROCESS_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_OneProcess_AMI_Sender_Impl (void);
}

#endif /* ifndef */
