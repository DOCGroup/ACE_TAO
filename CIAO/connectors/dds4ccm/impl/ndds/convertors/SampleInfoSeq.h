/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef SAMPLEINFOSEQ_H
#define SAMPLEINFOSEQ_H

#include "dds4ccm/impl/dds4ccm_conf.h"

#include "Time_t.h"
#include "InstanceHandle_t.h"

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
operator<<= (::DDS::SampleInfoSeq &ddsinfo_seq, const ::DDS_SampleInfoSeq & info_seq)
{
  ddsinfo_seq.length (info_seq.length ());
  for (::DDS_Long i = 0; i < info_seq.length (); ++i)
    {
      ddsinfo_seq[i] <<= info_seq[i];
    }
}

#endif /* DDS4CCM_SAMPLELOSTSTATUS_H */
