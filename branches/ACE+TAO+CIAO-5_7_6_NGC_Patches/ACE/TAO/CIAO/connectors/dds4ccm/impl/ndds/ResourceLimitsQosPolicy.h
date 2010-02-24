/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_RESOURCELIMITSQOSPOLICY_H
#define CIAO_RTI_RESOURCELIMITSQOSPOLICY_H

inline void
operator<<= (::DDS::ResourceLimitsQosPolicy &ddsqos, const ::DDS_ResourceLimitsQosPolicy & qos)
{
  ddsqos.max_samples = qos.max_samples;
  ddsqos.max_instances = qos.max_instances;
  ddsqos.max_samples_per_instance = qos.max_samples_per_instance;
}

inline void
operator<<= (::DDS_ResourceLimitsQosPolicy &ddsqos, const ::DDS::ResourceLimitsQosPolicy & qos)
{
  ddsqos.max_samples = qos.max_samples;
  ddsqos.max_instances = qos.max_instances;
  ddsqos.max_samples_per_instance = qos.max_samples_per_instance;
}

inline void
operator>>= (const ::DDS_ResourceLimitsQosPolicy &qos, ::DDS::ResourceLimitsQosPolicy & ddsqos)
{
  ddsqos.max_samples = qos.max_samples;
  ddsqos.max_instances = qos.max_instances;
  ddsqos.max_samples_per_instance = qos.max_samples_per_instance;
}

inline void
operator>>= (const ::DDS::ResourceLimitsQosPolicy &qos, ::DDS_ResourceLimitsQosPolicy & ddsqos)
{
  ddsqos.max_samples = qos.max_samples;
  ddsqos.max_instances = qos.max_instances;
  ddsqos.max_samples_per_instance = qos.max_samples_per_instance;
}

#endif /* CIAO_RTI_RESOURCELIMITSQOSPOLICY_H */
