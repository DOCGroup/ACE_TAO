/**
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_TYPECONSISTENCYENFORCEMENTQOSPOLICY_H
#define DDS4CCM_TYPECONSISTENCYENFORCEMENTQOSPOLICY_H

#include "dds4ccm/impl/dds4ccm_conf.h"

#if defined (RTI_DDS_VERSION_MAJOR) && (RTI_DDS_VERSION_MAJOR >= 5)

inline void
operator<<= (::DDS::TypeConsistencyEnforcementQosPolicy &ddsqos, const ::DDS_TypeConsistencyEnforcementQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS::TypeConsistencyKind> (qos.kind);
}

inline void
operator<<= (::DDS_TypeConsistencyEnforcementQosPolicy &ddsqos, const ::DDS::TypeConsistencyEnforcementQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS_TypeConsistencyKind> (qos.kind);
}

#endif

#endif /* DDS4CCM_TYPECONSISTENCYENFORCEMENTQOSPOLICY_H */
