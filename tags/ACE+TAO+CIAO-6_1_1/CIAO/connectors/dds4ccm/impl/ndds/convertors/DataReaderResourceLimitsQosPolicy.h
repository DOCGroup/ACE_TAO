/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DDS_DATAREADERRESOURCELIMITSQOSPOLICY_H
#define DDS4CCM_DDS_DATAREADERRESOURCELIMITSQOSPOLICY_H

#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::DataReaderResourceLimitsQosPolicy &ddsqos, const ::DDS_DataReaderResourceLimitsQosPolicy & qos)
{
  ddsqos.max_remote_writers = qos.max_remote_writers;
  ddsqos.max_remote_writers_per_instance = qos.max_remote_writers_per_instance;
  ddsqos.max_samples_per_remote_writer = qos.max_samples_per_remote_writer;
  ddsqos.max_infos = qos.max_infos;
  ddsqos.initial_remote_writers = qos.initial_remote_writers;
  ddsqos.initial_remote_writers_per_instance = qos.initial_remote_writers_per_instance;
  ddsqos.initial_infos = qos.initial_infos;
  ddsqos.initial_outstanding_reads = qos.initial_outstanding_reads;
  ddsqos.max_outstanding_reads = qos.max_outstanding_reads;
  ddsqos.max_samples_per_read = qos.max_samples_per_read;
  ddsqos.disable_fragmentation_support = qos.disable_fragmentation_support;
  ddsqos.max_fragmented_samples = qos.max_fragmented_samples;
  ddsqos.initial_fragmented_samples = qos.initial_fragmented_samples;
  ddsqos.max_fragmented_samples_per_remote_writer = qos.max_fragmented_samples_per_remote_writer;
  ddsqos.max_fragments_per_sample = qos.max_fragments_per_sample;
  ddsqos.dynamically_allocate_fragmented_samples = qos.dynamically_allocate_fragmented_samples;
  ddsqos.max_total_instances = qos.max_total_instances;
  ddsqos.max_remote_virtual_writers_per_instance = qos.max_remote_virtual_writers_per_instance;
  ddsqos.initial_remote_virtual_writers_per_instance = qos.initial_remote_virtual_writers_per_instance;
  ddsqos.max_query_condition_filters = qos.max_query_condition_filters;
}

inline void
operator<<= (::DDS_DataReaderResourceLimitsQosPolicy &ddsqos, const ::DDS::DataReaderResourceLimitsQosPolicy & qos)
{
  ddsqos.max_remote_writers = qos.max_remote_writers;
  ddsqos.max_remote_writers_per_instance = qos.max_remote_writers_per_instance;
  ddsqos.max_samples_per_remote_writer = qos.max_samples_per_remote_writer;
  ddsqos.max_infos = qos.max_infos;
  ddsqos.initial_remote_writers = qos.initial_remote_writers;
  ddsqos.initial_remote_writers_per_instance = qos.initial_remote_writers_per_instance;
  ddsqos.initial_infos = qos.initial_infos;
  ddsqos.initial_outstanding_reads = qos.initial_outstanding_reads;
  ddsqos.max_outstanding_reads = qos.max_outstanding_reads;
  ddsqos.max_samples_per_read = qos.max_samples_per_read;
  ddsqos.disable_fragmentation_support = qos.disable_fragmentation_support;
  ddsqos.max_fragmented_samples = qos.max_fragmented_samples;
  ddsqos.initial_fragmented_samples = qos.initial_fragmented_samples;
  ddsqos.max_fragmented_samples_per_remote_writer = qos.max_fragmented_samples_per_remote_writer;
  ddsqos.max_fragments_per_sample = qos.max_fragments_per_sample;
  ddsqos.dynamically_allocate_fragmented_samples = qos.dynamically_allocate_fragmented_samples;
  ddsqos.max_total_instances = qos.max_total_instances;
  ddsqos.max_remote_virtual_writers_per_instance = qos.max_remote_virtual_writers_per_instance;
  ddsqos.initial_remote_virtual_writers_per_instance = qos.initial_remote_virtual_writers_per_instance;
  ddsqos.max_query_condition_filters = qos.max_query_condition_filters;
}

#endif /* DDS4CCM_DDS_DATAREADERRESOURCELIMITSQOSPOLICY_H */
