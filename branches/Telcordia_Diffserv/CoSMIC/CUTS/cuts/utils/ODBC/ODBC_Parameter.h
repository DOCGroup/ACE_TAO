// -*- C++ -*-

//=============================================================================
/**
 * @file      ODBC_Parameter.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ODBC_PARAMETER_H_
#define _ODBC_PARAMETER_H_

#include "ODBC_Base.h"
#include "ODBC_Exception.h"
#include "cuts/utils/DB_Parameter.h"

//=============================================================================
/**
 * @class ODBC_Parameter
 *
 * ODBC implementation of the CUTS_DB_Parameter abstract type.
 */
//=============================================================================

class CUTS_ODBC_Export ODBC_Parameter :
  public CUTS_DB_Parameter,
  public ODBC_Base
{
public:
  /**
   * Initializing constructor.
   *
   * @param[in]     handle      Handle to the ODBC statement.
   */
  ODBC_Parameter (HSTMT handle, int index);

  /// Destructor.
  virtual ~ODBC_Parameter (void);

  virtual void bind (char * buffer, size_t bufsize)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void bind (short * buffer)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void bind (u_short * buffer)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void bind (long * buffer)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void bind (u_long * buffer)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void bind (float * buffer)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void bind (double * buffer)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void null (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  virtual void bind (CUTS_DB_Date_Time_Impl * datetime)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

private:
  /// Helper method for binding SQL parameters.
  void bind_i (SQLSMALLINT iotype,
               SQLSMALLINT valuetype,
               SQLSMALLINT paramtype,
               SQLUINTEGER columnsize,
               SQLSMALLINT decimals,
               SQLPOINTER  valueptr,
               SQLINTEGER  buffer_length)
               ACE_THROW_SPEC ((CUTS_DB_Exception));

  /// Handle to the ODBC statement.
  HSTMT handle_;

  /// Size of the parameter buffer.
  SQLINTEGER intptr_;
};

#endif  // !defined _ODBC_PARAMETER_H_
