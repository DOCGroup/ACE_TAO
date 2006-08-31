// $Id$

#include "ODBC_Types.h"

#if !defined (__CUTS_INLINE__)
#include "ODBC_Types.inl"
#endif

#include "ace/OS_NS_string.h"
#include "ace/Date_Time.h"

//=============================================================================
/*
 * ODBC_Date_Time
 */
//=============================================================================

//
// ODBC_Date_Time
//
ODBC_Date_Time::ODBC_Date_Time (void)
{
  ACE_OS::memset (&this->datetime_,
                  0,
                  sizeof (SQL_TIMESTAMP_STRUCT));
}

//
// ~ODBC_Date_Time
//
ODBC_Date_Time::~ODBC_Date_Time (void)
{

}

//
// set
//
void ODBC_Date_Time::set (const ACE_Date_Time & datetime)
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
void ODBC_Date_Time::set (const ODBC_Date_Time & datetime)
{
  ACE_OS::memcpy (&this->datetime_,
                  &datetime.datetime_,
                  sizeof (SQL_TIMESTAMP_STRUCT));
}

//
// operator >>=
//
void ODBC_Date_Time::operator >>= (ACE_Date_Time & datetime)
{
  datetime.month (this->datetime_.month);
  datetime.day (this->datetime_.day);
  datetime.year (this->datetime_.year);
  datetime.hour (this->datetime_.hour);
  datetime.minute (this->datetime_.minute);
  datetime.second (this->datetime_.second);
  datetime.microsec (this->datetime_.fraction);
}
