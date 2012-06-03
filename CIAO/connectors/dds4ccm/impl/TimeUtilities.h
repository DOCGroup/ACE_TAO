// $Id$

//=============================================================================
/**
 *  @file    TimeUtilities.h
 *
 *  $Id$
 *
 *  @author  Marcel Smit (msmit@remedy.nl)
 */
//=============================================================================


#ifndef DDS4CCM_TIME_UTILITIES_H
#define DDS4CCM_TIME_UTILITIES_H

#include "ace/Time_Value.h"
#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

inline void
operator<<= (::DDS::Time_t & dds_time, const ::ACE_Time_Value & ace_time)
{
  dds_time.sec = static_cast < ::CORBA::Long> (ace_time.sec ());
  dds_time.nanosec = ace_time.usec () * 1000;
}

inline void
operator<<= (::ACE_Time_Value & ace_time, const ::DDS::Time_t &dds_time)
{
  ace_time.sec (dds_time.sec);
  ace_time.usec (dds_time.nanosec/1000);
}

#endif /* DDS4CCM_TIME_UTILITIES_H */
