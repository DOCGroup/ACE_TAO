/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_REQUESTEDDEADLINEMISSEDSTATUS_H
#define CIAO_RTI_REQUESTEDDEADLINEMISSEDSTATUS_H

#include "InstanceHandle_t.h"

inline void
operator<<= (::DDS::RequestedDeadlineMissedStatus &ddsstatus, const ::DDS_RequestedDeadlineMissedStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_instance_handle <<= status.last_instance_handle;
}

inline void
operator<<= (::DDS_RequestedDeadlineMissedStatus &ddsstatus, const ::DDS::RequestedDeadlineMissedStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_instance_handle <<= status.last_instance_handle;
}

inline void
operator>>= (const ::DDS_RequestedDeadlineMissedStatus &status, ::DDS::RequestedDeadlineMissedStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_instance_handle <<= status.last_instance_handle;
}

inline void
operator>>= (const ::DDS::RequestedDeadlineMissedStatus &status, ::DDS_RequestedDeadlineMissedStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_instance_handle <<= status.last_instance_handle;
}

#endif /* CIAO_RTI_REQUESTEDDEADLINEMISSEDSTATUS_H */
