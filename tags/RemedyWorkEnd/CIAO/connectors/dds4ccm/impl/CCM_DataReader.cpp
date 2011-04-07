// $Id$

#include "CCM_DataReader.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

CIAO::DDS4CCM::CCM_DataReader::CCM_DataReader (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::CCM_DataReader");
}

CIAO::DDS4CCM::CCM_DataReader::~CCM_DataReader (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::~CCM_DataReader");
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::enable (void)
{
  return this->dds_entity ()->enable ();
}

::DDS::StatusCondition_ptr
CIAO::DDS4CCM::CCM_DataReader::get_statuscondition (void)
{
  return this->dds_entity ()->get_statuscondition ();
}

::DDS::StatusMask
CIAO::DDS4CCM::CCM_DataReader::get_status_changes (void)
{
  return this->dds_entity ()->get_status_changes ();
}

::DDS::InstanceHandle_t
CIAO::DDS4CCM::CCM_DataReader::get_instance_handle (void)
{
  return this->dds_entity ()->get_instance_handle ();
}

::DDS::ReadCondition_ptr
CIAO::DDS4CCM::CCM_DataReader::create_readcondition (
  ::DDS::SampleStateMask sample_states,
  ::DDS::ViewStateMask view_states,
  ::DDS::InstanceStateMask instance_states)
{
  return this->dds_entity ()->create_readcondition (sample_states, view_states, instance_states);
}

::DDS::QueryCondition_ptr
CIAO::DDS4CCM::CCM_DataReader::create_querycondition (
  ::DDS::SampleStateMask sample_states,
  ::DDS::ViewStateMask view_states,
  ::DDS::InstanceStateMask instance_states,
  const char * query_expression,
  const ::DDS::StringSeq & query_parameters)
{
  return this->dds_entity ()->create_querycondition (sample_states, view_states, instance_states, query_expression, query_parameters);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::delete_readcondition (
  ::DDS::ReadCondition_ptr a_condition)
{
  return this->dds_entity ()->delete_readcondition (a_condition);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::delete_contained_entities (void)
{
  return this->dds_entity ()->delete_contained_entities ();
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::set_qos (
  const ::DDS::DataReaderQos & qos)
{
  return this->dds_entity ()->set_qos (qos);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_qos (
  ::DDS::DataReaderQos & qos)
{
  return this->dds_entity ()->get_qos (qos);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::set_listener (
  ::DDS::DataReaderListener_ptr a_listener,
  ::DDS::StatusMask mask)
{
  return this->dds_entity ()->set_listener (a_listener, mask);
}

::DDS::DataReaderListener_ptr
CIAO::DDS4CCM::CCM_DataReader::get_listener (void)
{
  return this->dds_entity ()->get_listener ();
}

::DDS::TopicDescription_ptr
CIAO::DDS4CCM::CCM_DataReader::get_topicdescription (void)
{
  return this->dds_entity ()->get_topicdescription();
}

::DDS::Subscriber_ptr
CIAO::DDS4CCM::CCM_DataReader::get_subscriber (void)
{
  return this->dds_entity ()->get_subscriber ();
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_sample_rejected_status (
  ::DDS::SampleRejectedStatus & status)
{
  return this->get_dds_entity ()->get_sample_rejected_status (status);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_liveliness_changed_status (
  ::DDS::LivelinessChangedStatus & status)
{
  return this->dds_entity ()->get_liveliness_changed_status (status);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_requested_deadline_missed_status (
  ::DDS::RequestedDeadlineMissedStatus & status)
{
  return this->dds_entity ()->get_requested_deadline_missed_status (status);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_requested_incompatible_qos_status (
  ::DDS::RequestedIncompatibleQosStatus & status)
{
  return this->dds_entity ()->get_requested_incompatible_qos_status (status);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_subscription_matched_status (
  ::DDS::SubscriptionMatchedStatus & status)
{
  return this->dds_entity ()->get_subscription_matched_status (status);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_sample_lost_status (
  ::DDS::SampleLostStatus & status)
{
  return this->dds_entity ()->get_sample_lost_status (status);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::wait_for_historical_data (
  const ::DDS::Duration_t & max_wait)
{
  return this->dds_entity ()->wait_for_historical_data (max_wait);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_matched_publications (
  ::DDS::InstanceHandleSeq & publication_handles)
{
  return this->dds_entity ()->get_matched_publications (publication_handles);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_matched_publication_data (
  ::DDS::PublicationBuiltinTopicData & publication_data,
  const ::DDS::InstanceHandle_t & publication_handle)
{
  return this->dds_entity ()->get_matched_publication_data (publication_data, publication_handle);
}


void
CIAO::DDS4CCM::CCM_DataReader::set_dds_entity (::DDS::DataReader_ptr dr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::set_dds_entity");
  this->dds_entity_ = ::DDS::DataReader::_duplicate (dr);
}

::DDS::DataReader_ptr
CIAO::DDS4CCM::CCM_DataReader::get_dds_entity (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::get_dds_entity");

  return ::DDS::DataReader::_duplicate (this->dds_entity_);
}

::DDS::DataReader_ptr
CIAO::DDS4CCM::CCM_DataReader::dds_entity (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::dds_entity");

  if (::CORBA::is_nil (this->dds_entity_.in ()))
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                    "CIAO::DDS4CCM::CCM_DataReader::dds_entity - "
                    "Throwing BAD_INV_ORDER.\n"));
      throw ::CORBA::BAD_INV_ORDER ();
    }
  else
    {
      return this->dds_entity_.in ();
    }
}
