// $Id$

#include "authorization_test_i.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "authorization_test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Authorize, authorization_test_i, "$Id$")

CORBA::Boolean
Authorization_Server_i::check_access (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

void
Authorization_Server_i::shutdown (CORBA::Environment&)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0);
}
