/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_PARTITIONQOSPOLICY_H
#define CIAO_RTI_PARTITIONQOSPOLICY_H

/// @todo

inline void
operator<<= (::DDS::PartitionQosPolicy &ddsqos, const ::DDS_PartitionQosPolicy & qos)
{
//  ddsqos.name <<= qos.name;
}

inline void
operator<<= (::DDS_PartitionQosPolicy &ddsqos, const ::DDS::PartitionQosPolicy & qos)
{
//  ddsqos.name <<= qos.name;
}

inline void
operator>>= (const ::DDS_PartitionQosPolicy &qos, ::DDS::PartitionQosPolicy & ddsqos)
{
//  ddsqos.name <<= qos.name;
}

inline void
operator>>= (const ::DDS::PartitionQosPolicy &qos, ::DDS_PartitionQosPolicy & ddsqos)
{
//  ddsqos.name <<= qos.name;
}

#endif /* CIAO_RTI_PARTITIONQOSPOLICY_H */
