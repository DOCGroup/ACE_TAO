#include "test_i.h"

#if !defined(__ACE_INLINE__)
#include "test_i.inl"
#endif /* __ACE_INLINE__ */

CORBA::Boolean
Simple_Server_i::test_is_a (char const * /* type */)
{
  ++this->ncalls_;

  if ((this->raise_exception_ & TAO::FOE_OBJECT_NOT_EXIST) == TAO::FOE_OBJECT_NOT_EXIST)
  {
    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) test_is_a called %d and raise OBJECT_NOT_EXIST\n", ncalls_));
    throw ::CORBA::OBJECT_NOT_EXIST ();
  }

  if ((this->raise_exception_ & TAO::FOE_COMM_FAILURE) == TAO::FOE_COMM_FAILURE)
  {
    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) test_is_a called %d and raise COMM_FAILURE\n", ncalls_));
    throw ::CORBA::COMM_FAILURE ();
  }

  if ((this->raise_exception_ & TAO::FOE_TRANSIENT) == TAO::FOE_TRANSIENT)
  {
    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) test_is_a called %d and raise TRANSIENT\n", ncalls_));
    throw ::CORBA::TRANSIENT ();
  }

  if ((this->raise_exception_ & TAO::FOE_INV_OBJREF) == TAO::FOE_INV_OBJREF)
  {
    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) test_is_a called %d and raise INV_OBJREF\n", ncalls_));
    throw ::CORBA::INV_OBJREF ();
  }

  return false;
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
