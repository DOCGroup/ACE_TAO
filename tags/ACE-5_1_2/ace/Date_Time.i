/* -*- C++ -*- */
// $Id$

// Date_Time.i

// constructor with init values, no check for validy
ASYS_INLINE
ACE_Date_Time::ACE_Date_Time(long day,
			     long month,
			     long year,
			     long hour,
			     long minute,
			     long second,
			     long microsec)
: day_ (day),
  month_ (month),
  year_ (year),
  hour_ (hour),
  minute_ (minute),
  second_ (second),
  microsec_ (microsec)
{
  ACE_TRACE ("ACE_Date_Time::ACE_Date_Time");
}

// set/get portions of ACE_Date_Time, no check for validy

// get day
ASYS_INLINE long 
ACE_Date_Time::day (void)
{
  ACE_TRACE ("ACE_Date_Time::day");
  return day_;
}

// set day
ASYS_INLINE void 
ACE_Date_Time::day (long day)
{
  ACE_TRACE ("ACE_Date_Time::day");
  day_ = day;
}

// get month
ASYS_INLINE long 
ACE_Date_Time::month (void)
{
  ACE_TRACE ("ACE_Date_Time::month");
  return month_;
}

// set month
ASYS_INLINE void 
ACE_Date_Time::month (long month)
{
  ACE_TRACE ("ACE_Date_Time::month");
  month_ = month;
}

// get year
ASYS_INLINE long 
ACE_Date_Time::year (void)
{
  ACE_TRACE ("ACE_Date_Time::year");
  return year_;
}

// set year
ASYS_INLINE void 
ACE_Date_Time::year (long year)
{
  ACE_TRACE ("ACE_Date_Time::year");
  year_ = year;
}

// get hour
ASYS_INLINE long 
ACE_Date_Time::hour (void)
{
  ACE_TRACE ("ACE_Date_Time::hour");
  return hour_;
}

// set hour
ASYS_INLINE void 
ACE_Date_Time::hour (long hour)
{
  ACE_TRACE ("ACE_Date_Time::hour");
  hour_ = hour;
}

// get minute
ASYS_INLINE long 
ACE_Date_Time::minute (void)
{
  ACE_TRACE ("ACE_Date_Time::minute");
  return minute_;
}

// set minute
ASYS_INLINE void 
ACE_Date_Time::minute (long minute)
{
  ACE_TRACE ("ACE_Date_Time::minute");
  minute_ = minute;
}

// get second
ASYS_INLINE long 
ACE_Date_Time::second (void)
{
  ACE_TRACE ("ACE_Date_Time::second");
  return second_;
}

// set second
ASYS_INLINE void 
ACE_Date_Time::second (long second)
{
  ACE_TRACE ("ACE_Date_Time::second");
  second_ = second;
}

// get microsec
ASYS_INLINE long 
ACE_Date_Time::microsec (void)
{
  ACE_TRACE ("ACE_Date_Time::microsec");
  return microsec_;
}

// set microsec
ASYS_INLINE void 
ACE_Date_Time::microsec (long microsec)
{
  ACE_TRACE ("ACE_Date_Time::microsec");
  microsec_ = microsec;
}


