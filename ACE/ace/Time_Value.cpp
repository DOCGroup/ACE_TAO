#include "ace/Time_Value.h"

#if defined (ACE_HAS_ALLOC_HOOKS)
# include "ace/Malloc_Base.h"
#endif /* ACE_HAS_ALLOC_HOOKS */

#if !defined (__ACE_INLINE__)
#include "ace/Time_Value.inl"
#endif /* __ACE_INLINE__ */

#include "ace/If_Then_Else.h"
#include "ace/OS_NS_math.h"
#include "ace/Time_Policy.h"
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <cmath>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/// Static constant representing `zero-time'.
/// Note: this object requires static construction.
const ACE_Time_Value ACE_Time_Value::zero;

/// Constant for maximum time representable.  Note that this time
/// is not intended for use with select () or other calls that may
/// have *their own* implementation-specific maximum time representations.
/// Its primary use is in time computations such as those used by the
/// dynamic subpriority strategies in the ACE_Dynamic_Message_Queue class.
/// Note: this object requires static construction.
const ACE_Time_Value ACE_Time_Value::max_time (
  std::numeric_limits<time_t>::max (),
  ACE_ONE_SECOND_IN_USECS - 1);

ACE_ALLOC_HOOK_DEFINE (ACE_Time_Value)

/// Increment microseconds (the only reason this is here is to allow
/// the use of ACE_Atomic_Op with ACE_Time_Value).
ACE_Time_Value
ACE_Time_Value::operator ++ (int)
{
  // ACE_OS_TRACE ("ACE_Time_Value::operator ++ (int)");
  ACE_Time_Value tv (*this);
  ++*this;
  return tv;
}

ACE_Time_Value &
ACE_Time_Value::operator ++ ()
{
  // ACE_OS_TRACE ("ACE_Time_Value::operator ++ ()");
  return *this += ACE_Time_Value (0, 1);
}

/// Decrement microseconds (the only reason this is here is / to allow
/// the use of ACE_Atomic_Op with ACE_Time_Value).
ACE_Time_Value
ACE_Time_Value::operator -- (int)
{
  // ACE_OS_TRACE ("ACE_Time_Value::operator -- (int)");
  ACE_Time_Value tv (*this);
  --*this;
  return tv;
}

ACE_Time_Value &
ACE_Time_Value::operator -- ()
{
  // ACE_OS_TRACE ("ACE_Time_Value::operator -- ()");
  return *this -= ACE_Time_Value (0, 1);
}

#if defined (ACE_WIN32)
/// Static constant to remove time skew between FILETIME and POSIX
/// time.  POSIX and Win32 use different epochs (Jan. 1, 1970 v.s.
/// Jan. 1, 1601).  The following constant defines the difference
/// in 100ns ticks.
///
/// In the beginning (Jan. 1, 1601), there was no time and no computer.
/// And Bill said: "Let there be time," and there was time....
const DWORDLONG ACE_Time_Value::FILETIME_to_timval_skew =
ACE_INT64_LITERAL (0x19db1ded53e8000);

///  Initializes the ACE_Time_Value object from a Win32 FILETIME
ACE_Time_Value::ACE_Time_Value (const FILETIME &file_time)
{
  // // ACE_OS_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (file_time);
}

void ACE_Time_Value::set (const FILETIME &file_time)
{
  // Initializes the ACE_Time_Value object from a Win32 FILETIME
  // Don't use a struct initializer, gcc don't like it.
  ULARGE_INTEGER _100ns;
  _100ns.LowPart = file_time.dwLowDateTime;
  _100ns.HighPart = file_time.dwHighDateTime;

  _100ns.QuadPart -= ACE_Time_Value::FILETIME_to_timval_skew;

  // Convert 100ns units to seconds;
  this->tv_.tv_sec = (time_t) (_100ns.QuadPart / (10000 * 1000));
  // Convert remainder to microseconds;
  this->tv_.tv_usec = (suseconds_t) ((_100ns.QuadPart % (10000 * 1000)) / 10);

  this->normalize ();
}

/// Returns the value of the object as a Win32 FILETIME.
ACE_Time_Value::operator FILETIME () const
{
  FILETIME file_time;
  // ACE_OS_TRACE ("ACE_Time_Value::operator FILETIME");

  ULARGE_INTEGER _100ns;
  _100ns.QuadPart = (((DWORDLONG) this->tv_.tv_sec * (10000 * 1000) +
                     this->tv_.tv_usec * 10) +
                     ACE_Time_Value::FILETIME_to_timval_skew);

  file_time.dwLowDateTime = _100ns.LowPart;
  file_time.dwHighDateTime = _100ns.HighPart;

  return file_time;
}
#endif /* ACE_WIN32 */

ACE_Time_Value
ACE_Time_Value::now () const
{
  ACE_System_Time_Policy systp;
  return systp ();
}

ACE_Time_Value
ACE_Time_Value::to_relative_time () const
{
  ACE_System_Time_Policy systp;
  return (*this) - systp ();
}

ACE_Time_Value
ACE_Time_Value::to_absolute_time () const
{
  ACE_System_Time_Policy systp;
  return (*this) + systp ();
}

ACE_Time_Value *
ACE_Time_Value::duplicate () const
{
  ACE_Time_Value * tmp = nullptr;
  ACE_NEW_RETURN (tmp, ACE_Time_Value (*this), nullptr);
  return tmp;
}

void
ACE_Time_Value::dump () const
{
}

void
ACE_Time_Value::normalize (bool saturate)
{
  // ACE_OS_TRACE ("ACE_Time_Value::normalize");
  if (this->tv_.tv_usec >= ACE_ONE_SECOND_IN_USECS ||
      this->tv_.tv_usec <= -ACE_ONE_SECOND_IN_USECS)
    {
      time_t const sec = this->tv_.tv_usec / ACE_ONE_SECOND_IN_USECS;
      suseconds_t const usec = static_cast<suseconds_t> (this->tv_.tv_usec - sec * ACE_ONE_SECOND_IN_USECS);

      if (saturate && this->tv_.tv_sec > 0 && sec > 0 &&
          std::numeric_limits<time_t>::max () - this->tv_.tv_sec < sec)
        {
          this->tv_.tv_sec = std::numeric_limits<time_t>::max ();
          this->tv_.tv_usec = ACE_ONE_SECOND_IN_USECS - 1;
        }
      else if (saturate && this->tv_.tv_sec < 0 && sec < 0 &&
               std::numeric_limits<time_t>::min () - this->tv_.tv_sec > sec)
        {
          this->tv_.tv_sec = std::numeric_limits<time_t>::min ();
          this->tv_.tv_usec = -ACE_ONE_SECOND_IN_USECS + 1;
        }
      else
        {
          this->tv_.tv_sec += sec;
          this->tv_.tv_usec = usec;
        }
    }

  if (this->tv_.tv_sec >= 1 && this->tv_.tv_usec < 0)
    {
      --this->tv_.tv_sec;
      this->tv_.tv_usec += ACE_ONE_SECOND_IN_USECS;
    }
  // tv_sec in qnxnto is unsigned
#if !defined ( __QNX__)
  else if (this->tv_.tv_sec < 0 && this->tv_.tv_usec > 0)
    {
      ++this->tv_.tv_sec;
      this->tv_.tv_usec -= ACE_ONE_SECOND_IN_USECS;
    }
#endif /* __QNX__  */
}

ACE_Time_Value &
ACE_Time_Value::operator+= (const ACE_Time_Value &tv)
{
  // Normalize copies first so the seconds and microseconds can be checked
  // independently without overflowing either field.
  ACE_Time_Value lhs (*this);
  ACE_Time_Value rhs (tv);
  lhs.normalize (true);
  rhs.normalize (true);

  time_t const lhs_sec = lhs.sec ();
  time_t const rhs_sec = rhs.sec ();
  bool overflow =
    rhs_sec > 0 && lhs_sec > std::numeric_limits<time_t>::max () - rhs_sec;
#if !defined (__QNX__)
  overflow = overflow ||
    (rhs_sec < 0 && lhs_sec < std::numeric_limits<time_t>::min () - rhs_sec);
#endif
  if (overflow)
    {
      this->set (rhs_sec > 0 ? std::numeric_limits<time_t>::max ()
                 : std::numeric_limits<time_t>::min (),
                 rhs_sec > 0 ? ACE_ONE_SECOND_IN_USECS - 1
                 : -ACE_ONE_SECOND_IN_USECS + 1);
      return *this;
    }

  this->sec (lhs_sec + rhs_sec);
  this->usec (lhs.usec () + rhs.usec ());
  this->normalize (true);
  return *this;
}

ACE_Time_Value &
ACE_Time_Value::operator+= (time_t tv)
{
  return *this += ACE_Time_Value (tv);
}

ACE_Time_Value &
ACE_Time_Value::operator-= (const ACE_Time_Value &tv)
{
  ACE_Time_Value lhs (*this);
  ACE_Time_Value rhs (tv);
  lhs.normalize (true);
  rhs.normalize (true);

  time_t const lhs_sec = lhs.sec ();
  time_t const rhs_sec = rhs.sec ();
  bool overflow =
    rhs_sec > 0 && lhs_sec < std::numeric_limits<time_t>::min () + rhs_sec;
#if !defined (__QNX__)
  overflow = overflow ||
    (rhs_sec < 0 && lhs_sec > std::numeric_limits<time_t>::max () + rhs_sec);
#endif
  if (overflow)
    {
      this->set (rhs_sec > 0 ? std::numeric_limits<time_t>::min ()
                 : std::numeric_limits<time_t>::max (),
                 rhs_sec > 0 ? -ACE_ONE_SECOND_IN_USECS + 1
                 : ACE_ONE_SECOND_IN_USECS - 1);
      return *this;
    }

  this->sec (lhs_sec - rhs_sec);
  this->usec (lhs.usec () - rhs.usec ());
  this->normalize (true);
  return *this;
}

ACE_Time_Value &
ACE_Time_Value::operator-= (time_t tv)
{
  return *this -= ACE_Time_Value (tv);
}


ACE_Time_Value &
ACE_Time_Value::operator *= (double d)
{
  if (d == 1.0)
    return *this;

  if (d == 0.0)
    {
      this->sec (0);
      this->usec (0);
      return *this;
    }

  // To work around the lack of precision of a long double to contain
  // a 64-bits time_t + 6 digits after the decimal point for the usec part,
  // we perform the multiplication of the 2 timeval parts separately.
  //
  // This extra precision step is adding a cost when transfering the
  // seconds resulting from the usec multiplication.  This operation
  // correspond to the normalization process performed in normalize()
  // but we must absolutly do it here because the usec multiplication
  // result value could exceed what can be stored in a suseconds_t
  // type variable.
  //
  // Since this is a costly operation, we try to detect as soon as
  // possible if we are having a saturation in order to abort the rest
  // of the computation.
  using float_type = ACE::If_Then_Else<(sizeof(double) > sizeof(time_t)), double, long double>::result_type;

  float_type sec_total = static_cast<float_type> (this->sec());
  sec_total *= d;

  // shall we saturate the result?
  // Use exclusive limits.  Adding 0.999999 to time_t::max() can round back
  // to max() when float_type has only double precision, which allowed an
  // out-of-range floating-to-integer conversion below.
  static const float_type max_exclusive =
    static_cast<float_type> (std::numeric_limits<time_t>::max ()) + 1;
  static const float_type min_exclusive =
    static_cast<float_type> (std::numeric_limits<time_t>::min ()) - 1;

  if (sec_total >= max_exclusive)
    {
      this->set(std::numeric_limits<time_t>::max (), ACE_ONE_SECOND_IN_USECS-1);
    }
  else if (sec_total <= min_exclusive)
    {
      this->set(std::numeric_limits<time_t>::min (), -ACE_ONE_SECOND_IN_USECS+1);
    }
  else
    {
      time_t time_sec = static_cast<time_t> (sec_total);

      float_type usec_total = this->usec();
      usec_total *= d;

      // adding usec resulting from tv_sec mult
      usec_total += (sec_total-time_sec) * ACE_ONE_SECOND_IN_USECS;

      // extract seconds component of the usec mult
      sec_total = usec_total / ACE_ONE_SECOND_IN_USECS;
      // keep remaining usec
      if (sec_total > 0)
        {
          usec_total = (sec_total - ACE_OS::floor(sec_total));
        }
      else
        {
          usec_total = (sec_total - ACE_OS::ceil(sec_total));
        }

      sec_total -= usec_total;
      usec_total *= ACE_ONE_SECOND_IN_USECS;

      // add the seconds component of the usec mult with the tv_sec mult prod.
      sec_total += time_sec;

      // recheck for saturation
      if (sec_total >= max_exclusive)
        {
          this->set (std::numeric_limits<time_t>::max (), ACE_ONE_SECOND_IN_USECS - 1);
        }
      else if (sec_total <= min_exclusive)
        {
          this->set (std::numeric_limits<time_t>::min (), -ACE_ONE_SECOND_IN_USECS + 1);
        }
      else
        {
          time_sec = static_cast<time_t> (sec_total);
          // Round to the nearest microsecond, with ties away from zero.
          // Applying the rounding before conversion also handles values in
          // (-1, 0), where conversion to suseconds_t would otherwise lose
          // the sign before the tie check.
          suseconds_t time_usec = static_cast<suseconds_t> (
            usec_total >= 0
            ? ACE_OS::floor (usec_total + 0.5)
            : ACE_OS::ceil (usec_total - 0.5));

          this->set (time_sec, time_usec);
        }
    }
  return *this;
}

std::ostream &operator<<(std::ostream &o, const ACE_Time_Value &v)
{
  char const oldFiller = o.fill ();
  o.fill ('0');
  const timeval *tv = v;
  if (tv->tv_sec)
    {
      o << tv->tv_sec;
      if (tv->tv_usec)
        o << '.' << std::setw (6) << std::labs (tv->tv_usec);
    }
  else if (tv->tv_usec < 0)
    o << "-0." << std::setw (6) << - tv->tv_usec;
  else
    {
      o << '0';
      if (tv->tv_usec > 0)
        o << '.'<< std::setw (6) << tv->tv_usec;
    }

  o.fill (oldFiller);
  return o;
}

ACE_END_VERSIONED_NAMESPACE_DECL
