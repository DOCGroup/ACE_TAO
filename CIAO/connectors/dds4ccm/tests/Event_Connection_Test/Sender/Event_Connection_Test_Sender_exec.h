// -*- C++ -*-
// $Id$

#ifndef EVENT_CONNECTION_SENDER_EXEC_H_
#define EVENT_CONNECTION_SENDER_EXEC_H_


#include "Event_Connection_Test_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

namespace CIAO_Event_Connection_Test_Sender_Impl
{
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

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Event_Connection_Test::CCM_Sender_Context_var context_;
    ::Event_ConnectionConnector::Writer_var writer_;
    DDS::DataWriter_var writer_dds_datawriter_;

    bool writer_ok_;
    bool writer_dds_datawriter_ok_;
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Event_Connection_Test_Sender_Impl (void);
}

#endif /* ifndef */

