/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_TRANSPORTPRIOTITYQOSPOLICY_H
#define CIAO_RTI_TRANSPORTPRIOTITYQOSPOLICY_H

inline void
operator<<= (::DDS::TransportPriorityQosPolicy &ddsqos, const ::DDS_TransportPriorityQosPolicy & qos)
{
  ddsqos.value <<= qos.value;
}

inline void
operator<<= (::DDS_TransportPriorityQosPolicy &ddsqos, const ::DDS::TransportPriorityQosPolicy & qos)
{
  ddsqos.value <<= qos.value;
}

inline void
operator>>= (const ::DDS_TransportPriorityQosPolicy &qos, ::DDS::TransportPriorityQosPolicy & ddsqos)
{
  ddsqos.value <<= qos.value;
}

inline void
operator>>= (const ::DDS::TransportPriorityQosPolicy &qos, ::DDS_TransportPriorityQosPolicy & ddsqos)
{
  ddsqos.value <<= qos.value;
}

#endif /* CIAO_RTI_TRANSPORTPRIOTITYQOSPOLICY_H */
