/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef SAMPLEINFOSEQ_H
#define SAMPLEINFOSEQ_H

#include "dds4ccm/impl/dds4ccm_conf.h"
#include "SampleInfo.h"

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
