// -*- C++ -*-
// $Id$

namespace ADBC
{
//
// Connection
//
ADBC_INLINE
Connection::Connection (void)
: connected_ (false)
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

//
// is_connected
//
ADBC_INLINE
bool Connection::is_connected (void) const
{
  return this->connected_;
}

//
// disconnect
//
ADBC_INLINE
void Connection::disconnect (void)
{
  this->connected_ = false;
}

//
// connect
//
ADBC_INLINE
void Connection::connect (const ACE_CString & connstr)
{
  this->connected_ = true;
  this->connstr_ = connstr;
}
}
