/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_TIME_T_H
#define CIAO_RTI_TIME_T_H

#if defined (DDS_MAJOR_VERSION)
typedef ::DDS::Time_t DDS_Time_t;
#endif

inline void
operator<<= (::DDS::Time_t &ddstime, const ::DDS_Time_t & time)
{
  ddstime.sec = time.sec;
  ddstime.nanosec = time.nanosec;
}

#if !defined (DDS_MAJOR_VERSION)
inline void
operator<<= (::DDS_Time_t &ddstime, const ::DDS::Time_t & time)
{
  ddstime.sec = time.sec;
  ddstime.nanosec = time.nanosec;
}
#endif

inline void
operator>>= (const ::DDS_Time_t &time, ::DDS::Time_t & ddstime)
{
  ddstime.sec = time.sec;
  ddstime.nanosec = time.nanosec;
}

#endif /* CIAO_RTI_TIME_T_H */

