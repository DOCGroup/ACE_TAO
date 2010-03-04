/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_USERDATAQOSPOLICY_H
#define DDS4CCM_USERDATAQOSPOLICY_H

#include "OctetSeq.h"

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::UserDataQosPolicy DDS_UserDataQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::UserDataQosPolicy &ddsqos, const ::DDS_UserDataQosPolicy & qos)
{
  ddsqos.value <<= qos.value;
}

inline void
operator<<= (::DDS_UserDataQosPolicy &ddsqos, const ::DDS::UserDataQosPolicy & qos)
{
  ddsqos.value <<= qos.value;
}

#endif

#endif /* DDS4CCM_USERDATAQOSPOLICY_H */
