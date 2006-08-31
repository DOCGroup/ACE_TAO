// $Id$

#include "DB_Connection.h"

#if !defined (__CUTS_INLINE__)
#include "DB_Connection.inl"
#endif

#include "DB_Query.h"

//
// CUTS_DB_Connection
//
CUTS_DB_Connection::CUTS_DB_Connection (void)
: connected_ (false)
{

}

//
// ~CUTS_DB_Connection
//
CUTS_DB_Connection::~CUTS_DB_Connection (void)
{
  this->disconnect ();
}
