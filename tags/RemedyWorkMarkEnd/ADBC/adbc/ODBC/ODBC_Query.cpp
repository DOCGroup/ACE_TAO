// $Id$

#include "ODBC_Query.h"

#if !defined (__ADBC_INLINE__)
#include "ODBC_Query.inl"
#endif

#include "ODBC_Connection.h"
#include "ODBC_Record.h"
#include "ODBC_Parameter.h"
#include "ODBC_Parameter_List.h"

namespace ADBC
{
namespace ODBC
{
//
// ~Query
//
Query::~Query (void)
{
  if (this->handle_ != SQL_NULL_HSTMT)
    ::SQLFreeHandle (SQL_HANDLE_STMT, this->handle_);
}

//
// init
//
void Query::init (void)
{
  SQL_CONN_VERIFY (::SQLAllocHandle (SQL_HANDLE_STMT,
                   this->parent_.handle (),
                   &this->handle_),
                   this->parent_.handle ());

  // Set the attributes for the statement. We need to find a
  // better method for setting attributes. Maybe we can overload
  // the operator [].
  SQL_STMT_VERIFY (::SQLSetStmtAttr (this->handle_,
                   SQL_ATTR_CONCURRENCY,
                   (SQLPOINTER) SQL_CONCUR_ROWVER,
                   0),
                   this->handle_);

  SQL_STMT_VERIFY (::SQLSetStmtAttr (this->handle_,
                   SQL_ATTR_CURSOR_TYPE,
                   (SQLPOINTER) SQL_CURSOR_KEYSET_DRIVEN,
                   0),
                   this->handle_);
}

//
// prepare
//
void Query::prepare (const char * stmt)
{
  // Prepare the SQL statement.
  SQL_STMT_VERIFY (::SQLPrepare (this->handle_,
                   (SQLCHAR *) stmt,
                   SQL_NTS),
                   this->handle_);

  /// Update the parameters for this statement.
  this->params_.update ();
  this->is_prepared_ = true;
}

//
// execute_no_record
//
void Query::execute_no_record (void)
{
  if (!this->is_prepared_)
    throw ::ADBC::ODBC::Exception ("statement is not prepared");

  SQL_STMT_VERIFY (::SQLExecute (this->handle_),
                   this->handle_);

  // There is no cursor associated with non-record statements.
  this->cursor_open_ = false;
}

//
// execute_no_record
//
void Query::execute_no_record (const char * query)
{
  SQL_STMT_VERIFY (::SQLExecDirect (this->handle_, (SQLCHAR *)query, SQL_NTS),
                   this->handle_);

  // The statement is not prepared, and there is no cursor
  // associated with non-record statements.
  this->is_prepared_ = false;
  this->cursor_open_ = false;
}

//
// last_insert_id
//
long Query::last_insert_id (void)
{
  // Execute the statement and move to the first and only row in
  // the result set. There has to be some way to get the last insert
  // id without having to execute an SQL statement.
  Query temp_query (this->parent_);
  Record & record = temp_query.execute ("SELECT LAST_INSERT_ID()");

  // Get the data returned from the statement.
  long last_id = 0;
  record.get_data (1, last_id);

  return last_id;
}

//
// cancel
//
void Query::cancel (void)
{
  SQL_STMT_VERIFY (::SQLCancel (this->handle_),
                   this->handle_);
}

//
// execute
//
Record & Query::execute (const char * query)
{
  // Execute the query and set the <cursor_open_> to open.
  this->execute_no_record (query);

  // Move to the first record, if available.
  this->record_.reset ();
  return this->record_;
}

//
// execute
//
Record & Query::execute (void)
{
  // Execute the prepared SQL statement.
  this->execute_no_record ();

  // Move to the first record, if available.
  this->record_.reset ();
  return this->record_;
}

//
// reset
//
void Query::reset (void)
{
  SQL_STMT_VERIFY (::SQLFreeStmt (this->handle_, SQL_CLOSE),
                   this->handle_);
}

//
// count
//
size_t Query::count (void) const
{
  SQLINTEGER count;
  SQL_STMT_VERIFY (::SQLRowCount (this->handle_, &count),
                   this->handle_);

  return count;
}

}
}
