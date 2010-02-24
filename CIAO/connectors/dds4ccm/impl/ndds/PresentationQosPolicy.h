/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_PRESENTATIONQOSPOLICY_H
#define CIAO_RTI_PRESENTATIONQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::PresentationQosPolicy DDS_PresentationQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::PresentationQosPolicy &ddsqos, const ::DDS_PresentationQosPolicy & qos)
{
  ddsqos.coherent_access = qos.coherent_access;
  ddsqos.ordered_access = qos.ordered_access;
}

inline void
operator<<= (::DDS_PresentationQosPolicy &ddsqos, const ::DDS::PresentationQosPolicy & qos)
{
  ddsqos.coherent_access = qos.coherent_access;
  ddsqos.ordered_access = qos.ordered_access;
}

inline void
operator>>= (const ::DDS_PresentationQosPolicy &qos, ::DDS::PresentationQosPolicy & ddsqos)
{
  ddsqos.coherent_access = qos.coherent_access;
  ddsqos.ordered_access = qos.ordered_access;
}

inline void
operator>>= (const ::DDS::PresentationQosPolicy &qos, ::DDS_PresentationQosPolicy & ddsqos)
{
  ddsqos.coherent_access = qos.coherent_access;
  ddsqos.ordered_access = qos.ordered_access;
}
#endif

#endif /* CIAO_RTI_PRESENTATIONQOSPOLICY_H */
