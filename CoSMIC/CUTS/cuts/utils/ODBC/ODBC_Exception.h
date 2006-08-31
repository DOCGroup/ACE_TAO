// -*- C++ -*-

//=============================================================================
/**
 * @file      ODBC_Exception.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_ODBC_EXCEPTION_H_
#define _CUTS_ODBC_EXCEPTION_H_

#include "ODBC.h"
#include "cuts/utils/DB_Exception.h"

//=============================================================================
/**
 * @class ODBC_Exception
 *
 * @brief Base class for all ODBC exception classes.
 */
//=============================================================================

class CUTS_ODBC_Export ODBC_Exception : public CUTS_DB_Exception
{
public:
  /// Default constructor.
  ODBC_Exception (void);

  /**
   * Initializing constructor.
   *
   * @param[in]       handle        Faulting ODBC handle.
   * @param[in]       type          Type of handle.
   */
  ODBC_Exception (SQLHANDLE handle, SQLSMALLINT type);

  /**
   * Initializing constructor.
   *
   * @param[in]     message     Set the error message.
   */
  ODBC_Exception (const char * message);

  /// Default destructor.
  virtual ~ODBC_Exception (void);
};

/// Exception for an ODBC environment \a handle.
#define ODBC_Env_Exception(handle) \
  ODBC_Exception (handle, SQL_HANDLE_ENV)

/// Exception for an ODBC connection \a handle.
#define ODBC_Conn_Exception(handle) \
  ODBC_Exception (handle, SQL_HANDLE_DBC)

/// Exception for an ODBC statement \a handle.
#define ODBC_Stmt_Exception(handle) \
  ODBC_Exception (handle, SQL_HANDLE_STMT)

/// Exception for an ODBC description \a handle.
#define ODBC_Desc_Exception(handle) \
  ODBC_Exception (handle, SQL_HANDLE_DESC)

#if defined (__CUTS_INLINE__)
#include "ODBC_Exception.inl"
#endif

#endif  // !defined _CUTS_ODBC_EXCEPTION_H_
