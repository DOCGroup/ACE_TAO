// -*- C++ -*-
//
// $Id$

#include "Event_Connection_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"

namespace CIAO_Event_Connection_Test_Receiver_Impl
{
  //============================================================
  // Component Executor Implementation Class: Receiver_exec_i
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : getter_ok_ (false),
      reader_listen_ok_ (false),
      dds_data_reader_listen_ok_ (false),
      data_listener_control_ok_ (false),
      raw_listener_created_ (false),
      listen_port_status_created_ (false),
      get_port_status_created_ (false),
      get_status_listener_created_ (false)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Port operations.
  ::CCM_DDS::Event_ConnectionTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new Event_ConnectionTest RAW listener\n")));
    this->raw_listener_created_ = true;
    return ::CCM_DDS::Event_ConnectionTest::CCM_Listener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener for DDS_Listen\n")));
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
    this->getter_ =
        this->context_->get_connection_info_get_fresh_data ();
    if (!CORBA::is_nil (this->getter_))
      {
        this->getter_ok_ = true;
      }
    this->reader_listen_ =
        this->context_->get_connection_info_listen_data ();
    if (!CORBA::is_nil (this->reader_listen_))
      {
        this->reader_listen_ok_ = true;
      }
    this->dds_data_reader_listen_ =
        this->context_->get_connection_info_listen_dds_entity ();
    if (!CORBA::is_nil (this->dds_data_reader_listen_))
      {
        this->dds_data_reader_listen_ok_ = true;
      }
    ::CCM_DDS::DataListenerControl_var lc =
        this->context_->get_connection_info_listen_data_control ();

    if (!CORBA::is_nil (lc.in ()))
      {
        this->data_listener_control_ok_ = true;
        lc->mode (::CCM_DDS::ONE_BY_ONE);
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
    if (!this->getter_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get getter failed\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Receiver : Get getter passed\n")));
      }
    if (!this->reader_listen_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get reader of DDS_Listen failed\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Receiver : Get reader of DDS_Listen passed\n")));
      }
    if (!this->dds_data_reader_listen_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get dds reader of DDS_Listen failed\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Receiver : Get dds reader of DDS_Listen passed\n")));
      }
    if (!this->data_listener_control_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get data listen control of DDS_Listen failed\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Receiver : Get data listen control of DDS_Listen passed\n")));
      }
    if (!this->raw_listener_created_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : listener not created\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Receiver : Listener was created\n")));
      }
    if (!this->listen_port_status_created_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : PortStatusListener of DDS_Listen not created\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Receiver : PortStatusListener of DDS_Listen was created\n")));
      }
    if (!this->get_port_status_created_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : PortStatusListener of DDS_Get not created\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Receiver : PortStatusListener of DDS_Get was created\n")));
      }
    if (!this->get_status_listener_created_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : ConnectorStatusListener not created\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Receiver : ConnectorStatusListener was created\n")));
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

