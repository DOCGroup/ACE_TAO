#include "IdUniquenessPolicy.h"
#include "PortableServer.h"

ACE_RCSID (PortableServer,
           IdUniquenessPolicy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessPolicy::IdUniquenessPolicy ()
    {
    }

    void
    IdUniquenessPolicy::init (
      const CORBA::Any &value ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      ::PortableServer::IdUniquenessPolicyValue thrvalue;
      if ((value >>= thrvalue) == 0)
        ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

      (void) this->init (thrvalue);
    }

    void
    IdUniquenessPolicy::init (
      ::PortableServer::IdUniquenessPolicyValue value)
    {
      value_ = value;
    }

    CORBA::Policy_ptr
    IdUniquenessPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      IdUniquenessPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        IdUniquenessPolicy,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      (void) copy->init (this->value_);

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

