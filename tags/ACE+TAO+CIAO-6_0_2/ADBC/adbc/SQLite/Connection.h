// -*- C++ -*-

//=============================================================================
/**
 * @file        Connection.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _ADBC_SQLITE_CONNECTION_H_
#define _ADBC_SQLITE_CONNECTION_H_

#include "adbc/Connection.h"
#include "ace/SString.h"
#include "Query.h"

// Forward decl.
struct sqlite3;

namespace ADBC
{
namespace SQLite
{
// Forward decl.
class Exception;

/**
 * @class Connection
 *
 * Implementation of a SQLite connection.
 */
class ADBC_SQLITE_Export Connection : public ADBC::Connection
{
  // Friend decl.
  friend class Query;

  // Friend decl.
  friend class Exception;

public:
  /// Default constructor
  Connection (void);

  /// Destructor.
  virtual ~Connection (void);

  /**
   * Connect to the specified database.
   *
   * @param[in]       connstr       Connection string for database
   * @param[in]       flags         Flags for the connection
   * @param[in]       vfs           File system flags
   */
  void connect (const ACE_CString & connstr, long flags, const char * vfs = 0);

  /**
   * Connect to the specified database.
   *
   * @param[in]       connstr       Connection string for database.
   */
  virtual void connect (const ACE_CString & connstr);

  /// Disconnect from the database.
  virtual void disconnect (void);

  /// Create a new SQLite query.
  virtual Query * create_query (void);

private:
  /// Connection the database.
  sqlite3 * conn_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "Connection.inl"
#endif

#endif  // !defined _ADBC_SQLITE_CONNECTION_H_
