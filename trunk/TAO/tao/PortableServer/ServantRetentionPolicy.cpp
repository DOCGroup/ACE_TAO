#include "tao/PortableServer/ServantRetentionPolicy.h"
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID (PortableServer,
           ServantRetentionPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionPolicy::ServantRetentionPolicy (
      ::PortableServer::ServantRetentionPolicyValue value) :
        value_ (value)
    {
    }

    CORBA::Policy_ptr
    ServantRetentionPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ServantRetentionPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        ServantRetentionPolicy (this->value_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return copy;
    }

    void
    ServantRetentionPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::ServantRetentionPolicyValue
    ServantRetentionPolicy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return this->value_;
    }

    CORBA::PolicyType
    ServantRetentionPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::SERVANT_RETENTION_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    ServantRetentionPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_SERVANT_RETENTION;
    }

    TAO_Policy_Scope
    ServantRetentionPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */
