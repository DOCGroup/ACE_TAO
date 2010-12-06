// -*- C++ -*-

//=============================================================================
/**
 * @file      Query.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _ADBC_QUERY_H_
#define _ADBC_QUERY_H_

#include "ADBC_export.h"
#include "config.h"

namespace ADBC
{
// Forward decl.
class Record;

// Forward decl.
class Parameter_List;

/**
 * @class Query
 *
 * Base class implemenation/interface for database query objects.
 */
class ADBC_Export Query
{
public:
  /// Default constructor.
  Query (void);

  /// Destructor.
  virtual ~Query (void);

  /**
   * Prepare a statement for execution.
   *
   * @param[in]     query     NULL-terminated query string.
   */
  virtual void prepare (const char * query) = 0;

  /// Destroy the query. The query is no longer usable after
  /// this method returns.
  virtual void destroy (void);

  /**
   * Directly execute a database query.
   *
   * @param[in]     query     NULL-terminated query string.
   */
  virtual void execute_no_record (const char * query) = 0;

  /// Execute an already prepared query.
  virtual void execute_no_record (void) = 0;

  /**
   * Execute a query. This method is useful with the query is known
   * to return results that will consist of multiple data rows, and
   * columns. The client has the responsibility of delete the record
   * once its done with it.
   *
   * @return  Pointer to a record.
   */
  virtual Record & execute (const char * query) = 0;

  /**
   * Execute a prepared query. This method is useful with the query is
   * known to return results that will consist of multiple data rows,
   * and columns. The client has the responsibility of delete the record
   * once its done with it.
   *
   * @return  Pointer to a record.
   */
  virtual Record & execute (void) = 0;

  /// Cancel the current query.
  virtual void cancel (void) = 0;

  /**
   * Get the last insert id. This method is only value if an
   * insert was made to a table with an \a auto_increment field.
   *
   * @return The last id inserted.
   */
  virtual long last_insert_id (void) = 0;

  /**
   * Create a parameter for the statement. The parameter must
   * still be added to the collection of the parameters
   *
   * @return        Pointer to the new parameter.
   */
  virtual Parameter_List & parameters (void) = 0;

  /**
   * Create a parameter for the statement. The parameter must
   * still be added to the collection of the parameters
   *
   * @return        Pointer to the new parameter.
   */
  virtual const Parameter_List & parameters (void) const = 0;

  /// Reset the query string.
  virtual void reset (void) = 0;

  /**
   * Determine the number of rows affected by the query. This method
   * is useful for DELETE, INSERT, and UPDATE statements. It should
   * not be used to determine the number of rows in a record after
   * executing a SELECT statement since it may result in unreliable
   * results depending on the driver.
   */
  virtual size_t count (void) const = 0;

private:
  // prevent the following operations
  Query (const Query &);
  const Query & operator = (const Query &);
};
}

#if defined (__ADBC_INLINE__)
#include "Query.inl"
#endif

#endif  // !defined _ADBC_DB_QUERY_H_
