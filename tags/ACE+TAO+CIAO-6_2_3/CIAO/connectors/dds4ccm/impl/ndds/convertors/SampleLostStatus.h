/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_SAMPLELOSTSTATUS_H
#define DDS4CCM_SAMPLELOSTSTATUS_H

#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::SampleLostStatus &ddsstatus, const ::DDS_SampleLostStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
}

inline void
operator>>= (const ::DDS_SampleLostStatus &status, ::DDS::SampleLostStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
}

#endif /* DDS4CCM_SAMPLELOSTSTATUS_H */
