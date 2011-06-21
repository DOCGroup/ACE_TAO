// $Id$

#include "tao/PI/PolicyFactory_Registry.h"
#include "tao/PI/PolicyFactoryC.h"
#include "tao/ORB_Constants.h"
#include "tao/PolicyC.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PolicyFactory_Registry::TAO_PolicyFactory_Registry (void)
  : factories_ (TAO_DEFAULT_POLICY_FACTORY_REGISTRY_SIZE)
{
}

TAO_PolicyFactory_Registry::~TAO_PolicyFactory_Registry (void)
{
  const TABLE::iterator end (this->factories_.end ());

  for (TABLE::iterator i = this->factories_.begin (); i != end; ++i)
    {
      ::CORBA::release ((*i).int_id_);
    }

  this->factories_.close ();
}

void
TAO_PolicyFactory_Registry::register_policy_factory (
  CORBA::PolicyType type,
  PortableInterceptor::PolicyFactory_ptr policy_factory)
{
  if (CORBA::is_nil (policy_factory))
    {
      throw ::CORBA::BAD_PARAM (
                   CORBA::SystemException::_tao_minor_code (
                     0,
                     EINVAL),
                   CORBA::COMPLETED_NO);
    }

  PortableInterceptor::PolicyFactory_ptr factory =
    PortableInterceptor::PolicyFactory::_duplicate (policy_factory);

  const int result = this->factories_.bind (type, factory);

  if (result != 0)
    {
      // Release the duplicated factory to prevent a memory leak
      ::CORBA::release (factory);

      if (result == 1)
        {
          // PolicyFactory of given type already exists.
          throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 16,
                                        CORBA::COMPLETED_NO);
        }
      else
        {
          // Could not add PolicyFactory due to internal bind failures.
          throw ::CORBA::INTERNAL ();
        }
    }
}

CORBA::Policy_ptr
TAO_PolicyFactory_Registry::create_policy (CORBA::PolicyType type,
                                           const CORBA::Any &value)
{
  PortableInterceptor::PolicyFactory_ptr policy_factory =
    PortableInterceptor::PolicyFactory::_nil ();

  if (this->factories_.find (type, policy_factory) == -1)
    {
      // Policy factory corresponding to given policy type does not
      // exist in policy factory map.
      throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
    }

  return policy_factory->create_policy (type, value);
}

CORBA::Policy_ptr
TAO_PolicyFactory_Registry::_create_policy (CORBA::PolicyType type)
{
  PortableInterceptor::PolicyFactory_ptr policy_factory =
    PortableInterceptor::PolicyFactory::_nil ();

  if (this->factories_.find (type, policy_factory) == -1)
    {
      // Policy factory corresponding to given policy type does not
      // exist in policy factory map.
      throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
    }

  return policy_factory->_create_policy (type);
}

bool
TAO_PolicyFactory_Registry::factory_exists (CORBA::PolicyType & type) const
{
  return (this->factories_.find (type) == 0);
}


TAO_END_VERSIONED_NAMESPACE_DECL
