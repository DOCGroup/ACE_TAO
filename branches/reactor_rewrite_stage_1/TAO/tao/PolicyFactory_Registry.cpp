// -*- C++ -*-
//
// $Id$


#include "PolicyFactory_Registry.h"

ACE_RCSID(tao, PolicyFactory_Registry, "$Id$")


TAO_PolicyFactory_Registry::TAO_PolicyFactory_Registry (void)
  : factories_ ()
{
}

TAO_PolicyFactory_Registry::~TAO_PolicyFactory_Registry (void)
{
  TABLE::ITERATOR end = this->factories_.end ();

  for (TABLE::ITERATOR i = this->factories_.begin (); i != end; ++i)
    CORBA::release ((*i).int_id_);

  this->factories_.close ();
}

void
TAO_PolicyFactory_Registry::register_policy_factory (
  CORBA::PolicyType type,
  PortableInterceptor::PolicyFactory_ptr policy_factory
  TAO_ENV_ARG_DECL)
{
  if (CORBA::is_nil (policy_factory))
    ACE_THROW (CORBA::BAD_PARAM (
                 CORBA_SystemException::_tao_minor_code (
                   TAO_DEFAULT_MINOR_CODE,
                   EINVAL),
                 CORBA::COMPLETED_NO));

  PortableInterceptor::PolicyFactory_ptr factory =
    PortableInterceptor::PolicyFactory::_duplicate (policy_factory);

  int result = this->factories_.bind (type,
                                      factory);

  if (result == 1)
    {
      // PolicyFactory of given type already exists.

      ACE_THROW (CORBA::BAD_INV_ORDER (TAO_OMG_VMCID | 16,
                                       CORBA::COMPLETED_NO));
    }
  else if (result == -1)
    {
      // Could not add PolicyFactory due to internal bind failures.
      ACE_THROW (CORBA::INTERNAL ());
    }
}

CORBA::Policy_ptr
TAO_PolicyFactory_Registry::create_policy (CORBA::PolicyType type,
                                           const CORBA::Any &value
                                           TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
  PortableInterceptor::PolicyFactory_ptr policy_factory =
    PortableInterceptor::PolicyFactory::_nil ();

  if (this->factories_.find (type,
                             policy_factory) == -1)
    {
      // Policy factory corresponding to given policy type does not
      // exist in policy factory map.
      ACE_THROW_RETURN (
         CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),  // @@ Right exception?
         CORBA::Policy::_nil ());
    }

  return policy_factory->create_policy (type,
                                        value
                                        TAO_ENV_ARG_PARAMETER);
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Map_Entry<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr>;
template class ACE_Map_Iterator_Base<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr, ACE_Null_Mutex>;
template class ACE_Map_Iterator<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr, ACE_Null_Mutex>;
template class ACE_Map_Manager<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Map_Entry<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr>
#pragma instantiate ACE_Map_Iterator_Base<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Manager<CORBA::PolicyType, PortableInterceptor::PolicyFactory_ptr, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
