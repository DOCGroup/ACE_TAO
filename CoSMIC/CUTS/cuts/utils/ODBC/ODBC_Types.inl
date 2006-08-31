// $Id$

//
// ODBC_Date_Time
//
CUTS_INLINE
ODBC_Date_Time::ODBC_Date_Time (const ODBC_Date_Time & datetime)
: CUTS_DB_Date_Time_Impl (datetime)
{
  this->set (datetime);
}


//
// ODBC_Date_Time
//
CUTS_INLINE
ODBC_Date_Time::ODBC_Date_Time (const ACE_Date_Time & datetime)
{
  this->set (datetime);
}

//
// month
//
CUTS_INLINE
void ODBC_Date_Time::month (long value)
{
  this->datetime_.month = ACE_static_cast (SQLUSMALLINT, value);
}

//
// month
//
CUTS_INLINE
long ODBC_Date_Time::month (void) const
{
  return this->datetime_.month;
}

//
// day
//
CUTS_INLINE
void ODBC_Date_Time::day (long value)
{
  this->datetime_.day = ACE_static_cast (SQLUSMALLINT, value);
}

//
// day
//
CUTS_INLINE
long ODBC_Date_Time::day (void) const
{
  return this->datetime_.day;
}

//
// year
//
CUTS_INLINE
void ODBC_Date_Time::year (long value)
{
  this->datetime_.year = ACE_static_cast (SQLUSMALLINT, value);
}

//
// year
//
CUTS_INLINE
long ODBC_Date_Time::year (void) const
{
  return this->datetime_.year;
}

//
// hour
//
CUTS_INLINE
void ODBC_Date_Time::hour (long value)
{
  this->datetime_.hour = ACE_static_cast (SQLSMALLINT, value);
}

//
// hour
//
CUTS_INLINE
long ODBC_Date_Time::hour (void) const
{
  return this->datetime_.hour;
}

//
// minute
//
CUTS_INLINE
void ODBC_Date_Time::minute (long value)
{
  this->datetime_.minute = ACE_static_cast (SQLUSMALLINT, value);
}

//
// minute
//
CUTS_INLINE
long ODBC_Date_Time::minute (void) const
{
  return this->datetime_.minute;
}

//
// second
//
CUTS_INLINE
void ODBC_Date_Time::second (long value)
{
  this->datetime_.second = ACE_static_cast (SQLUSMALLINT, value);
}

//
// second
//
CUTS_INLINE
long ODBC_Date_Time::second (void) const
{
  return this->datetime_.second;
}

//
// fraction
//
CUTS_INLINE
void ODBC_Date_Time::fraction (long value)
{
  this->datetime_.fraction = ACE_static_cast (SQLUSMALLINT, value);
}

//
// fraction
//
CUTS_INLINE
long ODBC_Date_Time::fraction (void) const
{
  return this->datetime_.fraction;
}

//
// fraction
//
CUTS_INLINE
void * ODBC_Date_Time::value_i (void)
{
  return &this->datetime_;
}

//
// operator =
//
CUTS_INLINE
void ODBC_Date_Time::operator = (const ODBC_Date_Time & datetime)
{
  this->set (datetime);
}

//
// operator <<=
//
CUTS_INLINE
void ODBC_Date_Time::operator <<= (const ACE_Date_Time & datetime)
{
  this->set (datetime);
}
