// -*- C++ -*-
// $Id$

#ifndef CIAO_SENDER_EXEC_H_
#define CIAO_SENDER_EXEC_H_


#include "CoherentWrite_Test_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_CoherentWrite_Test_Sender_Impl
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
    : public virtual ::CCM_CoherentWriteRestarter,
      public virtual ::CORBA::LocalObject
  {
  public:
    Restarter_exec_i (Sender_exec_i & callback);
    virtual ~Restarter_exec_i (void);

    virtual void restart_write ();

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

    virtual ::CCM_CoherentWriteRestarter_ptr
    get_restart_writer (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

    void start (void);
    void restart (void);

  private:
    ::CoherentWrite_Test::CCM_Sender_Context_var context_;
    CoherentWriteTestConnector::Writer_var writer_;
    CoherentWriteStarter_var starter_;

    CORBA::UShort iterations_;
    CORBA::UShort run_;
    CORBA::ULong total_iter;
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoherentWrite_Test_Sender_Impl (void);
}

#endif /* ifndef */

