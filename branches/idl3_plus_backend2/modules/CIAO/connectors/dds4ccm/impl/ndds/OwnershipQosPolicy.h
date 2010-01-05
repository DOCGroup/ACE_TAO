/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_OWNERSHIPQOSPOLICY_H
#define CIAO_RTI_OWNERSHIPQOSPOLICY_H

inline void
operator<<= (::DDS::OwnershipQosPolicy &ddsqos, const ::DDS_OwnershipQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS::OwnershipQosPolicyKind> (qos.kind);
}

inline void
operator<<= (::DDS_OwnershipQosPolicy &ddsqos, const ::DDS::OwnershipQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS_OwnershipQosPolicyKind> (qos.kind);
}

inline void
operator>>= (const ::DDS_OwnershipQosPolicy &qos, ::DDS::OwnershipQosPolicy & ddsqos)
{
  ddsqos.kind = static_cast < ::DDS::OwnershipQosPolicyKind> (qos.kind);
}

inline void
operator>>= (const ::DDS::OwnershipQosPolicy &qos, ::DDS_OwnershipQosPolicy & ddsqos)
{
  ddsqos.kind = static_cast < ::DDS_OwnershipQosPolicyKind> (qos.kind);
}

#endif /* CIAO_RTI_OWNERSHIPQOSPOLICY_H */
