/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_RELIABILITYSQOSPOLICY_H
#define DDS4CCM_RELIABILITYSQOSPOLICY_H

#include "Duration_t.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::ReliabilityQosPolicy DDS_ReliabilityQosPolicy;
#endif

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::ReliabilityQosPolicy &ddsqos, const ::DDS_ReliabilityQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS::ReliabilityQosPolicyKind> (qos.kind);
  ddsqos.max_blocking_time <<= qos.max_blocking_time;
}

inline void
operator<<= (::DDS_ReliabilityQosPolicy &ddsqos, const ::DDS::ReliabilityQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS_ReliabilityQosPolicyKind> (qos.kind);
  ddsqos.max_blocking_time <<= qos.max_blocking_time;
}

#endif

#endif /* DDS4CCM_RELIABILITYSQOSPOLICY_H */
