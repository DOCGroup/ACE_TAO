// $Id$
#include "DataReaderQos_Handler.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

#include "QOS_DataReader_T.h"

bool
DataReaderQos_Handler::get_datareader_qos (DDS::DataReaderQos& dr_qos,
                                           dds::qosProfile * profile,
                                           const ACE_TCHAR * name)
{
  DDS4CCM_TRACE ("DataReaderQos_Handler::get_datareader_qos");

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
      if (profile->count_datareader_qos () == 0)
        {
          DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_TRACE, DDS4CCM_INFO
            ACE_TEXT ("DataReaderQos_Handler::get_datareader_qos - ")
            ACE_TEXT ("No DataReader QOS available in profile <%C>\n"),
            profile->name ().c_str ()));
          return true;
        }
      // get the first DataReader in the XML
      dds::qosProfile::datareader_qos_iterator dr_it = profile->begin_datareader_qos ();
      return DataReaderQos_Handler::get_datareader_qos (dr_qos, dr_it->get ());
    }

  DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_DEBUG, DDS4CCM_INFO
    ACE_TEXT ("DataReaderQos_Handler::get_datareader_qos - ")
    ACE_TEXT ("Unable to find DataReaderQos <%C>\n"),
    name));
  return false;
}

bool
DataReaderQos_Handler::get_datareader_qos (DDS::DataReaderQos& dr_qos,
                                           dds::datareaderQos * dr)
{
  DDS4CCM_TRACE ("DataReaderQos_Handler::get_datareader_qos");

  typedef QOS_DataReader_T<dds::datareaderQos*, DDS::DataReaderQos> dr_type;
  dr_type datareader_qos_handler;
  datareader_qos_handler.read_qos (dr_qos, dr);

  return true;
}
