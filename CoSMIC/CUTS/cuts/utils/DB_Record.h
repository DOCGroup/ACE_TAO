// -*- C++ -*-

//=============================================================================
/**
 * @file      DB_Record.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_DB_RECORD_H_
#define _CUTS_DB_RECORD_H_

#include "DB_Exception.h"

// Forward decl.
class ACE_Date_Time;

//=============================================================================
/**
 * @class CUTS_DB_Record
 *
 * Base class implementation/interface for database records.
 */
//=============================================================================

class CUTS_DB_UTILS_Export CUTS_DB_Record
{
public:
  /// Default constructor.
  CUTS_DB_Record (void);

  /// Destructor.
  virtual ~CUTS_DB_Record (void) = 0;

  /**
   * Get the number of records in the result.
   *
   * @return Number of records.
   */
  virtual size_t count (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /// Fetch the next row in the records.
  virtual void fetch (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get a string data value at the specified column.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    buffer        Pointer to the output buffer.
   * @param[in]     bufsize       Size of the output buffer.
   */
  virtual void get_data (size_t column, char * buffer, size_t bufsize)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, char & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, unsigned char & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, short & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, unsigned short & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, long & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, unsigned long & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get a character value.
   *
   * @param[in]     column        1-based column index.
   * @param[out]    value         Character value.
   */
  virtual void get_data (size_t column, ACE_Date_Time & datetime)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get the number of columns in the result.
   *
   * @return Number of columns.
   */
  virtual size_t columns (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

private:
  /// prevent the following operations
  CUTS_DB_Record (const CUTS_DB_Record &);
  const CUTS_DB_Record & operator = (const CUTS_DB_Record &);
};

#if defined (__CUTS_INLINE__)
#include "DB_Record.inl"
#endif

#endif  // !defined _CUTS_DB_RECORD_H_
