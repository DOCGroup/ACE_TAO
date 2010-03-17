// -*- C++ -*-
//
// $Id$

#include "Event_Connection_Test_Receiver_exec.h"
#include "ace/Log_Msg.h"

namespace CIAO_Event_Connection_Test_Receiver_Impl
{
  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : //DDS_Get
      getter_ok_ (false),
      getter_dds_data_reader_ok_ (false),
      getter_reader_ok_ (false),
      //DDS_Listen
      listen_data_control_ok_ (false),
      listen_reader_ok_ (false),
      listen_dds_data_reader_ok_ (false),
      //Provides
      listen_port_status_created_ (false),
      get_port_status_created_ (false),
      get_status_listener_created_ (false),
      raw_listener_created_ (false)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Port operations.
  ::CCM_DDS::Event_ConnectionTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new Event_ConnectionTest RAW listener\n")));
    this->raw_listener_created_ = true;
    return ::CCM_DDS::Event_ConnectionTest::CCM_Listener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener for DDS_Listen\n")));
    this->listen_port_status_created_ = true;
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    this->get_port_status_created_ = true;
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_status_listener(void)
  {
    this->get_status_listener_created_ = true;
    return ::CCM_DDS::CCM_ConnectorStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Event_Connection_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    //DDS_Get
    this->getter_getter_ =
        this->context_->get_connection_info_get_fresh_data ();
    if (! ::CORBA::is_nil (this->getter_getter_))
      {
        this->getter_ok_ = true;
      }
    this->getter_dds_data_reader_ =
        this->context_->get_connection_info_get_dds_entity ();
    if (! ::CORBA::is_nil (this->getter_dds_data_reader_))
      {
        this->getter_dds_data_reader_ok_ = true;
      }
    this->getter_reader_ =
        this->context_->get_connection_info_get_data ();
    if (! ::CORBA::is_nil (this->getter_reader_))
      {
        this->getter_reader_ok_ = true;
      }
    //DDS_Listen
    this->listen_data_control_ =
        this->context_->get_connection_info_listen_data_control ();

    if (! ::CORBA::is_nil (this->listen_data_control_))
      {
        this->listen_data_control_ok_ = true;
      }
    this->listen_reader_ =
        this->context_->get_connection_info_listen_data ();
    if (! ::CORBA::is_nil (this->listen_reader_))
      {
        this->listen_reader_ok_ = true;
      }
    this->listen_dds_data_reader_ =
        this->context_->get_connection_info_listen_dds_entity ();
    if (! ::CORBA::is_nil (this->listen_dds_data_reader_))
      {
        this->listen_dds_data_reader_ok_ = true;
      }
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
    //DDS_Get
    if (!this->getter_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get getter failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get getter passed\n")));
      }
    if (!this->getter_dds_data_reader_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get DDS reader of DDS_Get failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get DDS reader of DDS_Get passed\n")));
      }
    if (!this->getter_reader_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get reader of DDS_Get failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get reader of DDS_Get passed\n")));
      }
    //DDS_Listen
    if (!this->listen_data_control_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get data listen control of DDS_Listen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get data listen control of DDS_Listen passed\n")));
      }
    if (!this->listen_reader_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get reader of DDS_Listen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get reader of DDS_Listen passed\n")));
      }
    if (!this->listen_dds_data_reader_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get dds reader of DDS_Listen failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Get dds reader of DDS_Listen passed\n")));
      }
    //Provides
    if (!this->raw_listener_created_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : listener not created\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : Listener was created\n")));
      }
    if (!this->listen_port_status_created_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : PortStatusListener of DDS_Listen not created\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : PortStatusListener of DDS_Listen was created\n")));
      }
    if (!this->get_port_status_created_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : PortStatusListener of DDS_Get not created\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : PortStatusListener of DDS_Get was created\n")));
      }
    if (!this->get_status_listener_created_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : ConnectorStatusListener not created\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Receiver : ConnectorStatusListener was created\n")));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Event_Connection_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

