// -*- C++ -*-

//=============================================================================
/**
 * @file          Types.h
 *
 * $Id$
 *
 * @author        James H. Hill
 */
//=============================================================================

#ifndef _ADBC_SQLITE_TYPES_H_
#define _ADBC_SQLITE_TYPES_H_

#include "adbc/Types.h"
#include "ace/Date_Time.h"
#include "SQLite_export.h"

namespace ADBC
{
namespace SQLite
{

class ADBC_SQLITE_Export Date_Time :
  public ::ADBC::Date_Time
{
public:
  /// Default constructor.
  Date_Time (void);

  /**
   * Initializing constructor.
   *
   * @param[in]       dt        Source data and time.
   */
  Date_Time (const ACE_Date_Time & dt);

  /**
   * Initializing constructor.
   */
  Date_Time (long month,
             long day,
             long year,
             long hour,
             long min,
             long sec,
             long frac);

  /**
   * Copy constructor.
   *
   * @param[in]       dt        Source data and time
   */
  Date_Time (const Date_Time & dt);

  /// Destructor.
  virtual ~Date_Time (void);

  virtual long month (void) const;

  virtual void month (long val);

  virtual long day (void) const;

  virtual void day (long val);

  virtual long year (void) const;

  virtual void year (long value);

  virtual long hour (void) const;

  virtual void hour (long);

  virtual long minute (void) const;

  virtual void minute (long value);

  virtual long second (void) const;

  virtual void second (long value);

  virtual long fraction (void) const;

  virtual void fraction (long val);

  virtual void * value (void);

private:
  /// Binary version of the date time.
  ACE_Date_Time dt_;

  /// String version of the time.
  char buffer_[20];

  /// The string version is out of date.
  bool out_of_date_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "Types.inl"
#endif

#endif  // !defined _ADBC_DB_SQLITE_TYPES_H_
