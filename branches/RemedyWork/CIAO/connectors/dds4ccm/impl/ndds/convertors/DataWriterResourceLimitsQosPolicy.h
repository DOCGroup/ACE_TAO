/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DDS_DATAWRITERRESOURCELIMITSQOSPOLICY_H
#define DDS4CCM_DDS_DATAWRITERRESOURCELIMITSQOSPOLICY_H

#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::DataWriterResourceLimitsQosPolicy &ddsqos, const ::DDS_DataWriterResourceLimitsQosPolicy & qos)
{
  ddsqos.initial_concurrent_blocking_threads = qos.initial_concurrent_blocking_threads;
  ddsqos.max_concurrent_blocking_threads = qos.max_concurrent_blocking_threads;
  ddsqos.max_remote_reader_filters = qos.max_remote_reader_filters;
  ddsqos.initial_batches = qos.initial_batches;
  ddsqos.max_batches = qos.max_batches;
  ddsqos.cookie_max_length = qos.cookie_max_length;
  ddsqos.instance_replacement = static_cast < ::DDS::DataWriterResourceLimitsInstanceReplacementKind> (qos.instance_replacement);
  ddsqos.replace_empty_instances = qos.replace_empty_instances;
  ddsqos.autoregister_instances = qos.autoregister_instances;
}

inline void
operator<<= (::DDS_DataWriterResourceLimitsQosPolicy &ddsqos, const ::DDS::DataWriterResourceLimitsQosPolicy & qos)
{
  ddsqos.initial_concurrent_blocking_threads = qos.initial_concurrent_blocking_threads;
  ddsqos.max_concurrent_blocking_threads = qos.max_concurrent_blocking_threads;
  ddsqos.max_remote_reader_filters = qos.max_remote_reader_filters;
  ddsqos.initial_batches = qos.initial_batches;
  ddsqos.max_batches = qos.max_batches;
  ddsqos.cookie_max_length = qos.cookie_max_length;
  ddsqos.instance_replacement = static_cast < ::DDS_DataWriterResourceLimitsInstanceReplacementKind> (qos.instance_replacement);
  ddsqos.replace_empty_instances = qos.replace_empty_instances;
  ddsqos.autoregister_instances = qos.autoregister_instances;
}

#endif /* DDS4CCM_DDS_DATAWRITERRESOURCELIMITSQOSPOLICY_H */
