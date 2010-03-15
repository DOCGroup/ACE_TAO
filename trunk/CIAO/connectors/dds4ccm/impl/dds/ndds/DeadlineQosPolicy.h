/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DEADLINEQOSPOLICY_H
#define DDS4CCM_DEADLINEQOSPOLICY_H

#include "Duration_t.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DeadlineQosPolicy DDS_DeadlineQosPolicy;
#endif

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::DeadlineQosPolicy &ddsqos, const ::DDS_DeadlineQosPolicy & qos)
{
  ddsqos.period <<= qos.period;
}

inline void
operator<<= (::DDS_DeadlineQosPolicy &ddsqos, const ::DDS::DeadlineQosPolicy & qos)
{
  ddsqos.period <<= qos.period;
}

#endif

#endif /* DDS4CCM_DEADLINEQOSPOLICY_H */
