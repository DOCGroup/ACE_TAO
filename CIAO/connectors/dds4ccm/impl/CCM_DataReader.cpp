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
  return this->impl ()->enable ();
}

::DDS::StatusCondition_ptr
CIAO::DDS4CCM::CCM_DataReader::get_statuscondition (void)
{
  return this->impl ()->get_statuscondition ();
}

::DDS::StatusMask
CIAO::DDS4CCM::CCM_DataReader::get_status_changes (void)
{
  return this->impl ()->get_status_changes ();
}

::DDS::InstanceHandle_t
CIAO::DDS4CCM::CCM_DataReader::get_instance_handle (void)
{
  return this->impl ()->get_instance_handle ();
}

::DDS::ReadCondition_ptr
CIAO::DDS4CCM::CCM_DataReader::create_readcondition (
  ::DDS::SampleStateMask /*sample_states*/,
  ::DDS::ViewStateMask /*view_states*/,
  ::DDS::InstanceStateMask /*instance_states*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::ReadCondition::_nil ();
}

::DDS::QueryCondition_ptr
CIAO::DDS4CCM::CCM_DataReader::create_querycondition (
  ::DDS::SampleStateMask /*sample_states*/,
  ::DDS::ViewStateMask /*view_states*/,
  ::DDS::InstanceStateMask /*instance_states*/,
  const char * /*query_expression*/,
  const ::DDS::StringSeq & /*query_parameters*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::QueryCondition::_nil ();
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::delete_readcondition (
  ::DDS::ReadCondition_ptr /*a_condition*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::delete_contained_entities (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::set_qos (
  const ::DDS::DataReaderQos & /*qos*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_qos (
  ::DDS::DataReaderQos & /*qos*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::set_listener (
  ::DDS::DataReaderListener_ptr /*a_listener*/,
  ::DDS::StatusMask /*mask*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::DataReaderListener_ptr
CIAO::DDS4CCM::CCM_DataReader::get_listener (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::DataReaderListener::_nil ();
}

::DDS::TopicDescription_ptr
CIAO::DDS4CCM::CCM_DataReader::get_topicdescription (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::TopicDescription::_nil ();
}

::DDS::Subscriber_ptr
CIAO::DDS4CCM::CCM_DataReader::get_subscriber (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::Subscriber::_nil ();
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_sample_rejected_status (
  ::DDS::SampleRejectedStatus & /*status*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_liveliness_changed_status (
  ::DDS::LivelinessChangedStatus & /*status*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_requested_deadline_missed_status (
  ::DDS::RequestedDeadlineMissedStatus & /*status*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_requested_incompatible_qos_status (
  ::DDS::RequestedIncompatibleQosStatus & /*status*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_subscription_matched_status (
  ::DDS::SubscriptionMatchedStatus & /*status*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_sample_lost_status (
  ::DDS::SampleLostStatus & /*status*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::wait_for_historical_data (
  const ::DDS::Duration_t & /*max_wait*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_matched_publications (
  ::DDS::InstanceHandleSeq & /*publication_handles*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataReader::get_matched_publication_data (
  ::DDS::PublicationBuiltinTopicData & /*publication_data*/,
  const ::DDS::InstanceHandle_t & /*publication_handle*/)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}


void
CIAO::DDS4CCM::CCM_DataReader::set_impl (::DDS::DataReader_ptr dr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::set_impl");
  this->impl_ = ::DDS::DataReader::_duplicate (dr);
}

::DDS::DataReader_ptr
CIAO::DDS4CCM::CCM_DataReader::get_impl (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::get_impl");

  return ::DDS::DataReader::_duplicate (this->impl ());
}

::DDS::DataReader_ptr
CIAO::DDS4CCM::CCM_DataReader::impl (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataReader::impl");

  if (::CORBA::is_nil (this->impl_.in ()))
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                    "CIAO::DDS4CCM::CCM_DataReader::impl - "
                    "Throwing BAD_INV_ORDER.\n"));
      throw ::CORBA::BAD_INV_ORDER ();
    }
  else
    {
      return this->impl_.in ();
    }
}
