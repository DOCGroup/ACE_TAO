/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_QOSPOLICYCOUNTSEQ_H
#define DDS4CCM_QOSPOLICYCOUNTSEQ_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::QosPolicyCountSeq DDS_QosPolicyCountSeq;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::QosPolicyCountSeq & ddspolicycount, const ::DDS_QosPolicyCountSeq & policycount)
{
  ddspolicycount.length (policycount.length ());
  for (::DDS_Long i = 0 ; i < policycount.length (); ++i)
    {
      ddspolicycount[i].policy_id = policycount[i].policy_id;
      ddspolicycount[i].count = policycount[i].count;
    }
}

inline void
operator<<= (::DDS_QosPolicyCountSeq & ddspolicycount, const ::DDS::QosPolicyCountSeq & policycount)
{
  ddspolicycount.length (policycount.length ());
  for (::CORBA::ULong i = 0 ; i < policycount.length (); ++i)
    {
      ddspolicycount[i].policy_id = static_cast <DDS_QosPolicyId_t> (policycount[i].policy_id);
      ddspolicycount[i].count = policycount[i].count;
    }
}

inline void
operator>>= (const ::DDS_QosPolicyCountSeq & policycount, ::DDS::QosPolicyCountSeq & ddspolicycount)
{
  ddspolicycount.length (policycount.length ());
  for (::DDS_Long i = 0 ; i < policycount.length (); ++i)
    {
      ddspolicycount[i].policy_id = policycount[i].policy_id;
      ddspolicycount[i].count = policycount[i].count;
    }
}

inline void
operator>>= (const ::DDS::QosPolicyCountSeq & policycount, ::DDS_QosPolicyCountSeq & ddspolicycount)
{
  ddspolicycount.length (policycount.length ());
  for (::CORBA::ULong i = 0 ; i < policycount.length (); ++i)
    {
      ddspolicycount[i].policy_id = static_cast <DDS_QosPolicyId_t> (policycount[i].policy_id);
      ddspolicycount[i].count = policycount[i].count;
    }
}
#endif

#endif /* DDS4CCM_QOSPOLICYCOUNTSEQ_H */
