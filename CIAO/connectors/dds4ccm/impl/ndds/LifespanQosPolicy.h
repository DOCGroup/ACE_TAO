/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_LIFESPANQOSPOLICY_H
#define CIAO_RTI_LIFESPANQOSPOLICY_H

#include "Duration_t.h"

inline void
operator<<= (::DDS::LifespanQosPolicy &ddsqos, const ::DDS_LifespanQosPolicy & qos)
{
  ddsqos.duration <<= qos.duration;
}

inline void
operator<<= (::DDS_LifespanQosPolicy &ddsqos, const ::DDS::LifespanQosPolicy & qos)
{
  ddsqos.duration <<= qos.duration;
}

inline void
operator>>= (const ::DDS_LifespanQosPolicy &qos, ::DDS::LifespanQosPolicy & ddsqos)
{
  ddsqos.duration <<= qos.duration;
}

inline void
operator>>= (const ::DDS::LifespanQosPolicy &qos, ::DDS_LifespanQosPolicy & ddsqos)
{
  ddsqos.duration <<= qos.duration;
}

#endif /* CIAO_RTI_LIFESPANQOSPOLICY_H */
