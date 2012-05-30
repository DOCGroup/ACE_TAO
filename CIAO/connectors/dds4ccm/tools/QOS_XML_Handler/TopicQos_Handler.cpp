// $Id$
#include "TopicQos_Handler.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

#include "QOS_Topic_T.h"

bool
TopicQos_Handler::get_topic_qos (DDS::TopicQos& tp_qos,
                                      dds::qosProfile * profile,
                                      const ACE_TCHAR * name)
{
  DDS4CCM_TRACE ("TopicQos_Handler::get_topic_qos");
  if (name)
    {
//       // find the correct DataReader_qos
//       for (dds::qosProfile::DataReader_qos_iterator dw_it = profile->begin_DataReader_qos ();
//           dw_it != profile->end_DataReader_qos();
//           ++dw_it)
//         {
//           if (ACE_OS::strcmp (dw_name.c_str (), name) == 0)
//             {
//               return get_DataReader_qos (dw_qos, *dw_it->get());
//             }
//         }
    }
  else
    {
      if (profile->count_topic_qos () == 0)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
            ACE_TEXT ("TopicQos_Handler::get_topic_qos - ")
            ACE_TEXT ("No Topic QOS available in profile <%C>\n"),
            profile->name ().c_str ()));
          return true;
        }
      // get the first DataReader in the XML
      dds::qosProfile::topic_qos_iterator tp_it = profile->begin_topic_qos ();
      return TopicQos_Handler::get_topic_qos (tp_qos, tp_it->get ());
    }

  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_DEBUG, DDS4CCM_INFO
    ACE_TEXT ("TopicQos_Handler::get_topic_qos - ")
    ACE_TEXT ("Unable to find TopicQos <%C>\n"),
    name));
  return false;
}

bool
TopicQos_Handler::get_topic_qos (DDS::TopicQos& tp_qos,
                                 dds::topicQos * tp)
{
  DDS4CCM_TRACE ("TopicQos_Handler::get_topic_qos");

  typedef QOS_Topic_T<dds::topicQos*, DDS::TopicQos> tp_type;
  tp_type topic_qos_handler;
  topic_qos_handler.read_qos (tp_qos, tp);

  return true;
}
