/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_SAMPLEREJECTEDSTATUS_H
#define DDS4CCM_SAMPLEREJECTEDSTATUS_H

#include "InstanceHandle_t.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::SampleRejectedStatus DDS_SampleRejectedStatus;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::SampleRejectedStatus &ddsstatus, const ::DDS_SampleRejectedStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_reason = static_cast < ::DDS::SampleRejectedStatusKind> (status.last_reason);
  ddsstatus.last_instance_handle <<= status.last_instance_handle;
}

inline void
operator<<= (::DDS_SampleRejectedStatus &ddsstatus, const ::DDS::SampleRejectedStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_reason = static_cast < ::DDS_SampleRejectedStatusKind> (status.last_reason);
  ddsstatus.last_instance_handle <<= status.last_instance_handle;
}

#endif

#endif /* DDS4CCM_SAMPLEREJECTEDSTATUS_H */
