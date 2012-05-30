// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds/DdsDcpsInfrastructureC.h"
#include "QOS_Common.h"


template <typename XML_QOS_TYPE, typename DDS_QOS_TYPE>
QOS_DataWriter_T<XML_QOS_TYPE, DDS_QOS_TYPE>::QOS_DataWriter_T (void)
{
}

template <typename XML_QOS_TYPE, typename DDS_QOS_TYPE>
QOS_DataWriter_T<XML_QOS_TYPE, DDS_QOS_TYPE>::~QOS_DataWriter_T (void)
{
}

template <typename XML_QOS_TYPE, typename DDS_QOS_TYPE>
void
QOS_DataWriter_T<XML_QOS_TYPE, DDS_QOS_TYPE>::read_qos (DDS_QOS_TYPE& dds_qos, const XML_QOS_TYPE xml_qos)
{
  DwTpBase::read_qos (dds_qos, xml_qos);
//  if (xml_qos->user_data ())
//    {
      // TODO: Have a good look at this.

//       const std::string value = *xml_qos->user_data ()->value ();
//
//       DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_TRACE, DDS4CCM_INFO
//         ACE_TEXT ("QOS_DataWriter_T<XML_QOS_TYPE, DDS_QOS_TYPE>::read_qos - ")
//         ACE_TEXT ("Set user_data to <%C>\n"),
//         value.c_str ()));
//
//       dds_qos.user_data.value =
//         *xml_qos->user_data ()->value ();
//     }
 if (xml_qos->ownership_strength_p ())
   {
      dds_qos.ownership_strength.value = xml_qos->ownership_strength ().value ();

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_TRACE, DDS4CCM_INFO
        ACE_TEXT ("QOS_DataWriter_T<XML_QOS_TYPE, DDS_QOS_TYPE>::read_qos - ")
        ACE_TEXT ("Set ownership_strength to <%u>\n"),
        dds_qos.ownership_strength.value));
   }
  if (xml_qos->writer_data_lifecycle_p () &&
      xml_qos->writer_data_lifecycle ().autodispose_unregistered_instances_p ())
    {
      dds_qos.writer_data_lifecycle.autodispose_unregistered_instances =
        xml_qos->writer_data_lifecycle ().autodispose_unregistered_instances ();

      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_TRACE, DDS4CCM_INFO
        ACE_TEXT ("QOS_DataWriter_T<XML_QOS_TYPE, DDS_QOS_TYPE>::read_qos - ")
        ACE_TEXT ("Set autodispose_unregistered_instances to <%d>\n"),
        dds_qos.writer_data_lifecycle.autodispose_unregistered_instances));
    }
}
