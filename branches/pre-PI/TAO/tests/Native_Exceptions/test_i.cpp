// $Id$

#include "test_i.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Native_Exceptions, test_i, "$Id$")

CORBA::Long
Simple_Server_i::test_method (CORBA::Long x)
  throw ()
{
  return x;
}

CORBA::Long
Simple_Server_i::test_raise (CORBA::Long x)
  throw (Simple_Server::Failure)
{
  throw Simple_Server::Failure ();
}

void
Simple_Server_i::shutdown (void) throw ()
{
  this->orb_->shutdown ();
}
