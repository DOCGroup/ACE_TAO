/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_PUBLICATIONMATCHEDSTATUS_H
#define DDS4CCM_PUBLICATIONMATCHEDSTATUS_H

#include "InstanceHandle_t.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::PublicationMatchedStatus &ddsstatus, const ::DDS_PublicationMatchedStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.current_count = status.current_count;
  ddsstatus.current_count_change = status.current_count_change;
  ddsstatus.last_subscription_handle <<= status.last_subscription_handle;
}

inline void
operator<<= (::DDS_PublicationMatchedStatus &ddsstatus, const ::DDS::PublicationMatchedStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
  ddsstatus.current_count = status.current_count;
  ddsstatus.current_count_change = status.current_count_change;
  ddsstatus.last_subscription_handle <<= status.last_subscription_handle;
}

#endif /* DDS4CCM_PUBLICATIONMATCHEDSTATUS_H */
