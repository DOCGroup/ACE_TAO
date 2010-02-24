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

inline void
operator<<= (::DDS_QosPolicyCountSeq &/*ddsstatus*/, const ::DDS::QosPolicyCountSeq & /*status*/)
{
}

inline void
operator>>= (const ::DDS_QosPolicyCountSeq &/*status*/, ::DDS::QosPolicyCountSeq & /*ddsstatus*/)
{
}

inline void
operator>>= (const ::DDS::QosPolicyCountSeq &/*status*/, ::DDS_QosPolicyCountSeq & /*ddsstatus*/)
{
}

#endif /* CIAO_RTI_QOSPOLICYCOUNTSEQ_H */
