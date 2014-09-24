// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "SenderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Sender_exec_export.h"
#include "tao/LocalObject.h"


namespace CIAO_Thread_Sender_Impl
{
  class Sender_exec_i;

  class SENDER_EXEC_Export ReadMessage_exec_i
    : public virtual ::Thread::CCM_ReadMessage,
      public virtual ::CORBA::LocalObject
  {
  public:
    ReadMessage_exec_i (Sender_exec_i& component);

    virtual ~ReadMessage_exec_i (void);

    // Operations and attributes from Thread::ReadMessage

    virtual char *
    get_message (void);

  private:
    Sender_exec_i& component_;
  };

  class SENDER_EXEC_Export Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (const ACE_Thread_ID &thr);
    virtual ~Sender_exec_i (void);

    //@{
    /** Supported operations and attributes. */


    virtual void
    start (void);
    //@}

    //@{
    /** Component attributes and port operations. */

    virtual ::Thread::CCM_ReadMessage_ptr
    get_push_message (void);

    //@{
    /** Operations from Components::SessionComponent. */

    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}


  private:
    ::Thread::CCM_Sender_Context_var context_;
    ACE_Thread_ID thr_;
    CORBA::String_var message_;
    CORBA::Boolean log_time_;
    friend class ReadMessage_exec_i;
  };
}

namespace CIAO_Thread_Sender_Impl
{
  class SENDER_EXEC_Export SenderHome_exec_i
    : public virtual SenderHome_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    SenderHome_exec_i (const ACE_Thread_ID &thr);

    virtual ~SenderHome_exec_i (void);

    // All operations and attributes.

    // Factory operations.

    // Finder operations.

    // Implicit operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (void);
  private:
    ACE_Thread_ID thr_;

  };

  extern "C" SENDER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Thread_SenderHome_Impl (void);
}

#endif /* ifndef */

