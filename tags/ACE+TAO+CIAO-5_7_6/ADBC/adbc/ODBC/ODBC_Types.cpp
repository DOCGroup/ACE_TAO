// $Id$

#include "ODBC_Types.h"

#if !defined (__ADBC_INLINE__)
#include "ODBC_Types.inl"
#endif

#include "ace/OS_NS_string.h"
#include "ace/Date_Time.h"

namespace ADBC
{
namespace ODBC
{
//
// Date_Time
//
Date_Time::Date_Time (void)
{
  ACE_OS::memset (&this->datetime_,
                  0,
                  sizeof (SQL_TIMESTAMP_STRUCT));
}

//
// set
//
void Date_Time::set (const ACE_Date_Time & datetime)
{
  this->month (datetime.month ());
  this->day (datetime.day ());
  this->year (datetime.year ());
  this->hour (datetime.hour ());
  this->minute (datetime.minute ());
  this->second (datetime.second ());
  this->fraction (datetime.microsec ());
}

//
// set
//
void Date_Time::set (const Date_Time & datetime)
{
  ACE_OS::memcpy (&this->datetime_,
                  &datetime.datetime_,
                  sizeof (SQL_TIMESTAMP_STRUCT));
}
}
}
