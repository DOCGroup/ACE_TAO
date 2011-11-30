// $Id$

#include "ODBC_Connection.h"

#if !defined (__ADBC_INLINE__)
#include "ODBC_Connection.inl"
#endif

#include "ODBC_Query.h"
#include "ace/CORBA_macros.h"

namespace ADBC
{
namespace ODBC
{
//
// init
//
void Connection::init (void)
{
  // Allocate the connection handle.
  SQL_ENV_VERIFY (::SQLAllocHandle (SQL_HANDLE_DBC, 
                                    this->env_->handle (), 
                                    &this->handle_),
                  this->env_->handle ());
}

//
// ~Connection
//
Connection::~Connection (void)
{
  if (this->handle_ != SQL_NULL_HANDLE)
    ::SQLFreeHandle (SQL_HANDLE_DBC, this->handle_);
}

//
// connect
//
void Connection::connect (const ACE_CString & connstr)
{
  // Remove this connection.
  this->disconnect ();

  // Establish a connection with the database.
  short result = 0;
  static SQLCHAR tempstr[1024];

  SQL_CONN_VERIFY (::SQLDriverConnect (this->handle_,
                   0,
                   (SQLCHAR *)connstr.c_str (),
                   connstr.length (),
                   tempstr,
                   sizeof (tempstr),
                   &result,
                   SQL_DRIVER_NOPROMPT),
                   this->handle_);

  // Pass control to the base class.
  ::ADBC::Connection::connect (connstr);
}

//
// disconnect
//
void Connection::disconnect (void)
{
  if (this->handle_ != SQL_NULL_HDBC && this->is_connected ())
    SQL_CONN_VERIFY (::SQLDisconnect (this->handle_),
                     this->handle_);

  // Pass control to the base class.
  ::ADBC::Connection::disconnect ();
}

//
// create_query
//
Query * Connection::create_query (void)
{
  Query * query = 0;

  ACE_NEW_THROW_EX (query,
                    Query (*this),
                    ACE_bad_alloc ());

  return query;
}
}
}
