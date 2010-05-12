// -*- C++ -*-
// $Id$

#ifndef STATE_CONNECTION_SENDER_EXEC_H_
#define STATE_CONNECTION_SENDER_EXEC_H_


#include "State_Connection_Test_SenderEC.h"

#include /**/ "Sender_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

#include <map>

namespace CIAO_State_Connection_Test_Sender_Impl
{
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
    ::State_Connection_Test::CCM_Sender_Context_var context_;
    ::State_Connection_Test
      ::State_ConnectionTestConnector
      ::Updater_var       updater_;
    ::DDS::DataWriter_var updater_dds_datawriter_;

    bool updater_ok_;
    bool updater_dds_datawriter_ok_;
  };

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_State_Connection_Test_Sender_Impl (void);
}

#endif /* ifndef */

