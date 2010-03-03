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

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::LifespanQosPolicy DDS_LifespanQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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

inline void
operator>>= (const ::DDS_LifespanQosPolicy &qos, ::DDS::LifespanQosPolicy & ddsqos)
{
  ddsqos.duration <<= qos.duration;
}

inline void
operator>>= (const ::DDS::LifespanQosPolicy &qos, ::DDS_LifespanQosPolicy & ddsqos)
{
  ddsqos.duration <<= qos.duration;
}
#endif

#endif /* DDS4CCM_LIFESPANQOSPOLICY_H */
