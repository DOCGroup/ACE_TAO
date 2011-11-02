// -*- C++ -*-
// $Id$

#ifndef CIAO_CFTLS_TEST_SENDER_EXEC_NYP61D_H_
#define CIAO_CFTLS_TEST_SENDER_EXEC_NYP61D_H_

#include /**/ "ace/pre.h"

#include "CFTLS_Test_SenderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Sender_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_CFTLS_Test_Sender_Impl
{
  class Sender_exec_i;

  /**
   * StartHandler
   */
  class StartHandler : public ACE_Event_Handler
  {
  public:
    StartHandler (Sender_exec_i &callback);
    virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
  private:
    Sender_exec_i &callback_;
  };

  /**
   * UpdateTicker
   */
  class UpdateTicker : public ACE_Event_Handler
  {
  public:
    UpdateTicker (Sender_exec_i &callback);
    int handle_timeout (const ACE_Time_Value &, const void *);
  private:
    /// Maintains a handle that actually process the event
    Sender_exec_i &callback_;
    CORBA::UShort last_iter_;
  };


  /**
   * Component Executor Implementation Class: Sender_exec_i
   */

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    //@{
    /** Supported operations and attributes. */

    //@}

    //@{
    /** Component attributes and port operations. */

    virtual ::CORBA::UShort keys (void);

    virtual void keys (::CORBA::UShort keys);

    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);
    //@}

    //@{
    /** Operations from Components::SessionComponent. */
    virtual void set_session_context (::Components::SessionContext_ptr ctx);
    virtual void configuration_complete (void);
    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);
    //@}

    //@{
    /** User defined public operations. */
    void start (void);
    void run (void);
    void update_one (CORBA::UShort iter);
    //@}

  private:
    ::CFTLS_Test::CCM_Sender_Context_var ciao_context_;

    //@{
    /** Component attributes. */

    ::CORBA::UShort keys_;

    ::CORBA::UShort iterations_;
    //@}

    //@{
    /** User defined members. */
    UpdateTicker *ticker_;
    //@}

    //@{
    /** User defined private operations. */

    //@}

    /// Get the ACE_Reactor
    ACE_Reactor* reactor (void);
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CFTLS_Test_Sender_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
