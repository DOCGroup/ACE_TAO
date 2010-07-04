// $Id$

namespace ADBC
{
namespace ODBC
{
//
// Connection
//
ADBC_INLINE
Connection::Connection (Environment * env)
: handle_ (SQL_NULL_HANDLE),
  env_ (env)
{
  this->init ();
}

//
// handle
//
ADBC_INLINE
HDBC Connection::handle (void) const
{
  return this->handle_;
}

}
}
