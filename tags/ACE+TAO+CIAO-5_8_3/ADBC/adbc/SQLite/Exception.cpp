// $Id$

#include "Exception.h"

#if !defined (__ADBC_INLINE__)
#include "Exception.inl"
#endif

#include "Connection.h"
#include "sqlite3.h"

namespace ADBC
{
namespace SQLite
{
//
// Exception
//
Exception::
Exception (const Connection & conn)
: ::ADBC::Exception (::sqlite3_errcode (conn.conn_),
                     "",
                     ::sqlite3_errmsg (conn.conn_))
{

}

}
}
