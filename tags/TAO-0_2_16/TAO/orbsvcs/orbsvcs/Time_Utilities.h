/* -*- C++ -*- */
//
// $Id$
//

#ifndef ORBSVCS_TIME_UTILITIES_H
#define ORBSVCS_TIME_UTILITIES_H

#include "orbsvcs/CosTimeBaseC.h"
#include "orbsvcs/orbsvcs_export.h"

class TAO_ORBSVCS_Export ORBSVCS_Time
{
  // = TITLE
  //   Time conversion utilities.
  //
  // = DESCRIPTION
  //   This class define several static methods to convert between
  //   several time structures and the CORBA TimeT structure.
  //
  //   Recall that TimeT stores time in a 64-bit long long (or
  //   structure when that is not available) and uses hundreds of
  //   nanoseconds as time units (i.e. 1 second is 10^7 TimeT units).
  //
public:

  // = A helper constant, IDL does not have constant for structures.
  static TimeBase::TimeT zero;

  // = ACE_hrtime_t conversion routines, we interpret an ACE_hrtime_t
  // as nanoseconds.
  static void
  hrtime_to_TimeT (TimeBase::TimeT& lhs,
                   ACE_hrtime_t rhs);
  static void
  TimeT_to_hrtime (ACE_hrtime_t &lhs,
                   const TimeBase::TimeT& rhs);
  static ACE_hrtime_t
  to_hrtime (const TimeBase::TimeT& t);

  // = ACE_Time_Value conversion routines, recall that they are
  // <seconds,useconds> pairs.
  static void
  Time_Value_to_TimeT (TimeBase::TimeT& lhs,
                       const ACE_Time_Value& rhs);
  static void
  TimeT_to_Time_Value (ACE_Time_Value& lrs,
                       const TimeBase::TimeT& rhs);
  static ACE_Time_Value
  to_Time_Value (const TimeBase::TimeT& rhs);
};

#if defined (__ACE_INLINE__)
#include "orbsvcs/Time_Utilities.i"
#endif /* __ACE_INLINE__ */

#endif /* ORBSVCS_TIME_UTILITIES_H */
