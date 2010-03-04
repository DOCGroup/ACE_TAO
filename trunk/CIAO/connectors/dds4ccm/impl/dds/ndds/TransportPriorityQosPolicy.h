/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_TRANSPORTPRIOTITYQOSPOLICY_H
#define DDS4CCM_TRANSPORTPRIOTITYQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::TransportPriorityQosPolicy DDS_TransportPriorityQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::TransportPriorityQosPolicy &ddsqos, const ::DDS_TransportPriorityQosPolicy & qos)
{
  ddsqos.value = qos.value;
}

inline void
operator<<= (::DDS_TransportPriorityQosPolicy &ddsqos, const ::DDS::TransportPriorityQosPolicy & qos)
{
  ddsqos.value = qos.value;
}

inline void
operator>>= (const ::DDS_TransportPriorityQosPolicy &qos, ::DDS::TransportPriorityQosPolicy & ddsqos)
{
  ddsqos.value = qos.value;
}

inline void
operator>>= (const ::DDS::TransportPriorityQosPolicy &qos, ::DDS_TransportPriorityQosPolicy & ddsqos)
{
  ddsqos.value = qos.value;
}
#endif

#endif /* DDS4CCM_TRANSPORTPRIOTITYQOSPOLICY_H */
