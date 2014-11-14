// $Id$

#include "test_i.h"
#include "tao/Invocation_Utils.h"

extern int raise_exception;

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb, int num_exceptions_to_throw)
  : orb_ (CORBA::ORB::_duplicate (orb))
  , num_exceptions_to_throw_(num_exceptions_to_throw)
  , num_exceptions_thrown_(0)
  , raise_exception_ (TAO::FOE_NON)
{
}

CORBA::Boolean
Simple_Server_i::test_is_a (const char * /* type */)
{
  if (this->num_exceptions_thrown_ ==
      this->num_exceptions_to_throw_)
    return 0;

  if ((raise_exception & TAO::FOE_OBJECT_NOT_EXIST) == TAO::FOE_OBJECT_NOT_EXIST)
  {
    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) test_is_a called and raise OBJECT_NOT_EXIST\n"));
    ++this->num_exceptions_thrown_;
    throw ::CORBA::OBJECT_NOT_EXIST (CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO);
  }

  if ((raise_exception & TAO::FOE_COMM_FAILURE) == TAO::FOE_COMM_FAILURE)
  {
    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) test_is_a called and raise COMM_FAILURE\n"));
    ++this->num_exceptions_thrown_;
    throw ::CORBA::COMM_FAILURE (CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO);
  }

  if ((raise_exception & TAO::FOE_TRANSIENT) == TAO::FOE_TRANSIENT)
  {
    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) \t called and raise TRANSIENT\n"));
    ++this->num_exceptions_thrown_;
    throw ::CORBA::TRANSIENT (CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO);
  }

  if ((raise_exception & TAO::FOE_INV_OBJREF) == TAO::FOE_INV_OBJREF)
  {
    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) test_is_a called %d and raise INV_OBJREF\n", ncalls_));
    ++this->num_exceptions_thrown_;
    throw ::CORBA::INV_OBJREF (CORBA::OMGVMCID | 1, CORBA::COMPLETED_NO);
  }

  return 0;
}

void
Simple_Server_i::shutdown (void)
{
  this->orb_->shutdown (0);
}
