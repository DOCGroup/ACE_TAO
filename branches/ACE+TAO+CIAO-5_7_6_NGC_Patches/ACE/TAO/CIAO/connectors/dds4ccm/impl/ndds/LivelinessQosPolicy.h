/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_LIVELINESSQOSPOLICY_H
#define CIAO_RTI_LIVELINESSQOSPOLICY_H

#include "Duration_t.h"

inline void
operator<<= (::DDS::LivelinessQosPolicy &ddsqos, const ::DDS_LivelinessQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS::LivelinessQosPolicyKind> (qos.kind);
  ddsqos.lease_duration <<= qos.lease_duration;
}

inline void
operator<<= (::DDS_LivelinessQosPolicy &ddsqos, const ::DDS::LivelinessQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS_LivelinessQosPolicyKind> (qos.kind);
  ddsqos.lease_duration <<= qos.lease_duration;
}

inline void
operator>>= (const ::DDS_LivelinessQosPolicy &qos, ::DDS::LivelinessQosPolicy & ddsqos)
{
  ddsqos.kind = static_cast < ::DDS::LivelinessQosPolicyKind> (qos.kind);
  ddsqos.lease_duration <<= qos.lease_duration;
}

inline void
operator>>= (const ::DDS::LivelinessQosPolicy &qos, ::DDS_LivelinessQosPolicy & ddsqos)
{
  ddsqos.kind = static_cast < ::DDS_LivelinessQosPolicyKind> (qos.kind);
  ddsqos.lease_duration <<= qos.lease_duration;
}

#endif /* CIAO_RTI_LIVELINESSQOSPOLICY_H */
