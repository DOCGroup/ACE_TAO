// -*- C++ -*-
// $Id$

namespace ADBC
{
namespace SQLite
{

//
// Connection
//
ADBC_INLINE
Connection::Connection (void)
: conn_ (0)
{

}

//
// ~Connection
//
ADBC_INLINE
Connection::~Connection (void)
{
  this->disconnect ();
}

}
}
