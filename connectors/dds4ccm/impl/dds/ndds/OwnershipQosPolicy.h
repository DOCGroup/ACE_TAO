/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_OWNERSHIPQOSPOLICY_H
#define DDS4CCM_OWNERSHIPQOSPOLICY_H

#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::OwnershipQosPolicy DDS_OwnershipQosPolicy;
#endif

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::OwnershipQosPolicy &ddsqos, const ::DDS_OwnershipQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS::OwnershipQosPolicyKind> (qos.kind);
}

inline void
operator<<= (::DDS_OwnershipQosPolicy &ddsqos, const ::DDS::OwnershipQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS_OwnershipQosPolicyKind> (qos.kind);
}

#endif

#endif /* DDS4CCM_OWNERSHIPQOSPOLICY_H */
