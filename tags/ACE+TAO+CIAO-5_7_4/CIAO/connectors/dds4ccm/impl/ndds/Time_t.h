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

inline void
operator<<= (::DDS::Time_t &ddsstatus, const ::DDS_Time_t & status)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

inline void
operator<<= (::DDS_Time_t &ddsstatus, const ::DDS::Time_t & status)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

inline void
operator>>= (const ::DDS_Time_t &status, ::DDS::Time_t & ddsstatus)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

#endif /* CIAO_RTI_TIME_T_H */

