#include "tao/PortableServer/IdUniquenessPolicy.h"
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID (PortableServer,
           IdUniquenessPolicy,
           "$Id$")

#if !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessPolicy::IdUniquenessPolicy (
      ::PortableServer::IdUniquenessPolicyValue value) :
        value_ (value)
    {
    }

    CORBA::Policy_ptr
    IdUniquenessPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      IdUniquenessPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        IdUniquenessPolicy (this->value_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return copy;
    }

    void
    IdUniquenessPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::IdUniquenessPolicyValue
    IdUniquenessPolicy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_;
    }

    CORBA::PolicyType
    IdUniquenessPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::ID_UNIQUENESS_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    IdUniquenessPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_ID_UNIQUENESS;
    }

    TAO_Policy_Scope
    IdUniquenessPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
