/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_RELIABLEWRITERCACHECHANGEDSTATUS_H
#define DDS4CCM_RELIABLEWRITERCACHECHANGEDSTATUS_H

#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::ReliableWriterCacheChangedStatus &ddsstatus, const ::DDS_ReliableWriterCacheChangedStatus & status)
{
  ddsstatus.empty_reliable_writer_cache.total_count = status.empty_reliable_writer_cache.total_count;
  ddsstatus.empty_reliable_writer_cache.total_count_change = status.empty_reliable_writer_cache.total_count_change;
  ddsstatus.full_reliable_writer_cache.total_count = status.full_reliable_writer_cache.total_count;
  ddsstatus.full_reliable_writer_cache.total_count_change = status.full_reliable_writer_cache.total_count_change;
  ddsstatus.low_watermark_reliable_writer_cache.total_count = status.low_watermark_reliable_writer_cache.total_count;
  ddsstatus.low_watermark_reliable_writer_cache.total_count_change = status.low_watermark_reliable_writer_cache.total_count_change;
  ddsstatus.high_watermark_reliable_writer_cache.total_count = status.high_watermark_reliable_writer_cache.total_count;
  ddsstatus.high_watermark_reliable_writer_cache.total_count_change = status.high_watermark_reliable_writer_cache.total_count_change;
  ddsstatus.unacknowledged_sample_count = status.unacknowledged_sample_count;
  ddsstatus.unacknowledged_sample_count_peak = status.unacknowledged_sample_count_peak;
}

inline void
operator<<= (::DDS_ReliableWriterCacheChangedStatus &ddsstatus, const ::DDS::ReliableWriterCacheChangedStatus & status)
{
  ddsstatus.empty_reliable_writer_cache.total_count = status.empty_reliable_writer_cache.total_count;
  ddsstatus.empty_reliable_writer_cache.total_count_change = status.empty_reliable_writer_cache.total_count_change;
  ddsstatus.full_reliable_writer_cache.total_count = status.full_reliable_writer_cache.total_count;
  ddsstatus.full_reliable_writer_cache.total_count_change = status.full_reliable_writer_cache.total_count_change;
  ddsstatus.low_watermark_reliable_writer_cache.total_count = status.low_watermark_reliable_writer_cache.total_count;
  ddsstatus.low_watermark_reliable_writer_cache.total_count_change = status.low_watermark_reliable_writer_cache.total_count_change;
  ddsstatus.high_watermark_reliable_writer_cache.total_count = status.high_watermark_reliable_writer_cache.total_count;
  ddsstatus.high_watermark_reliable_writer_cache.total_count_change = status.high_watermark_reliable_writer_cache.total_count_change;
  ddsstatus.unacknowledged_sample_count = status.unacknowledged_sample_count;
  ddsstatus.unacknowledged_sample_count_peak = status.unacknowledged_sample_count_peak;
}

#endif /* DDS4CCM_RELIABLEWRITERCACHECHANGEDSTATUS_H */
