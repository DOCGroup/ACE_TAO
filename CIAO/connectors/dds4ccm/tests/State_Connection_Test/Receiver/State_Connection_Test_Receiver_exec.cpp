// -*- C++ -*-
//
// $Id$

#include "State_Connection_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"


namespace CIAO_State_Connection_Test_Receiver_Impl
{
  //============================================================
  // Component Executor Implementation Class: Receiver_exec_i
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : //DDS_Read
      reader_data_ok_ (false),
      reader_dds_entity_ok_ (false),
      reader_filter_config_ok_ (false),
      //DDS_Get
      getter_data_ok_ (false),
      getter_fresh_data_ok_ (false),
      getter_dds_entity_ok_ (false),
      getter_filter_config_ok_ (false),
      //DDS_Listen
      listen_data_ok_ (false),
      listen_data_control_ok_ (false),
      listen_dds_entity_ok_ (false),
      listen_filter_config_ok_ (false),
      //DDS_StateListen
      statelisten_data_ok_ (false),
      statelisten_data_control_ok_ (false),
      statelisten_dds_entity_ok_ (false),
      statelisten_filter_config_ok_ (false),
      //Provides for DDS_Read
      reader_port_status_created_ (false),
      //Provides for DDS_Get
      getter_port_status_created_ (false),
      //Provides for DDS_Listen
      listen_data_listener_created_ (false),
      listen_port_status_created_ (false),
      //Provides for DDS_StateListen
      statelisten_data_listener_created_ (false),
      statelisten_port_status_created_ (false)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_read_status (void)
  {
    this->reader_port_status_created_ = true;
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  //Provides for DDS_Get
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    this->getter_port_status_created_ = true;
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  //Provides for DDS_Listen
  ::State_Connection_Test::State_ConnectionTestConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener ()
  {
    this->listen_data_listener_created_ = true;
    return ::State_Connection_Test
             ::State_ConnectionTestConnector
             ::CCM_Listener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    this->listen_port_status_created_ = true;
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  //Provides for DDS_StateListen
  ::State_Connection_Test::State_ConnectionTestConnector::CCM_StateListener_ptr
  Receiver_exec_i::get_info_statelisten_data_listener (void)
  {
    this->statelisten_data_listener_created_ = true;
    return ::State_Connection_Test
             ::State_ConnectionTestConnector
             ::CCM_StateListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_statelisten_status (void)
  {
    this->statelisten_port_status_created_ = true;
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::State_Connection_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    //DDS_Read
    ::State_Connection_Test::State_ConnectionTestConnector::Reader_var reader_data =
      this->context_->get_connection_info_read_data ();
    this->reader_data_ok_ = ! ::CORBA::is_nil (reader_data);

    ::DDS::DataReader_var reader_dds_entity =
      this->context_->get_connection_info_read_dds_entity ();
    this->reader_dds_entity_ok_ = ! ::CORBA::is_nil (reader_dds_entity);
    ::CCM_DDS::ContentFilterSetting_var reader_filter_config =
      this->context_->get_connection_info_read_filter_config ();
    this->reader_filter_config_ok_ = ! ::CORBA::is_nil (reader_filter_config);

    //DDS_Get
    ::State_Connection_Test::State_ConnectionTestConnector::Reader_var getter_data =
      this->context_->get_connection_info_get_data ();
    this->getter_data_ok_ = ! ::CORBA::is_nil (getter_data);
    ::State_Connection_Test::State_ConnectionTestConnector::Getter_var getter_fresh_data =
      this->context_->get_connection_info_get_fresh_data ();
    this->getter_fresh_data_ok_ = ! ::CORBA::is_nil (getter_fresh_data);
    ::DDS::DataReader_var getter_dds_entity =
      this->context_->get_connection_info_get_dds_entity ();
    this->getter_dds_entity_ok_ = ! ::CORBA::is_nil (getter_dds_entity);
    ::CCM_DDS::ContentFilterSetting_var getter_filter_config =
      this->context_->get_connection_info_get_filter_config ();
    this->getter_filter_config_ok_ = ! ::CORBA::is_nil (getter_filter_config);

    //DDS_Listen
    ::State_Connection_Test::State_ConnectionTestConnector::Reader_var listen_data =
      this->context_->get_connection_info_listen_data ();
    this->listen_data_ok_ = ! ::CORBA::is_nil (listen_data);
    ::CCM_DDS::DataListenerControl_var  listen_data_control =
      this->context_->get_connection_info_listen_data_control ();
    this->listen_data_control_ok_ = ! ::CORBA::is_nil (listen_data_control);
    ::DDS::DataReader_var listen_dds_entity =
      this->context_->get_connection_info_listen_dds_entity ();
    this->listen_dds_entity_ok_ = ! ::CORBA::is_nil (listen_dds_entity);
    ::CCM_DDS::ContentFilterSetting_var listen_filter_config =
      this->context_->get_connection_info_listen_filter_config ();
    this->listen_filter_config_ok_ = ! ::CORBA::is_nil (listen_filter_config);

    //DDS_StateListen
    ::State_Connection_Test::State_ConnectionTestConnector::Reader_var statelisten_data =
      this->context_->get_connection_info_statelisten_data ();
    this->statelisten_data_ok_ = ! ::CORBA::is_nil (statelisten_data);
    ::CCM_DDS::StateListenerControl_var statelisten_data_control =
      this->context_->get_connection_info_statelisten_data_control ();
    this->statelisten_data_control_ok_ = ! ::CORBA::is_nil (statelisten_data_control);
    ::DDS::DataReader_var statelisten_dds_entity =
      this->context_->get_connection_info_statelisten_dds_entity ();
    this->statelisten_dds_entity_ok_ = ! ::CORBA::is_nil (statelisten_dds_entity);
    ::CCM_DDS::ContentFilterSetting_var statelisten_filter_config =
      this->context_->get_connection_info_statelisten_filter_config ();
    this->statelisten_filter_config_ok_ = ! ::CORBA::is_nil (statelisten_filter_config);
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    //DDS_Read
    if (!this->reader_data_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get reader of DDS_Read failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get reader of DDS_Read passed\n")));
      }
    if (!this->reader_dds_entity_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get DDS reader of DDS_Read failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get DDS reader of DDS_Read passed\n")));
      }
    if (!this->reader_filter_config_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get filter config of DDS_Read failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get filter config of DDS_Read passed\n")));
      }
    //DDS_Get
    if (!this->getter_data_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get reader of DDS_Get failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get reader of DDS_Get passed\n")));
      }

    if (!this->getter_fresh_data_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get getter of DDS_Get failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get getter of DDS_Get passed\n")));
      }
    if (!this->getter_dds_entity_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get DDS reader of DDS_Get failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get DDS reader of DDS_Get passed\n")));
      }
    if (!this->getter_filter_config_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get filter config of DDS_Get failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get filter config of DDS_Get passed\n")));
      }
    //DDS_Listen
    if (!this->listen_data_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get reader of DDS_Listen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get reader of DDS_Listen passed\n")));
      }
    if (!this->listen_data_control_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get data control of DDS_Listen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get data control of DDS_Listen passed\n")));
      }
    if (!this->listen_dds_entity_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get DDS reader of DDS_Listen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get DDS reader of DDS_Listen passed\n")));
      }
    if (!this->listen_filter_config_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get filter config of DDS_Listen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get filter config of DDS_Listen passed\n")));
      }
    //DDS_StateListen
    if (!this->statelisten_data_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get reader of DDS_StateListen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get reader of DDS_StateListen passed\n")));
      }
    if (!this->statelisten_data_control_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get data control of DDS_StateListen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get data control of DDS_StateListen passed\n")));
      }
    if (!this->statelisten_dds_entity_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get DDS reader of DDS_StateListen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get DDS reader of DDS_StateListen passed\n")));
      }
    if (!this->statelisten_filter_config_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get filter config of DDS_StateListen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get filter config of DDS_StateListen passed\n")));
      }
    //Provides for DDS_Read
    if (!this->reader_port_status_created_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : PortStatusListener of DDS_Read not created\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : PortStatusListener of DDS_Read was created\n")));
      }
    //Provides for DDS_Get
    if (!this->getter_port_status_created_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : PortStatusListener of DDS_Get not created\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : PortStatusListener of DDS_Get was created\n")));
      }
    //Provides for DDS_Listen
    if (!this->listen_data_listener_created_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : DataListener of DDS_Listen not created\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : DataListener of DDS_Listen was created\n")));
      }
    if (!this->listen_port_status_created_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : PortStatusListener of DDS_Listen not created\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : PortStatusListener of DDS_Listen was created\n")));
      }
    //Provides for DDS_StateListen
    if (!this->statelisten_data_listener_created_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : DataListener of DDS_StateListen not created\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : DataListener of DDS_StateListen was created\n")));
      }
    if (!this->statelisten_port_status_created_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : PortStatusListener of DDS_StateListen not created\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : PortStatusListener of DDS_StateListen was created\n")));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_State_Connection_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

