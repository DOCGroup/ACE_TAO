// -*- C++ -*-
// $Id$

#ifndef CIAO_COHERENTUPDATE_TEST_SENDER_EXEC_VI4L1L_H_
#define CIAO_COHERENTUPDATE_TEST_SENDER_EXEC_VI4L1L_H_

#include /**/ "ace/pre.h"

#include "CoherentUpdate_Test_SenderEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Sender_exec_export.h"
#include "tao/LocalObject.h"

namespace CIAO_CoherentUpdate_Test_Sender_Impl
{
  class Sender_exec_i;

  //============================================================
  // WriteHandler
  //============================================================
  class WriteHandler :
    public ACE_Event_Handler
  {
  public:
    WriteHandler (Sender_exec_i &callback);
    virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
  private:
    Sender_exec_i &callback_;
  };


  //============================================================
  // Provider Executor Implementation Class: restart_updater_exec_i
  //============================================================

  class restart_updater_exec_i
    : public virtual ::CCM_CoherentUpdateRestarter,
      public virtual ::CORBA::LocalObject
  {
  public:
    restart_updater_exec_i (
      ::CoherentUpdate_Test::CCM_Sender_Context_ptr ctx,
      Sender_exec_i & callback);
    virtual ~restart_updater_exec_i (void);

    //@{
    /** Operations and attributes from ::CoherentUpdateRestarter. */

    virtual
    void restart_update (void);
    //@}

  private:
    ::CoherentUpdate_Test::CCM_Sender_Context_var ciao_context_;
    Sender_exec_i &callback_;
  };

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

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

    virtual ::CCM_CoherentUpdateRestarter_ptr
    get_restart_updater (void);

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
    void restart (void);
    //@}

  private:
    ::CoherentUpdate_Test::CCM_Sender_Context_var ciao_context_;

    //@{
    /** Component attributes and port operations. */
    ::CCM_CoherentUpdateRestarter_var ciao_restart_updater_;

    ::CORBA::UShort iterations_;
    //@}

    //@{
    /** User defined members. */
    CORBA::UShort run_;
    CORBA::ULong total_iter;
    WriteHandler *wh_;
    //@}

    //@{
    /** User defined private operations. */

    //@}

    /// Get the ACE_Reactor
    ACE_Reactor* reactor (void);
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoherentUpdate_Test_Sender_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */
