// $Id$

namespace ADBC
{
namespace SQLite
{

//
// Date_Time
//
ADBC_INLINE
Date_Time::Date_Time (void)
: out_of_date_ (true)
{

}

ADBC_INLINE
Date_Time::
Date_Time (const ACE_Date_Time & dt)
: dt_ (dt),
  out_of_date_ (true)
{

}

ADBC_INLINE
Date_Time::~Date_Time (void)
{

}

ADBC_INLINE
long Date_Time::month (void) const
{
  return this->dt_.month ();
}

ADBC_INLINE
void Date_Time::month (long val)
{
  this->dt_.month (val);
  this->out_of_date_ = true;
}

ADBC_INLINE
long Date_Time::day (void) const
{
  return this->dt_.day ();
}

ADBC_INLINE
void Date_Time::day (long value)
{
  this->dt_.day (value);
  this->out_of_date_ = true;
}

ADBC_INLINE
long Date_Time::year (void) const
{
  return this->dt_.year ();
}

ADBC_INLINE
void Date_Time::year (long value)
{
  this->dt_.year (value);
  this->out_of_date_ = true;
}

ADBC_INLINE
long Date_Time::hour (void) const
{
  return this->dt_.hour ();
}

ADBC_INLINE
void Date_Time::hour (long value)
{
  this->dt_.hour (value);
  this->out_of_date_ = true;
}

ADBC_INLINE
long Date_Time::minute (void) const
{
  return this->dt_.minute ();
}

ADBC_INLINE
void Date_Time::minute (long value)
{
  this->dt_.minute (value);
  this->out_of_date_ = true;
}

ADBC_INLINE
long Date_Time::second (void) const
{
  return this->dt_.second ();
}

ADBC_INLINE
void Date_Time::second (long value)
{
  this->dt_.second (value);
  this->out_of_date_ = true;
}

ADBC_INLINE
long Date_Time::fraction (void) const
{
  return this->dt_.microsec  ();
}

ADBC_INLINE
void Date_Time::fraction (long val)
{
  this->dt_.microsec (val);
  this->out_of_date_ = true;
}
}
}
