/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DURATION_T_H
#define DDS4CCM_DURATION_T_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::Duration_t DDS_Duration_t;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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
#endif

#endif /* DDS4CCM_DURATION_T_H */

