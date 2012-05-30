// $Id$
#include "SubscriberQos_Handler.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

#include "QOS_PubSub_T.h"

bool
SubscriberQos_Handler::get_subscriber_qos (DDS::SubscriberQos& sub_qos,
                                      dds::qosProfile * profile,
                                      const ACE_TCHAR * name)
{
  DDS4CCM_TRACE ("SubscriberQos_Handler::get_subscriber_qos");
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
      if (profile->count_subscriber_qos () == 0)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
            ACE_TEXT ("SubscriberQos_Handler::get_subscriber_qos - ")
            ACE_TEXT ("No Subscriber QOS available in profile <%C>\n"),
            profile->name ().c_str ()));
          return true;
        }
      // get the first DataReader in the XML
      dds::qosProfile::subscriber_qos_iterator sub_it = profile->begin_subscriber_qos ();
      return SubscriberQos_Handler::get_subscriber_qos (sub_qos, sub_it->get ());
    }

  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_DEBUG, DDS4CCM_INFO
    ACE_TEXT ("SubscriberQos_Handler::get_subscriber_qos - ")
    ACE_TEXT ("Unable to find SubscriberQos <%C>\n"),
    name));
  return false;
}

bool
SubscriberQos_Handler::get_subscriber_qos (DDS::SubscriberQos& sub_qos,
                                         dds::subscriberQos * sub)
{
  DDS4CCM_TRACE ("SubscriberQos_Handler::get_subscriber_qos");

  typedef QOS_PubSub_T<dds::subscriberQos*, DDS::SubscriberQos> sub_type;
  sub_type subscriber_qos_handler;
  subscriber_qos_handler.read_qos (sub_qos, sub);

  return true;
}
