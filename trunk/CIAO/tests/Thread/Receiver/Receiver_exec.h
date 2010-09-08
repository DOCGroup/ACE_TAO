// -*- C++ -*-
// $Id$

#ifndef CIAO_RECEIVER_EXEC_H_
#define CIAO_RECEIVER_EXEC_H_


#include "ReceiverEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Receiver_exec_export.h"
#include "tao/LocalObject.h"


namespace CIAO_Thread_Receiver_Impl
{
  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (const ACE_Thread_ID &thr);
    virtual ~Receiver_exec_i (void);

    virtual void
    push_click_in (
      ::Thread::TimeOut * ev);

    virtual ::CORBA::Short
    iterations (void);

    virtual void
    iterations (
      ::CORBA::Short iterations);

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Thread::CCM_Receiver_Context_var context_;
    CORBA::Short iterations_;
    ACE_Thread_ID thr_;
  };
}

namespace CIAO_Thread_Receiver_Impl
{
  class RECEIVER_EXEC_Export ReceiverHome_exec_i
    : public virtual ReceiverHome_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    ReceiverHome_exec_i (const ACE_Thread_ID &thr);

    virtual ~ReceiverHome_exec_i (void);

    // All operations and attributes.

    // Factory operations.

    // Finder operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (void);
  private:
    ACE_Thread_ID thr_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Thread_ReceiverHome_Impl (void);
}

#endif /* ifndef */
