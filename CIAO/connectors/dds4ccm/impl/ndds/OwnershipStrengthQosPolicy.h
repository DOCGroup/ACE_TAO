/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_OWNERSHIPSTRENGTHQOSPOLICY_H
#define CIAO_RTI_OWNERSHIPSTRENGTHQOSPOLICY_H

inline void
operator<<= (::DDS::OwnershipStrengthQosPolicy &ddsqos, const ::DDS_OwnershipStrengthQosPolicy & qos)
{
  ddsqos.value = qos.value;
}

inline void
operator<<= (::DDS_OwnershipStrengthQosPolicy &ddsqos, const ::DDS::OwnershipStrengthQosPolicy & qos)
{
  ddsqos.value = qos.value;
}

inline void
operator>>= (const ::DDS_OwnershipStrengthQosPolicy &qos, ::DDS::OwnershipStrengthQosPolicy & ddsqos)
{
  ddsqos.value = qos.value;
}

inline void
operator>>= (const ::DDS::OwnershipStrengthQosPolicy &qos, ::DDS_OwnershipStrengthQosPolicy & ddsqos)
{
  ddsqos.value = qos.value;
}

#endif /* CIAO_RTI_OWNERSHIPSTRENGTHQOSPOLICY_H */
