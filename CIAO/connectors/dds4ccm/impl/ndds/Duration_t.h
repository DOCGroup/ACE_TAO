/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_DURATION_T_H
#define CIAO_RTI_DURATION_T_H

inline void
operator<<= (::DDS::Duration_t &ddsstatus, const ::DDS_Duration_t & status)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

inline void
operator<<= (::DDS_Duration_t &ddsstatus, const ::DDS::Duration_t & status)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

inline void
operator>>= (const ::DDS_Duration_t &status, ::DDS::Duration_t & ddsstatus)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

#endif /* CIAO_RTI_DURATION_T_H */

