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

#ifndef _CUTS_ODBC_QUERY_H_
#define _CUTS_ODBC_QUERY_H_

#include "ODBC_Base.h"
#include "ODBC_Exception.h"
#include "cuts/utils/DB_Query.h"
#include "ace/Auto_Ptr.h"

// Forward decl.
class ODBC_Record;

// Forward decl.
class ODBC_Parameter;

// Forward decl.
class ODBC_Parameter_List;

//=============================================================================
/**
 * @class ODBC_Query
 *
 * @brief Wrapper class for using ODBC statement handles.
 *
 * This class contains common operations used by CUTS for executing
 * SQL statements using ODBC. One cannot instantiate an object of this
 * class alone. Instead, it can only be created by an ODBC_Connection
 * object. This prevents the creation of dangling ODBC_Query objects
 * (those not associated with a connection).
 */
//=============================================================================

class CUTS_ODBC_Export ODBC_Query :
  public ODBC_Base,
  public CUTS_DB_Query
{
public:
  /**
   * Initializing constructor.
   *
   * @param[in]    conn     Parent database connection.
   */
  ODBC_Query (HDBC conn);

  /// Destructor.
  virtual ~ODBC_Query (void);

  /// Execute a prepare query.
  virtual void execute_no_record (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Execute a query.
   *
   * @param[in]     query       Query to execute.
   */
  virtual void execute_no_record (const char * query)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Prepare an SQL query for execution.
   *
   * @param[in]     query       NULL-terminate SQL query string.
   */
  virtual void prepare (const char * query)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Get the last insert id. This method is only value if an
   * insert was made to a table with an \a auto_increment field.
   *
   * @return The last id inserted.
   */
  virtual long last_insert_id (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /// Cancel the current operation.
  virtual void cancel (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Execute a query. This method is useful with the query is known
   * to return results that will consist of multiple data rows, and
   * columns. The client has the responsibility of delete the record
   * once its done with it.
   *
   * @return  Pointer to a record.
   */
  virtual CUTS_DB_Record * execute (const char * query)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Execute a prepared query. This method is useful with the query is
   * known to return results that will consist of multiple data rows,
   * and columns. The client has the responsibility of delete the record
   * once its done with it.
   *
   * @return  Pointer to a record.
   */
  virtual CUTS_DB_Record * execute (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  void bind_parameter (SQLUSMALLINT index,
                       SQLSMALLINT io_type,
                       SQLSMALLINT value_type,
                       SQLSMALLINT param_type,
                       SQLUINTEGER column_size,
                       SQLSMALLINT decimal_digits,
                       SQLPOINTER param_value_ptr,
                       SQLINTEGER buffer_length,
                       SQLINTEGER * strlen_indptr)
                       ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Get the parameter.
   *
   * @param[in]     index     Index of the parameter.
   * @return        Pointer to the parameter.
   */
  CUTS_DB_Parameter * parameter (size_t index)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /**
   * Get the number of parameters.
   *
   * @return        The number of parameters.
   */
  size_t parameter_count (void) const
    ACE_THROW_SPEC ((CUTS_DB_Exception));

private:
  void reset_cursor (void)
    ACE_THROW_SPEC ((CUTS_DB_Exception));

  /// Get the pointer to the record.
  ODBC_Record * record_i (void);

  /// handle to the ODBC statement
  HSTMT stmt_;

  /// The state of the cursor.
  int cursor_open_;

  /// Pointer to one and only record.
  ACE_Auto_Ptr <ODBC_Record> record_;

  /// Pointer to the parameters for the method.
  ACE_Auto_Ptr <ODBC_Parameter_List> params_;
};

#if defined (__CUTS_INLINE__)
#include "ODBC_Query.inl"
#endif

#endif  // !defined _CUTS_ODBC_QUERY_H_
