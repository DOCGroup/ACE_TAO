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
      throw ::CORBA::OBJECT_NOT_EXIST ();
    }

  if ((this->raise_exception_ & TAO::FOE_COMM_FAILURE) == TAO::FOE_COMM_FAILURE)
    {
      throw ::CORBA::COMM_FAILURE ();
    }

  if ((this->raise_exception_ & TAO::FOE_TRANSIENT) == TAO::FOE_TRANSIENT)
    {
      throw ::CORBA::TRANSIENT ();
    }

  if ((this->raise_exception_ & TAO::FOE_INV_OBJREF) == TAO::FOE_INV_OBJREF)
    {
      throw ::CORBA::INV_OBJREF ();
    }

  return false;
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
