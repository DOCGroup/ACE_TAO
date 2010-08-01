// -*- C++ -*-

//=============================================================================
/**
 * @file        Record.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _ADBC_DB_SQLITE_RECORD_H_
#define _ADBC_DB_SQLITE_RECORD_H_

#include "adbc/Record.h"
#include "SQLite_export.h"
#include "sqlite3.h"

namespace ADBC
{
namespace SQLite
{
// Forward decl.
class Query;

/**
 * @class Record
 *
 * Implemenation of the Record for SQLite.
 */
class ADBC_SQLITE_Export Record : public ADBC::Record
{
public:
  // Friend decl.
  friend class Query;

  /// Destructor
  virtual ~Record (void);

  /**
   * Get the number of columns in the record.
   *
   * @return        Number of columns.
   */
  virtual size_t columns (void) const;

  /**
   * Get the name of the column.
   *
   * @param[in]     index         Index of the column.
   * @param[in]     name          Name of the column.
   */
  const char * column_name (size_t index);

  /// Move to the next row in the record.
  virtual void advance (void);

  /// Test if reached the last row in the record.
  virtual bool done (void) const;

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

  virtual void get_data (size_t column, ACE_CString & value);

  /// Reset the record.
  virtual void reset (void);

private:
  /**
   * Initializing constructor. The ::ADBC::SQLite::Query class is the
   * only object that has access to this method.
   *
   * @param[in]       query         Parent of the record
   * @param[in]       state         Initial state
   */
  Record (const Query & query);

  /// Parent of the record.
  const Query & query_;

  /// The record's state.
  int state_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "Record.inl"
#endif

#endif  // !defined _ADBC_DB_SQLITE_RECORD_H_
