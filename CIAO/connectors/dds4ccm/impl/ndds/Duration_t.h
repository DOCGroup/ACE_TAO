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

inline void
operator>>= (const ::DDS_Duration_t &duration, ::DDS::Duration_t & ddsduration)
{
  ddsduration.sec = duration.sec;
  ddsduration.nanosec = duration.nanosec;
}

#endif /* CIAO_RTI_DURATION_T_H */

