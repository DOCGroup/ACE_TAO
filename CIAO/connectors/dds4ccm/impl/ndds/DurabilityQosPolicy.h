/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DURABILITYQOSPOLICY_H
#define DDS4CCM_DURABILITYQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DurabilityQosPolicy DDS_DurabilityQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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
#endif

#endif /* DDS4CCM_DURABILITYQOSPOLICY_H */
