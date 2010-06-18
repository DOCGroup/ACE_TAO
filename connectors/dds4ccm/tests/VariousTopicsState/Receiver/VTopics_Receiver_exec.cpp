// -*- C++ -*-
//
// $Id$

//Test for different topic types , with DDS_STATE connector, look for 'SUBSCRIPTION_MATCHED_STATUS

#include "VTopics_Receiver_exec.h"
#include "dds4ccm/impl/dds/Utils.h"


namespace CIAO_VTopics_Receiver_Impl
{
//============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (
    Atomic_Boolean &subscription_matched)
    : subscription_matched_ (subscription_matched)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic (
     ::DDS::Topic_ptr /*the_topic*/,
     const DDS::InconsistentTopicStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos (
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected (
    ::DDS::DataReader_ptr /*the_reader*/,
    const DDS::SampleRejectedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed (
    ::DDS::DataWriter_ptr /*the_writer*/,
    const DDS::OfferedDeadlineMissedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos (
    ::DDS::DataWriter_ptr /*the_writer*/,
    const DDS::OfferedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status (
    ::DDS::Entity_ptr the_entity,
    ::DDS::StatusKind  status_kind)
  {
    if (! ::CORBA::is_nil(the_entity) &&
        status_kind == DDS::SUBSCRIPTION_MATCHED_STATUS)
      {
        this->subscription_matched_ = true;
      }
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : subscription_matched_ (false)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Port operations.
  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (this->subscription_matched_);
  }

  ::VTopics::TestTopicConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return ::VTopics::TestTopicConnector::CCM_Listener::_nil ();
  }

  CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::VTopics::CCM_Receiver_Context::_narrow (ctx);
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
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
     if(!this->subscription_matched_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                              ACE_TEXT ("status 'SUBSCRIPTION_MATCHED_STATUS' ")
                              ACE_TEXT ("in Receiver\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Received the expected ")
                               ACE_TEXT ("'SUBSCRIPTION_MATCHED_STATUS' ")
                               ACE_TEXT ("in Receiver\n")
                    ));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VTopics_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
