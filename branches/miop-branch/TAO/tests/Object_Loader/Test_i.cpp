// $Id$

#include "Test_i.h"

#if !defined(__ACE_INLINE__)
#include "Test_i.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Object_Loader, Test_i, "$Id$")

CORBA::Long
Test_i::instance_count_ = 0;

Test_i::Test_i (PortableServer::POA_ptr poa)
  :  poa_ (PortableServer::POA::_duplicate (poa))
{
  Test_i::instance_count_++;
}

CORBA::Long
Test_i::instance_count (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return Test_i::instance_count_;
}

void
Test_i::destroy (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_var poa =
    this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  poa->deactivate_object (oid.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::POA_ptr
Test_i::_default_POA (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}
