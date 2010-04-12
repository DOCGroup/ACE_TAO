// -*- C++ -*-
// $Id$


//Test different topic types, with DDS_EVENT connectors,  look for 'PUBLICATION_MATCHED_STATUS
#include "VTopic_Sender_exec.h"
#include "ace/Log_Msg.h"
#include "dds4ccm/impl/dds/Utils.h"

namespace CIAO_VTopic_Sender_Impl
{
  //============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (
    Atomic_Boolean &publication_matched)
    : publication_matched_ (publication_matched)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
    ::DDS::Topic_ptr /*the_topic*/,
    const DDS::InconsistentTopicStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
    const DDS::RequestedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected(
    ::DDS::DataReader_ptr /*the_reader*/,
    const DDS::SampleRejectedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
    ::DDS::DataWriter_ptr /*the_writer*/,
    const DDS::OfferedDeadlineMissedStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
    ::DDS::DataWriter_ptr /*the_writer*/,
    const DDS::OfferedIncompatibleQosStatus & /*status*/)
  {
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr the_entity,
    ::DDS::StatusKind  status_kind)
  {
    if (! ::CORBA::is_nil (the_entity) &&
        status_kind == ::DDS::PUBLICATION_MATCHED_STATUS)
      {
        this->publication_matched_ = true;
      }
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : publication_matched_ (false)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_connector_status (void)

  {
    return new ConnectorStatusListener_exec_i (this->publication_matched_);
  }

  // Supported operations and attributes.
  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::VTopic::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
   }

  void
  Sender_exec_i::configuration_complete (void)
  {
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if(!this->publication_matched_.value ())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                               ACE_TEXT ("states 'PUBLICATION_MATCHED_STATUS' in Sender\n")
                    ));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Received the expected ")
                               ACE_TEXT ("'PUBLICATION_MATCHED_STATUS' in Sender\n")
                    ));
      }
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_VTopic_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();
    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
