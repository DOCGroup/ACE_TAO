/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef SAMPLE_INFO_H
#define SAMPLE_INFO_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#include "Time_t.h"
#include "InstanceHandle_t.h"

inline void
operator<<= (::CCM_DDS::AccessStatus & access_status, const ::DDS_SampleStateKind & sample_state)
{
  if (sample_state == ::DDS::NOT_READ_SAMPLE_STATE)
    {
      access_status = ::CCM_DDS::FRESH_INFO;
    }
  else if (sample_state == ::DDS::READ_SAMPLE_STATE)
    {
      access_status = ::CCM_DDS::ALREADY_SEEN;
    }
}

inline void
operator<<= (::CCM_DDS::ReadInfo& ccm_dds_readinfo, const ::DDS_SampleInfo& sample_info)
{
  ccm_dds_readinfo.source_timestamp <<= sample_info.source_timestamp;
  ccm_dds_readinfo.access_status <<= sample_info.sample_state;
  ccm_dds_readinfo.instance_handle <<= sample_info.instance_handle;

  if (sample_info.instance_state == ::DDS::ALIVE_INSTANCE_STATE &&
      sample_info.view_state == ::DDS::NEW_VIEW_STATE)
    {
      // Instance_status new
      ccm_dds_readinfo.instance_status = ::CCM_DDS::INSTANCE_CREATED;
    }
  else if (sample_info.instance_state == ::DDS::NOT_ALIVE_DISPOSED_INSTANCE_STATE)
    {
      // Instance_status deleted
      ccm_dds_readinfo.instance_status = ::CCM_DDS::INSTANCE_DELETED;
    }
  else if ((sample_info.instance_state == ::DDS::ALIVE_INSTANCE_STATE &&
            sample_info.view_state == ::DDS::NOT_NEW_VIEW_STATE) ||
            sample_info.instance_state == ::DDS::NOT_ALIVE_NO_WRITERS_INSTANCE_STATE)
    {
      // Instance_status updated
      ccm_dds_readinfo.instance_status = ::CCM_DDS::INSTANCE_UPDATED;
    }
}

inline void
operator<<= (::DDS::SampleInfo& sample_info, const ::DDS_SampleInfo& dds_sample_info)
{
  sample_info.sample_state = dds_sample_info.sample_state;
  sample_info.view_state = dds_sample_info.view_state;
  sample_info.instance_state = dds_sample_info.instance_state;
  sample_info.source_timestamp <<= dds_sample_info.source_timestamp;
  sample_info.instance_handle <<= dds_sample_info.instance_handle;
  sample_info.publication_handle <<= dds_sample_info.publication_handle;
  sample_info.disposed_generation_count = dds_sample_info.disposed_generation_count;
  sample_info.no_writers_generation_count = dds_sample_info.no_writers_generation_count;
  sample_info.sample_rank = dds_sample_info.sample_rank;
  sample_info.generation_rank = dds_sample_info.generation_rank;
  sample_info.absolute_generation_rank = dds_sample_info.absolute_generation_rank;
  sample_info.valid_data = dds_sample_info.valid_data;
}

inline void
operator<<= (::DDS_SampleInfo& sample_info, const ::DDS::SampleInfo& dds_sample_info)
{
  sample_info.sample_state = static_cast< ::DDS_SampleStateKind >(dds_sample_info.sample_state);
  sample_info.view_state = static_cast< ::DDS_ViewStateKind >(dds_sample_info.view_state);
  sample_info.instance_state = static_cast< ::DDS_InstanceStateKind >(dds_sample_info.instance_state);
  sample_info.source_timestamp <<= dds_sample_info.source_timestamp;
  sample_info.instance_handle <<= dds_sample_info.instance_handle;
  sample_info.publication_handle <<= dds_sample_info.publication_handle;
  sample_info.disposed_generation_count = dds_sample_info.disposed_generation_count;
  sample_info.no_writers_generation_count = dds_sample_info.no_writers_generation_count;
  sample_info.sample_rank = dds_sample_info.sample_rank;
  sample_info.generation_rank = dds_sample_info.generation_rank;
  sample_info.absolute_generation_rank = dds_sample_info.absolute_generation_rank;
  sample_info.valid_data = dds_sample_info.valid_data;
}

#endif
