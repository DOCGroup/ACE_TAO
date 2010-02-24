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

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::Time_t DDS_Time_t;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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

inline void
operator>>= (const ::DDS_Time_t &time, ::DDS::Time_t & ddstime)
{
  ddstime.sec = time.sec;
  ddstime.nanosec = time.nanosec;
}
#endif

#endif /* CIAO_RTI_TIME_T_H */

