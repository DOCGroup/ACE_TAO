// -*- C++ -*-
//
// $Id$

#include "LNE_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_LNE_Test_Receiver_Impl
{
  //============================================================
  // ListenNotEnabledTest_Listener_exec_i
  //============================================================
  ListenNotEnabledTest_Listener_exec_i::ListenNotEnabledTest_Listener_exec_i (
                                              Atomic_ULong &received_one_by_one,
                                              Atomic_ULong &received_many_by_many)
    : received_one_by_one_ (received_one_by_one),
      received_many_by_many_ (received_many_by_many)
  {
  }

  ListenNotEnabledTest_Listener_exec_i::~ListenNotEnabledTest_Listener_exec_i (void)
  {
  }

  void
  ListenNotEnabledTest_Listener_exec_i::on_one_data (
                                  const ListenNotEnabledTest & /*an_instance*/,
                                  const ::CCM_DDS::ReadInfo & /*info*/)
  {
    ++this->received_one_by_one_;
  }

  void
  ListenNotEnabledTest_Listener_exec_i::on_many_data (
                                  const ListenNotEnabledTest_Seq & /*an_instance*/,
                                  const ::CCM_DDS::ReadInfoSeq & /*info*/)
  {
    ++this->received_many_by_many_;
  }

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (
            Atomic_Bool &data_received)
    : data_received_ (data_received)
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
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  status_kind)
  {
    if (status_kind == ::DDS::DATA_ON_READERS_STATUS)
      {
        this->data_received_ = true;
      }
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : received_one_by_one_ (0),
      received_many_by_many_ (0),
      data_received_ (false)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Port operations.
  ::CCM_DDS::ListenNotEnabledTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    return new ListenNotEnabledTest_Listener_exec_i (
                this->received_one_by_one_,
                this->received_many_by_many_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_listen_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->data_received_);
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::LNE_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::DataListenerControl_var dlc =
        this->context_->get_connection_info_listen_data_control ();
    dlc->mode (::CCM_DDS::NOT_ENABLED);
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    if (this->received_one_by_one_.value () > 0)
      {
        CIAO_ERROR ((LM_ERROR, "ERROR: NOT ENABLED: "
                               "Received samples on one_by_one "
                               "callback while mode is NOT_ENABLED "
                               "expected <0> - received <%u>\n",
                               this->received_one_by_one_.value ()));
      }
    if (this->received_many_by_many_.value () > 0)
      {
        CIAO_ERROR ((LM_ERROR, "ERROR: NOT ENABLED: "
                               "Received samples on many_by_many "
                               "callback while mode is NOT_ENABLED "
                               "expected <0> - received <%u>\n",
                               this->received_many_by_many_.value ()));
      }
    if (this->data_received_.value ())
      {
        CIAO_ERROR ((LM_ERROR, "ERROR: NOT ENABLED: "
                               "Received DATA_ON_READERS_STATUS on "
                               "ConnectorStatusListener\n"));
      }
    if (this->received_one_by_one_.value () == 0   &&
        this->received_many_by_many_.value () == 0 &&
        !this->data_received_.value ())
      {
        CIAO_DEBUG ((LM_DEBUG, "NOT ENABLED: "
                               "Received no data at all."
                               "Test passed!\n"));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LNE_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

