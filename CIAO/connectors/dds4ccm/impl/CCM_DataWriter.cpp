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
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::StatusCondition_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_statuscondition (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::StatusCondition::_nil ();
}

::DDS::StatusMask
CIAO::DDS4CCM::CCM_DataWriter::get_status_changes (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return 0;
}

::DDS::InstanceHandle_t
CIAO::DDS4CCM::CCM_DataWriter::get_instance_handle (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::HANDLE_NIL;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::set_qos (
  const ::DDS::DataWriterQos & )
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_qos (::DDS::DataWriterQos & )
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::set_listener (::DDS::DataWriterListener_ptr ,
  ::DDS::StatusMask )
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::DataWriterListener_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_listener (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::DataWriterListener::_nil ();
}

::DDS::Topic_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_topic (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::Topic::_nil ();
}

::DDS::Publisher_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_publisher (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::Publisher::_nil ();
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::wait_for_acknowledgments (const ::DDS::Duration_t & )
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_liveliness_lost_status (::DDS::LivelinessLostStatus & )
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_offered_deadline_missed_status (::DDS::OfferedDeadlineMissedStatus & )
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_offered_incompatible_qos_status (::DDS::OfferedIncompatibleQosStatus & )
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_publication_matched_status (::DDS::PublicationMatchedStatus & )
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::assert_liveliness (void)
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_matched_subscriptions (::DDS::InstanceHandleSeq & )
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

::DDS::ReturnCode_t
CIAO::DDS4CCM::CCM_DataWriter::get_matched_subscription_data (
  ::DDS::SubscriptionBuiltinTopicData & ,
  const ::DDS::InstanceHandle_t & )
{
  throw ::CORBA::NO_IMPLEMENT ();
  return ::DDS::RETCODE_OK;
}

void
CIAO::DDS4CCM::CCM_DataWriter::set_impl (::DDS::DataWriter_ptr dr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::set_impl");
  this->impl_ = ::DDS::DataWriter::_duplicate (dr);
}

::DDS::DataWriter_ptr
CIAO::DDS4CCM::CCM_DataWriter::get_impl (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::get_impl");

  return ::DDS::DataWriter::_duplicate (this->impl ());
}

::DDS::DataWriter_ptr
CIAO::DDS4CCM::CCM_DataWriter::impl (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::CCM_DataWriter::impl");

  if (::CORBA::is_nil (this->impl_.in ()))
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
  else
    {
      return this->impl_.in ();
    }
}
