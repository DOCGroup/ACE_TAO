// -*- C++ -*-

//=============================================================================
/**
 * @file        Query.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _ADBC_SQLITE_QUERY_H_
#define _ADBC_SQLITE_QUERY_H_

#include "adbc/Query.h"
#include "sqlite3.h"
#include "Parameter_List.h"
#include "Record.h"

namespace ADBC
{
namespace SQLite
{
// Forward decl.
class Connection;

/**
 * @class Query
 *
 * Implemenation of the Query class for SQLite
 */
class ADBC_SQLITE_Export Query : public ::ADBC::Query
{
  // Friend decl.
  friend class Record;

  // Friend decl.
  friend class Parameter_List;

  // Friend decl.
  friend class Parameter;

public:
  /// Default constructor
  Query (Connection & parent);

  /// Destructor
  virtual ~Query (void);

  /**
   * Prepare a statement for execution.
   *
   * @param[in]     query     NULL-terminated query string.
   */
  virtual void prepare (const char * query);

  /**
   * Prepare a statement for execution.
   *
   * @param[in]     query     NULL-terminated query string.
   */
  virtual void prepare (const char * query, size_t len);

  /**
   * Directly execute a database query.
   *
   * @param[in]     query     NULL-terminated query string.
   */
  virtual void execute_no_record (const char * query);

  /// Execute an already prepared query.
  virtual void execute_no_record (void);

  /**
   * Execute a query. This method is useful with the query is known
   * to return results that will consist of multiple data rows, and
   * columns. The client has the responsibility of delete the record
   * once its done with it.
   *
   * @return  Pointer to a record.
   */
  virtual Record & execute (const char * query);

  /**
   * Execute a prepared query. This method is useful with the query is
   * known to return results that will consist of multiple data rows,
   * and columns. The client has the responsibility of delete the record
   * once its done with it.
   *
   * @return  Pointer to a record.
   */
  virtual Record & execute (void);

  /// Cancel the current query.
  virtual void cancel (void);

  /**
   * Get the last insert id. This method is only value if an
   * insert was made to a table with an \a auto_increment field.
   *
   * @return The last id inserted.
   */
  virtual long last_insert_id (void);

  // Get the number of rows affects by last SQL statement.
  virtual size_t count (void) const;

  // Get a list of the parameters.
  virtual Parameter_List & parameters (void);

  // Get a read-only list of the parameters.
  virtual const Parameter_List & parameters (void) const;

  /// Reset the query string.
  virtual void reset (void);

private:
  void finalize (void);

  /// Owner of the query.
  Connection & parent_;

  /// Actual SQLite3 statement.
  ::sqlite3_stmt * stmt_;

  /// Collection of parameters for this query.
  Parameter_List params_;

  /// Record associated with the statement.
  Record record_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "Query.inl"
#endif

#endif  // !defined _ADBC_SQLITE_QUERY_H_
