/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_TOPICDATAQOSPOLICY_H
#define DDS4CCM_TOPICDATAQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::TopicDataQosPolicy DDS_TopicDataQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::TopicDataQosPolicy &ddsqos, const ::DDS_TopicDataQosPolicy & qos)
{
  // @todo
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//  ddsqos.value = qos.value;
}

inline void
operator<<= (::DDS_TopicDataQosPolicy &ddsqos, const ::DDS::TopicDataQosPolicy & qos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//  ddsqos.value = qos.value;
}

inline void
operator>>= (const ::DDS_TopicDataQosPolicy &qos, ::DDS::TopicDataQosPolicy & ddsqos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//  ddsqos.value = qos.value;
}

inline void
operator>>= (const ::DDS::TopicDataQosPolicy &qos, ::DDS_TopicDataQosPolicy & ddsqos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//  ddsqos.value = qos.value;
}
#endif

#endif /* DDS4CCM_TOPICDATAQOSPOLICY_H */
