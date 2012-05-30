// $Id$

#include "dds4ccm/tools/QOS_XML_Handler/QOS_XML_Loader.h"
#include "dds4ccm/tools/QOS_XML_Handler/XML_File_Intf.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

namespace DDS4CCM
{
  QOS_XML_Loader::QOS_XML_Loader (void)
  {
  }

  QOS_XML_Loader::~QOS_XML_Loader (void)
  {
  }

  bool
  QOS_XML_Loader::init (const ACE_TCHAR *filename)
  {
    DDS4CCM_TRACE ("QOS_XML_Loader::init");

    if (!filename)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::init - ")
          ACE_TEXT ("Passed an empty file name, returning.\n")));
        return false;
      }

    if (!this->xml_file_.init (filename))
      return false;

    this->xml_file_.add_search_path (
      ACE_TEXT("ACE_ROOT"), ACE_TEXT("/docs/schema/"));
    this->xml_file_.add_search_path (
      ACE_TEXT("TAO_ROOT"), ACE_TEXT("/docs/schema/"));
    this->xml_file_.add_search_path (
      ACE_TEXT("CIAO_ROOT"), ACE_TEXT("/docs/schema/"));
    this->xml_file_.add_search_path (
      ACE_TEXT("DANCE_ROOT"), ACE_TEXT("/docs/schema/"));
    this->xml_file_.add_search_path (
      ACE_TEXT("CIAO_ROOT"), ACE_TEXT("connectors/dds4ccm/docs/schema/"));
    return true;
  }

  bool
  QOS_XML_Loader::get_datawriter_qos (
    ::DDS::DataWriterQos& dw_qos,
    const ACE_TCHAR * profile_name,
    const char * topic_name)
  {
    DDS4CCM_TRACE ("QOS_XML_Loader::get_datawriter_qos");

    if (!profile_name)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_datawriter_qos - ")
          ACE_TEXT ("Passed an empty profile name, returning.\n")));
        return false;
      }

    try
      {
        return this->xml_file_.get_datawriter_qos (dw_qos,
                                                   profile_name,
                                                   topic_name);
      }
    catch (...)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_datawriter_qos - ")
          ACE_TEXT ("Caught unexpected exception.\n")));
        return false;
      }
  }

  bool
  QOS_XML_Loader::get_datareader_qos (
    DDS::DataReaderQos& dr_qos,
    const ACE_TCHAR *profile_name,
    const char* topic_name)
  {
    DDS4CCM_TRACE ("QOS_XML_Loader::get_datareader_qos");

    if (!profile_name)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_datareader_qos - ")
          ACE_TEXT ("Passed an empty profile name, returning.\n")));
        return false;
      }

    try
      {
        return this->xml_file_.get_datareader_qos (dr_qos,
                                                   profile_name,
                                                   topic_name);
      }
    catch (...)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_datareader_qos - ")
          ACE_TEXT ("Caught unexpected exception.\n")));
        return false;
      }
  }

  bool
  QOS_XML_Loader::get_publisher_qos (
    DDS::PublisherQos& pub_qos,
    const ACE_TCHAR *profile_name)
  {
    DDS4CCM_TRACE ("QOS_XML_Loader::get_publisher_qos");

    if (!profile_name)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_publisher_qos - ")
          ACE_TEXT ("Passed an empty profile name, returning.\n")));
        return false;
      }

    try
      {
        return this->xml_file_.get_publisher_qos (pub_qos,
                                                  profile_name);
      }
    catch (...)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_publisher_qos - ")
          ACE_TEXT ("Caught unexpected exception.\n")));
        return false;
      }
  }

  bool
  QOS_XML_Loader::get_subscriber_qos (
    DDS::SubscriberQos& sub_qos,
    const ACE_TCHAR *profile_name)
  {
    DDS4CCM_TRACE ("QOS_XML_Loader::get_subscriber_qos");

    if (!profile_name)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_subscriber_qos - ")
          ACE_TEXT ("Passed an empty profile name, returning.\n")));
        return false;
      }

    try
      {
        return this->xml_file_.get_subscriber_qos (sub_qos,
                                                  profile_name);
      }
    catch (...)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_subscriber_qos - ")
          ACE_TEXT ("Caught unexpected exception.\n")));
        return false;
      }
  }

  bool
  QOS_XML_Loader::get_topic_qos (
    DDS::TopicQos& topic_qos,
    const ACE_TCHAR *profile_name,
    const char * topic_name)
  {
    DDS4CCM_TRACE ("QOS_XML_Loader::get_topic_qos");

    if (!profile_name)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_topic_qos - ")
          ACE_TEXT ("Passed an empty profile name, returning.\n")));
        return false;
      }

    try
      {
        return this->xml_file_.get_topic_qos (topic_qos,
                                              profile_name,
                                              topic_name);
      }
    catch (...)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_topic_qos - ")
          ACE_TEXT ("Caught unexpected exception.\n")));
        return false;
      }
  }

  bool
  QOS_XML_Loader::get_participant_qos (
    DDS::DomainParticipantQos& part_qos,
    const ACE_TCHAR *profile_name)
  {
    DDS4CCM_TRACE ("QOS_XML_Loader::get_participant_qos");

    if (!profile_name)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_participant_qos - ")
          ACE_TEXT ("Passed an empty profile name, returning.\n")));
        return false;
      }

    try
      {
        return this->xml_file_.get_participant_qos (part_qos,
                                                    profile_name);
      }
    catch (...)
      {
        DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          ACE_TEXT ("QOS_XML_Loader::get_participant_qos - ")
          ACE_TEXT ("Caught unexpected exception.\n")));
        return false;
      }
    return true;
  }

}
