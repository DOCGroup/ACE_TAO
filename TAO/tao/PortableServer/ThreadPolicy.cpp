// -*- C++ -*-
#include "tao/orbconf.h"

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
    ThreadPolicy::copy ()
    {
      ThreadPolicy *copy = 0;
      ACE_NEW_THROW_EX (copy,
                        ThreadPolicy (this->value_),
                        CORBA::NO_MEMORY ());

      return copy;
    }

    void
    ThreadPolicy::destroy ()
    {
    }

    ::PortableServer::ThreadPolicyValue
    ThreadPolicy::value ()
    {
      return value_;
    }

    CORBA::PolicyType
    ThreadPolicy::policy_type ()
    {
      return ::PortableServer::THREAD_POLICY_ID;
    }

    TAO_Cached_Policy_Type
    ThreadPolicy::_tao_cached_type () const
    {
      return TAO_CACHED_POLICY_THREAD;
    }

    TAO_Policy_Scope
    ThreadPolicy::_tao_scope () const
    {
      return TAO_POLICY_POA_SCOPE;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA == 0 */
