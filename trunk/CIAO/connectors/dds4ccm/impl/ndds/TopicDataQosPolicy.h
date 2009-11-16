/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_TOPICDATAQOSPOLICY_H
#define CIAO_RTI_TOPICDATAQOSPOLICY_H

inline void
operator<<= (::DDS::TopicDataQosPolicy &ddsqos, const ::DDS_TopicDataQosPolicy & qos)
{
  // @todo
//  ddsqos.value = qos.value;
}

inline void
operator<<= (::DDS_TopicDataQosPolicy &ddsqos, const ::DDS::TopicDataQosPolicy & qos)
{
//  ddsqos.value = qos.value;
}

inline void
operator>>= (const ::DDS_TopicDataQosPolicy &qos, ::DDS::TopicDataQosPolicy & ddsqos)
{
//  ddsqos.value = qos.value;
}

inline void
operator>>= (const ::DDS::TopicDataQosPolicy &qos, ::DDS_TopicDataQosPolicy & ddsqos)
{
//  ddsqos.value = qos.value;
}

#endif /* CIAO_RTI_TOPICDATAQOSPOLICY_H */
