// -*- C++ -*-
// $Id$

namespace ADBC
{
namespace ODBC
{
//
// Environment
//
ADBC_INLINE
Environment::Environment (void)
: handle_ (SQL_NULL_HANDLE)
{
  this->init ();
}

//
// handle
//
ADBC_INLINE
HENV Environment::handle (void) const
{
  return this->handle_;
}

}
}
