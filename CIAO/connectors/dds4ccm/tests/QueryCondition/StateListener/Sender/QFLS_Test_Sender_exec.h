// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "QFLS_Test_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_QFLS_Test_Sender_Impl
{
  class Sender_exec_i;

  //============================================================
  // StartHandler
  //============================================================
  class StartHandler :
    public ACE_Event_Handler
  {
  public:
    StartHandler (Sender_exec_i &callback);
    virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
  private:
    Sender_exec_i &callback_;
  };

  //============================================================
  // UpdateTicker
  //============================================================
  class UpdateTicker :
    public ACE_Event_Handler
  {
  public:
    UpdateTicker (Sender_exec_i &callback);
    int handle_timeout (const ACE_Time_Value &, const void *);
  private:
    /// Maintains a handle that actually process the event
    Sender_exec_i &callback_;
    CORBA::UShort last_iter_;
  };

  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);

    virtual ::CORBA::UShort keys (void);

    virtual void keys (::CORBA::UShort keys);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    void start (void);
    void run (void);
    void update_one (CORBA::UShort iter);

  private:
    ::QFLS_Test::CCM_Sender_Context_var context_;
    ::QFLS_Test::QueryFilterListenStateTestConnector::Updater_var updater_;

    UpdateTicker *ticker_;

    CORBA::UShort iterations_;
    CORBA::UShort keys_;
 };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_QFLS_Test_Sender_Impl (void);
}

#endif /* ifndef */

