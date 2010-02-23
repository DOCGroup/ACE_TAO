/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_QOSPOLICYCOUNTSEQ_H
#define CIAO_RTI_QOSPOLICYCOUNTSEQ_H

inline void
operator<<= (::DDS::QosPolicyCountSeq &/*ddsstatus*/, const ::DDS_QosPolicyCountSeq & /*status*/)
{
  // todo
}

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS_QosPolicyCountSeq &/*ddsstatus*/, const ::DDS::QosPolicyCountSeq & /*status*/)
{
}
#endif

inline void
operator>>= (const ::DDS_QosPolicyCountSeq &/*status*/, ::DDS::QosPolicyCountSeq & /*ddsstatus*/)
{
}

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator>>= (const ::DDS::QosPolicyCountSeq &/*status*/, ::DDS_QosPolicyCountSeq & /*ddsstatus*/)
{
}
#endif

#endif /* CIAO_RTI_QOSPOLICYCOUNTSEQ_H */
