/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_INSTANCEHANDLESEQ_H
#define CIAO_RTI_INSTANCEHANDLESEQ_H

#include "InstanceHandle_t.h"

inline void
operator<<= (::DDS::InstanceHandleSeq &ddsinstancehandleseqseq, const ::DDS_InstanceHandleSeq & instancehandleseq)
{
  ddsinstancehandleseqseq.length (instancehandleseq.length ());
  for (::DDS_Long index = 0; index < instancehandleseq.length (); index++)
    {
      ddsinstancehandleseqseq[index] <<= instancehandleseq[index];
    }
}

inline void
operator<<= (::DDS_InstanceHandleSeq &ddsinstancehandleseqseq, const ::DDS::InstanceHandleSeq & instancehandleseq)
{
  ddsinstancehandleseqseq.length (instancehandleseq.length ());
  for (::DDS::InstanceHandleSeq::_size_type index = 0; index < instancehandleseq.length (); index++)
    {
      ddsinstancehandleseqseq[index] <<= instancehandleseq[index];
    }
}

inline void
operator>>= (const ::DDS_InstanceHandleSeq &instancehandle, ::DDS::InstanceHandleSeq & ddsinstancehandleseqseq)
{
  ddsinstancehandleseqseq.length (instancehandle.length ());
  for (::DDS_Long index = 0; index < instancehandle.length (); index++)
    {
      ddsinstancehandleseqseq[index] <<= instancehandle[index];
    }
}

inline void
operator>>= (const ::DDS::InstanceHandleSeq &instancehandle, ::DDS_InstanceHandleSeq & ddsinstancehandleseqseq)
{
  ddsinstancehandleseqseq.length (instancehandle.length ());
  for (::DDS::InstanceHandleSeq::_size_type index = 0; index < instancehandle.length (); index++)
    {
      ddsinstancehandleseqseq[index] <<= instancehandle[index];
    }
}

#endif /* CIAO_RTI_INSTANCEHANDLESEQ_H */
