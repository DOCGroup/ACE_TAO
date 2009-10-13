/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_OFFEREDINCOMPATIBLEQOSSTATUS_H
#define CIAO_RTI_OFFEREDINCOMPATIBLEQOSSTATUS_H

#include "QosPolicyCountSeq.h"

inline void
operator<<= (::DDS::OfferedIncompatibleQosStatus &ddsstatus, const ::DDS_OfferedIncompatibleQosStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_policy_id = status.last_policy_id;
  ddsstatus.policies <<= status.policies;
}

inline void
operator<<= (::DDS_OfferedIncompatibleQosStatus &ddsstatus, const ::DDS::OfferedIncompatibleQosStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_policy_id = static_cast <DDS_QosPolicyId_t>(status.last_policy_id);
  ddsstatus.policies <<= status.policies;
}

inline void
operator>>= (const ::DDS_OfferedIncompatibleQosStatus &status, ::DDS::OfferedIncompatibleQosStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_policy_id = status.last_policy_id;
  ddsstatus.policies <<= status.policies;
}

inline void
operator>>= (const ::DDS::OfferedIncompatibleQosStatus &status, ::DDS_OfferedIncompatibleQosStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_policy_id = static_cast <DDS_QosPolicyId_t>(status.last_policy_id);
  ddsstatus.policies <<= status.policies;
}

#endif /* CIAO_RTI_OFFEREDINCOMPATIBLEQOSSTATUS_H */
