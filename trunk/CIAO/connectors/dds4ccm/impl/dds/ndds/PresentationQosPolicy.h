/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_PRESENTATIONQOSPOLICY_H
#define DDS4CCM_PRESENTATIONQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::PresentationQosPolicy DDS_PresentationQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::PresentationQosPolicy &ddsqos, const ::DDS_PresentationQosPolicy & qos)
{
  ddsqos.access_scope = static_cast < ::DDS::PresentationQosPolicyAccessScopeKind> (qos.access_scope);
  ddsqos.coherent_access = qos.coherent_access;
  ddsqos.ordered_access = qos.ordered_access;
}

inline void
operator<<= (::DDS_PresentationQosPolicy &ddsqos, const ::DDS::PresentationQosPolicy & qos)
{
  ddsqos.access_scope = static_cast < DDS_PresentationQosPolicyAccessScopeKind> (qos.access_scope);
  ddsqos.coherent_access = qos.coherent_access;
  ddsqos.ordered_access = qos.ordered_access;
}

inline void
operator>>= (const ::DDS_PresentationQosPolicy &qos, ::DDS::PresentationQosPolicy & ddsqos)
{
  ddsqos.access_scope = static_cast < ::DDS::PresentationQosPolicyAccessScopeKind> (qos.access_scope);
  ddsqos.coherent_access = qos.coherent_access;
  ddsqos.ordered_access = qos.ordered_access;
}

inline void
operator>>= (const ::DDS::PresentationQosPolicy &qos, ::DDS_PresentationQosPolicy & ddsqos)
{
  ddsqos.access_scope = static_cast < DDS_PresentationQosPolicyAccessScopeKind> (qos.access_scope);
  ddsqos.coherent_access = qos.coherent_access;
  ddsqos.ordered_access = qos.ordered_access;
}
#endif

#endif /* DDS4CCM_PRESENTATIONQOSPOLICY_H */
