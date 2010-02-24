/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_TIMEBASEDFILTERQOSPOLICY_H
#define CIAO_RTI_TIMEBASEDFILTERQOSPOLICY_H

#include "Duration_t.h"

inline void
operator<<= (::DDS::TimeBasedFilterQosPolicy &ddsqos, const ::DDS_TimeBasedFilterQosPolicy & qos)
{
  ddsqos.minimum_separation <<= qos.minimum_separation;
}

inline void
operator<<= (::DDS_TimeBasedFilterQosPolicy &ddsqos, const ::DDS::TimeBasedFilterQosPolicy & qos)
{
  ddsqos.minimum_separation <<= qos.minimum_separation;
}

inline void
operator>>= (const ::DDS_TimeBasedFilterQosPolicy &qos, ::DDS::TimeBasedFilterQosPolicy & ddsqos)
{
  ddsqos.minimum_separation <<= qos.minimum_separation;
}

inline void
operator>>= (const ::DDS::TimeBasedFilterQosPolicy &qos, ::DDS_TimeBasedFilterQosPolicy & ddsqos)
{
  ddsqos.minimum_separation <<= qos.minimum_separation;
}

#endif /* CIAO_RTI_TIMEBASEDFILTERQOSPOLICY_H */
