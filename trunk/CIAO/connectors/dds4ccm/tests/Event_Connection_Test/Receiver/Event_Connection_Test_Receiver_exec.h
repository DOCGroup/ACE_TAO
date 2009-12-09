// -*- C++ -*-
// $Id$

#ifndef EVENT_CONNECTION_RECEIVER_EXEC_H_
#define EVENT_CONNECTION_RECEIVER_EXEC_H_

#include "Event_Connection_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

namespace CIAO_Event_Connection_Test_Receiver_Impl
{
  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Port operations.
    virtual ::CCM_DDS::Event_ConnectionTest::CCM_Listener_ptr
    get_info_listen_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_listen_status (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_get_status (void);

    virtual CCM_DDS::CCM_ConnectorStatusListener_ptr
    get_status_listener(void);

    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Event_Connection_Test::CCM_Receiver_Context_var context_;
    ::CCM_DDS::Event_ConnectionTest::Reader_var reader_;
    ::CCM_DDS::Event_ConnectionTest::Getter_var getter_;
    ::CCM_DDS::Event_ConnectionTest::Reader_var reader_listen_;
    ::DDS::DataReader_var dds_data_reader_;
    ::DDS::DataReader_var dds_data_reader_listen_;
    
    bool getter_ok_;
    bool reader_listen_ok_;
    bool dds_data_reader_listen_ok_;
    bool data_listener_control_ok_;
    bool raw_listener_created_;
    bool listen_port_status_created_;
    bool get_port_status_created_;
    bool get_status_listener_created_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Event_Connection_Test_Receiver_Impl (void);
}

#endif /* ifndef */

