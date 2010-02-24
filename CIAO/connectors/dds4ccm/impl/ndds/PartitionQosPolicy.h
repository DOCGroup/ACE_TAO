/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_PARTITIONQOSPOLICY_H
#define CIAO_RTI_PARTITIONQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::PartitionQosPolicy DDS_PartitionQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::PartitionQosPolicy &ddsqos, const ::DDS_PartitionQosPolicy & qos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//  ddsqos.name <<= qos.name;
}

inline void
operator<<= (::DDS_PartitionQosPolicy &ddsqos, const ::DDS::PartitionQosPolicy & qos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//  ddsqos.name <<= qos.name;
}

inline void
operator>>= (const ::DDS_PartitionQosPolicy &qos, ::DDS::PartitionQosPolicy & ddsqos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//  ddsqos.name <<= qos.name;
}

inline void
operator>>= (const ::DDS::PartitionQosPolicy &qos, ::DDS_PartitionQosPolicy & ddsqos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//  ddsqos.name <<= qos.name;
}
#endif

#endif /* CIAO_RTI_PARTITIONQOSPOLICY_H */
