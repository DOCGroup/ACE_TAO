/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_REQUESTEDINCOMPATIBLEQOSSTATUS_H
#define DDS4CCM_REQUESTEDINCOMPATIBLEQOSSTATUS_H

#include "QosPolicyCountSeq.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

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

#endif /* DDS4CCM_REQUESTEDINCOMPATIBLEQOSSTATUS_H */
