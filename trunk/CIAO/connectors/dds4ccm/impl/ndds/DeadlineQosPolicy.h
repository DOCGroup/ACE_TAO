/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_DEADLINEQOSPOLICY_H
#define CIAO_RTI_DEADLINEQOSPOLICY_H

#include "Duration_t.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DeadlineQosPolicy DDS_DeadlineQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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

inline void
operator>>= (const ::DDS_DeadlineQosPolicy &qos, ::DDS::DeadlineQosPolicy & ddsqos)
{
  ddsqos.period <<= qos.period;
}

inline void
operator>>= (const ::DDS::DeadlineQosPolicy &qos, ::DDS_DeadlineQosPolicy & ddsqos)
{
  ddsqos.period <<= qos.period;
}
#endif

#endif /* CIAO_RTI_DEADLINEQOSPOLICY_H */
