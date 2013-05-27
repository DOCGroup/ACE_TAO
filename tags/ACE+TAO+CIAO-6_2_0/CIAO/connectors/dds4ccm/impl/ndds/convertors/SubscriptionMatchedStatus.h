/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_SUBSCRIPTIONMATCHEDSTATUS_H
#define DDS4CCM_SUBSCRIPTIONMATCHEDSTATUS_H

#include "InstanceHandle_t.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::SubscriptionMatchedStatus &ddsstatus, const ::DDS_SubscriptionMatchedStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.current_count = status.current_count;
  ddsstatus.current_count_change = status.current_count_change;
  ddsstatus.last_publication_handle <<= status.last_publication_handle;
}

inline void
operator<<= (::DDS_SubscriptionMatchedStatus &ddsstatus, const ::DDS::SubscriptionMatchedStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.current_count = status.current_count;
  ddsstatus.current_count_change = status.current_count_change;
  ddsstatus.last_publication_handle <<= status.last_publication_handle;
}

#endif /* DDS4CCM_SUBSCRIPTIONMATCHEDSTATUS_H */

