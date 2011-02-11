// -*- C++ -*-

//=============================================================================
/**
 * @file        ODBC_Connection.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _ADBC_ODBC_CONNECTION_H_
#define _ADBC_ODBC_CONNECTION_H_

#include "ODBC_Env.h"
#include "ODBC_Exception.h"
#include "ODBC_Query.h"
#include "adbc/Connection.h"

namespace ADBC
{
namespace ODBC
{
// Forward decl.
class Query;

/**
 * @class Connection
 *
 * @brief Wrapper class for handling ODBC connection objects.
 *
 * This class provides a minimal set of operations used in ADBC for
 * creating connection to a database using the ODBC protocol. Futhermore,
 * this class is the only way to create an ODBC_Query object for performing
 * SQL queries using the ODBC protocol.
 */
class ADBC_ODBC_Export Connection :
  public ::ADBC::Connection
{
  // Friend decl.
  friend class Query;

public:
  /// Constructor
  Connection (Environment * env = Environment::instance ());

  /// Destructor
  virtual ~Connection (void);

  /**
   * Establish connection of an ODBC database. The connection string
   * is a full connection string that depends on the target database
   * driver. For a complete list of ODBC connection string formats,
   * please see the following location:
   *
   *   http://www.connectionstrings.com
   *
   * @param[in]     connstr       The connection string.
   */
  virtual void connect (const ACE_CString & connstr);

  /// Close the existing connection
  virtual void disconnect (void);

  /**
   * Create an ODBC query object.
   *
   * @return Pointer to the database query.
   */
  virtual ::ADBC::ODBC::Query * create_query (void);

  /// Get the underlying handle to the connection.
  HDBC handle (void) const;

private:
  /// Initialize the connection.
  void init (void);

  /// Handle to the database connection
  HDBC handle_;

  /// Pointer to the target environment.
  Environment * env_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "ODBC_Connection.inl"
#endif

#endif  // !defined _ADBC_ODBC_CONNECTION_H_
