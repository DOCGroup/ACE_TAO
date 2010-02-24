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

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::TimeBasedFilterQosPolicy DDS_TimeBasedFilterQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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
#endif

#endif /* CIAO_RTI_TIMEBASEDFILTERQOSPOLICY_H */
