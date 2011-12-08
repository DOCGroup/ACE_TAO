// -*- C++ -*-

//=============================================================================
/**
 * @file        ODBC_Types.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _ADBC_ODBC_TYPES_H_
#define _ADBC_ODBC_TYPES_H_

#include "ODBC.h"
#include "adbc/Types.h"

// Forward decl.
ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Date_Time;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace ADBC
{
namespace ODBC
{
/**
 * @class Date_Time
 *
 * ODBC implementation of the ::ADBC::Date_Time class.
 */
class ADBC_ODBC_Export Date_Time : public ::ADBC::Date_Time
{
public:
  /// Default constructor.
  Date_Time (void);

  /// Destructor.
  virtual ~Date_Time (void);

  long month (void) const;

  void month (long);

  long day (void) const;

  void day (long);

  long year (void) const;

  void year (long);

  long hour (void) const;

  void hour (long);

  long minute (void) const;

  void minute (long);

  long second (void) const;

  void second (long);

  long fraction (void) const;

  void fraction (long);

  void * value (void);

  void set (const ACE_Date_Time & datetime);

  void set (const Date_Time & datetime);

private:
  /// SQL timestamp data value.
  SQL_TIMESTAMP_STRUCT datetime_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "ODBC_Types.inl"
#endif

#endif  // !defined _ADBC_ODBC_TYPES_H_
