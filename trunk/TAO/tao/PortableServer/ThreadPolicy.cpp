#include "tao/orbconf.h"

ACE_RCSID (PortableServer,
           ThreadPolicy,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "tao/PortableServer/ThreadPolicy.h"
#include "tao/PortableServer/PortableServer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    ThreadPolicy::ThreadPolicy (::PortableServer::ThreadPolicyValue value)
      : value_ (value)
    {
    }

    CORBA::Policy_ptr
    ThreadPolicy::copy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ThreadPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        ThreadPolicy (this->value_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());

      return copy;
    }

    void
    ThreadPolicy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    }

    ::PortableServer::ThreadPolicyValue
    ThreadPolicy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return value_;
    }

    CORBA::PolicyType
    ThreadPolicy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
      return ::PortableServer::THREAD_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    ThreadPolicy::_tao_cached_type (void) const
    {
      return TAO_CACHED_POLICY_THREAD;
    }

    TAO_Policy_Scope
    ThreadPolicy::_tao_scope (void) const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */
