// -*- C++ -*-

//=============================================================================
/**
 *  @file    Time_Value.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_TIME_VALUE_H
#define ACE_TIME_VALUE_H

#include "ace/pre.h"

#include "ace/OS_Export.h"
#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


# if !defined (ACE_HAS_WINCE) && !defined (ACE_PSOS_DIAB_MIPS)
#     include /**/ <time.h>
#   if defined (__Lynx__)
#     include /**/ <st.h>
#     include /**/ <sem.h>
#   endif /* __Lynx__ */
# endif /* ACE_HAS_WINCE ACE_PSOS_DIAB_MIPS */

# if defined (ACE_LACKS_SYSTIME_H)
// Some platforms may need to include this, but I suspect that most
// will get it from <time.h>
#   if defined (VXWORKS)
#     include /**/ <sys/times.h>
#   else
#     include /**/ <sys/time.h>
#   endif /* VXWORKS */
# endif /* ACE_LACKS_SYSTIME_H */

// HP-UX 10.20 doesn't define timespec_t - it defined struct timespec.
#if defined (HPUX_10)
typedef struct timespec timespec_t;
#endif /* HPUX_10 */

// Define some helpful constants.
// Not type-safe, and signed.  For backward compatibility.
#define ACE_ONE_SECOND_IN_MSECS 1000L
#define ACE_ONE_SECOND_IN_USECS 1000000L
#define ACE_ONE_SECOND_IN_NSECS 1000000000L

// -------------------------------------------------------------------
// These forward declarations are only used to circumvent a bug in
// MSVC 6.0 compiler.  They shouldn't cause any problem for other
// compilers and they can be removed once MS release a SP that contains
// the fix.
class ACE_Time_Value;
ACE_OS_Export ACE_Time_Value operator + (const ACE_Time_Value &tv1,
                                         const ACE_Time_Value &tv2);

ACE_OS_Export ACE_Time_Value operator - (const ACE_Time_Value &tv1,
                                         const ACE_Time_Value &tv2);

// This forward declaration is needed by the set() and FILETIME() functions
#if defined (ACE_LACKS_LONGLONG_T)
class ACE_Export ACE_U_LongLong;
#endif  /* ACE_LACKS_LONGLONG_T */
// -------------------------------------------------------------------

# if !defined (ACE_HAS_POSIX_TIME) && !defined (ACE_PSOS)
// Definition per POSIX.
typedef struct timespec
{
  /// Seconds
  time_t tv_sec;
  /// Nanoseconds
  long tv_nsec;
} timespec_t;
# elif defined (ACE_HAS_BROKEN_POSIX_TIME)
// OSF/1 defines struct timespec in <sys/timers.h> - Tom Marrs
#   include /**/ <sys/timers.h>
# endif /* !ACE_HAS_POSIX_TIME */

# if defined(ACE_LACKS_TIMESPEC_T)
typedef struct timespec timespec_t;
# endif /* ACE_LACKS_TIMESPEC_T */

// -------------------------------------------------------------------

/**
 * @class ACE_Time_Value
 *
 * @brief Operations on "timeval" structures, which express time in
 * seconds (secs) and microseconds (usecs).
 *
 * This class centralizes all the time related processing in
 * ACE.  These time values are typically used in conjunction with OS
 * mechanisms like <select>, <poll>, or <cond_timedwait>.
 */
class ACE_OS_Export ACE_Time_Value
{
public:

  /// Constant "0".
  static const ACE_Time_Value zero;

  /**
   * Constant for maximum time representable.  Note that this time is
   * not intended for use with <select> or other calls that may have
   * *their own* implementation-specific maximum time representations.
   * Its primary use is in time computations such as those used by the
   * dynamic subpriority strategies in the ACE_Dynamic_Message_Queue
   * class.
   */
  static const ACE_Time_Value max_time;

  // = Initialization methods.

  /// Default Constructor.
  ACE_Time_Value (void);

  /// Constructor.
  ACE_Time_Value (long sec, long usec = 0);

  // = Methods for converting to/from various time formats.

  /// Construct the ACE_Time_Value from a timeval.
  ACE_Time_Value (const struct timeval &t);

  /// Construct the ACE_Time_Value object from a timespec_t.
  ACE_Time_Value (const timespec_t &t);

# if defined (ACE_WIN32)
  /// Construct the ACE_Time_Value object from a Win32 FILETIME
  ACE_Time_Value (const FILETIME &ft);
# endif /* ACE_WIN32 */

  /// Initializes the ACE_Time_Value from two longs.
  void set (long sec, long usec);

  /// Initializes the ACE_Time_Value from a double, which is assumed to be
  /// in second format, with any remainder treated as microseconds.
  void set (double d);

  /// Initializes the ACE_Time_Value from a timeval.
  void set (const timeval &t);

  /// Initializes the ACE_Time_Value object from a timespec_t.
  void set (const timespec_t &t);

# if defined (ACE_WIN32)
  ///  Initializes the ACE_Time_Value object from a Win32 FILETIME.
  void set (const FILETIME &ft);
# endif /* ACE_WIN32 */

  /// Converts from ACE_Time_Value format into milli-seconds format.
  /**
   * @return Sum of second field (in milliseconds) and microsecond field
   *         (in milliseconds).
   *
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   */
  long msec (void) const;

  /// Converts from milli-seconds format into ACE_Time_Value format.
  /**
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   */
  void msec (long);

  /// Returns the value of the object as a timespec_t.
  operator timespec_t () const;

  /// Returns the value of the object as a timeval.
  operator timeval () const;

  /// Returns a pointer to the object as a timeval.
  operator const timeval *() const;

# if defined (ACE_WIN32)
  /// Returns the value of the object as a Win32 FILETIME.
  operator FILETIME () const;
# endif /* ACE_WIN32 */

  // = The following are accessor/mutator methods.

  /// Get seconds.
  /**
   * @return The second field/component of this ACE_Time_Value.
   *
   * @note The semantics of this method differs from the msec()
   *       method.
   */
  long sec (void) const;

  /// Set seconds.
  void sec (long sec);

  /// Get microseconds.
  /**
   * @return The microsecond field/component of this ACE_Time_Value.
   *
   * @note The semantics of this method differs from the msec()
   *       method.
   */
  long usec (void) const;

  /// Set microseconds.
  void usec (long usec);

  // = The following arithmetic methods operate on ACE_Time_Value's.

  /// Add @a tv to this.
  ACE_Time_Value &operator += (const ACE_Time_Value &tv);

  /// Subtract @a tv to this.
  ACE_Time_Value &operator -= (const ACE_Time_Value &tv);

  /// Multiply the time value by the @a d factor, which must be >= 0.
  ACE_Time_Value &operator *= (double d);

  /// Increment microseconds as postfix.
  /**
   * @note The only reason this is here is to allow the use of ACE_Atomic_Op
   * with ACE_Time_Value.
   */
  ACE_Time_Value operator++ (int);

  /// Increment microseconds as prefix.
  /**
   * @note The only reason this is here is to allow the use of ACE_Atomic_Op
   * with ACE_Time_Value.
   */
  ACE_Time_Value &operator++ (void);

  /// Decrement microseconds as postfix.
  /**
   * @note The only reason this is here is to allow the use of ACE_Atomic_Op
   * with ACE_Time_Value.
   */
  ACE_Time_Value operator-- (int);

  /// Decrement microseconds as prefix.
  /**
   * @note The only reason this is here is to allow the use of ACE_Atomic_Op
   * with ACE_Time_Value.
   */
  ACE_Time_Value &operator-- (void);

  /// Adds two ACE_Time_Value objects together, returns the sum.
  friend ACE_OS_Export ACE_Time_Value operator + (const ACE_Time_Value &tv1,
                                                  const ACE_Time_Value &tv2);

  /// Subtracts two ACE_Time_Value objects, returns the difference.
  friend ACE_OS_Export ACE_Time_Value operator - (const ACE_Time_Value &tv1,
                                                  const ACE_Time_Value &tv2);

  /// True if @a tv1 < @a tv2.
  friend ACE_OS_Export int operator < (const ACE_Time_Value &tv1,
                                       const ACE_Time_Value &tv2);

  /// True if @a tv1 > @a tv2.
  friend ACE_OS_Export int operator > (const ACE_Time_Value &tv1,
                                       const ACE_Time_Value &tv2);

  /// True if @a tv1 <= @a tv2.
  friend ACE_OS_Export int operator <= (const ACE_Time_Value &tv1,
                                        const ACE_Time_Value &tv2);

  /// True if @a tv1 >= @a tv2.
  friend ACE_OS_Export int operator >= (const ACE_Time_Value &tv1,
                                        const ACE_Time_Value &tv2);

  /// True if @a tv1 == @a tv2.
  friend ACE_OS_Export int operator == (const ACE_Time_Value &tv1,
                                        const ACE_Time_Value &tv2);

  /// True if @a tv1 != @a tv2.
  friend ACE_OS_Export int operator != (const ACE_Time_Value &tv1,
                                        const ACE_Time_Value &tv2);

  //@{
  /// Multiplies the time value by @a d
  friend ACE_OS_Export ACE_Time_Value operator * (double d,
                                                  const ACE_Time_Value &tv);

  friend ACE_OS_Export ACE_Time_Value operator * (const ACE_Time_Value &tv,
                                                  double d);
  //@}

  /// Dump is a no-op.
  /**
   * The dump() method is a no-op.  It's here for backwards compatibility
   * only, but does not dump anything. Invoking logging methods here
   * violates layering restrictions in ACE because this class is part
   * of the OS layer and @c ACE_Log_Msg is at a higher level.
   */
  void dump (void) const;

# if defined (ACE_WIN32)
  /// Const time difference between FILETIME and POSIX time.
#  if defined (ACE_LACKS_LONGLONG_T)
  static const ACE_U_LongLong FILETIME_to_timval_skew;
#  else
  static const DWORDLONG FILETIME_to_timval_skew;
#  endif // ACE_LACKS_LONGLONG_T
# endif /* ACE_WIN32 */

private:
  /// Put the timevalue into a canonical form.
  void normalize (void);

  /// Store the values as a timeval.
  timeval tv_;
};

#if defined (__ACE_INLINE__)
#include "ace/Time_Value.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* ACE_TIME_VALUE_H */
