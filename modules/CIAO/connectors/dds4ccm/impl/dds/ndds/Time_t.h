/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_TIME_T_H
#define DDS4CCM_TIME_T_H

#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::Time_t DDS_Time_t;
#endif

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::Time_t &ddstime, const ::DDS_Time_t & time)
{
  ddstime.sec = time.sec;
  ddstime.nanosec = time.nanosec;
}

inline void
operator<<= (::DDS_Time_t &ddstime, const ::DDS::Time_t & time)
{
  ddstime.sec = time.sec;
  ddstime.nanosec = time.nanosec;
}

#endif

#endif /* DDS4CCM_TIME_T_H */

