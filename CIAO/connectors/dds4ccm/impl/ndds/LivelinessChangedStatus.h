/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_LIVELINESSCHANGEDSTATUS_H
#define CIAO_RTI_LIVELINESSCHANGEDSTATUS_H

#include "InstanceHandle_t.h"

inline void
operator<<= (::DDS::LivelinessChangedStatus &ddsstatus, const ::DDS_LivelinessChangedStatus & status)
{
  ddsstatus.alive_count = status.alive_count;
  ddsstatus.not_alive_count = status.not_alive_count;
  ddsstatus.alive_count_change = status.alive_count_change;
  ddsstatus.not_alive_count_change = status.not_alive_count_change;
  ddsstatus.last_publication_handle <<= status.last_publication_handle;
}

inline void
operator<<= (::DDS_LivelinessChangedStatus &ddsstatus, const ::DDS::LivelinessChangedStatus & status)
{
  ddsstatus.alive_count = status.alive_count;
  ddsstatus.not_alive_count = status.not_alive_count;
  ddsstatus.alive_count_change = status.alive_count_change;
  ddsstatus.not_alive_count_change = status.not_alive_count_change;
  ddsstatus.last_publication_handle <<= status.last_publication_handle;
}

inline void
operator>>= (const ::DDS_LivelinessChangedStatus &status, ::DDS::LivelinessChangedStatus & ddsstatus)
{
  ddsstatus.alive_count = status.alive_count;
  ddsstatus.not_alive_count = status.not_alive_count;
  ddsstatus.alive_count_change = status.alive_count_change;
  ddsstatus.not_alive_count_change = status.not_alive_count_change;
  ddsstatus.last_publication_handle <<= status.last_publication_handle;
}

inline void
operator>>= (const ::DDS::LivelinessChangedStatus &status, ::DDS_LivelinessChangedStatus & ddsstatus)
{
  ddsstatus.alive_count = status.alive_count;
  ddsstatus.not_alive_count = status.not_alive_count;
  ddsstatus.alive_count_change = status.alive_count_change;
  ddsstatus.not_alive_count_change = status.not_alive_count_change;
  ddsstatus.last_publication_handle <<= status.last_publication_handle;
}

#endif /* CIAO_RTI_LIVELINESSCHANGEDSTATUS_H */
