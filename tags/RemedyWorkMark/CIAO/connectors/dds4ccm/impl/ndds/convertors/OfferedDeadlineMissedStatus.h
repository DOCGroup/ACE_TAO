/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_OFFEREDDEADLINEMISSEDSTATUS_H
#define DDS4CCM_OFFEREDDEADLINEMISSEDSTATUS_H

#include "InstanceHandle_t.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::OfferedDeadlineMissedStatus &ddsstatus, const ::DDS_OfferedDeadlineMissedStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_instance_handle <<= status.last_instance_handle;
}

inline void
operator<<= (::DDS_OfferedDeadlineMissedStatus &ddsstatus, const ::DDS::OfferedDeadlineMissedStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_instance_handle <<= status.last_instance_handle;
}

#endif /* DDS4CCM_OFFEREDDEADLINEMISSEDSTATUS_H */
