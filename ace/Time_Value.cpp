// Time_Value.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/ACE.h"
#include "ace/Time_Value.h"

#if !defined (__ACE_INLINE__)
#include "ace/Time_Value.i"
#endif /* __ACE_INLINE__ */

// Static constant representing `zero-time'.
const ACE_Time_Value ACE_Time_Value::zero;

ACE_ALLOC_HOOK_DEFINE(ACE_Time_Value)

// Initializes the ACE_Time_Value object from a timeval.

ACE_Time_Value::ACE_Time_Value (const timeval &tv)
{
  // ACE_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (tv);
}

#if defined(ACE_WIN32)
//  Initializes the ACE_Time_Value object from a Win32 FILETIME

ACE_Time_Value::ACE_Time_Value (const FILETIME &file_time)
{
  // ACE_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (file_time);
}

void ACE_Time_Value::set (const FILETIME &file_time)
{
  //  Initializes the ACE_Time_Value object from a Win32 FILETIME 
  ACE_QWORD _100ns = ACE_MAKE_QWORD (file_time.dwLowDateTime, 
				     file_time.dwHighDateTime);
  // Convert 100ns units to seconds;
  this->tv_sec_ = long (_100ns / (10000 * 1000));
  // Convert remainder to microseconds;
  this->tv_usec_ = long ((_100ns - (this->tv_sec_ * (10000 * 1000))) / 10);
}

// Returns the value of the object as a Win32 FILETIME.

ACE_Time_Value::operator FILETIME () const
{
  // ACE_TRACE ("ACE_Time_Value::operator FILETIME");
  ACE_QWORD _100ns = ((ACE_QWORD) this->tv_sec_ * (1000 * 1000) + this->tv_usec_) * 10;
  FILETIME file_time;
  file_time.dwLowDateTime = ACE_LOW_DWORD (_100ns);
  file_time.dwHighDateTime = ACE_HIGH_DWORD (_100ns);
  return file_time;
}

#endif

void
ACE_Time_Value::dump (void) const
{
  // ACE_TRACE ("ACE_Time_Value::dump");
#if 0
  if (tv.usec () < 0 || tv.sec () < 0)
    stream << "-";

  stream << dec << abs (int (tv.sec ())) << "."
//	 << setw (6) << setfill ('0') 
	 << dec << abs (int (tv.usec ()));
// I assume
   inline int abs(int d) { return (d>0)?d:-d; }
   is defined somewhere */
#endif /* 0 */
}

void
ACE_Time_Value::set (long sec, long usec)
{
  // ACE_TRACE ("ACE_Time_Value::set");
  this->tv_sec_ = sec;
  this->tv_usec_ = usec;
}

ACE_Time_Value::ACE_Time_Value (long sec, long usec)
{
  // ACE_TRACE ("ACE_Time_Value::ACE_Time_Value");
  this->set (sec, usec);
  this->normalize ();
}

// Returns the value of the object as a timeval.

ACE_Time_Value::operator timeval () const
{
  // ACE_TRACE ("ACE_Time_Value::operator timeval");
  timeval tv;
  tv.tv_sec = this->tv_sec_;
  tv.tv_usec = this->tv_usec_;
  return tv;
}

// Add TV to this.

void
ACE_Time_Value::operator+= (const ACE_Time_Value &tv)
{
  // ACE_TRACE ("ACE_Time_Value::operator+=");
  this->tv_sec_ += tv.tv_sec_;
  this->tv_usec_ += tv.tv_usec_;
  this->normalize ();
}

// Subtract TV to this.

void
ACE_Time_Value::operator-= (const ACE_Time_Value &tv)
{
  // ACE_TRACE ("ACE_Time_Value::operator-=");
  this->tv_sec_ -= tv.tv_sec_;
  this->tv_usec_ -= tv.tv_usec_;
  this->normalize ();
}

// Adds two ACE_Time_Value objects together, returns the sum.

ACE_Time_Value 
operator + (const ACE_Time_Value &tv1, const ACE_Time_Value &tv2)
{
  // ACE_TRACE ("operator +");
  ACE_Time_Value sum (tv1.tv_sec_ + tv2.tv_sec_, 
		      tv1.tv_usec_ + tv2.tv_usec_); 

  sum.normalize ();
  return sum;
}

// Subtracts two ACE_Time_Value objects, returns the difference.

ACE_Time_Value 
operator - (const ACE_Time_Value &tv1, const ACE_Time_Value &tv2)
{
  // ACE_TRACE ("operator -");
  ACE_Time_Value delta (tv1.tv_sec_ - tv2.tv_sec_, 
			tv1.tv_usec_ - tv2.tv_usec_); 
  delta.normalize ();
  return delta;
}

// True if tv1 > tv2.

int
operator > (const ACE_Time_Value &tv1, const ACE_Time_Value &tv2)
{
  // ACE_TRACE ("operator >");
  if (tv1.tv_sec_ > tv2.tv_sec_)
    return 1;
  else if (tv1.tv_sec_ == tv2.tv_sec_ 
	   && tv1.tv_usec_ > tv2.tv_usec_) 
    return 1;
  else
    return 0;
}

// True if tv1 >= tv2.

int
operator >= (const ACE_Time_Value &tv1, const ACE_Time_Value &tv2)
{
  // ACE_TRACE ("operator >=");
  if (tv1.tv_sec_ > tv2.tv_sec_)
    return 1;
  else if (tv1.tv_sec_ == tv2.tv_sec_ 
	   && tv1.tv_usec_ >= tv2.tv_usec_)
    return 1;
  else
    return 0;
}

void
ACE_Time_Value::normalize (void)
{
  // ACE_TRACE ("ACE_Time_Value::normalize");
  // New code from Hans Rohnert...

  if (this->tv_usec_ >= ONE_SECOND)
    {
      do
	{ 
	  this->tv_sec_++;
	  this->tv_usec_ -= ONE_SECOND;
	}
      while (this->tv_usec_ >= ONE_SECOND);
    }
  else if (this->tv_usec_ <= -ONE_SECOND)
    {
      do
	{ 
	  this->tv_sec_--;
	  this->tv_usec_ += ONE_SECOND;
	}
      while (this->tv_usec_ <= -ONE_SECOND);
    }
 
  if (this->tv_sec_ >= 1 && this->tv_usec_ < 0)
    {
      this->tv_sec_--;
      this->tv_usec_ += ONE_SECOND;
    }
  else if (this->tv_sec_ < 0 && this->tv_usec_ > 0)
    {
      this->tv_sec_++;
      this->tv_usec_ -= ONE_SECOND;
    }

#if 0
  // Old code...
  while ((this->tv_usec_ >= ONE_SECOND) 
	 || (this->tv_sec_ < 0 && this->tv_usec_ > 0 ))
    {
      this->tv_usec_ -= ONE_SECOND;
      this->tv_sec_++;
    }

  while ((this->tv_usec_ <= -ONE_SECOND) 
	 || (this->tv_sec_ > 0 && this->tv_usec_ < 0))
    {
      this->tv_usec_ += ONE_SECOND;
      this->tv_sec_--;
    }
#endif 
}

int
ACE_Countdown_Time::start (void)
{
  this->start_time_ = ACE_OS::gettimeofday ();
  this->stopped_ = 0;
  return 0;
}

int
ACE_Countdown_Time::stop (void)
{
  if (this->max_wait_time_ != 0 && this->stopped_ == 0)
    {
      ACE_Time_Value elapsed_time = 
	ACE_OS::gettimeofday () - this->start_time_;

      if (*this->max_wait_time_ > elapsed_time)
	*this->max_wait_time_ -= elapsed_time;
      else
	{
	  // Used all of timeout.
	  *this->max_wait_time_ = ACE_Time_Value::zero; 
	  errno = ETIME;
	}
      this->stopped_ = 1;
    }
  return 0;
}

ACE_Countdown_Time::ACE_Countdown_Time (ACE_Time_Value *max_wait_time)
  : max_wait_time_ (max_wait_time),
    stopped_ (0)
{
  if (max_wait_time != 0)
    this->start ();
}

ACE_Countdown_Time::~ACE_Countdown_Time (void)
{
  this->stop ();
}

