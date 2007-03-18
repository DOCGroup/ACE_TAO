// -*- C++ -*-


//=============================================================================
/**
 *  @file    Time_Utilities.h
 *
 *  $Id$
 *
 *  @author  David Levine <levine@cs.wustl.edu>
 */
//=============================================================================


#ifndef ORBSVCS_TIME_UTILITIES_H
#define ORBSVCS_TIME_UTILITIES_H
#include /**/ "ace/pre.h"

#include "orbsvcs/TimeBaseC.h"
#include "orbsvcs/svc_utils_export.h"
#include "ace/OS_NS_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ORBSVCS_Time
 * 
 * @brief Time conversion utilities.
 *
 * This class defines several static methods to convert between
 * several ACE time structures and the CORBA TimeT structure.
 *
 * Recall that TimeBase::TimeT stores time in a 64-bit long long 
 * (or structure when that is not available) and uses hundreds of
 * nanoseconds as time units (i.e. 1 second is 10^7 TimeT units).
 *
 * To convert between the CORBA and the UNIX epochs, we need to adjust
 * absolute times by the difference between October 15th, 1582 (Julian
 * Date 2299160.5) and January 1st, 1970 (Julian Date 2440578.5):
 * 141418.0 days.
 */
class TAO_Svc_Utils_Export ORBSVCS_Time
{
public:
  /// @brief A helper constant, IDL does not have constant for structures.
  static TimeBase::TimeT zero ();

  /**
   * @name ACE_hrtime_t conversion routines
   */
  //@{
  // We interpret an ACE_hrtime_t as nanoseconds.

  /// @brief Convert relative high-resolution time to a <TimeBase::TimeT>.
  static void hrtime_to_TimeT (TimeBase::TimeT &lhs,
                               ACE_hrtime_t rhs);

  /// @brief Convert relative <TimeBase::TimeT> to a high-resolution time.
  static void TimeT_to_hrtime (ACE_hrtime_t &lhs,
                               const TimeBase::TimeT &rhs);

  /// @brief Convert relative <TimeBase::TimeT> to a high-resolution time.
  static ACE_hrtime_t to_hrtime (const TimeBase::TimeT &t);
  //@}


  /**
   * @name ACE_Time_Value conversion routines
   */
  //@{
  // Recall that <ACE_Time_Value>s are <seconds, useconds> pairs.

  /// @brief Convert relative <ACE_Time_Value> to a <TimeBase::TimeT>.
  static void Time_Value_to_TimeT (TimeBase::TimeT& lhs,
                                   const ACE_Time_Value& rhs);

  /// @brief Convert relative <TimeBase::TimeT> to a <ACE_Time_Value>.
  static void TimeT_to_Time_Value (ACE_Time_Value& lrs,
                                   const TimeBase::TimeT& rhs);

  /// @brief Convert relative <TimeBase::TimeT> to a <ACE_Time_Value>.
  static ACE_Time_Value to_Time_Value (const TimeBase::TimeT &rhs);

  /// @brief Convert absolute <ACE_Time_Value> to a <TimeBase::TimeT>
  static void Absolute_Time_Value_to_TimeT (TimeBase::TimeT& lhs,
					    const ACE_Time_Value& rhs);

  /// @brief Convert absolute <TimeBase::TimeT> to a <ACE_Time_Value>
  static void Absolute_TimeT_to_Time_Value (ACE_Time_Value& lrs,
					    const TimeBase::TimeT& rhs);

  /// @brief Convert absolute <TimeBase::TimeT> to a <ACE_Time_Value>.
  static ACE_Time_Value to_Absolute_Time_Value (const TimeBase::TimeT &rhs);
  //@}

private:
  /// @brief Number of nanoseconds between the CORBA and UNIX epochs.
  static const ACE_UINT64 Time_Base_Offset;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Time_Utilities.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ORBSVCS_TIME_UTILITIES_H */
