// $Id$

#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Native_Exceptions, test_i, "$Id$")

CORBA::Long
Simple_Server_i::test_method (CORBA::Long x)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return x;
}

CORBA::Long
Simple_Server_i::test_raise (CORBA::Long)
  ACE_THROW_SPEC ((Simple_Server::Failure))
{
  throw Simple_Server::Failure ();
}

void
Simple_Server_i::shutdown (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown ();
}
