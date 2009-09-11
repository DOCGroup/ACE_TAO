/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
 
#ifndef CIAO_RTI_SAMPLEREJECTEDSTATUS_H
#define CIAO_RTI_SAMPLEREJECTEDSTATUS_H

#include "InstanceHandle_t.h"

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

inline void
operator>>= (const ::DDS_SampleRejectedStatus &status, ::DDS::SampleRejectedStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_reason = static_cast < ::DDS::SampleRejectedStatusKind> (status.last_reason);
  ddsstatus.last_instance_handle <<= status.last_instance_handle;
}

inline void
operator>>= (const ::DDS::SampleRejectedStatus &status, ::DDS_SampleRejectedStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_reason = static_cast < ::DDS_SampleRejectedStatusKind> (status.last_reason);
  ddsstatus.last_instance_handle <<= status.last_instance_handle; 
}

#endif /* CIAO_RTI_SAMPLEREJECTEDSTATUS_H */
