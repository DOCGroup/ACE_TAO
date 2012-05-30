// $Id$

#include "XML_File_Intf.h"
#include "XML/XML_Typedefs.h"
#include "XMLSchema/id_map.hpp"

#include "dds/DdsDcpsInfrastructureC.h"
#include "dds4ccm/impl/logger/Log_Macros.h"

#include "DataReaderQos_Handler.h"
#include "DataWriterQos_Handler.h"
#include "TopicQos_Handler.h"
#include "PublisherQos_Handler.h"
#include "SubscriberQos_Handler.h"
#include "ParticipantQos_Handler.h"

namespace DDS4CCM
{
  namespace QOS_XML_Handler
  {
    QOS_XML_File_Handler::QOS_XML_File_Handler (void)
    {
      DDS4CCM_TRACE ("QOS_XML_File_Handler::constructor");
    }

    QOS_XML_File_Handler::~QOS_XML_File_Handler (void)
    {
      DDS4CCM_TRACE ("QOS_XML_File_Handler::destructor");

      //      XML_Helper::XML_HELPER.terminate_parser ();
    }

    bool
    QOS_XML_File_Handler::init (const ACE_TCHAR * file)
    {
      try
        {
          if (!XML_Helper_type::XML_HELPER.is_initialized ())
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                ACE_TEXT ("QOS_XML_File_Handler::init - ")
                ACE_TEXT ("Unable to initialize XML_Helper.\n")));
              return false;
            }

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::init - ")
            ACE_TEXT ("Constructing DOM\n")));

          XERCES_CPP_NAMESPACE::DOMDocument *dom =
            XML_Helper_type::XML_HELPER.create_dom (file);

          if (dom == 0)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                ACE_TEXT ("QOS_XML_File_Handler::init - ")
                ACE_TEXT ("Failed to open file %s\n"),
                file));
              return false;
            }

          XERCES_CPP_NAMESPACE::DOMElement *profile_dom = dom->getDocumentElement ();

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::init - ")
            ACE_TEXT ("DOMElement pointer: %u\n"), profile_dom));

          ID_Map::TSS_ID_Map* TSS_ID_Map (ACE_Singleton<ID_Map::TSS_ID_Map, ACE_Null_Mutex>::instance());
          (*TSS_ID_Map)->reset ();

          this->profiles_ = dds::reader::dds (dom);
        }
      catch (CORBA::Exception &ex)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::init - ")
            ACE_TEXT ("Caught CORBA exception whilst parsing XML <%C> into IDL: %C\n"),
            file,
            ex._info ().c_str ()));
          return false;
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::init - ")
            ACE_TEXT ("Unexpected exception whilst parsing XML <%C> into IDL.\n"),
            file));
          return false;
        }
      return true;
    }

    ::dds::qosProfile *
    QOS_XML_File_Handler::get_profile (const ACE_TCHAR * profile_name)
    {
      for (::dds::qosProfile_seq::qos_profile_const_iterator it = this->profiles_.begin_qos_profile ();
          it != this->profiles_.end_qos_profile ();
          ++it)
        {
          if (ACE_OS::strcmp ((*it)->name ().c_str (), profile_name) == 0)
            {
              DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
                ACE_TEXT ("QOS_XML_File_Handler::get_profile - ")
                ACE_TEXT ("Found profile <%C>\n"),
                (*it)->name ().c_str ()));
              return it->get();
            }
        }
      if (ACE_OS::strlen (profile_name) == 0)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_TRACE,
            ACE_TEXT ("QOS_XML_File_Handler::get_profile - ")
            ACE_TEXT ("No profile specified\n")));
        }
      else
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_TRACE,
            ACE_TEXT ("QOS_XML_File_Handler::get_profile - ")
            ACE_TEXT ("Did not find profile <%C>\n"),
            profile_name));
        }
      return 0;
    }

    bool
    QOS_XML_File_Handler::get_datawriter_qos (::DDS::DataWriterQos& dw_qos,
                                              const ACE_TCHAR * profile_name,
                                              const char * topic_name)
    {
      DDS4CCM_TRACE ("QOS_XML_File_Handler::get_datawriter_qos");

      ACE_UNUSED_ARG (topic_name);

      try
        {
          ::dds::qosProfile * profile = this->get_profile (profile_name);
          if (profile != 0)
            {
              DataWriterQos_Handler::get_datawriter_qos (dw_qos, profile);
              return true;
            }
          else
            return false;
        }
      catch (CORBA::Exception &ex)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_datawriter_qos - ")
            ACE_TEXT ("Caught CORBA exception whilst parsing XML into IDL: %C\n"),
            ex._info ().c_str ()));
          return false;
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_datawriter_qos - ")
            ACE_TEXT ("Unexpected exception whilst parsing XML into IDL.\n")));
          return false;
        }

      return false;
    }

    bool
    QOS_XML_File_Handler::get_datareader_qos (::DDS::DataReaderQos& dr_qos,
                                              const ACE_TCHAR * profile_name,
                                              const char * topic_name)
    {
      DDS4CCM_TRACE ("QOS_XML_File_Handler::get_datareader_qos");

      ACE_UNUSED_ARG (topic_name);

      try
        {
          ::dds::qosProfile * profile = this->get_profile (profile_name);
          if (profile != 0)
            {
              DataReaderQos_Handler::get_datareader_qos (dr_qos, profile);
              return true;
            }
          else
            return false;
        }
      catch (CORBA::Exception &ex)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_datareader_qos - ")
            ACE_TEXT ("Caught CORBA exception whilst parsing XML into IDL: %C\n"),
            ex._info ().c_str ()));
          return false;
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_datareader_qos - ")
            ACE_TEXT ("Unexpected exception whilst parsing XML into IDL.\n")));
          return false;
        }

      return false;
    }

    bool
    QOS_XML_File_Handler::get_topic_qos (::DDS::TopicQos& tp_qos,
                                         const ACE_TCHAR * profile_name,
                                         const char * topic_name)
    {
      DDS4CCM_TRACE ("QOS_XML_File_Handler::get_topic_qos");

      ACE_UNUSED_ARG (topic_name);

      try
        {
          ::dds::qosProfile * profile = this->get_profile (profile_name);
          if (profile != 0)
            {
              TopicQos_Handler::get_topic_qos (tp_qos, profile);
              return true;
            }
          else
            return false;
        }
      catch (CORBA::Exception &ex)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_topic_qos - ")
            ACE_TEXT ("Caught CORBA exception whilst parsing XML into IDL: %C\n"),
            ex._info ().c_str ()));
          return false;
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_topic_qos - ")
            ACE_TEXT ("Unexpected exception whilst parsing XML into IDL.\n")));
          return false;
        }

      return false;
    }

    bool
    QOS_XML_File_Handler::get_publisher_qos (::DDS::PublisherQos& pub_qos,
                                             const ACE_TCHAR * profile_name)
    {
      DDS4CCM_TRACE ("QOS_XML_File_Handler::get_publisher_qos");

      try
        {
          ::dds::qosProfile * profile = this->get_profile (profile_name);
          if (profile != 0)
            {
              PublisherQos_Handler::get_publisher_qos (pub_qos, profile);
              return true;
            }
          else
            return false;
        }
      catch (CORBA::Exception &ex)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_publisher_qos - ")
            ACE_TEXT ("Caught CORBA exception whilst parsing XML into IDL: %C\n"),
            ex._info ().c_str ()));
          return false;
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_publisher_qos - ")
            ACE_TEXT ("Unexpected exception whilst parsing XML into IDL.\n")));
          return false;
        }

      return false;
    }

    bool
    QOS_XML_File_Handler::get_subscriber_qos (::DDS::SubscriberQos& sub_qos,
                                              const ACE_TCHAR * profile_name)
    {
      DDS4CCM_TRACE ("QOS_XML_File_Handler::get_subscriber_qos");

      try
        {
          ::dds::qosProfile * profile = this->get_profile (profile_name);
          if (profile != 0)
            {
              SubscriberQos_Handler::get_subscriber_qos (sub_qos, profile);
              return true;
            }
          else
            return false;
        }
      catch (const CORBA::Exception &ex)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_subscriber_qos - ")
            ACE_TEXT ("Caught CORBA exception whilst parsing XML into IDL: %C\n"),
            ex._info ().c_str ()));
          return false;
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_subscriber_qos - ")
            ACE_TEXT ("Unexpected exception whilst parsing XML into IDL.\n")));
          return false;
        }

      return false;
    }

    bool
    QOS_XML_File_Handler::get_participant_qos (::DDS::DomainParticipantQos& sub_qos,
                                              const ACE_TCHAR * profile_name)
    {
      DDS4CCM_TRACE ("QOS_XML_File_Handler::get_participant_qos");

      try
        {
          ::dds::qosProfile * profile = this->get_profile (profile_name);
          if (profile != 0)
            {
              ParticipantQos_Handler::get_participant_qos (sub_qos, profile);
              return true;
            }
          else
            return false;
        }
      catch (const CORBA::Exception &ex)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_participant_qos - ")
            ACE_TEXT ("Caught CORBA exception whilst parsing XML into IDL: %C\n"),
            ex._info ().c_str ()));
          return false;
        }
      catch (...)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
            ACE_TEXT ("QOS_XML_File_Handler::get_participant_qos - ")
            ACE_TEXT ("Unexpected exception whilst parsing XML into IDL.\n")));
          return false;
        }

      return false;
    }

    void
    QOS_XML_File_Handler::add_search_path (const ACE_TCHAR *environment,
                                    const ACE_TCHAR *relpath)
    {
      DDS4CCM_TRACE ("QOS_XML_File_Handler::add_search_path");
      XML_Helper_type::XML_HELPER.get_resolver ().get_resolver ().add_path (environment, relpath);
    }

  }
}
