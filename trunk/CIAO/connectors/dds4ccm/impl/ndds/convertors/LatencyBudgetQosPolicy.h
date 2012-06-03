/**
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_LATENCYBUDGETQOSPOLICY_H
#define DDS4CCM_LATENCYBUDGETQOSPOLICY_H

#include "Duration_t.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

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

#endif /* DDS4CCM_LATENCYBUDGETQOSPOLICY_H */
