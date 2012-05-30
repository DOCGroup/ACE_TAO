// $Id$
#include "DataWriterQos_Handler.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

#include "QOS_DataWriter_T.h"

bool
DataWriterQos_Handler::get_datawriter_qos (DDS::DataWriterQos& dw_qos,
                                           dds::qosProfile * profile,
                                           const ACE_TCHAR * name)
{
  DDS4CCM_TRACE ("DataWriterQos_Handler::get_datawriter_qos");

  if (name)
    {
//       // find the correct datawriter_qos
//       for (dds::qosProfile::datawriter_qos_iterator dw_it = profile->begin_datawriter_qos ();
//           dw_it != profile->end_datawriter_qos();
//           ++dw_it)
//         {
//           if (ACE_OS::strcmp (dw_name.c_str (), name) == 0)
//             {
//               return get_datawriter_qos (dw_qos, *dw_it->get());
//             }
//         }
    }
  else
    {
      if (profile->count_datawriter_qos () == 0)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
            ACE_TEXT ("DataWriterQos_Handler::get_datawriter_qos - ")
            ACE_TEXT ("No DataWriter QOS available in profile <%C>\n"),
            profile->name ().c_str ()));
          return true;
        }
      // get the first datawriter in the XML
      dds::qosProfile::datawriter_qos_iterator dw_it = profile->begin_datawriter_qos ();
      return DataWriterQos_Handler::get_datawriter_qos (dw_qos, dw_it->get ());
    }

  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_DEBUG, DDS4CCM_INFO
    ACE_TEXT ("DataWriterQos_Handler::get_datawriter_qos - ")
    ACE_TEXT ("Unable to find DataWriterQos <%C>\n"),
    name));
  return false;
}

bool
DataWriterQos_Handler::get_datawriter_qos (DDS::DataWriterQos& dw_qos,
                                           dds::datawriterQos * dw)
{
  DDS4CCM_TRACE ("DataWriterQos_Handler::get_datawriter_qos");

  typedef QOS_DataWriter_T<dds::datawriterQos*, DDS::DataWriterQos> dw_type;
  dw_type datawriter_qos_handler;
  datawriter_qos_handler.read_qos (dw_qos, dw);

  return true;
}
