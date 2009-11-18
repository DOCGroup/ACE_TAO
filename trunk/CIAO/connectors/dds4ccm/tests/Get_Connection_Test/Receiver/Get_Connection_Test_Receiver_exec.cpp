// -*- C++ -*-
//
// $Id$

#include "Get_Connection_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"

namespace CIAO_Get_Connection_Test_Receiver_Impl
{
  //============================================================
  // Facet Executor Implementation Class: Get_ConnectionTest_Listener_exec_i
  //============================================================

  Get_ConnectionTest_Listener_exec_i::Get_ConnectionTest_Listener_exec_i ()
  {
  }

  Get_ConnectionTest_Listener_exec_i::~Get_ConnectionTest_Listener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::Get_ConnectionTest_Listener
  void
  Get_ConnectionTest_Listener_exec_i::on_many_data (
    const Get_ConnectionTest_Seq & ,
    const ::CCM_DDS::ReadInfoSeq & )
  {
  }

  void
  Get_ConnectionTest_Listener_exec_i::on_one_data (
    const Get_ConnectionTest &  ,
    const ::CCM_DDS::ReadInfo & )
  {
  }

  //============================================================
  // Facet Executor Implementation Class: PortStatusListener_exec_i
  //============================================================

  PortStatusListener_exec_i::PortStatusListener_exec_i (void)
  {
  }

  PortStatusListener_exec_i::~PortStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PortStatusListener

  void
  PortStatusListener_exec_i::on_requested_deadline_missed (
    ::DDS::DataReader_ptr ,
    const ::DDS::RequestedDeadlineMissedStatus & )
  {
  }

  void
  PortStatusListener_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr ,
    const ::DDS::SampleLostStatus & )
  {
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iGet_ConnectionTest_Listener_exec_i ();
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : getter_ok_ (false),
      reader_listen_ok_ (false),
      dds_data_reader_listen_ok_ (false),
      data_listener_control_ok_ (false),
      raw_listener_created_ (false),
      listen_port_status_created_ (false),
      get_port_status_created_ (false)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Port operations.
  ::CCM_DDS::Get_ConnectionTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new Get_ConnectionTest RAW listener\n")));
    this->raw_listener_created_ = true;
    return new Get_ConnectionTest_Listener_exec_i ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener for DDS_Listen\n")));
    this->listen_port_status_created_ = true;
    return new PortStatusListener_exec_i ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener for DDS_Get\n")));
    this->get_port_status_created_ = true;
    return new PortStatusListener_exec_i ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Get_Connection_Test::CCM_Receiver_Context::_narrow (ctx);
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
    if (!this->reader_listen_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get reader of DDS_Listen failed\n")));
      }
    if (!this->dds_data_reader_listen_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get dds reader of DDS_Listen failed\n")));
      }
    if (!this->data_listener_control_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : get data listen control of DDS_Listen failed\n")));
      }
    if (!this->raw_listener_created_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : raw listener not created\n")));
      }
    if (!this->listen_port_status_created_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : PortStatusListener of DDS_Listen not created\n")));
      }
    if (!this->get_port_status_created_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Receiver : PortStatusListener of DDS_Get not created\n")));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Get_Connection_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

