// -*- C++ -*-
//
// $Id$


// Returns the value of the object as a timeval.

ACE_INLINE
ACE_Time_Value::operator timeval () const
{
  // ACE_OS_TRACE ("ACE_Time_Value::operator timeval");
  return this->tv_;
}

// Returns a pointer to the object as a timeval.

ACE_INLINE
ACE_Time_Value::operator const timeval * () const
{
  // ACE_OS_TRACE ("ACE_Time_Value::operator const timeval *");
  return (const timeval *) &this->tv_;
}

ACE_INLINE void
ACE_Time_Value::set (long sec, long usec)
{
  // ACE_OS_TRACE ("ACE_Time_Value::set");
  this->tv_.tv_sec = sec;
  this->tv_.tv_usec = usec;
  this->normalize ();
}

ACE_INLINE void
ACE_Time_Value::set (double d)
{
  // ACE_OS_TRACE ("ACE_Time_Value::set");
  long l = (long) d;
  this->tv_.tv_sec = l;
  this->tv_.tv_usec = (long) ((d - (double) l) * ACE_ONE_SECOND_IN_USECS);
  this->normalize ();
}

// Initializes a timespec_t.  Note that this approach loses precision
// since it converts the nano-seconds into micro-seconds.  But then
// again, do any real systems have nano-second timer precision?!

ACE_INLINE void
ACE_Time_Value::set (const timespec_t &tv)
{
  // ACE_OS_TRACE ("ACE_Time_Value::set");
#if ! defined(ACE_HAS_BROKEN_TIMESPEC_MEMBERS)
  this->tv_.tv_sec = tv.tv_sec;
  // Convert nanoseconds into microseconds.
  this->tv_.tv_usec = tv.tv_nsec / 1000;
#else
  this->tv_.tv_sec = tv.ts_sec;
  // Convert nanoseconds into microseconds.
  this->tv_.tv_usec = tv.ts_nsec / 1000;
#endif /* ACE_HAS_BROKEN_TIMESPEC_MEMBERS */

  this->normalize ();
}

ACE_INLINE void
ACE_Time_Value::set (const timeval &tv)
{
  // ACE_OS_TRACE ("ACE_Time_Value::set");
  this->tv_.tv_sec = tv.tv_sec;
  this->tv_.tv_usec = tv.tv_usec;

  this->normalize ();
}

ACE_INLINE
ACE_Time_Value::ACE_Time_Value (const struct timeval &tv)
  // : tv_ ()
{
  // ACE_OS_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (tv);
}

ACE_INLINE
ACE_Time_Value::ACE_Time_Value (void)
  // : tv_ ()
{
  // ACE_OS_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (0, 0);
}

ACE_INLINE
ACE_Time_Value::ACE_Time_Value (long sec, long usec)
{
  // ACE_OS_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (sec, usec);
}

// Returns number of seconds.

ACE_INLINE long
ACE_Time_Value::sec (void) const
{
  // ACE_OS_TRACE ("ACE_Time_Value::sec");
  return this->tv_.tv_sec;
}

// Sets the number of seconds.

ACE_INLINE void
ACE_Time_Value::sec (long sec)
{
  // ACE_OS_TRACE ("ACE_Time_Value::sec");
  this->tv_.tv_sec = sec;
}

// Converts from Time_Value format into milli-seconds format.

ACE_INLINE long
ACE_Time_Value::msec (void) const
{
  // ACE_OS_TRACE ("ACE_Time_Value::msec");
  return this->tv_.tv_sec * 1000 + this->tv_.tv_usec / 1000;
}

// Converts from milli-seconds format into Time_Value format.

ACE_INLINE void
ACE_Time_Value::msec (long milliseconds)
{
  // ACE_OS_TRACE ("ACE_Time_Value::msec");
  // Convert millisecond units to seconds;
  this->tv_.tv_sec = milliseconds / 1000;
  // Convert remainder to microseconds;
  this->tv_.tv_usec = (milliseconds - (this->tv_.tv_sec * 1000)) * 1000;
}

// Returns number of micro-seconds.

ACE_INLINE long
ACE_Time_Value::usec (void) const
{
  // ACE_OS_TRACE ("ACE_Time_Value::usec");
  return this->tv_.tv_usec;
}

// Sets the number of micro-seconds.

ACE_INLINE void
ACE_Time_Value::usec (long usec)
{
  // ACE_OS_TRACE ("ACE_Time_Value::usec");
  this->tv_.tv_usec = usec;
}

ACE_INLINE ACE_Time_Value &
ACE_Time_Value::operator *= (double d)
{
  double time =
    ((double) this->sec ()) * ACE_ONE_SECOND_IN_USECS + this->usec ();
  time *= d;
  this->sec ((long)(time / ACE_ONE_SECOND_IN_USECS));
  this->usec (((long)time) % ACE_ONE_SECOND_IN_USECS);
  this->normalize ();
  return *this;
}

ACE_INLINE ACE_Time_Value
operator * (double d, const ACE_Time_Value &tv)
{
  return ACE_Time_Value (tv) *= d;
}

ACE_INLINE ACE_Time_Value
operator * (const ACE_Time_Value &tv, double d)
{
  return ACE_Time_Value (tv) *= d;
}

// True if tv1 > tv2.

ACE_INLINE int
operator > (const ACE_Time_Value &tv1,
            const ACE_Time_Value &tv2)
{
  // ACE_OS_TRACE ("operator >");
  if (tv1.sec () > tv2.sec ())
    return 1;
  else if (tv1.sec () == tv2.sec ()
           && tv1.usec () > tv2.usec ())
    return 1;
  else
    return 0;
}

// True if tv1 >= tv2.

ACE_INLINE int
operator >= (const ACE_Time_Value &tv1,
             const ACE_Time_Value &tv2)
{
  // ACE_OS_TRACE ("operator >=");
  if (tv1.sec () > tv2.sec ())
    return 1;
  else if (tv1.sec () == tv2.sec ()
           && tv1.usec () >= tv2.usec ())
    return 1;
  else
    return 0;
}

// Returns the value of the object as a timespec_t.

ACE_INLINE
ACE_Time_Value::operator timespec_t () const
{
  // ACE_OS_TRACE ("ACE_Time_Value::operator timespec_t");
  timespec_t tv;
#if ! defined(ACE_HAS_BROKEN_TIMESPEC_MEMBERS)
  tv.tv_sec = this->sec ();
  // Convert microseconds into nanoseconds.
  tv.tv_nsec = this->tv_.tv_usec * 1000;
#else
  tv.ts_sec = this->sec ();
  // Convert microseconds into nanoseconds.
  tv.ts_nsec = this->tv_.tv_usec * 1000;
#endif /* ACE_HAS_BROKEN_TIMESPEC_MEMBERS */
  return tv;
}

// Initializes the ACE_Time_Value object from a timespec_t.

ACE_INLINE
ACE_Time_Value::ACE_Time_Value (const timespec_t &tv)
  // : tv_ ()
{
  // ACE_OS_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (tv);
}

// True if tv1 < tv2.

ACE_INLINE int
operator < (const ACE_Time_Value &tv1,
            const ACE_Time_Value &tv2)
{
  // ACE_OS_TRACE ("operator <");
  return tv2 > tv1;
}

// True if tv1 >= tv2.

ACE_INLINE int
operator <= (const ACE_Time_Value &tv1,
             const ACE_Time_Value &tv2)
{
  // ACE_OS_TRACE ("operator <=");
  return tv2 >= tv1;
}

// True if tv1 == tv2.

ACE_INLINE int
operator == (const ACE_Time_Value &tv1,
             const ACE_Time_Value &tv2)
{
  // ACE_OS_TRACE ("operator ==");
  return tv1.sec () == tv2.sec ()
    && tv1.usec () == tv2.usec ();
}

// True if tv1 != tv2.

ACE_INLINE int
operator != (const ACE_Time_Value &tv1,
             const ACE_Time_Value &tv2)
{
  // ACE_OS_TRACE ("operator !=");
  return !(tv1 == tv2);
}

// Add TV to this.

ACE_INLINE ACE_Time_Value &
ACE_Time_Value::operator+= (const ACE_Time_Value &tv)
{
  // ACE_OS_TRACE ("ACE_Time_Value::operator+=");
  this->sec (this->sec () + tv.sec ());
  this->usec (this->usec () + tv.usec ());
  this->normalize ();
  return *this;
}

// Subtract TV to this.

ACE_INLINE ACE_Time_Value &
ACE_Time_Value::operator-= (const ACE_Time_Value &tv)
{
  // ACE_OS_TRACE ("ACE_Time_Value::operator-=");
  this->sec (this->sec () - tv.sec ());
  this->usec (this->usec () - tv.usec ());
  this->normalize ();
  return *this;
}

// Adds two ACE_Time_Value objects together, returns the sum.

ACE_INLINE ACE_Time_Value
operator + (const ACE_Time_Value &tv1,
            const ACE_Time_Value &tv2)
{
  // ACE_OS_TRACE ("operator +");
  ACE_Time_Value sum (tv1.sec () + tv2.sec (),
                      tv1.usec () + tv2.usec ());

  sum.normalize ();
  return sum;
}

// Subtracts two ACE_Time_Value objects, returns the difference.

ACE_INLINE ACE_Time_Value
operator - (const ACE_Time_Value &tv1,
            const ACE_Time_Value &tv2)
{
  // ACE_OS_TRACE ("operator -");
  ACE_Time_Value delta (tv1.sec () - tv2.sec (),
                        tv1.usec () - tv2.usec ());
  delta.normalize ();
  return delta;
}
