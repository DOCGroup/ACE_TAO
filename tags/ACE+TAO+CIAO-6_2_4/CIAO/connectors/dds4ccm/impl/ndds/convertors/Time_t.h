/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_TIME_T_H_
#define DDS4CCM_TIME_T_H_

#include "dds4ccm/impl/dds4ccm_conf.h"

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

#endif /* DDS4CCM_TIME_T_H_ */

