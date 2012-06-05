// $Id$

#include "Utils.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    QOS_XML_Noop::QOS_XML_Noop (void)
    {
    }

    QOS_XML_Noop::~QOS_XML_Noop (void)
    {
    }

    DDS::ReturnCode_t
    QOS_XML_Noop::init (
      const ACE_TCHAR * /*qos_profile*/)
    {
      return ::DDS::RETCODE_OK;
    }

    DDS::ReturnCode_t
    QOS_XML_Noop::get_datawriter_qos (
      DDS::DataWriterQos& /*dw_qos*/,
      const char */*qos_profile*/,
      const char* /*topic_name*/)
    {
      return ::DDS::RETCODE_OK;
    }

    DDS::ReturnCode_t
    QOS_XML_Noop::get_datareader_qos (
      DDS::DataReaderQos& /*dr_qos*/,
      const char */*qos_profile*/,
      const char* /*topic_name*/)
    {
      return ::DDS::RETCODE_OK;
    }

    DDS::ReturnCode_t
    QOS_XML_Noop::get_publisher_qos (
      DDS::PublisherQos& /*pub_qos*/,
      const char */*qos_profile*/)
    {
      return ::DDS::RETCODE_OK;
    }

    DDS::ReturnCode_t
    QOS_XML_Noop::get_subscriber_qos (
      DDS::SubscriberQos& /*sub_qos*/,
      const char */*qos_profile*/)
    {
      return ::DDS::RETCODE_OK;
    }

    DDS::ReturnCode_t
    QOS_XML_Noop::get_topic_qos (
      DDS::TopicQos& /*topic_qos*/,
      const char */*qos_profile*/,
      const ACE_TCHAR */*topic_name*/)
    {
      return ::DDS::RETCODE_OK;
    }

    DDS::ReturnCode_t
    QOS_XML_Noop::get_participant_qos (
      DDS::DomainParticipantQos& /*part_qos*/,
      const char */*qos_profile*/)
    {
      return ::DDS::RETCODE_OK;
    }
  }
}
