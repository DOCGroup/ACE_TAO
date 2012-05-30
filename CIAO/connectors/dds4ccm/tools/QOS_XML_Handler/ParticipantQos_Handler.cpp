// $Id$
#include "ParticipantQos_Handler.h"

#include "dds4ccm/impl/logger/Log_Macros.h"
#include <dds/DdsDcpsInfrastructureC.h>

bool
ParticipantQos_Handler::get_participant_qos (DDS::DomainParticipantQos& dp_qos,
                                           dds::qosProfile * profile,
                                           const ACE_TCHAR * name)
{
  DDS4CCM_TRACE ("ParticipantQos_Handler::get_participant_qos");

  if (name)
    {
//       // find the correct Participant_qos
//       for (dds::qosProfile::Participant_qos_iterator dw_it = profile->begin_Participant_qos ();
//           dw_it != profile->end_Participant_qos();
//           ++dw_it)
//         {
//           if (ACE_OS::strcmp (dw_name.c_str (), name) == 0)
//             {
//               return get_Participant_qos (dw_qos, *dw_it->get());
//             }
//         }
    }
  else
    {
      if (profile->count_domainparticipant_qos () == 0)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
            ACE_TEXT ("ParticipantQos_Handler::get_participant_qos - ")
            ACE_TEXT ("No Participant QOS available in profile <%C>\n"),
            profile->name ().c_str ()));
          return true;
        }
      // get the first Participant in the XML
      dds::qosProfile::domainparticipant_qos_iterator dr_it = profile->begin_domainparticipant_qos ();
      return ParticipantQos_Handler::get_participant_qos (dp_qos, dr_it->get ());
    }

  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_DEBUG, DDS4CCM_INFO
    ACE_TEXT ("ParticipantQos_Handler::get_participant_qos - ")
    ACE_TEXT ("Unable to find ParticipantQos <%C>\n"),
    name));
  return false;
}

bool
ParticipantQos_Handler::get_participant_qos (DDS::DomainParticipantQos& dp_qos,
                                           dds::domainparticipantQos * dp)
{
  DDS4CCM_TRACE ("ParticipantQos_Handler::get_participant_qos");

//  if (xml_qos->user_data ())
//    {
      // TODO: Have a good look at this.

//       const std::string value = *xml_qos->user_data ()->value ();
//
//       DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_TRACE, DDS4CCM_INFO
//         ACE_TEXT ("QOS_DataReader_T<XML_QOS_TYPE, DDS_QOS_TYPE>::read_qos - ")
//         ACE_TEXT ("Set user_data to <%C>\n"),
//         value.c_str ()));
//
//       dds_qos.user_data.value =
//         *xml_qos->user_data ()->value ();
//     }


  if (dp->entity_factory_p ())
    {
      if (dp->entity_factory ().autoenable_created_entities_p ())
        {
          dp_qos.entity_factory.autoenable_created_entities =
            dp->entity_factory ().autoenable_created_entities ();

          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_TRACE, DDS4CCM_INFO
            ACE_TEXT ("ParticipantQos_Handler::get_participant_qos - ")
            ACE_TEXT ("Set entity_factory autoenable_created_entities to <%d>\n"),
            dp_qos.entity_factory.autoenable_created_entities));
        }
    }




//   typedef QOS_Participant_T<dds::domainparticipantQos*, DDS::DomainParticipantQos> dr_type;
//   dr_type participant_qos_handler;
//   participant_qos_handler.read_qos (dr_qos, dr);

  return true;
}
