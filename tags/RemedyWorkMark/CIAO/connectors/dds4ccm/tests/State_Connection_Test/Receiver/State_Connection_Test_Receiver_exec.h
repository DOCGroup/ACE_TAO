// -*- C++ -*-
// $Id$

#ifndef STATE_CONNECTION_RECEIVER_EXEC_H_
#define STATE_CONNECTION_RECEIVER_EXEC_H_

#include "State_Connection_Test_ReceiverEC.h"

#include /**/ "Receiver_exec_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"



namespace CIAO_State_Connection_Test_Receiver_Impl
{
  class RECEIVER_EXEC_Export Receiver_exec_i
    : public virtual Receiver_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Receiver_exec_i (void);
    virtual ~Receiver_exec_i (void);

    // Port operations.
    //Provides for DDS_Read
    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_read_status (void);

    //Provides for DDS_Get
    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_get_status (void);

    //Provides for DDS_Listen
    virtual ::State_Connection_Test
            ::State_ConnectionTestConnector
            ::CCM_Listener_ptr
    get_info_listen_data_listener ();

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_listen_status (void);

    //Provides for DDS_StateListen
    virtual ::State_Connection_Test
            ::State_ConnectionTestConnector
            ::CCM_StateListener_ptr
    get_info_statelisten_data_listener (void);

    virtual ::CCM_DDS::CCM_PortStatusListener_ptr
    get_info_statelisten_status (void);


    // Operations from Components::SessionComponent.
    virtual void
    set_session_context (
      ::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::State_Connection_Test::CCM_Receiver_Context_var context_;

    //DDS_Read
    bool reader_data_ok_;
    bool reader_dds_entity_ok_;
    bool reader_filter_config_ok_;

    //DDS_Get
    bool getter_data_ok_;
    bool getter_fresh_data_ok_;
    bool getter_dds_entity_ok_;
    bool getter_filter_config_ok_;

    //DDS_Listen
    bool listen_data_ok_;
    bool listen_data_control_ok_;
    bool listen_dds_entity_ok_;
    bool listen_filter_config_ok_;

    //DDS_StateListen
    bool statelisten_data_ok_;
    bool statelisten_data_control_ok_;
    bool statelisten_dds_entity_ok_;
    bool statelisten_filter_config_ok_;

    //Provides for DDS_Read
    bool reader_port_status_created_;

    //Provides for DDS_Get
    bool getter_port_status_created_;

    //Provides for DDS_Listen
    bool listen_data_listener_created_;
    bool listen_port_status_created_;

    //Provides for DDS_StateListen
    bool statelisten_data_listener_created_;
    bool statelisten_port_status_created_;
  };

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_State_Connection_Test_Receiver_Impl (void);
}

#endif /* ifndef */

