// $Id$

namespace ADBC
{
namespace ODBC
{
//
// ~Date_Time
//
ADBC_INLINE
Date_Time::~Date_Time (void)
{

}

//
// month
//
ADBC_INLINE
void Date_Time::month (long value)
{
  this->datetime_.month = static_cast <SQLUSMALLINT> (value);
}

//
// month
//
ADBC_INLINE
long Date_Time::month (void) const
{
  return this->datetime_.month;
}

//
// day
//
ADBC_INLINE
void Date_Time::day (long value)
{
  this->datetime_.day = static_cast <SQLUSMALLINT> (value);
}

//
// day
//
ADBC_INLINE
long Date_Time::day (void) const
{
  return this->datetime_.day;
}

//
// year
//
ADBC_INLINE
void Date_Time::year (long value)
{
  this->datetime_.year = static_cast <SQLUSMALLINT> (value);
}

//
// year
//
ADBC_INLINE
long Date_Time::year (void) const
{
  return this->datetime_.year;
}

//
// hour
//
ADBC_INLINE
void Date_Time::hour (long value)
{
  this->datetime_.hour = static_cast <SQLSMALLINT> (value);
}

//
// hour
//
ADBC_INLINE
long Date_Time::hour (void) const
{
  return this->datetime_.hour;
}

//
// minute
//
ADBC_INLINE
void Date_Time::minute (long value)
{
  this->datetime_.minute = static_cast <SQLUSMALLINT> (value);
}

//
// minute
//
ADBC_INLINE
long Date_Time::minute (void) const
{
  return this->datetime_.minute;
}

//
// second
//
ADBC_INLINE
void Date_Time::second (long value)
{
  this->datetime_.second = static_cast <SQLUSMALLINT> (value);
}

//
// second
//
ADBC_INLINE
long Date_Time::second (void) const
{
  return this->datetime_.second;
}

//
// fraction
//
ADBC_INLINE
void Date_Time::fraction (long value)
{
  this->datetime_.fraction = static_cast <SQLUSMALLINT> (value);
}

//
// fraction
//
ADBC_INLINE
long Date_Time::fraction (void) const
{
  return this->datetime_.fraction;
}

//
// fraction
//
ADBC_INLINE
void * Date_Time::value (void)
{
  return &this->datetime_;
}

}
}
