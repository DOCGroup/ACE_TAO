// -*- C++ -*-

//=============================================================================
/**
 * @file      Exception.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ADBC_ODBC_EXCEPTION_H_
#define _ADBC_ODBC_EXCEPTION_H_

#include "adbc/Exception.h"
#include "ODBC.h"

namespace ADBC
{
namespace ODBC
{
/**
 * @class Exception
 *
 * @brief Base class for all ODBC exception classes.
 */
class ADBC_ODBC_Export Exception : public ::ADBC::Exception
{
public:
  /// Default constructor.
  Exception (void);

  /**
   * Initializing constructor.
   *
   * @param[in]       handle        Faulting ODBC handle.
   * @param[in]       type          Type of handle.
   */
  Exception (SQLHANDLE handle, SQLSMALLINT type);

  /**
   * Initializing constructor.
   *
   * @param[in]     message     Set the error message.
   */
  Exception (const char * message);

  /// Default destructor.
  virtual ~Exception (void);
};
}
}

#define SQL_ENV_VERIFY(op, handle) \
  SQL_VERIFY (op, ::ADBC::ODBC::Exception (handle, SQL_HANDLE_ENV))

/// Exception for an ODBC connection \a handle.
#define SQL_CONN_VERIFY(op, handle) \
  SQL_VERIFY (op, ::ADBC::ODBC::Exception (handle, SQL_HANDLE_DBC))

/// Exception for an ODBC statement \a handle.
#define SQL_STMT_VERIFY(op, handle) \
  SQL_VERIFY (op, ::ADBC::ODBC::Exception (handle, SQL_HANDLE_STMT))

/// Exception for an ODBC description \a handle.
#define SQL_DESC_VERIFY(op, handle) \
  SQL_VERIFY (op, ::ADBC::ODBC::Exception (handle, SQL_HANDLE_DESC))

#if defined (__ADBC_INLINE__)
#include "ODBC_Exception.inl"
#endif

#endif  // !defined _ADBC_ODBC_EXCEPTION_H_
