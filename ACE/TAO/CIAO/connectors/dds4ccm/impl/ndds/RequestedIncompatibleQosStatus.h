/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_REQUESTEDINCOMPATIBLEQOSSTATUS_H
#define CIAO_RTI_REQUESTEDINCOMPATIBLEQOSSTATUS_H

#include "QosPolicyCountSeq.h"

inline void
operator<<= (::DDS::RequestedIncompatibleQosStatus &ddsstatus, const ::DDS_RequestedIncompatibleQosStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_policy_id = status.last_policy_id;
  ddsstatus.policies <<= status.policies;
}

inline void
operator<<= (::DDS_RequestedIncompatibleQosStatus &ddsstatus, const ::DDS::RequestedIncompatibleQosStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_policy_id = static_cast <DDS_QosPolicyId_t>(status.last_policy_id);
  ddsstatus.policies <<= status.policies;
}

inline void
operator>>= (const ::DDS_RequestedIncompatibleQosStatus &status, ::DDS::RequestedIncompatibleQosStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_policy_id = status.last_policy_id;
  ddsstatus.policies <<= status.policies;
}

inline void
operator>>= (const ::DDS::RequestedIncompatibleQosStatus &status, ::DDS_RequestedIncompatibleQosStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.last_policy_id = static_cast <DDS_QosPolicyId_t>(status.last_policy_id);
  ddsstatus.policies <<= status.policies;
}

#endif /* CIAO_RTI_REQUESTEDINCOMPATIBLEQOSSTATUS_H */
