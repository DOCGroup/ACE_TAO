/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_RELIABILITYSQOSPOLICY_H
#define CIAO_RTI_RELIABILITYSQOSPOLICY_H

#include "Duration_t.h"

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

inline void
operator>>= (const ::DDS_ReliabilityQosPolicy &qos, ::DDS::ReliabilityQosPolicy & ddsqos)
{
  ddsqos.kind = static_cast < ::DDS::ReliabilityQosPolicyKind> (qos.kind);
  ddsqos.max_blocking_time <<= qos.max_blocking_time;
}

inline void
operator>>= (const ::DDS::ReliabilityQosPolicy &qos, ::DDS_ReliabilityQosPolicy & ddsqos)
{
  ddsqos.kind = static_cast < ::DDS_ReliabilityQosPolicyKind> (qos.kind);
  ddsqos.max_blocking_time <<= qos.max_blocking_time;
}

#endif /* CIAO_RTI_RELIABILITYSQOSPOLICY_H */
