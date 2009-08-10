/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_SUBSCRIPTIONMATCHEDSTATUS_H
#define CIAO_RTI_SUBSCRIPTIONMATCHEDSTATUS_H

#include "InstanceHandle_t.h"

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

inline void
operator>>= (const ::DDS_SubscriptionMatchedStatus &status, ::DDS::SubscriptionMatchedStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.current_count = status.current_count;
  ddsstatus.current_count_change = status.current_count_change;
  ddsstatus.last_publication_handle <<= status.last_publication_handle;
}

inline void
operator>>= (const ::DDS::SubscriptionMatchedStatus &status, ::DDS_SubscriptionMatchedStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.current_count = status.current_count;
  ddsstatus.current_count_change = status.current_count_change;
  ddsstatus.last_publication_handle <<= status.last_publication_handle;
}

#endif /* CIAO_RTI_SUBSCRIPTIONMATCHEDSTATUS_H */

