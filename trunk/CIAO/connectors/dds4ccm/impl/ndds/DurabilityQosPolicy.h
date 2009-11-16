/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_DURABILITYQOSPOLICY_H
#define CIAO_RTI_DURABILITYQOSPOLICY_H

inline void
operator<<= (::DDS::DurabilityQosPolicy &ddsqos, const ::DDS_DurabilityQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS::DurabilityQosPolicyKind> (qos.kind);
}

inline void
operator<<= (::DDS_DurabilityQosPolicy &ddsqos, const ::DDS::DurabilityQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS_DurabilityQosPolicyKind> (qos.kind);
}

inline void
operator>>= (const ::DDS_DurabilityQosPolicy &qos, ::DDS::DurabilityQosPolicy & ddsqos)
{
  ddsqos.kind = static_cast < ::DDS::DurabilityQosPolicyKind> (qos.kind);
}

inline void
operator>>= (const ::DDS::DurabilityQosPolicy &qos, ::DDS_DurabilityQosPolicy & ddsqos)
{
  ddsqos.kind = static_cast < ::DDS_DurabilityQosPolicyKind> (qos.kind);
}

#endif /* CIAO_RTI_DURABILITYQOSPOLICY_H */
