// -*- C++ -*-
// $Id$

#include "Connection.h"

namespace ADBC
{
namespace SQLite
{
//
// Exception
//
ADBC_INLINE
Exception::Exception (void)
{

}

//
// Exception
//
ADBC_INLINE
Exception::
Exception (const Connection & conn)
: ::ADBC::Exception (::sqlite3_errcode (conn.conn_),
                     "",
                     ::sqlite3_errmsg (conn.conn_))
{

}

//
// Exception
//
ADBC_INLINE
Exception::Exception (const char * message)
: ::ADBC::Exception (message)
{

}

//
// ~Exception
//
ADBC_INLINE
Exception::~Exception (void)
{

}
}
}
