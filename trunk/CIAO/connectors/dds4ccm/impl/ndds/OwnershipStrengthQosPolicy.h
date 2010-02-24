/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_OWNERSHIPSTRENGTHQOSPOLICY_H
#define DDS4CCM_OWNERSHIPSTRENGTHQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::OwnershipStrengthQosPolicy DDS_OwnershipStrengthQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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
#endif

#endif /* DDS4CCM_OWNERSHIPSTRENGTHQOSPOLICY_H */
