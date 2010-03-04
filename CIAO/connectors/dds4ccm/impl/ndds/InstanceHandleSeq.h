/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_INSTANCEHANDLESEQ_H
#define DDS4CCM_INSTANCEHANDLESEQ_H

#include "InstanceHandle_t.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::InstanceHandleSeq DDS_InstanceHandleSeq;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
template <typename corbaseq, typename ddsseq>
void convert_dds_to_corba_sequence(corbaseq & target, const ddsseq& source) {
  target.length (source.length ());
  for (::DDS_Long index = 0; index < source.length (); index++)
    {
      target[index] <<= source[index];
    }
}

inline void
operator<<= (::DDS::InstanceHandleSeq &ddsinstancehandleseqseq, const ::DDS_InstanceHandleSeq & instancehandleseq)
{
  convert_dds_to_corba_sequence (ddsinstancehandleseqseq, instancehandleseq);
}

inline void
operator<<= (::DDS_InstanceHandleSeq &ddsinstancehandleseqseq, const ::DDS::InstanceHandleSeq & instancehandleseq)
{
  ddsinstancehandleseqseq.length (instancehandleseq.length ());
  for (::DDS::InstanceHandleSeq::size_type index = 0; index < instancehandleseq.length (); index++)
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
  for (::DDS::InstanceHandleSeq::size_type index = 0; index < instancehandle.length (); index++)
    {
      ddsinstancehandleseqseq[index] <<= instancehandle[index];
    }
}
#endif

#endif /* DDS4CCM_INSTANCEHANDLESEQ_H */
