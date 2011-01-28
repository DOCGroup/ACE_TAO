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
#include "dds4ccm/impl/dds4ccm_conf.h"

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

#endif /* DDS4CCM_DURABILITYSERVICEQOSPOLICY_H */
