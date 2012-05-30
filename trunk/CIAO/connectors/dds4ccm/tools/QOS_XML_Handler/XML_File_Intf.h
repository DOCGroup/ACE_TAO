//==============================================================
/**
 *  @file  XML_File_Intf.h
 *
 *  $Id$
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */
//================================================================

#ifndef DDS4CCM_CONFIG_XML_FILE_INTF_H
#define DDS4CCM_CONFIG_XML_FILE_INTF_H
#include /**/ "ace/pre.h"

#include "XML_QOS_Handler_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Auto_Ptr.h"

#include "dds_qos.hpp"

#include <string>

namespace DDS
{
  struct DataWriterQos;
  struct DataReaderQos;
  struct TopicQos;
  struct PublisherQos;
  struct SubscriberQos;
  struct DomainParticipantQos;
}

namespace XML
{
  class XML_Typedef;
}

namespace DDS4CCM
{
  namespace QOS_XML_Handler
  {
    class XML_QOS_Handler_Export QOS_XML_File_Handler
    {
    public:
      QOS_XML_File_Handler (void);

      ~QOS_XML_File_Handler (void);

      bool
      get_datawriter_qos (::DDS::DataWriterQos& dw_qos,
                          const ACE_TCHAR * profile_name,
                          const char * topic_name);

      bool
      get_datareader_qos (::DDS::DataReaderQos& dr_qos,
                          const ACE_TCHAR * profile_name,
                          const char * topic_name);

      bool
      get_topic_qos (::DDS::TopicQos& tp_qos,
                     const ACE_TCHAR * profile_name,
                     const char * topic_name);

      bool
      get_publisher_qos (::DDS::PublisherQos& pub_qos,
                         const ACE_TCHAR * profile_name);

      bool
      get_subscriber_qos (::DDS::SubscriberQos& sub_qos,
                          const ACE_TCHAR * profile_name);

      bool
      get_participant_qos (::DDS::DomainParticipantQos& sub_qos,
                           const ACE_TCHAR * profile_name);

      void
      add_search_path (const ACE_TCHAR *environment,
                       const ACE_TCHAR *relpath);

      bool
      init (const ACE_TCHAR * file);

    private:
      ::dds::qosProfile_seq profiles_;
      typedef XML::XML_Typedef XML_Helper_type;

      ::dds::qosProfile * get_profile (const ACE_TCHAR * profile_name);
    };
  }
}

#include /**/ "ace/post.h"

#endif /* DDS4CCM_CONFIG_XML_FILE_INTF_H */
