// $Id$

#include "CCM_DataWriter.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

CIAO::DDS4CCM::CCM_DataWriter::CCM_DataWriter (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::CCM_DataWriter");
}

CIAO::DDS4CCM::CCM_DataWriter::~CCM_DataWriter (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::~CCM_DataWriter");
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::enable (void)
{
  return this->impl ()->enable ();
}

::DDS::StatusCondition_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_statuscondition (void)
{
  return this->impl ()->get_statuscondition ();
}

::DDS::StatusMask
CIAO::DDS4CCM::CCM_DataWriter::get_status_changes (void)
{
  return this->impl ()->get_status_changes ();
}

::DDS::InstanceHandle_t
CIAO::DDS4CCM::CCM_DataWriter::get_instance_handle (void)
{
  return this->impl ()->get_instance_handle ();
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::set_qos (
  const ::DDS::DataWriterQos &qos)
{
  return this->impl ()->set_qos (qos);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_qos (::DDS::DataWriterQos &qos)
{
  return this->impl ()->get_qos (qos);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::set_listener (::DDS::DataWriterListener_ptr listener,
  ::DDS::StatusMask mask)
{
  return this->impl ()->set_listener (listener, mask);
}

::DDS::DataWriterListener_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_listener (void)
{
  return this->impl ()->get_listener ();
}

::DDS::Topic_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_topic (void)
{
  return this->impl ()->get_topic ();
}

::DDS::Publisher_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_publisher (void)
{
  return this->impl ()->get_publisher ();
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::wait_for_acknowledgments (const ::DDS::Duration_t &duration)
{
  return this->impl ()->wait_for_acknowledgments (duration);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_liveliness_lost_status (::DDS::LivelinessLostStatus &status)
{
  return this->impl ()->get_liveliness_lost_status (status);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus &status)
{
  return this->impl ()->get_offered_deadline_missed_status (status);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus &status)
{
  return this->impl ()->get_offered_incompatible_qos_status (status);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_publication_matched_status (::DDS::PublicationMatchedStatus &status)
{
  return this->impl ()->get_publication_matched_status (status);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::assert_liveliness (void)
{
  return this->impl ()->assert_liveliness ();
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_matched_subscriptions (::DDS::InstanceHandleSeq &seq)
{
  return this->impl ()->get_matched_subscriptions (seq);
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_matched_subscription_data (
  ::DDS::SubscriptionBuiltinTopicData & data,
  const ::DDS::InstanceHandle_t &handle)
{
  return this->impl ()->get_matched_subscription_data (data, handle);
}

void
CIAO::DDS4CCM::CCM_DataWriter::set_dds_entity (::DDS::DataWriter_ptr dw)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::set_dds_entity");

  this->impl_ = ::DDS::DataWriter::_duplicate (dw);
}

::DDS::DataWriter_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_dds_entity (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::get_dds_entity");

  return ::DDS::DataWriter::_duplicate (this->impl ());
}

::DDS::DataWriter_ptr
CIAO::DDS4CCM::CCM_DataWriter::impl (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::impl");

  if (::CORBA::is_nil (this->impl_.in ()))
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG,
                    "CIAO::DDS4CCM::CCM_DataWriter::impl - "
                    "Throwing BAD_INV_ORDER.\n"));
      throw ::CORBA::BAD_INV_ORDER ();
    }
  else
    {
      return this->impl_.in ();
    }
}
