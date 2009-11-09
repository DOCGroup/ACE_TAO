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
#include "Time_t.h"

inline void
operator<<= (::CCM_DDS::AccessStatus & access_status, const ::DDS_SampleStateKind  & sample_state)
{
  if (sample_state == DDS_NOT_READ_SAMPLE_STATE)
    access_status = ::CCM_DDS::FRESH_INFO;
  else if (sample_state == DDS_READ_SAMPLE_STATE)
    access_status = ::CCM_DDS::ALREADY_SEEN;
}

inline void
operator<<= (::CCM_DDS::InstanceStatus & instance_status, const ::DDS_InstanceStateKind  & instance_state)
{
  if (instance_state == DDS_ALIVE_INSTANCE_STATE)
    instance_status = ::CCM_DDS::INSTANCE_CREATED;
  else if (instance_state == DDS_NOT_ALIVE_DISPOSED_INSTANCE_STATE)
    instance_status = ::CCM_DDS::INSTANCE_DELETED;
  else if (instance_state == DDS_NOT_ALIVE_NO_WRITERS_INSTANCE_STATE)
    instance_status = ::CCM_DDS::INSTANCE_UPDATED;
}

inline void
operator<<= (::CCM_DDS::ReadInfo & ccm_dds_readinfo, const ::DDS_SampleInfoSeq & sample_info)
{
  ccm_dds_readinfo.timestamp <<= sample_info[sample_info.length () - 1].reception_timestamp;
  ccm_dds_readinfo.access_status <<= sample_info[sample_info.length () - 1].sample_state;
  ccm_dds_readinfo.instance_status <<= sample_info[sample_info.length () - 1].instance_state;
  ccm_dds_readinfo.instance_rank = sample_info[sample_info.length () - 1].sample_rank;
}

inline void
operator<<= (::CCM_DDS::ReadInfoSeq & ccm_dds_readinfo_seq, const ::DDS_SampleInfoSeq & sample_info)
{
  ccm_dds_readinfo_seq.length(sample_info.length ());
  for (::DDS_Long i = 0; i < sample_info.length(); i++)
    {
      if (sample_info[i].valid_data)
        {
          ccm_dds_readinfo_seq[i].timestamp <<= sample_info[i].reception_timestamp;
          ccm_dds_readinfo_seq[i].access_status <<= sample_info[i].sample_state;
          ccm_dds_readinfo_seq[i].instance_status <<= sample_info[i].instance_state;
          ccm_dds_readinfo_seq[i].instance_rank = sample_info[i].sample_rank;
        }
    }
}

#endif
