/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DURATION_T_H_
#define DDS4CCM_DURATION_T_H_

#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::Duration_t DDS_Duration_t;
#endif

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::Duration_t &ddsduration, const ::DDS_Duration_t & duration)
{
  ddsduration.sec = duration.sec;
  ddsduration.nanosec = duration.nanosec;
}

inline void
operator<<= (::DDS_Duration_t &ddsduration, const ::DDS::Duration_t & duration)
{
  ddsduration.sec = duration.sec;
  ddsduration.nanosec = duration.nanosec;
}

#endif

#endif /* DDS4CCM_DURATION_T_H_ */

