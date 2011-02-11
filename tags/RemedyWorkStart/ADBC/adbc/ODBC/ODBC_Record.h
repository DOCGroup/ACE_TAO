// -*- C++ -*-

//=============================================================================
/**
 * @file      ODBC_Record.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ADBC_ODBC_RECORD_H_
#define _ADBC_ODBC_RECORD_H_

#include "adbc/Record.h"
#include "ODBC.h"

namespace ADBC
{
namespace ODBC
{
// Forward decl.
class Query;

/**
 * @class ODBC_Record
 *
 * Implementation of the ::ADBC::Record for ODBC.
 */
class ADBC_ODBC_Export Record : public ::ADBC::Record
{
public:
  /**
   * Initializing constructor.
   *
   * @param[in]     handle      Statement handle.
   */
  Record (const Query & query);

  /// Destructor.
  virtual ~Record (void);

  // Move to the next record.
  virtual void advance (void);

  // Test if the iterator is done.
  virtual bool done (void) const;

  virtual void reset (void);

  /**
   * Get the number of columns in the result.
   *
   * @return Number of columns.
   */
  virtual size_t columns (void) const;

  virtual void get_data (size_t column, char * buffer, size_t bufsize);

  virtual void get_data (size_t column, char & value);

  virtual void get_data (size_t column, unsigned char & value);

  virtual void get_data (size_t column, short & value);

  virtual void get_data (size_t column, unsigned short & value);

  virtual void get_data (size_t column, long & value);

  virtual void get_data (size_t column, unsigned long & value);

  virtual void get_data (size_t column, float & value);

  virtual void get_data (size_t column, double & value);

  virtual void get_data (size_t column, ACE_Date_Time & datetime);

private:
  /// Help method that wraps the SQLGetData method.
  void get_data_i (SQLUSMALLINT column,
                   SQLSMALLINT target_type,
                   SQLPOINTER target,
                   SQLINTEGER bufsize,
                   SQLINTEGER * result);

  /// Parent of the record.
  const Query & query_;

  /// Number of columns in the record set.
  size_t columns_;

  /// State of the iterator.
  SQLRETURN state_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "ODBC_Record.inl"
#endif

#endif  // !defined _ADBC_ODBC_RECORD_H_
