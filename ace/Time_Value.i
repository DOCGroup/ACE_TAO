/* -*- C++ -*- */
// $Id$

// Time_Value.i

#include "ace/Trace.h"

// Don't put this in the class since it will expand the size!  Also,
// can't make this an enum due to compiler bugs on some platforms...
static const long ONE_SECOND = 1000000L;

// Initializes the ACE_Time_Value object.

// Initializes a timestruc_t.  Note that this approach loses precision
// since it converts the nano-seconds into micro-seconds.  But then
// again, do any real systems have nano-second timer precision
// anyway?!

ACE_INLINE void
ACE_Time_Value::set (const timestruc_t &tv)
{
  ACE_TRACE ("ACE_Time_Value::set");
  this->tv_sec_ = tv.tv_sec;
  this->tv_usec_ = tv.tv_nsec / 1000;

  this->normalize ();
}

// Returns the value of the object as a timestruc_t. 

ACE_INLINE 
ACE_Time_Value::operator timestruc_t () const
{
  ACE_TRACE ("ACE_Time_Value::operator timestruc_t");
  timestruc_t tv;
  tv.tv_sec = this->tv_sec_;
  tv.tv_nsec = this->tv_usec_ * 1000;
  return tv;
}

// Initializes the ACE_Time_Value object from a timestruc_t.

ACE_INLINE
ACE_Time_Value::ACE_Time_Value (const timestruc_t &tv)
{
  ACE_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (tv);
}

ACE_INLINE void
ACE_Time_Value::set (const timeval &tv)
{
  ACE_TRACE ("ACE_Time_Value::set");
  this->tv_sec_ = tv.tv_sec;
  this->tv_usec_ = tv.tv_usec;

  this->normalize ();
}

// Initializes the ACE_Time_Value object from another ACE_Time_Value

ACE_INLINE
ACE_Time_Value::ACE_Time_Value (const ACE_Time_Value &tv)
  : tv_sec_ (tv.tv_sec_), 
    tv_usec_ (tv.tv_usec_)
{
  ACE_TRACE ("ACE_Time_Value::ACE_Time_Value");
}

// Returns number of seconds.

ACE_INLINE long 
ACE_Time_Value::sec (void) const
{
  ACE_TRACE ("ACE_Time_Value::sec");
  return this->tv_sec_;
}

// Sets the number of seconds.

ACE_INLINE void 
ACE_Time_Value::sec (long sec) 
{
  ACE_TRACE ("ACE_Time_Value::sec");
  this->tv_sec_ = sec;
}

// Converts from Time_Value format into milli-seconds format.

ACE_INLINE long 
ACE_Time_Value::msec (void) const
{
  ACE_TRACE ("ACE_Time_Value::msec");
  return this->tv_sec_ * 1000 + this->tv_usec_ / 1000;
}

// Converts from milli-seconds format into Time_Value format.

ACE_INLINE void
ACE_Time_Value::msec (long milliseconds)
{
  ACE_TRACE ("ACE_Time_Value::msec");
  // Convert millisecond units to seconds;
  this->tv_sec_ = milliseconds / 1000;
  // Convert remainder to microseconds;
  this->tv_usec_ = (milliseconds - (this->tv_sec_ * 1000)) * 1000;
}

// Returns number of micro-seconds.

ACE_INLINE long 
ACE_Time_Value::usec (void) const
{
  ACE_TRACE ("ACE_Time_Value::usec");
  return this->tv_usec_;
}

// Sets the number of micro-seconds.

ACE_INLINE void 
ACE_Time_Value::usec (long usec) 
{
  ACE_TRACE ("ACE_Time_Value::usec");
  this->tv_usec_ = usec;
}

// True if tv1 < tv2.

ACE_INLINE int
operator < (const ACE_Time_Value &tv1, 
	    const ACE_Time_Value &tv2)
{
  ACE_TRACE ("operator <");
  return tv2 > tv1;
}

// True if tv1 >= tv2.

ACE_INLINE int
operator <= (const ACE_Time_Value &tv1, 
	     const ACE_Time_Value &tv2)
{
  ACE_TRACE ("operator <=");
  return tv2 >= tv1;
}

// True if tv1 == tv2.

ACE_INLINE int
operator == (const ACE_Time_Value &tv1, 
	     const ACE_Time_Value &tv2)
{
  ACE_TRACE ("operator ==");
  return tv1.tv_sec_ == tv2.tv_sec_ 
    && tv1.tv_usec_ == tv2.tv_usec_;
}

// True if tv1 != tv2.

ACE_INLINE int
operator != (const ACE_Time_Value &tv1, 
	     const ACE_Time_Value &tv2)
{
  ACE_TRACE ("operator !=");
  return !(tv1 == tv2);
}
