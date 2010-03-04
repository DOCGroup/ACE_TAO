/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DURABILITYSERVICEQOSPOLICY_H
#define DDS4CCM_DURABILITYSERVICEQOSPOLICY_H

#include "Duration_t.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DurabilityServiceQosPolicy DDS_DurabilityServiceQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::DurabilityServiceQosPolicy &ddsqos, const ::DDS_DurabilityServiceQosPolicy & qos)
{
  ddsqos.service_cleanup_delay <<= qos.service_cleanup_delay;
  ddsqos.history_kind = static_cast < ::DDS::HistoryQosPolicyKind> (qos.history_kind);
  ddsqos.history_depth = qos.history_depth;
  ddsqos.max_samples = qos.max_samples;
  ddsqos.max_instances = qos.max_instances;
  ddsqos.max_samples_per_instance = qos.max_samples_per_instance;
}

inline void
operator<<= (::DDS_DurabilityServiceQosPolicy &ddsqos, const ::DDS::DurabilityServiceQosPolicy & qos)
{
  ddsqos.service_cleanup_delay <<= qos.service_cleanup_delay;
  ddsqos.history_kind = static_cast < ::DDS_HistoryQosPolicyKind> (qos.history_kind);
  ddsqos.history_depth = qos.history_depth;
  ddsqos.max_samples = qos.max_samples;
  ddsqos.max_instances = qos.max_instances;
  ddsqos.max_samples_per_instance = qos.max_samples_per_instance;
}

inline void
operator>>= (const ::DDS_DurabilityServiceQosPolicy &qos, ::DDS::DurabilityServiceQosPolicy & ddsqos)
{
  ddsqos.service_cleanup_delay <<= qos.service_cleanup_delay;
  ddsqos.history_kind = static_cast < ::DDS::HistoryQosPolicyKind> (qos.history_kind);
  ddsqos.history_depth = qos.history_depth;
  ddsqos.max_samples = qos.max_samples;
  ddsqos.max_instances = qos.max_instances;
  ddsqos.max_samples_per_instance = qos.max_samples_per_instance;
}

inline void
operator>>= (const ::DDS::DurabilityServiceQosPolicy &qos, ::DDS_DurabilityServiceQosPolicy & ddsqos)
{
  ddsqos.service_cleanup_delay <<= qos.service_cleanup_delay;
  ddsqos.history_kind = static_cast < ::DDS_HistoryQosPolicyKind> (qos.history_kind);
  ddsqos.history_depth = qos.history_depth;
  ddsqos.max_samples = qos.max_samples;
  ddsqos.max_instances = qos.max_instances;
  ddsqos.max_samples_per_instance = qos.max_samples_per_instance;
}
#endif

#endif /* DDS4CCM_DURABILITYSERVICEQOSPOLICY_H */
