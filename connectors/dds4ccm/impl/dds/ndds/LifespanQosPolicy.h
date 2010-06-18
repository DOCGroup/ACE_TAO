/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_LIFESPANQOSPOLICY_H
#define DDS4CCM_LIFESPANQOSPOLICY_H

#include "Duration_t.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::LifespanQosPolicy DDS_LifespanQosPolicy;
#endif

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::LifespanQosPolicy &ddsqos, const ::DDS_LifespanQosPolicy & qos)
{
  ddsqos.duration <<= qos.duration;
}

inline void
operator<<= (::DDS_LifespanQosPolicy &ddsqos, const ::DDS::LifespanQosPolicy & qos)
{
  ddsqos.duration <<= qos.duration;
}

#endif

#endif /* DDS4CCM_LIFESPANQOSPOLICY_H */
