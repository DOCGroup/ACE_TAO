/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_LATENCYBUDGETQOSPOLICY_H
#define CIAO_RTI_LATENCYBUDGETQOSPOLICY_H

#include "Duration_t.h"

inline void
operator<<= (::DDS::LatencyBudgetQosPolicy &ddsqos, const ::DDS_LatencyBudgetQosPolicy & qos)
{
  ddsqos.duration <<= qos.duration;
}

inline void
operator<<= (::DDS_LatencyBudgetQosPolicy &ddsqos, const ::DDS::LatencyBudgetQosPolicy & qos)
{
  ddsqos.duration <<= qos.duration;
}

inline void
operator>>= (const ::DDS_LatencyBudgetQosPolicy &qos, ::DDS::LatencyBudgetQosPolicy & ddsqos)
{
  ddsqos.duration <<= qos.duration;
}

inline void
operator>>= (const ::DDS::LatencyBudgetQosPolicy &qos, ::DDS_LatencyBudgetQosPolicy & ddsqos)
{
  ddsqos.duration <<= qos.duration;
}

#endif /* CIAO_RTI_LATENCYBUDGETQOSPOLICY_H */
