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

#include "dds4ccm/impl/dds4ccm_conf.h"

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

#endif /* DDS4CCM_QOSPOLICYCOUNTSEQ_H */
