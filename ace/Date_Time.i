/* -*- C++ -*- */
// $Id$

// Date_Time.i

ASYS_INLINE void
ACE_Date_Time::update (void)
{
  ACE_TRACE ("ACE_Date_Time::update");

  update(ACE_OS::gettimeofday ());
}

ASYS_INLINE void
ACE_Date_Time::update (const ACE_Time_Value& timevalue)
{
  time_t time = timevalue.sec ();
  struct tm tm_time;
  ACE_OS::localtime_r (&time, &tm_time);
  this->day_ = tm_time.tm_mday;
  this->month_ = tm_time.tm_mon + 1;    // localtime's months are 0-11
  this->year_ = tm_time.tm_year + 1900; // localtime reports years since 1900
  this->hour_ = tm_time.tm_hour;
  this->minute_ = tm_time.tm_min;
  this->second_ = tm_time.tm_sec;
  this->microsec_ = timevalue.usec ();
  this->wday_ = tm_time.tm_wday;
}

ASYS_INLINE
ACE_Date_Time::ACE_Date_Time (void)
{
  ACE_TRACE ("ACE_Date_Time::ACE_Date_Time");
  this->update ();
}

ASYS_INLINE
ACE_Date_Time::ACE_Date_Time (const ACE_Time_Value& timevalue)
{
  ACE_TRACE ("ACE_Date_Time::ACE_Date_Time: timevalue");
  this->update (timevalue);
}

// Constructor with init values, no check for validy
ASYS_INLINE
ACE_Date_Time::ACE_Date_Time (long day,
                              long month,
                              long year,
                              long hour,
                              long minute,
                              long second,
                              long microsec,
                              long wday)
  : day_ (day),
    month_ (month),
    year_ (year),
    hour_ (hour),
    minute_ (minute),
    second_ (second),
    microsec_ (microsec),
    wday_ (wday)
{
  ACE_TRACE ("ACE_Date_Time::ACE_Date_Time");
}

// set/get portions of ACE_Date_Time, no check for validy

// get day
ASYS_INLINE long
ACE_Date_Time::day (void) const
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
ACE_Date_Time::month (void) const
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
ACE_Date_Time::year (void) const
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
ACE_Date_Time::hour (void) const
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
ACE_Date_Time::minute (void) const
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
ACE_Date_Time::second (void) const
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
ACE_Date_Time::microsec (void) const
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

// get wday
ASYS_INLINE long
ACE_Date_Time::weekday (void) const
{
  ACE_TRACE ("ACE_Date_Time::weekday");
  return wday_;
}

// set wday
ASYS_INLINE void
ACE_Date_Time::weekday (long wday)
{
  ACE_TRACE ("ACE_Date_Time::weekday");
  wday_ = wday;
}
