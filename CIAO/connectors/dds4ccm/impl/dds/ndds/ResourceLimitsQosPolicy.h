/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_RESOURCELIMITSQOSPOLICY_H
#define DDS4CCM_RESOURCELIMITSQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::ResourceLimitsQosPolicy DDS_ResourceLimitsQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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
#endif

#endif /* DDS4CCM_RESOURCELIMITSQOSPOLICY_H */
