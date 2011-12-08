// $Id$

#include "Connection.h"

#if !defined (__ADBC_INLINE__)
#include "Connection.inl"
#endif

#include "Exception.h"
#include "ace/CORBA_macros.h"
#include "sqlite3.h"

namespace ADBC
{
namespace SQLite
{
//
// connect
//
void Connection::connect (const ACE_CString & connstr)
{
  long flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
  this->connect (connstr, flags, 0);
}

//
// connect
//
void Connection::
connect (const ACE_CString & connstr, long flags, const char * vfs)
{
  int retval = ::sqlite3_open_v2 (connstr.c_str (),
                                  &this->conn_,
                                  flags,
                                  vfs);

  if (retval != SQLITE_OK)
    throw Exception (*this);

  ::ADBC::Connection::connect (connstr);
}

//
// disconnect
//
void Connection::disconnect (void)
{
  if (this->conn_ == 0)
    return;

  ::sqlite3_close (this->conn_);
  this->conn_ = 0;

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
