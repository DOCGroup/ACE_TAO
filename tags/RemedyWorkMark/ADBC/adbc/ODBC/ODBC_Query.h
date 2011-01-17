// -*- C++ -*-

//=============================================================================
/**
 * @file    ODBC_Query.h
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _ADBC_ODBC_QUERY_H_
#define _ADBC_ODBC_QUERY_H_

#include "adbc/Query.h"
#include "ODBC_Parameter_List.h"
#include "ODBC_Record.h"

namespace ADBC
{
namespace ODBC
{
// Forward decl.
class Connection;

/**
 * @class ODBC_Query
 *
 * @brief Wrapper class for using ODBC statement handles.
 *
 * This class contains common operations used by ADBC for executing
 * SQL statements using ODBC. One cannot instantiate an object of this
 * class alone. Instead, it can only be created by an Connection
 * object. This prevents the creation of dangling ODBC_Query objects
 * (those not associated with a connection).
 */
class ADBC_ODBC_Export Query : public ::ADBC::Query
{
public:
  /**
   * Initializing constructor.
   *
   * @param[in]    conn     Parent database connection.
   */
  Query (Connection & conn);

  /// Destructor.
  virtual ~Query (void);

  /**
   * Prepare an SQL query for execution.
   *
   * @param[in]     query       NULL-terminate SQL query string.
   */
  virtual void prepare (const char * query);

  /// Execute a prepare query.
  virtual void execute_no_record (void);

  /**
   * Execute a query.
   *
   * @param[in]     query       Query to execute.
   */
  virtual void execute_no_record (const char * query);

  /**
   * Get the last insert id. This method is only value if an
   * insert was made to a table with an \a auto_increment field.
   *
   * @return The last id inserted.
   */
  virtual long last_insert_id (void);

  /// Cancel the current operation.
  virtual void cancel (void);

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

  /// Get a read-only list of the parameters.
  virtual const Parameter_List & parameters (void) const;

  /// Get a list of the parameters.
  virtual Parameter_List & parameters (void);

  /// Reset the query string.
  void reset (void);

  virtual size_t count (void) const;

  /// Get the handle to the underlying statement.
  HSTMT handle (void) const;

private:
  /// Initialize the query.
  void init (void);

  /// Parent of the query.
  Connection & parent_;

  /// handle to the ODBC statement
  HSTMT handle_;

  /// The state of the cursor.
  bool cursor_open_;

  /// The query is prepared.
  bool is_prepared_;

  /// Pointer to the parameters for the method.
  Parameter_List params_;

  /// The record associated with this query.
  Record record_;
};
}
}

#if defined (__ADBC_INLINE__)
#include "ODBC_Query.inl"
#endif

#endif  // !defined _ADBC_ODBC_QUERY_H_
