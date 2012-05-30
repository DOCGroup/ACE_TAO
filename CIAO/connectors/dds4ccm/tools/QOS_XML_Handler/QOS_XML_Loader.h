// $Id$

/**
 * @file QOS_XML_Loader.h
 *
 * @author Marcel Smit <msmit@remedy.nl>
 */

#ifndef QOS_XML_LOADER_H
#define QOS_XML_LOADER_H

#include "dds/DdsDcpsInfrastructureC.h"
#include "dds4ccm/tools/QOS_XML_Handler/XML_QOS_Handler_Export.h"
#include "dds4ccm/tools/QOS_XML_Handler/XML_File_Intf.h"

namespace DDS4CCM
{
  class XML_QOS_Handler_Export QOS_XML_Loader
  {
  public:
    QOS_XML_Loader (void);
    ~QOS_XML_Loader (void);

    bool init (const ACE_TCHAR * filename);

    bool
    get_datawriter_qos (DDS::DataWriterQos& dw_qos,
                        const ACE_TCHAR *profile_name,
                        const char* topic_name);

    bool
    get_datareader_qos (DDS::DataReaderQos& dr_qos,
                        const ACE_TCHAR *profile_name,
                        const char* topic_name);

    bool
    get_publisher_qos (DDS::PublisherQos& pub_qos,
                        const ACE_TCHAR *profile_name);

    bool
    get_subscriber_qos (DDS::SubscriberQos& sub_qos,
                        const ACE_TCHAR *profile_name);

    bool
    get_topic_qos (DDS::TopicQos& topic_qos,
                   const ACE_TCHAR *profile_name,
                   const ACE_TCHAR *topic_name);

    bool
    get_participant_qos (DDS::DomainParticipantQos& part_qos,
                         const ACE_TCHAR *profile_name);
  private:
    QOS_XML_Handler::QOS_XML_File_Handler xml_file_;
  };
}

#endif /* QOS_XML_LOADER_H */
