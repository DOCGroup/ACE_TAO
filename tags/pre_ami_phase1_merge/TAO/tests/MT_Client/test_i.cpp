// $Id$

#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(MT_Client, test_i, "$Id$")

void
Simple_Server_i::test_method (CORBA::Environment&)
{
}

void
Simple_Server_i::shutdown (CORBA::Environment&)
{
  this->orb_->shutdown (1);
}
