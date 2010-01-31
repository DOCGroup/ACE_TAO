// -*- C++ -*-

//=============================================================================
/**
 * @file      Record.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ADBC_RECORD_H_
#define _ADBC_RECORD_H_

#include "Exception.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
// Forward decl.
class ACE_Date_Time;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace ADBC
{
/**
 * @class Record
 *
 * Base class implementation/interface for database records.
 */
class ADBC_Export Record
{
public:
  /// Default constructor.
  Record (void);

  /// Destructor.
  virtual ~Record (void);

  /**
   * Get the number of columns in the result.
   *
   * @return        Number of columns.
   */
  virtual size_t columns (void) const = 0;

  /// Move the next row in the records.
  virtual void advance (void) = 0;

  /// Determine if the record iterator is done.
  virtual bool done (void) const = 0;

  /// Reset to the first row in the record.
  virtual void reset (void) = 0;

  /**
   * Get a string data value at the specified column.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    buffer        Pointer to the output buffer.
   * @param[in]     bufsize       Size of the output buffer.
   */
  virtual void get_data (size_t column, char * buffer, size_t bufsize) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, char & value) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, unsigned char & value) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, short & value) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, unsigned short & value) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, long & value) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, unsigned long & value) = 0;

  /**
   * Get a float value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Float value.
   */
  virtual void get_data (size_t column, float & value) = 0;

  /**
   * Get a double value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Double value.
   */
  virtual void get_data (size_t column, double & value) = 0;

  /**
   * Get a date time value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, ACE_Date_Time & value) = 0;

private:
  // prevent the following operations
  Record (const Record &);
  const Record & operator = (const Record &);
};
}

#if defined (__ADBC_INLINE__)
#include "Record.inl"
#endif

#endif  // !defined _ADBC_DB_RECORD_H_
