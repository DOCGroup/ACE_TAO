// $Id$

#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(MT_Client, test_i, "$Id$")

void
Simple_Server_i::test_method (void)
{
}

void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
