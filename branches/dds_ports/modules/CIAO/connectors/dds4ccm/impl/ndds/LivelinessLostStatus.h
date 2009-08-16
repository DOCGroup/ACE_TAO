/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_LIVELINESSLOSTSTATUS_H
#define CIAO_RTI_LIVELINESSLOSTSTATUS_H

inline void
operator<<= (::DDS::LivelinessLostStatus &ddsstatus, const ::DDS_LivelinessLostStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
}

inline void
operator<<= (::DDS_LivelinessLostStatus &ddsstatus, const ::DDS::LivelinessLostStatus & status)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
}

inline void
operator>>= (const ::DDS_LivelinessLostStatus &status, ::DDS::LivelinessLostStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
}

inline void
operator>>= (const ::DDS::LivelinessLostStatus &status, ::DDS_LivelinessLostStatus & ddsstatus)
{
  ddsstatus.total_count = status.total_count;
  ddsstatus.total_count_change = status.total_count_change;
}

#endif /* CIAO_RTI_LIVELINESSLOSTSTATUS_H */
