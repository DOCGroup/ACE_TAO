// $Id$

#include "ODBC_Query.h"

#if !defined (__CUTS_INLINE__)
#include "ODBC_Query.inl"
#endif

#include "ODBC_Record.h"
#include "ODBC_Parameter.h"
#include "ODBC_Parameter_List.h"
#include "ace/Log_Msg.h"

//
// ODBC_Query
//
ODBC_Query::ODBC_Query (HDBC handle)
: stmt_ (SQL_NULL_HSTMT),
  cursor_open_ (0)
{
  SQL_VERIFY (
    ::SQLAllocHandle (SQL_HANDLE_STMT, handle, &this->stmt_),
    ODBC_Conn_Exception (handle));

  // Set the attributes for the statement
  this->return_ = ::SQLSetStmtAttr (this->stmt_, SQL_ATTR_CONCURRENCY,
    (SQLPOINTER) SQL_CONCUR_ROWVER, 0);
  this->return_ = ::SQLSetStmtAttr (this->stmt_, SQL_ATTR_CURSOR_TYPE,
    (SQLPOINTER) SQL_CURSOR_KEYSET_DRIVEN, 0);
}

//
// ~ODBC_Query
//
ODBC_Query::~ODBC_Query (void)
{
  // Free the statement handle.
  if (this->stmt_ != SQL_NULL_HSTMT)
    ::SQLFreeStmt (this->stmt_, SQL_RESET_PARAMS);
}

//
// execute_no_record
//
void ODBC_Query::execute_no_record (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  SQL_VERIFY (::SQLExecute (this->stmt_),
              ODBC_Stmt_Exception (this->stmt_));
  this->cursor_open_ = 0;
}

//
// execute_no_record
//
void ODBC_Query::execute_no_record (const char * query)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  SQL_VERIFY (::SQLExecDirect (this->stmt_, (SQLCHAR *)query, SQL_NTS),
              ODBC_Stmt_Exception (this->stmt_));
  this->cursor_open_ = 0;
}

//
// last_insert_id
//
long ODBC_Query::last_insert_id (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  long last_id = 0;

  // Execute the statement and move to the first and only
  // row in the result set.
  CUTS_DB_Record * record = this->execute ("SELECT LAST_INSERT_ID()");

  // Get the data returned from the statement.
  record->fetch ();
  record->get_data (1, last_id);

  return last_id;
}

//
// bind_parameter
//
void ODBC_Query::bind_parameter (SQLUSMALLINT index,
                                 SQLSMALLINT io_type,
                                 SQLSMALLINT value_type,
                                 SQLSMALLINT param_type,
                                 SQLUINTEGER column_size,
                                 SQLSMALLINT decimal_digits,
                                 SQLPOINTER param_value_ptr,
                                 SQLINTEGER buffer_length,
                                 SQLINTEGER * strlen_indptr)
                                 ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  SQL_VERIFY (::SQLBindParameter (this->stmt_,
                                  index,
                                  io_type,
                                  value_type,
                                  param_type,
                                  column_size,
                                  decimal_digits,
                                  param_value_ptr,
                                  buffer_length,
                                  strlen_indptr),
              ODBC_Stmt_Exception (this->stmt_));

}

//
// prepare
//
void ODBC_Query::prepare (const char * stmt)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  // Prepare the statement.
  SQL_VERIFY (::SQLPrepare (this->stmt_, (SQLCHAR *) stmt, SQL_NTS),
              ODBC_Stmt_Exception (this->stmt_));

  // Get the number of parameters in the prepared statement.
  SQLSMALLINT pcount;
  SQL_VERIFY (::SQLNumParams (this->stmt_, &pcount),
              ODBC_Stmt_Exception (this->stmt_));

  // If this is the first prepared statement on this handle
  // then we need to allocate a new parameter list.
  if (this->params_.get () == 0)
  {
    // Create a new parameter list.
    ODBC_Parameter_List * plist = 0;
    if (pcount)
    {
      ACE_NEW (plist, ODBC_Parameter_List (this->stmt_, pcount));
    }
    else
    {
      ACE_NEW (plist, ODBC_Parameter_List ());
    }

    if (plist == 0)
      throw CUTS_DB_Exception ("failed to allocate parameter list");

    this->params_.reset (plist);
  }

  // Set the <size> of the parameter list.
  this->params_->resize (this->stmt_, pcount);
}

//
// cancel
//
void ODBC_Query::cancel (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  SQL_VERIFY (::SQLCancel (this->stmt_),
              ODBC_Stmt_Exception (this->stmt_));
}

//
// execute
//
CUTS_DB_Record * ODBC_Query::execute (const char * query)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  // Reset the record just in case we executed a "SELECT" statement.
  this->reset_cursor ();

  // Execute the query and set the <cursor_state_> to open.
  this->execute_no_record (query);
  this->cursor_open_ = 1;

  return this->record_i ();
}

//
// execute
//
CUTS_DB_Record * ODBC_Query::execute (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  // Reset the record just in case we executed a "SELECT" statement.
  this->reset_cursor ();

  // Execute the query and set the <cursor_state_> to open.
  this->execute_no_record ();
  this->cursor_open_ = 1;

  return this->record_i ();
}

//
// record_i
//
ODBC_Record * ODBC_Query::record_i (void)
{
  if (this->record_.get () == 0)
  {
    ODBC_Record * record = 0;
    ACE_NEW_RETURN (record, ODBC_Record (this->stmt_), 0);
    this->record_.reset (record);
  }

  return this->record_.get ();
}

//
// reset
//
void ODBC_Query::reset_cursor (void)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  if (this->cursor_open_)
  {
    // We have to close the current cursor if we are executing
    // more than one "SELECT" statement sequentially.
    SQL_VERIFY (::SQLCloseCursor (this->stmt_),
                ODBC_Stmt_Exception (this->stmt_));
    this->cursor_open_ = 0;
  }
}

//
// parameter
//
CUTS_DB_Parameter * ODBC_Query::parameter (size_t index)
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  return this->params_->get (index);
}

//
// parameter_count
//
size_t ODBC_Query::parameter_count (void) const
ACE_THROW_SPEC ((CUTS_DB_Exception))
{
  return this->params_.get () != 0 ? this->params_->count () : 0;
}
