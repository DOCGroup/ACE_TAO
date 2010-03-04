/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_OFFEREDINCOMPATIBLEQOSSTATUS_H
#define DDS4CCM_OFFEREDINCOMPATIBLEQOSSTATUS_H

#include "QosPolicyCountSeq.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::OfferedIncompatibleQosStatus DDS_OfferedIncompatibleQosStatus;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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

#endif

#endif /* DDS4CCM_OFFEREDINCOMPATIBLEQOSSTATUS_H */
