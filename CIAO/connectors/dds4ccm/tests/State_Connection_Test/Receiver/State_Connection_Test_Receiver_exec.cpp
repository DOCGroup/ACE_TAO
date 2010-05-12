// -*- C++ -*-
//
// $Id$

#include "State_Connection_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"

namespace CIAO_State_Connection_Test_Receiver_Impl
{
  //============================================================
  // Component Executor Implementation Class: Receiver_exec_i
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : //DDS_Read
      reader_data_ok_ (false),
      reader_dds_entity_ok_ (false),
      //DDS_Get
      getter_data_ok_ (false),
      getter_fresh_data_ok_ (false),
      getter_dds_entity_ok_ (false),
      //DDS_Listen
      listen_data_ok_ (false),
      listen_data_control_ok_ (false),
      listen_dds_entity_ok_ (false),
      //DDS_StateListen
      statelisten_data_ok_ (false),
      statelisten_data_control_ok_ (false),
      statelisten_dds_entity_ok_ (false),
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
  ::State_Connection_Test
    ::State_ConnectionTestConnector
    ::CCM_Listener_ptr
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
  ::State_Connection_Test
    ::State_ConnectionTestConnector
    ::CCM_StateListener_ptr
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
    this->reader_data_ =
      this->context_->get_connection_info_read_data ();
    this->reader_data_ok_ = ! ::CORBA::is_nil (this->reader_data_);

    this->reader_dds_entity_ =
      this->context_->get_connection_info_read_dds_entity ();
    this->reader_dds_entity_ok_ = ! ::CORBA::is_nil (this->reader_dds_entity_);

    //DDS_Get
    this->getter_data_ =
      this->context_->get_connection_info_get_data ();
    this->getter_data_ok_ = ! ::CORBA::is_nil (this->getter_data_);
    this->getter_fresh_data_ =
      this->context_->get_connection_info_get_fresh_data ();
    this->getter_fresh_data_ok_ = ! ::CORBA::is_nil (this->getter_fresh_data_);
    this->getter_dds_entity_ =
      this->context_->get_connection_info_get_dds_entity ();
    this->getter_dds_entity_ok_ = ! ::CORBA::is_nil (this->getter_dds_entity_);

    //DDS_Listen
    this->listen_data_ =
      this->context_->get_connection_info_listen_data ();
    this->listen_data_ok_ = ! ::CORBA::is_nil (this->listen_data_);
    this->listen_data_control_ =
      this->context_->get_connection_info_listen_data_control ();
    this->listen_data_control_ok_ = ! ::CORBA::is_nil (this->listen_data_control_);
    this->listen_dds_entity_ =
      this->context_->get_connection_info_listen_dds_entity ();
    this->listen_dds_entity_ok_ = ! ::CORBA::is_nil (this->listen_dds_entity_);

    //DDS_StateListen
    this->statelisten_data_ =
      this->context_->get_connection_info_statelisten_data ();
    this->statelisten_data_ok_ = ! ::CORBA::is_nil (this->statelisten_data_);
    this->statelisten_data_control_ =
      this->context_->get_connection_info_statelisten_data_control ();
    this->statelisten_data_control_ok_ = ! ::CORBA::is_nil (this->statelisten_data_control_);
    this->statelisten_dds_entity_ =
      this->context_->get_connection_info_statelisten_dds_entity ();
    this->statelisten_dds_entity_ok_ = ! ::CORBA::is_nil (this->statelisten_dds_entity_);
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

