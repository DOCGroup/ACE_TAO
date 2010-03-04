// -*- C++ -*-
//
// $Id$

#include "Latency_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "ace/High_Res_Timer.h"

namespace CIAO_Latency_Test_Receiver_Impl
{
  //============================================================
  // LatencyTest_Listener_exec_i
  //============================================================
  LatencyTest_Listener_exec_i::LatencyTest_Listener_exec_i (
            Receiver_exec_i &callback)
    : callback_ (callback)
  {
  }

  LatencyTest_Listener_exec_i::~LatencyTest_Listener_exec_i (void)
  {
  }

  void
  LatencyTest_Listener_exec_i::on_one_data (
                                  const LatencyTest & an_instance,
                                  const ::CCM_DDS::ReadInfo & /*info*/)
  {
    // Send back a packet if this is a ping
    if (an_instance.ping == 1L)
      {
         this->callback_.write_one(an_instance);
      }
   }

  void
  LatencyTest_Listener_exec_i::on_many_data (
                                  const LatencyTest_Seq & /*an_instance*/,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
  }

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (void)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr ,
     const DDS::InconsistentTopicStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr ,
     const DDS::RequestedIncompatibleQosStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr ,
     const DDS::SampleRejectedStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr ,
     const DDS::OfferedDeadlineMissedStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr ,
     const DDS::OfferedIncompatibleQosStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr ,
    ::DDS::StatusKind /*status_kind*/)
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : count_ (0L)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  void
  Receiver_exec_i::write_one ( LatencyTest an_instance)
  {
    ++this->count_;
    //change ping , so this receiever doesn't sent this message back again
    an_instance.ping = 0L;
    this->writer_->write_one (an_instance, ::DDS::HANDLE_NIL);
  }

  void
  Receiver_exec_i::start ()
  {
    ::CCM_DDS::DataListenerControl_var dlc =
            this->context_->get_connection_info_listen_data_control ();
    dlc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  

  ::CCM_DDS::LatencyTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    return new LatencyTest_Listener_exec_i (*this);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_listen_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i ();
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Latency_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    //enable the datalistener
    this->start();
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
     this->writer_ = this->context_->get_connection_write_ping_data ();
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
     ACE_DEBUG((LM_DEBUG, "SUMMARY RECEIVER:\n "
                           "Messages sent back: %u \n",
                           this->count_.value()));
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Latency_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
