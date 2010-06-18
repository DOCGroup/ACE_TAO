// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_

#include "CoherentUpdate_Test_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
  // Restarter_exec_i
  //============================================================
  class Restarter_exec_i
    : public virtual ::CCM_CoherentUpdateRestarter,
      public virtual ::CORBA::LocalObject
  {
  public:
    Restarter_exec_i (Sender_exec_i & callback);
    virtual ~Restarter_exec_i (void);

    virtual void restart_update ();

  private:
    Sender_exec_i &callback_;
  };

  //============================================================
  // Sender_exec_i
  //============================================================
  class Sender_exec_i
    : public virtual Sender_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Sender_exec_i (void);
    virtual ~Sender_exec_i (void);

    virtual ::CORBA::UShort iterations (void);

    virtual void iterations (::CORBA::UShort iterations);

    virtual ::CCM_CoherentUpdateRestarter_ptr
    get_restart_updater (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    void start (void);
    void restart (void);

  private:
    ::CoherentUpdate_Test::CCM_Sender_Context_var context_;
    ::CoherentUpdate_Test::Updater_var updater_;
    CoherentUpdateStarter_var starter_;

    CORBA::UShort iterations_;
    CORBA::UShort run_;
    CORBA::ULong total_iter;

    WriteHandler *wh_;
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoherentUpdate_Test_Sender_Impl (void);
}

#endif /* ifndef */

