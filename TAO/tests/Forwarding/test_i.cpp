#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

CORBA::Boolean
Simple_Server_i::test_is_a (char const * /* type */)
{
  // ACE_DEBUG ((LM_DEBUG, "(%P|%t) test_is_a %s\n", type));
  return false;
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
