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

#ifndef _CUTS_ODBC_RECORD_H_
#define _CUTS_ODBC_RECORD_H_

#include "ODBC_Base.h"
#include "ODBC_Exception.h"
#include "cuts/utils/DB_Record.h"

//=============================================================================
/**
 * @class ODBC_Record
 *
 * @brief ODBC implementation of the CUTS_DB_Record class.
 */
//=============================================================================

class CUTS_ODBC_Export ODBC_Record :
  public ODBC_Base,
  public CUTS_DB_Record
{
public:
  /**
   * Initializing constructor.
   *
   * @param[in]     handle      Statement handle.
   */
  ODBC_Record (HSTMT handle);

  /// Destructor.
  virtual ~ODBC_Record (void);

  /// Fetch the next row in the records.
  virtual void fetch (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /// Get the number of records
  virtual size_t count (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void get_data (size_t column, char * buffer, size_t bufsize)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void get_data (size_t column, char & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void get_data (size_t column, unsigned char & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void get_data (size_t column, short & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void get_data (size_t column, unsigned short & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void get_data (size_t column, long & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void get_data (size_t column, unsigned long & value)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void get_data (size_t column, ACE_Date_Time & datetime)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Get the number of columns in the result.
   *
   * @return Number of columns.
   */
  virtual size_t columns (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

private:
  /// Help method that wraps the SQLGetData method.
  void get_data_i (SQLUSMALLINT column,
                   SQLSMALLINT target_type,
                   SQLPOINTER target,
                   SQLINTEGER bufsize,
                   SQLINTEGER * result)
                   ACE_THROW_SPEC ((CUTS_DB_Exception));

  /// ODBC statement resource handle.
  HSTMT handle_;
};

#if defined (__CUTS_INLINE__)
#include "ODBC_Record.inl"
#endif

#endif  // !defined _CUTS_ODBC_RECORD_H_
