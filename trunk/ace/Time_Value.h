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

#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


# if !defined (ACE_HAS_WINCE) && !defined (ACE_PSOS_DIAB_MIPS)
#   include "ace/os_include/sys/os_time.h"
#   if defined (__Lynx__)
#     include /**/ <st.h>
#     include /**/ <sem.h>
#   endif /* __Lynx__ */
#   if defined (VXWORKS)
#     include /**/ <sys/times.h>
#   endif /* VXWORKS */
# endif /* ACE_HAS_WINCE ACE_PSOS_DIAB_MIPS */

// HP-UX 10.20 doesn't define timespec_t - it defined struct timespec.
#if defined (HPUX_10)
typedef struct timespec timespec_t;
#endif /* HPUX_10 */

// Define some helpful constants.
// Not type-safe, and signed.  For backward compatibility.
#define ACE_ONE_SECOND_IN_MSECS 1000L
#define ACE_ONE_SECOND_IN_USECS 1000000L
#define ACE_ONE_SECOND_IN_NSECS 1000000000L

// This forward declaration is needed by the set() and FILETIME() functions
#if defined (ACE_LACKS_LONGLONG_T)
class ACE_Export ACE_U_LongLong;
#endif  /* ACE_LACKS_LONGLONG_T */

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
#if defined (ACE_WIN32) && defined (_WIN32_WCE)
// Something is a bit brain-damaged here and I'm not sure what... this code
// compiled before the OS reorg for ACE 5.4. Since then it hasn't - eVC
// complains that the operators that return ACE_Time_Value are C-linkage
// functions that can't return a C++ class. The only way I've found to
// defeat this is to wrap the whole class in extern "C++".
//    - Steve Huston, 23-Aug-2004
extern "C++" {
#endif
class ACE_Export ACE_Time_Value
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
   *         (in milliseconds).  Note that this method can overflow if
   *         the second and microsecond field values are large, so use
   *         the msec (ACE_UINT64 &ms) method instead.
   *
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   */
  unsigned long msec (void) const;

#if !defined (ACE_LACKS_LONGLONG_T)
  /**
   * @return Sum of second field (in milliseconds) and microsecond field
   *         (in milliseconds) and return them via the @param ms parameter.
   *
   * @note The semantics of this method differs from the sec() and
   *       usec() methods.  There is no analogous "millisecond"
   *       component in an ACE_Time_Value.
   */
  void msec (ACE_UINT64 &ms) const;

#endif /*ACE_LACKS_LONGLONG_T */

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

  /// Assign @ tv to this
  ACE_Time_Value &operator = (const ACE_Time_Value &tv);

  /// Subtract @a tv to this.
  ACE_Time_Value &operator -= (const ACE_Time_Value &tv);

    /** \brief Multiply the time value by the @a d factor.

    \note The result of the operator is valid for results from range
    < (ACE_INT32_MIN, -999999), (ACE_INT32_MAX, 999999) >. Result
    outside this range are saturated to a limit.
     */
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
  friend ACE_Export ACE_Time_Value operator + (const ACE_Time_Value &tv1,
                                                  const ACE_Time_Value &tv2);

  /// Subtracts two ACE_Time_Value objects, returns the difference.
  friend ACE_Export ACE_Time_Value operator - (const ACE_Time_Value &tv1,
                                                  const ACE_Time_Value &tv2);

  /// True if @a tv1 < @a tv2.
  friend ACE_Export bool operator < (const ACE_Time_Value &tv1,
                                     const ACE_Time_Value &tv2);

  /// True if @a tv1 > @a tv2.
  friend ACE_Export bool operator > (const ACE_Time_Value &tv1,
                                     const ACE_Time_Value &tv2);

  /// True if @a tv1 <= @a tv2.
  friend ACE_Export bool operator <= (const ACE_Time_Value &tv1,
                                      const ACE_Time_Value &tv2);

  /// True if @a tv1 >= @a tv2.
  friend ACE_Export bool operator >= (const ACE_Time_Value &tv1,
                                      const ACE_Time_Value &tv2);

  /// True if @a tv1 == @a tv2.
  friend ACE_Export bool operator == (const ACE_Time_Value &tv1,
                                      const ACE_Time_Value &tv2);

  /// True if @a tv1 != @a tv2.
  friend ACE_Export bool operator != (const ACE_Time_Value &tv1,
                                      const ACE_Time_Value &tv2);

  //@{
  /// Multiplies the time value by @a d
  friend ACE_Export ACE_Time_Value operator * (double d,
                                                  const ACE_Time_Value &tv);

  friend ACE_Export ACE_Time_Value operator * (const ACE_Time_Value &tv,
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
#if defined (ACE_WIN32) && defined (_WIN32_WCE)
}
#endif

/**
 * @class ACE_Countdown_Time
 *
 * @brief Keeps track of the amount of elapsed time.
 *
 * This class has a side-effect on the <max_wait_time> -- every
 * time the <stop> method is called the <max_wait_time> is
 * updated.
 */
class ACE_Export ACE_Countdown_Time
{
public:
  // = Initialization and termination methods.
  /// Cache the <max_wait_time> and call <start>.
  ACE_Countdown_Time (ACE_Time_Value *max_wait_time);

  /// Call <stop>.
  ~ACE_Countdown_Time (void);

  /// Cache the current time and enter a start state.
  int start (void);

  /// Subtract the elapsed time from max_wait_time_ and enter a stopped
  /// state.
  int stop (void);

  /// Calls stop and then start.  max_wait_time_ is modified by the
  /// call to stop.
  int update (void);

  /// Returns 1 if we've already been stopped, else 0.
  int stopped (void) const;

private:
  /// Maximum time we were willing to wait.
  ACE_Time_Value *max_wait_time_;

  /// Beginning of the start time.
  ACE_Time_Value start_time_;

  /// Keeps track of whether we've already been stopped.
  int stopped_;
};

#if defined (__ACE_INLINE__)
#include "ace/Time_Value.inl"
#endif /* __ACE_INLINE__ */

#if defined (__MINGW32__)
// The MingW linker has problems with the exported statics
// zero and max_time with these two statics the linker will be able to
// resolve the static exported symbols.
static const ACE_Time_Value& __zero_time = ACE_Time_Value::zero;
static const ACE_Time_Value& __max_time = ACE_Time_Value::max_time;
#endif /* __MINGW32__ */

#include /**/ "ace/post.h"

#endif /* ACE_TIME_VALUE_H */
