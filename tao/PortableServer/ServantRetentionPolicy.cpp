// $Id$

#include "tao/orbconf.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/ServantRetentionPolicy.h"
#include "tao/PortableServer/PortableServer.h"

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
    ServantRetentionPolicy::copy (void)
    {
      ServantRetentionPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        ServantRetentionPolicy (this->value_),
                        CORBA::NO_MEMORY ());

      return copy;
    }

    void
    ServantRetentionPolicy::destroy (void)
    {
    }

    ::PortableServer::ServantRetentionPolicyValue
    ServantRetentionPolicy::value (void)
    {
      return this->value_;
    }

    CORBA::PolicyType
    ServantRetentionPolicy::policy_type (void)
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
