#include "tao/PortableServer/LifespanPolicy.h"
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID (PortableServer,
           LifespanPolicy,
           "$Id$")

#if !defined (CORBA_E_MICRO)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    LifespanPolicy::LifespanPolicy (
      ::PortableServer::LifespanPolicyValue value) :
        value_ (value)
    {
    }

    CORBA::Policy_ptr
    LifespanPolicy::copy (void)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      LifespanPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        LifespanPolicy (this->value_),
                        CORBA::NO_MEMORY ());

      return copy;
    }

    void
    LifespanPolicy::destroy (void)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::LifespanPolicyValue
    LifespanPolicy::value (void)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_;
    }

    CORBA::PolicyType
    LifespanPolicy::policy_type (void)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::LIFESPAN_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    LifespanPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_LIFESPAN;
    }

    TAO_Policy_Scope
    LifespanPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
