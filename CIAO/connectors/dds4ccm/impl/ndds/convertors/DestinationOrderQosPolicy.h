/**
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DESTINATIONORDERQOSPOLICY_H
#define DDS4CCM_DESTINATIONORDERQOSPOLICY_H

#include "Duration_t.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::DestinationOrderQosPolicy &ddsqos, const ::DDS_DestinationOrderQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS::DestinationOrderQosPolicyKind> (qos.kind);
  ddsqos.scope = static_cast < ::DDS::DestinationOrderQosPolicyScopeKind> (qos.scope);
  ddsqos.source_timestamp_tolerance <<= qos.source_timestamp_tolerance;
}

inline void
operator<<= (::DDS_DestinationOrderQosPolicy &ddsqos, const ::DDS::DestinationOrderQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS_DestinationOrderQosPolicyKind> (qos.kind);
  ddsqos.scope = static_cast < ::DDS_DestinationOrderQosPolicyScopeKind> (qos.scope);
  ddsqos.source_timestamp_tolerance <<= qos.source_timestamp_tolerance;
}

#endif /* DDS4CCM_DESTINATIONORDERQOSPOLICY_H */
