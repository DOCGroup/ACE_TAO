/**
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_PRESENTATIONQOSPOLICY_H
#define DDS4CCM_PRESENTATIONQOSPOLICY_H

#include "dds4ccm/impl/dds4ccm_conf.h"

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

#endif /* DDS4CCM_PRESENTATIONQOSPOLICY_H */
