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
operator<<= (::CCM_DDS::ReadInfo & ccm_dds_readinfo, const ::DDS_SampleInfoSeq & sample_info)
{
  //an_instance = data[number_of_instances-1];
  ccm_dds_readinfo.timestamp <<= sample_info[sample_info.length () - 1].reception_timestamp;
  //what about the following attributes?
  //info.access_status     DDS_SampleStateKind  sample_state or   DDS_ViewStateKind view_state; ?
  //info.instance_status   DDS_InstanceStateKind instance_state;
  //info.instance_rank     DDS_Long sample_rank;   is always 0 with last sample 
}

inline void
operator<<= (::CCM_DDS::ReadInfoSeq & ccm_dds_readinfo_seq, const ::DDS_SampleInfoSeq & sample_info)
{
  ccm_dds_readinfo_seq.length(sample_info.length ());
  for (CORBA::ULong i = 0; i < (CORBA::ULong)sample_info.length(); i++)
    ccm_dds_readinfo_seq[i].timestamp <<= sample_info[i].reception_timestamp;
  
  //what about the following attributes?
  //info.access_status     DDS_SampleStateKind  sample_state or   DDS_ViewStateKind view_state; ?
  //info.instance_status   DDS_InstanceStateKind instance_state;
  //info.instance_rank     DDS_Long sample_rank;   is always 0 with last sample 
}

#endif
