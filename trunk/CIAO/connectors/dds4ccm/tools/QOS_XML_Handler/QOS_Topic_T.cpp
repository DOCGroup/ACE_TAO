// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "dds/DdsDcpsInfrastructureC.h"
#include "QOS_Common.h"


template <typename XML_QOS_TYPE, typename DDS_QOS_TYPE>
QOS_Topic_T<XML_QOS_TYPE, DDS_QOS_TYPE>::QOS_Topic_T (void)
{
}

template <typename XML_QOS_TYPE, typename DDS_QOS_TYPE>
QOS_Topic_T<XML_QOS_TYPE, DDS_QOS_TYPE>::~QOS_Topic_T (void)
{
}

template <typename XML_QOS_TYPE, typename DDS_QOS_TYPE>
void
QOS_Topic_T<XML_QOS_TYPE, DDS_QOS_TYPE>::read_qos (DDS_QOS_TYPE& dds_qos, const XML_QOS_TYPE xml_qos)
{
  DwDrTpBase::read_qos (dds_qos, xml_qos);
//  if (xml_qos->topic_data ())
//    {
      // TODO: Have a good look at this.

//       const std::string value = *xml_qos->topic_data ()->value ();
//
//       DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_DDS_STATUS, (LM_TRACE, DDS4CCM_INFO
//         ACE_TEXT ("QOS_Topic_T<XML_QOS_TYPE, DDS_QOS_TYPE>::read_qos - ")
//         ACE_TEXT ("Set topic_data to <%C>\n"),
//         value.c_str ()));
//
//       dds_qos.topic_data.value =
//         *xml_qos->topic_data ()->value ();
//     }
}
