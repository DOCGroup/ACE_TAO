// $Id$

#include "Query.h"

#if !defined (__ADBC_INLINE__)
#include "Query.inl"
#endif

#include "Connection.h"
#include "Exception.h"
#include "ace/CORBA_macros.h"
#include "sqlite3.h"

namespace ADBC
{
namespace SQLite
{
//
// prepare
//
void Query::prepare (const char * query)
{
  size_t len = ACE_OS::strlen (query);
  this->prepare (query, len);
}

//
// prepare
//
void Query::prepare (const char * query, size_t len)
{
  if (this->stmt_ != 0)
    this->finalize ();

  const char * tail = 0;

  int retval = ::sqlite3_prepare_v2 (this->parent_.conn_,
                                     query,
                                     len,
                                     &this->stmt_,
                                     &tail);

  if (retval != SQLITE_OK)
    throw Exception (this->parent_);

  // Reset the parameters for the statement.
  this->params_.reset ();
}

//
// destroy
//
void Query::destroy (void)
{
  delete this;
}

//
// execute_no_record
//
void Query::execute_no_record (void)
{
  if (this->stmt_ == 0)
    throw Exception ();

  if (this->needs_reseting_)
    this->reset ();

  // Execute the SQL statement.
  int retval = ::sqlite3_step (this->stmt_);
  this->needs_reseting_ = true;

  if (retval != SQLITE_DONE)
    throw Exception (this->parent_);
}

//
// execute
//
Record * Query::execute (void)
{
  if (this->stmt_ == 0)
    throw Exception ();

  if (this->needs_reseting_)
    this->reset ();

  // Initialize the cursor for the record.
  int retval = ::sqlite3_step (this->stmt_);

  Record * record = 0;

  ACE_NEW_THROW_EX (record,
                    Record (*this, retval),
                    ACE_bad_alloc ());

  this->needs_reseting_ = true;
  return record;
}

//
// last_insert_id
//
long Query::last_insert_id (void)
{
  return static_cast <long> (::sqlite3_last_insert_rowid (this->parent_.conn_));
}

//
// finalize
//
void Query::finalize (void)
{
  // Release the statements resources.
  ::sqlite3_finalize (this->stmt_);

  // Reset the variables.
  this->stmt_ = 0;
  this->needs_reseting_ = false;
}

//
// reset
//
void Query::reset (void)
{
  ::sqlite3_reset (this->stmt_);
  this->needs_reseting_ = false;
}

}
}
