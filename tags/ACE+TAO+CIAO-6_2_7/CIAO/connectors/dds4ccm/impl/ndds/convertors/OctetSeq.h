/**
 * @author Marcel Smit (msmit@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_OCTETSEQ_H
#define DDS4CCM_OCTETSEQ_H

inline void
operator<<= (::DDS::OctetSeq &ddsseq, const ::DDS_OctetSeq & seq)
{
  ddsseq.length (seq.length ());
  for (::DDS_Long i  = 0; i < seq.length (); ++i)
    {
      ddsseq[i] = seq[i];
    }
}

inline void
operator<<= (::DDS_OctetSeq &ddsseq, const ::DDS::OctetSeq & seq)
{
  ddsseq.length (seq.length ());
  for (::CORBA::ULong i  = 0; i < seq.length (); ++i)
    {
      ddsseq[i] = seq[i];
    }
}

#endif /* DDS4CCM_OCTETSEQ_H */
