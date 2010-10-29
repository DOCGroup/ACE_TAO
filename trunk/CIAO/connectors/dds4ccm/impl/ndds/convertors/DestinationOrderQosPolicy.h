/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
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
}

inline void
operator<<= (::DDS_DestinationOrderQosPolicy &ddsqos, const ::DDS::DestinationOrderQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS_DestinationOrderQosPolicyKind> (qos.kind);
}

#endif /* DDS4CCM_DESTINATIONORDERQOSPOLICY_H */
