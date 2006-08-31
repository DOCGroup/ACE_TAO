// -*- C++ -*-

//=============================================================================
/**
 * @file      DB_Query.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _CUTS_DB_QUERY_H_
#define _CUTS_DB_QUERY_H_

#include "DB_Exception.h"

// Forward decl.
class CUTS_DB_Record;

// Forward decl.
class CUTS_DB_Parameter;

//=============================================================================
/**
 * @class CUTS_DB_Query
 *
 * Base class implemenation/interface for database query objects.
 */
//=============================================================================

class CUTS_DB_UTILS_Export CUTS_DB_Query
{
public:
  /// Default constructor.
  CUTS_DB_Query (void);

  /// Destructor.
  virtual ~CUTS_DB_Query (void);

  /**
   * Prepare a statement for execution.
   *
   * @param[in]     query     NULL-terminated query string.
   */
  virtual void prepare (const char * query)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Directly execute a database query.
   *
   * @param[in]     query     NULL-terminated query string.
   */
  virtual void execute_no_record (const char * query)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /// Execute an already prepared query.
  virtual void execute_no_record (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Execute a query. This method is useful with the query is known
   * to return results that will consist of multiple data rows, and
   * columns. The client has the responsibility of delete the record
   * once its done with it.
   *
   * @return  Pointer to a record.
   */
  virtual CUTS_DB_Record * execute (const char * query)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Execute a prepared query. This method is useful with the query is
   * known to return results that will consist of multiple data rows,
   * and columns. The client has the responsibility of delete the record
   * once its done with it.
   *
   * @return  Pointer to a record.
   */
  virtual CUTS_DB_Record * execute (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /// Cancel the current query.
  virtual void cancel (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get the last insert id. This method is only value if an
   * insert was made to a table with an \a auto_increment field.
   *
   * @return The last id inserted.
   */
  virtual long last_insert_id (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Create a parameter for the statement. The parameter must
   * still be added to the collection of the parameters
   *
   * @return        Pointer to the new parameter.
   */
  virtual CUTS_DB_Parameter * parameter (size_t index)
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

  /**
   * Get the number of parameters.
   *
   * @return        The number of parameters.
   */
  virtual size_t parameter_count (void) const
    ACE_THROW_SPEC ((CUTS_DB_Exception)) = 0;

private:
  // prevent the following operations
  CUTS_DB_Query (const CUTS_DB_Query &);
  const CUTS_DB_Query & operator = (const CUTS_DB_Query &);
};

#if defined (__CUTS_INLINE__)
#include "DB_Query.inl"
#endif

#endif  // !defined _CUTS_DB_QUERY_H_
